#pragma once

#include "LogStream.h"

#include "Engine/World.h"

class UniversalLoggingImpl : public IUniversalLogging
{
public:
  /** IModuleInterface implementation */
  void StartupModule();
  void ShutdownModule();

  void OnSessionStart(UWorld*, const UWorld::InitializationValues);
  void OnSessionEnd(UWorld*);

  ILogStream* NewLogStream(const FString streamname) override;
  ILogStream* NewLogStream(const FString streamname, const FString filepath, const FString filename) override;
  ILogStream* GetLogStream(const FString streamname) override;
  ILogStream* GetDefaultLogStream() override;
  void Log(const FString text, const FString stream = "", bool omit_newline = false) override;

private:
  TMap<FString, TUniquePtr<LogStreamImpl>> _streams;
  TBaseDelegate<void, UWorld*, const UWorld::InitializationValues> on_post_world_initialization_delegate_;
  TBaseDelegate<void, UWorld*> on_pre_world_finish_destroy_delegate_;
};