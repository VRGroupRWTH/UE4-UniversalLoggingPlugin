#pragma once

#include "LogStream.h"

class UniversalLoggingImpl : public IUniversalLogging
{
public:
  /** IModuleInterface implementation */
  void StartupModule();
  void ShutdownModule();

  ILogStream* NewLogStream(const FString streamname) override;
  ILogStream* NewLogStream(const FString streamname, const FString filepath, const FString filename) override;
  ILogStream* GetLogStream(const FString streamname) override;
  ILogStream* GetDefaultLogStream() override;
  void Log(const FString text, const FString stream = "", bool omit_newline = false) override;

private:
  TMap<FString, TUniquePtr<LogStreamImpl>> _streams;
};