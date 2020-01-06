#pragma once

#include "LogStream.h"

#include "Engine/World.h"

class UniversalLoggingImpl : public IUniversalLogging
{
public:
  /** IModuleInterface implementation */
  void StartupModule() override;
  void ShutdownModule() override;

  void OnSessionStart(UWorld*, const UWorld::InitializationValues);
  void OnSessionEnd(UWorld*);

  ILogStream* NewLogStream(const FString StreamName) override;
  ILogStream* NewLogStream(const FString StreamName, const FString Filepath, const FString Filename, bool bPer_Session = false) override;
  ILogStream* GetLogStream(const FString StreamName) override;
  ILogStream* GetDefaultLogStream() override;
  FString GetSessionIdentifier() override;
  void Log(const FString Text, const FString Stream = "", const bool bOmit_Newline = false) override;

  void ResetSessionId(FString Prefix);

private:
  TMap<FString, TUniquePtr<LogStreamImpl>> Streams;
  FString Session_ID;
  TBaseDelegate<void, UWorld*, const UWorld::InitializationValues> On_Post_World_Initialization_Delegate;
  TBaseDelegate<void, UWorld*> On_Pre_World_Finish_Destroy_Delegate;
};