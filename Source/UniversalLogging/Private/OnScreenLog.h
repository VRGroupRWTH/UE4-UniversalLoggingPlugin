// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/Font.h"
#include "GameFramework/Actor.h"
#include "UObject/ConstructorHelpers.h"
#include "Containers/Queue.h"
#include "OnScreenLog.generated.h"

UCLASS()
class AOnScreenLog final : public AActor
{
  ConstructorHelpers::FObjectFinder<UFont> myFont{ TEXT("Font'/Engine/EngineFonts/DroidSansMono'") };
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AOnScreenLog();

  void EnqueueMessage(const FString Text, const FColor Color, const FColor BackgroundColor, const float Scale, const float Duration);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
  virtual void PostRenderFor(class APlayerController* PC, class UCanvas* Canvas, FVector CameraPosition, FVector CameraDir) override;

private:
  struct FMessage
  {
    FString Text;
    FColor Color;
    FColor BackgroundColor;
    float AlphaFactor;
    float Scale;
    float TimeToLive;
  };
  TArray<FMessage> Message_Queue;
};
