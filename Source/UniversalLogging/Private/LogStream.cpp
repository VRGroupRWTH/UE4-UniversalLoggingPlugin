#include "UniversalLoggingPrivatePCH.h"

#include "LogStream.h"

#include "HAL/PlatformFilemanager.h"

LogStreamImpl::LogStreamImpl(const FString filepath, const FString filename)
  : _filepath(filepath)
  , _filename(filename)
  , _is_open(false)
  , _is_valid(false)
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
  FString file_path = FPaths::ConvertRelativePathToFull(FPaths::ProjectDir() + _filepath) + "/" + _filename;
  IPlatformFile& platform_file = FPlatformFileManager::Get().GetPlatformFile();
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
  if (_file_handle)
    delete _file_handle;
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