#include "UniversalLoggingPrivatePCH.h"

#include "LogStream.h"

LogStreamImpl::LogStreamImpl(const FString filepath, const FString filename)
  : _filepath(filepath)
  , _filename(filename)
{
}

FString LogStreamImpl::GetFilepath()
{
  return _filepath;
}

FString LogStreamImpl::GetFilename()
{
  return _filename;
}