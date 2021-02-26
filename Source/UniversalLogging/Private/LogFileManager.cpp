#include "LogFileManager.h"

LogFileStream* LogFileManager::GetLogFileStream(FString FilePath, FString FileName)
{
  FString Full_Path = FPaths::Combine(FilePath, FileName);
  if (Streams.Contains(Full_Path))
    return Streams[Full_Path].Get();
  else
    Streams.Add(Full_Path, MakeUnique<LogFileStream>(FilePath, FileName));
  return Streams[Full_Path].Get();
}

LogFileManager::LogFileManager()
{
}

LogFileManager::~LogFileManager()
{
}
