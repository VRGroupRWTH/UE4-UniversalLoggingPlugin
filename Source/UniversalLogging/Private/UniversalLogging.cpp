#include "UniversalLoggingPrivatePCH.h"

#include "UniversalLogging.h"

#include "LogStream.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/HUD.h"
#include "GameFramework/GameModeBase.h"

void UniversalLoggingImpl::StartupModule()
{
  Streams.Add("", MakeUnique<LogStreamImpl>());

  On_Post_World_Initialization_Delegate.BindRaw(this, &UniversalLoggingImpl::OnSessionStart);
  FWorldDelegates::OnPostWorldInitialization.Add(On_Post_World_Initialization_Delegate);

  On_Pre_World_Finish_Destroy_Delegate.BindRaw(this, &UniversalLoggingImpl::OnSessionEnd);
  FWorldDelegates::OnPreWorldFinishDestroy.Add(On_Pre_World_Finish_Destroy_Delegate);

  On_World_Post_Actor_Tick_Delegate.BindRaw(this, &UniversalLoggingImpl::OnPostActorTick);
  FWorldDelegates::OnWorldPostActorTick.Add(On_World_Post_Actor_Tick_Delegate);

  Session_ID = "";
}

void UniversalLoggingImpl::ShutdownModule()
{
}

void UniversalLoggingImpl::OnSessionStart(UWorld* World, const UWorld::InitializationValues)
{
  if (!World->IsGameWorld())
    return;
  if (World->IsPlayInEditor())
    ResetSessionId("PlayInEditor");
  else if (World->IsPlayInPreview())
    ResetSessionId("PlayInPreview");
  else
    ResetSessionId("Play");

  On_Screen_Log_Actor = dynamic_cast<AOnScreenLog*>(World->SpawnActor(AOnScreenLog::StaticClass()));
}

void UniversalLoggingImpl::OnSessionEnd(UWorld* World)
{
  if (!World->IsGameWorld())
    return;
  ResetSessionId("Stopped");

  for (auto& Elem : Streams)
  {
    Elem.Value->Close();
  }
}

void UniversalLoggingImpl::OnPostActorTick(UWorld* World, ELevelTick LevelTick, float WhateverThisIsDeltaTimeMaybe)
{
  const auto PlayerController = World->GetFirstPlayerController();
  if (!PlayerController) 
    return;
  auto HUD = PlayerController->GetHUD();
  HUD->AddPostRenderedActor(On_Screen_Log_Actor); // Doing this every tick seems excessive (AddPostRenderActor checks for duplicates, though)
  HUD->bShowOverlays = true; // Yuck, but necessary as otherwise the Actor's PostRenderFor method is not called.
}

ILogStream* UniversalLoggingImpl::NewLogStream(const FString StreamName)
{
  Streams.Add(StreamName, MakeUnique<LogStreamImpl>("Saved/Logs", "UniversalLogging_" + StreamName + ".log"));
  return Streams[StreamName].Get();
}

ILogStream* UniversalLoggingImpl::NewLogStream(const FString StreamName, const FString Filepath, const FString Filename, bool bPer_Session /* = false*/)
{
  Streams.Add(StreamName, MakeUnique<LogStreamImpl>(Filepath, Filename, bPer_Session));
  return Streams[StreamName].Get();
}

ILogStream * UniversalLoggingImpl::GetLogStream(const FString StreamName)
{
  if (Streams.Contains(StreamName))
    return Streams[StreamName].Get();
  else 
    return nullptr;
}

ILogStream * UniversalLoggingImpl::GetDefaultLogStream()
{
  return GetLogStream("");
}

FString UniversalLoggingImpl::GetSessionIdentifier()
{
  return Session_ID;
}

void UniversalLoggingImpl::Log(const FString Text, const FString Stream /*= ""*/, const bool bOmit_Newline /*= false*/)
{
  LogStreamImpl* Stream_OBJ = nullptr;
  if (!Streams.Contains(Stream))
    NewLogStream(Stream);
  Stream_OBJ = Streams[Stream].Get();
  FString Full_Text = Text;
  if (!bOmit_Newline)
    Full_Text += "\n";
  Stream_OBJ->Write(Full_Text);

  if(Stream_OBJ->GetOnScreen() && On_Screen_Log_Actor)
  {
    On_Screen_Log_Actor->EnqueueMessage(Full_Text, Stream_OBJ->GetOnScreenColor());
  }
}

void UniversalLoggingImpl::ResetSessionId(FString Prefix)
{
  FString timestamp = FGenericPlatformTime::StrTimestamp();
  timestamp = timestamp.Replace(TEXT("/"), TEXT("-"));
  timestamp = timestamp.Replace(TEXT(" "), TEXT("_"));
  timestamp = timestamp.Replace(TEXT(":"), TEXT("-"));
  Session_ID = FString::Printf(TEXT("%s_%s"), *Prefix, *timestamp);
}

IMPLEMENT_MODULE(UniversalLoggingImpl, UniversalLogging)