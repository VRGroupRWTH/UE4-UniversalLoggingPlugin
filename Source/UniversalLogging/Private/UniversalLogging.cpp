#include "UniversalLoggingPrivatePCH.h"

#include "UniversalLogging.h"

#include "LogStream.h"

void UniversalLoggingImpl::StartupModule()
{
  _streams.Add("", TUniquePtr<LogStreamImpl>(new LogStreamImpl()));

  on_post_world_initialization_delegate_.BindRaw(this, &UniversalLoggingImpl::OnSessionStart);
  FWorldDelegates::OnPostWorldInitialization.Add(on_post_world_initialization_delegate_);

  on_pre_world_finish_destroy_delegate_.BindRaw(this, &UniversalLoggingImpl::OnSessionEnd);
  FWorldDelegates::OnPreWorldFinishDestroy.Add(on_pre_world_finish_destroy_delegate_);

  _session_id = "";
}

void UniversalLoggingImpl::ShutdownModule()
{
}

void UniversalLoggingImpl::OnSessionStart(UWorld* world, const UWorld::InitializationValues)
{
  if (!world->IsGameWorld())
    return;
  if (world->IsPlayInEditor())
    ResetSessionId("PlayInEditor");
  else if (world->IsPlayInPreview())
    ResetSessionId("PlayInPreview");
  else
    ResetSessionId("Play");
}

void UniversalLoggingImpl::OnSessionEnd(UWorld* world)
{
  if (!world->IsGameWorld())
    return;
  ResetSessionId("Stopped");

  for (auto& elem : _streams)
  {
    elem.Value->Close();
  }
}

ILogStream* UniversalLoggingImpl::NewLogStream(const FString streamname)
{
  _streams.Add(streamname, TUniquePtr<LogStreamImpl>(new LogStreamImpl("Saved/Logs", "UniversalLogging_" + streamname + ".log")));
  return _streams[streamname].Get();
}

ILogStream* UniversalLoggingImpl::NewLogStream(const FString streamname, const FString filepath, const FString filename, bool per_session /* = false*/)
{
  _streams.Add(streamname, TUniquePtr<LogStreamImpl>(new LogStreamImpl(filepath, filename, per_session)));
  return _streams[streamname].Get();
}

ILogStream * UniversalLoggingImpl::GetLogStream(const FString streamname)
{
  if (_streams.Contains(streamname))
    return _streams[streamname].Get();
  else 
    return nullptr;
}

ILogStream * UniversalLoggingImpl::GetDefaultLogStream()
{
  return GetLogStream("");
}

FString UniversalLoggingImpl::GetSessionIdentifier()
{
  return _session_id;
}

void UniversalLoggingImpl::Log(const FString text, const FString stream /*= ""*/, bool omit_newline /*= false*/)
{
  LogStreamImpl* stream_obj = nullptr;
  if (!_streams.Contains(stream))
    NewLogStream(stream);
  stream_obj = _streams[stream].Get();
  FString full_text = text;
  if (!omit_newline)
    full_text += "\n";
  stream_obj->Write(full_text);
}

void UniversalLoggingImpl::ResetSessionId(FString prefix)
{
  FString timestamp = FGenericPlatformTime::StrTimestamp();
  timestamp = timestamp.Replace(TEXT("/"), TEXT("-"));
  timestamp = timestamp.Replace(TEXT(" "), TEXT("_"));
  timestamp = timestamp.Replace(TEXT(":"), TEXT("-"));
  _session_id = FString::Printf(TEXT("%s_%s"), *prefix, *timestamp);
}

IMPLEMENT_MODULE(UniversalLoggingImpl, UniversalLogging)