#include "UniversalLoggingPrivatePCH.h"

#include "UniversalLogging.h"

#include "LogStream.h"

void UniversalLoggingImpl::StartupModule()
{
  _streams.Add("", TUniquePtr<LogStreamImpl>(new LogStreamImpl()));
}

void UniversalLoggingImpl::ShutdownModule()
{
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
  ILogStream* stream_obj = nullptr;
  if (_streams.Contains(stream))
    stream_obj = _streams[stream].Get();
  else
    stream_obj = NewLogStream(stream);
  FString FilePath = FPaths::ConvertRelativePathToFull(FPaths::ProjectDir() + stream_obj->GetFilepath()) + "/" + stream_obj->GetFilename();
  FString full_text = text;
  if (!omit_newline)
    full_text += "\n";
  FFileHelper::SaveStringToFile(full_text, *FilePath, FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get(), EFileWrite::FILEWRITE_Append);
}

IMPLEMENT_MODULE(UniversalLoggingImpl, UniversalLogging)