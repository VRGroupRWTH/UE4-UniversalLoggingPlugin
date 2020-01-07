// Fill out your copyright notice in the Description page of Project Settings.


#include "OnScreenLog.h"
#include "Engine/Canvas.h"

// Sets default values
AOnScreenLog::AOnScreenLog()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AOnScreenLog::EnqueueMessage(const FString Text, const FColor Color)
{
  FMessage Message;
  Message.Text = Text;
  Message.Color = Color;
  Message.TimeToLive = 5.0;
  Message_Queue.Insert(Message, 0);
}

// Called when the game starts or when spawned
void AOnScreenLog::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AOnScreenLog::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
  for (int i = 0; i < Message_Queue.Num(); i++)
  {
    Message_Queue[i].TimeToLive -= DeltaTime;
    if(Message_Queue[i].TimeToLive < 1.0) // Fade out
    {
      Message_Queue[i].Color.A = Message_Queue[i].TimeToLive * 255;
    }
  }
  FMessage LastMessage;
  while (Message_Queue.Num() > 0 && Message_Queue.Top().TimeToLive <= 0) 
  {
    Message_Queue.Pop();
  }
}

void AOnScreenLog::PostRenderFor(APlayerController* PC, UCanvas* Canvas, FVector CameraPosition, FVector CameraDir)
{
  float YOffset = 50;
  for (auto Message : Message_Queue)
  {
    Canvas->DrawColor = Message.Color;
    YOffset += Canvas->DrawText(myFont.Object, Message.Text, 10, YOffset);
  }
}

