#include "UniversalLoggingPrivatePCH.h"

#include "LogStream.h"

#include "HAL/PlatformFilemanager.h"

LogStreamImpl::LogStreamImpl(const FString filepath, const FString filename, const bool per_session)
  : _filepath(filepath)
  , _filename(filename)
  , _per_session(per_session)
  , _is_open(false)
  , _is_valid(false)
  , _file_handle(nullptr)
{
  Open();
  if (_is_open)
    _is_valid = true;
  Close();
}

LogStreamImpl::~LogStreamImpl()
{
  Close();
}

FString LogStreamImpl::GetFilepath()
{
  return _filepath;
}

FString LogStreamImpl::GetFilename()
{
  return _filename;
}

bool LogStreamImpl::GetIsValid()
{
  return _is_valid;
}

void LogStreamImpl::Open()
{
  IPlatformFile& platform_file = FPlatformFileManager::Get().GetPlatformFile();
  FString file_path = FPaths::ConvertRelativePathToFull(FPaths::ProjectDir() + _filepath);
  if(_per_session)
    file_path += "/" + UniLog.GetSessionIdentifier();
  platform_file.CreateDirectoryTree(*file_path);
  file_path += "/" + _filename;
  _file_handle = platform_file.OpenWrite(*file_path);
  if (!_file_handle)
  {
    UE_LOG(LogTemp, Error, TEXT("Universal Logging: The file %s could not be opened for writing."), *file_path);
    _is_open = false;
    _is_valid = false;
    return;
  }
  _is_open = true;
}

void LogStreamImpl::Close()
{
  delete _file_handle;
  _file_handle = nullptr;
  _is_open = false;
}

bool LogStreamImpl::GetIsOpen()
{
  return _is_open;
}

void LogStreamImpl::Write(const FString text)
{
  if (!_is_valid)
    return;
  if (!_is_open)
    Open();
  _file_handle->Write((const uint8*)TCHAR_TO_ANSI(*text), text.Len());
}