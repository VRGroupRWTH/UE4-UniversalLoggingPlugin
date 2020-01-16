// Fill out your copyright notice in the Description page of Project Settings.


#include "OnScreenLog.h"
#include "Engine/Canvas.h"

// Sets default values
AOnScreenLog::AOnScreenLog()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AOnScreenLog::EnqueueMessage(const FString Text, const FColor Color, const FColor BackgroundColor, const float Scale, const float Duration)
{
  FMessage Message;
  Message.Text = Text;
  Message.Color = Color;
  Message.BackgroundColor = BackgroundColor;
  Message.Scale = Scale;
  Message.TimeToLive = Duration;
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
  for (int i = Message_Queue.Num(); --i;)
  {
    Message_Queue[i].TimeToLive -= DeltaTime;
    if(Message_Queue[i].TimeToLive < 1.0) // Fade out
    {
      Message_Queue[i].Color.A = Message_Queue[i].TimeToLive * 255;
    }
    if(Message_Queue[i].TimeToLive <= 0.0) // Remove (works because array is traversed backwards)
    {
      Message_Queue.RemoveAt(i);
    }
  }
}

void AOnScreenLog::PostRenderFor(APlayerController* PC, UCanvas* Canvas, FVector CameraPosition, FVector CameraDir)
{
  float YOffset = 50;
  for (auto Message : Message_Queue)
  {
    int Height;
    int Width;
    myFont.Object->GetStringHeightAndWidth(Message.Text, Height, Width);
    Canvas->DrawColor = Message.BackgroundColor;
    FVector2D BoxPosition(10, YOffset);
    FVector2D BoxSize(Width * Message.Scale, Height * Message.Scale);
    FCanvasBoxItem Box(BoxPosition, BoxSize);
    Canvas->DrawItem(Box);
    Canvas->DrawColor = Message.Color;
    YOffset += Canvas->DrawText(myFont.Object, Message.Text, 10, YOffset, Message.Scale, Message.Scale);
  }
}

