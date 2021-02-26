#pragma once

#include "LogFileStream.h"

class LogFileManager
{
public:
  LogFileStream* GetLogFileStream(FString FilePath, FString FileName);

private:
  friend class UniversalLoggingImpl;
  LogFileManager();
  virtual ~LogFileManager();

private:
  TMap<FString, TSharedPtr<LogFileStream>> Streams;
};