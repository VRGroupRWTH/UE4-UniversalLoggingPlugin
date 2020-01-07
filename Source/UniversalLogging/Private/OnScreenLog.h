// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/Font.h"
#include "GameFramework/Actor.h"
#include "ConstructorHelpers.h"
#include "Queue.h"
#include "OnScreenLog.generated.h"

UCLASS()
class AOnScreenLog final : public AActor
{
  ConstructorHelpers::FObjectFinder<UFont> myFont{ TEXT("Font'/Engine/EngineFonts/DroidSansMono'") };
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AOnScreenLog();

  void EnqueueMessage(const FString Text, const FColor Color);

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
    float TimeToLive;
  };
  TArray<FMessage> Message_Queue;
};
