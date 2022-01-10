#include "LogFileStream.h"

#include "HAL/PlatformFilemanager.h"
#include "LogStream.h"
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
  File_Handle.Reset(platform_file.OpenWrite(*file_path));
  if (!File_Handle)
  {
    UE_LOG(LogUni, Error, TEXT("Universal Logging: The file %s could not be opened for writing."), *file_path);
    bIs_Open = false;
    return;
  }
  bIs_Open = true;
}

void LogFileStream::Close()
{
  File_Handle.Reset();
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
  const FTCHARToUTF8 StringUTF8(*Text);
  File_Handle->Write(reinterpret_cast<const uint8*>(StringUTF8.Get()), StringUTF8.Length());
}
