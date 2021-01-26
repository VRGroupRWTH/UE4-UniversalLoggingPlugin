#pragma once

#include "LogStream.h"
#include "OnScreenLog.h"
#include "IUniversalLogging.h"

#include "Engine/World.h"

class UniversalLoggingImpl : public IUniversalLogging
{
public:
  /** IModuleInterface implementation */
  void StartupModule() override;
  void ShutdownModule() override;

  void OnSessionStart(UWorld*, const UWorld::InitializationValues);
  void OnSessionEnd(UWorld*);

  void OnPostActorTick(UWorld*, ELevelTick, float);

  ILogStream* NewLogStream(const FString StreamName) override;
  ILogStream* NewLogStream(const FString StreamName, const FString Filepath, const FString Filename,
                           bool bPer_Session = false, const bool bLogOnMaster = true,
                           const bool bLogOnSlaves = false) override;
  ILogStream* GetLogStream(const FString StreamName) override;
  ILogStream* GetDefaultLogStream() override;
  FString GetSessionIdentifier() override;
  void Log(const FString Text, const FString Stream = "", const bool bOmit_Newline = false) override;

  void ResetSessionId(FString Prefix);
  // Helper functions for nDisplay
  static bool IsClusterMaster(); 
  static FString GetNodeName();

private:
  TMap<FString, TUniquePtr<LogStreamImpl>> Streams;
  FString Session_ID;
  TDelegate<void(UWorld*, const UWorld::InitializationValues)> On_Post_World_Initialization_Delegate;
  TDelegate<void(UWorld*)> On_Pre_World_Finish_Destroy_Delegate;
  TDelegate<void(UWorld*, ELevelTick, float)> On_World_Post_Actor_Tick_Delegate;
  AOnScreenLog* On_Screen_Log_Actor;
};