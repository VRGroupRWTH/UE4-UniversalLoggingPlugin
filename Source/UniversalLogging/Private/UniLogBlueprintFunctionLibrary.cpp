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
  const FString Filename, bool bPer_Session, bool bOnScreen/* = false*/, FColor OnScreenColor/* = FColor(0, 0, 255, 0)*/)
{
  auto LogStream = UniLog.NewLogStream(StreamName, Filepath, Filename, bPer_Session);
  LogStream->SetOnScreen(bOnScreen);
  LogStream->SetOnScreenColor(OnScreenColor);
}
