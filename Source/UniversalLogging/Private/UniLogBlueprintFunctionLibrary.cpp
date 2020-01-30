// Fill out your copyright notice in the Description page of Project Settings.


#include "UniLogBlueprintFunctionLibrary.h"
#include "IUniversalLogging.h"

//#include "IUniversalLogging.h"

UUniLogBlueprintFunctionLibrary::UUniLogBlueprintFunctionLibrary(const FObjectInitializer& ObjectInitializer)
  : Super(ObjectInitializer) {
}

bool UUniLogBlueprintFunctionLibrary::IsAvailable()
{
  return UniLog.IsAvailable();
}

void UUniLogBlueprintFunctionLibrary::Log(const FString Text, const FString Stream, const bool bOmit_Newline)
{
  UniLog.Log(Text, Stream, bOmit_Newline);
}

FString UUniLogBlueprintFunctionLibrary::GetSessionIdentifier()
{
  return UniLog.GetSessionIdentifier();
}

void UUniLogBlueprintFunctionLibrary::NewLogStream(const FString StreamName, const FString Filepath,
                                                   const FString Filename, bool bPer_Session, bool bOnScreen/* = false*/,
                                                   FColor OnScreenColor/* = FColor(0, 0, 255, 0)*/,
                                                   FColor OnScreenBackgroundColor, float OnScreenSize,
                                                   float OnScreenDuration,
                                                   bool bLogToDefaultLog, bool bLogOnMaster/* = true*/,
                                                   bool bLogOnSlaves/* = false*/,
                                                   bool bLogOnScreenOnMaster/* = true*/,
                                                   bool bLogOnScreenOnSlaves/* = false*/)
{
  auto LogStream = UniLog.NewLogStream(StreamName, Filepath, Filename, bPer_Session, bLogOnMaster, bLogOnSlaves);
  LogStream->SetOnScreen(bOnScreen);
  LogStream->SetOnScreenColor(OnScreenColor);
  LogStream->SetOnScreenBackgroundColor(OnScreenBackgroundColor);
  LogStream->SetOnScreenSize(OnScreenSize);
  LogStream->SetOnScreenDuration(OnScreenDuration);
  LogStream->SetLogToDefaultLog(bLogToDefaultLog);
  LogStream->SetLogOnScreenOnMaster(bLogOnScreenOnMaster);
  LogStream->SetLogOnScreenOnSlaves(bLogOnScreenOnSlaves);
}

void UUniLogBlueprintFunctionLibrary::ModifyLogStream(const FString StreamName, bool bOnScreen, FColor OnScreenColor,
                                                      FColor OnScreenBackgroundColor,
                                                      float OnScreenSize, float OnScreenDuration, bool bLogToDefaultLog, bool bLogOnScreenOnMaster/* = true*/,
                                                      bool bLogOnScreenOnSlaves/* = false*/)
{
  auto LogStream = UniLog.GetLogStream(StreamName);
  LogStream->SetOnScreen(bOnScreen);
  LogStream->SetOnScreenColor(OnScreenColor);
  LogStream->SetOnScreenBackgroundColor(OnScreenBackgroundColor);
  LogStream->SetOnScreenSize(OnScreenSize);
  LogStream->SetOnScreenDuration(OnScreenDuration);
  LogStream->SetLogToDefaultLog(bLogToDefaultLog);
  LogStream->SetLogOnScreenOnMaster(bLogOnScreenOnMaster);
  LogStream->SetLogOnScreenOnSlaves(bLogOnScreenOnSlaves);
}
