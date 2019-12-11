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
}

void UniversalLoggingImpl::ShutdownModule()
{
}

void UniversalLoggingImpl::OnSessionStart(UWorld* world, const UWorld::InitializationValues)
{
  if (world->IsGameWorld())
    Log("OnSessionStart", "Test");
}

void UniversalLoggingImpl::OnSessionEnd(UWorld* world)
{
  if (world->IsGameWorld())
    Log("OnSessionEnd", "Test");
}

ILogStream* UniversalLoggingImpl::NewLogStream(const FString streamname)
{
  _streams.Add(streamname, TUniquePtr<LogStreamImpl>(new LogStreamImpl("Saved/Logs", "UniversalLogging_" + streamname + ".log")));
  return _streams[streamname].Get();
}

ILogStream* UniversalLoggingImpl::NewLogStream(const FString streamname, const FString filepath, const FString filename)
{
  _streams.Add(streamname, TUniquePtr<LogStreamImpl>(new LogStreamImpl(filepath, filename)));
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

IMPLEMENT_MODULE(UniversalLoggingImpl, UniversalLogging)