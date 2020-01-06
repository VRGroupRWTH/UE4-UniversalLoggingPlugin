#include "UniversalLoggingPrivatePCH.h"

#include "LogStream.h"

#include "HAL/PlatformFilemanager.h"
#include "Paths.h"
#include "IPlatformFileProfilerWrapper.h"

LogStreamImpl::LogStreamImpl(const FString Filepath, const FString Filename, const bool bPer_Session)
  : Filepath(Filepath)
  , Filename(Filename)
  , bPer_Session(bPer_Session)
  , bIs_Open(false)
  , bIs_Valid(false)
  , File_Handle(nullptr)
{
  Open();
  if (bIs_Open)
    bIs_Valid = true;
  Close();
}

LogStreamImpl::~LogStreamImpl()
{
  Close();
}

FString LogStreamImpl::GetFilepath()
{
  return Filepath;
}

FString LogStreamImpl::GetFilename()
{
  return Filename;
}

bool LogStreamImpl::GetIsValid()
{
  return bIs_Valid;
}

void LogStreamImpl::Open()
{
  IPlatformFile& platform_file = FPlatformFileManager::Get().GetPlatformFile();
  FString file_path = FPaths::ConvertRelativePathToFull(FPaths::ProjectDir() + Filepath);
  if(bPer_Session)
    file_path += "/" + UniLog.GetSessionIdentifier();
  platform_file.CreateDirectoryTree(*file_path);
  file_path += "/" + Filename;
  File_Handle = platform_file.OpenWrite(*file_path);
  if (!File_Handle)
  {
    UE_LOG(LogTemp, Error, TEXT("Universal Logging: The file %s could not be opened for writing."), *file_path);
    bIs_Open = false;
    bIs_Valid = false;
    return;
  }
  bIs_Open = true;
}

void LogStreamImpl::Close()
{
  delete File_Handle;
  File_Handle = nullptr;
  bIs_Open = false;
}

bool LogStreamImpl::GetIsOpen() const
{
  return bIs_Open;
}

void LogStreamImpl::Write(const FString text)
{
  if (!bIs_Valid)
    return;
  if (!bIs_Open)
    Open();
  File_Handle->Write(reinterpret_cast<const uint8*>(TCHAR_TO_ANSI(*text)), text.Len());
}