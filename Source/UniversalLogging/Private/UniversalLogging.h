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
  ILogStream* NewLogStream(const FString streamname, const FString filepath, const FString filename, bool per_session = false) override;
  ILogStream* GetLogStream(const FString streamname) override;
  ILogStream* GetDefaultLogStream() override;
  FString GetSessionIdentifier() override;
  void Log(const FString text, const FString stream = "", bool omit_newline = false) override;

  void ResetSessionId(FString prefix);

private:
  TMap<FString, TUniquePtr<LogStreamImpl>> _streams;
  FString _session_id;
  TBaseDelegate<void, UWorld*, const UWorld::InitializationValues> on_post_world_initialization_delegate_;
  TBaseDelegate<void, UWorld*> on_pre_world_finish_destroy_delegate_;
};