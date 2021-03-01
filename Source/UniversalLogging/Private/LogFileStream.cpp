#include "LogFileStream.h"

#include "HAL/PlatformFilemanager.h"

#include "Misc/Paths.h"

LogFileStream::LogFileStream(const FString Filepath, const FString Filename)
  : Filepath(Filepath)
  , Filename(Filename)
  , bIs_Open(false)
  , File_Handle(nullptr)
{
}

LogFileStream::~LogFileStream()
{
  Close();
}

void LogFileStream::Open()
{
  IPlatformFile& platform_file = FPlatformFileManager::Get().GetPlatformFile();
  FString file_path = FPaths::ConvertRelativePathToFull(FPaths::ProjectDir() + Filepath);
  platform_file.CreateDirectoryTree(*file_path);
  file_path = FPaths::Combine(file_path, Filename);
  File_Handle = platform_file.OpenWrite(*file_path);
  if (!File_Handle)
  {
    UE_LOG(LogTemp, Error, TEXT("Universal Logging: The file %s could not be opened for writing."), *file_path);
    bIs_Open = false;
    return;
  }
  bIs_Open = true;
}

void LogFileStream::Close()
{
  delete File_Handle;
  File_Handle = nullptr;
  bIs_Open = false;
}

bool LogFileStream::GetIsOpen() const
{
  return bIs_Open;
}

void LogFileStream::Write(const FString Text)
{
  if (!bIs_Open)
    Open();
  File_Handle->Write(reinterpret_cast<const uint8*>(TCHAR_TO_ANSI(*Text)), Text.Len());
}