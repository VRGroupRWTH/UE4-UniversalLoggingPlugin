#pragma once

#include "LogStream.h"
#include "OnScreenLog.h"
#include "IUniversalLogging.h"
#include "LogFileManager.h"

#include "Engine/World.h"

class UniversalLoggingImpl : public IUniversalLogging
{
public:
  /** IModuleInterface implementation */
  void StartupModule() override;
  void ShutdownModule() override;

  void OnWorldStart(UWorld*, const UWorld::InitializationValues);
	void OnSessionStart(const bool);
  void OnSessionEnd(const bool);

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

  static LogFileManager* GetLogFileManager();

private:
  static LogFileManager Log_File_Manager;
  TMap<FString, TUniquePtr<LogStreamImpl>> Streams;
  FString Session_ID;
  AOnScreenLog* On_Screen_Log_Actor;
};