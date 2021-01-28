// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "UniLogBlueprintFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class UUniLogBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
  GENERATED_UCLASS_BODY()

  UFUNCTION(BlueprintCallable, Category = "UniLog")
    static bool IsAvailable();

  UFUNCTION(BlueprintCallable, Category = "UniLog")
    static void Log(const FString Text, const FString Stream = "", const bool bOmit_Newline = false);

  UFUNCTION(BlueprintCallable, Category = "UniLog")
    static FString GetSessionIdentifier();

  UFUNCTION(BlueprintCallable, Category = "UniLog")
    static void NewLogStream(const FString StreamName, const FString Filepath, const FString Filename, FString Prefix,
                             bool bPer_Session = false, bool bOnScreen = false,
                             FColor OnScreenColor = FColor(255, 255, 255, 255),
                             FColor OnScreenBackgroundColor = FColor(0, 0, 0, 128),
                             float OnScreenSize = 1.0, float OnScreenDuration = 5.0, bool bLogToDefaultLog = false, bool bLogOnMaster = true,
                             bool bLogOnSlaves = false, bool bLogOnScreenOnMaster = true,
                             bool bLogOnScreenOnSlaves = false);

  UFUNCTION(BlueprintCallable, Category = "UniLog")
    static void ModifyLogStream(const FString StreamName, bool bOnScreen = false,
                                FColor OnScreenColor = FColor(255, 255, 255, 255),
                                FColor OnScreenBackgroundColor = FColor(0, 0, 0, 128), float OnScreenSize = 1.0,
                                float OnScreenDuration = 5.0, bool bLogToDefaultLog = false, bool bLogOnScreenOnMaster = true,
                                bool bLogOnScreenOnSlaves = false);
};
