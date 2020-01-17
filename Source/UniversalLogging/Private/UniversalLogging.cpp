#include "UniversalLogging.h"

#include "LogStream.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/HUD.h"
#include "IDisplayCluster.h"
#include "Cluster/IDisplayClusterClusterManager.h"
#include "Misc/CommandLine.h"

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
  return NewLogStream(StreamName, "Saved/Logs", "UniversalLogging_" + StreamName + ".log");
}

ILogStream* UniversalLoggingImpl::NewLogStream(const FString StreamName, const FString Filepath, const FString Filename,
                                               bool bPer_Session /* = false*/, bool bLogOnMaster/* = true*/,
                                               const bool bLogOnSlaves/* = false*/)
{
  FString NewFilepath = Filepath;
  FString NewFilename = Filename;
  if(bLogOnSlaves && !IsClusterMaster())
  {
    NewFilepath = Filepath + "/slavelogs";
    NewFilename = GetNodeName() + "_" + Filename;
  }
  Streams.Add(StreamName, MakeUnique<LogStreamImpl>(NewFilepath, NewFilename, bPer_Session, bLogOnMaster, bLogOnSlaves));
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
  if (!Streams.Contains(Stream))
    NewLogStream(Stream);
  LogStreamImpl* Stream_OBJ = Streams[Stream].Get();

  FString Full_Text = Text;
  if (!bOmit_Newline)
    Full_Text += "\n";

  if ((Stream_OBJ->GetLogOnMaster() && IsClusterMaster()) || (Stream_OBJ->GetLogOnSlaves() && !IsClusterMaster()))
  {
    Stream_OBJ->Write(Full_Text);
  }

  if(Stream_OBJ->GetOnScreen() && On_Screen_Log_Actor)
  {
    if ((Stream_OBJ->GetLogOnScreenOnMaster() && IsClusterMaster()) || (Stream_OBJ->GetLogOnScreenOnSlaves() && !IsClusterMaster()))
    On_Screen_Log_Actor->EnqueueMessage(Full_Text, Stream_OBJ->GetOnScreenColor(),
                                        Stream_OBJ->GetOnScreenBackgroundColor(), Stream_OBJ->GetOnScreenSize(),
                                        Stream_OBJ->GetOnScreenDuration());
  }

  if(Stream_OBJ->GetLogToDefaultLog())
  {
    UE_LOG(LogTemp, Log, TEXT("[Universal Logging][%s] %s"), *Stream, *Full_Text);
  }
}

void UniversalLoggingImpl::ResetSessionId(FString Prefix)
{
  TArray<FString> Tokens;
  TArray<FString> Switches;
  FCommandLine::Parse(FCommandLine::Get(), Tokens, Switches);
  for (auto Token : Tokens)
  {
    if (Token.StartsWith("SessionID"))
    {
      Token.RemoveFromStart("SessionID=");
      Session_ID = Token;
      return;
    }
  }

  FString timestamp = FGenericPlatformTime::StrTimestamp();
  timestamp = timestamp.Replace(TEXT("/"), TEXT("-"));
  timestamp = timestamp.Replace(TEXT(" "), TEXT("_"));
  timestamp = timestamp.Replace(TEXT(":"), TEXT("-"));
  Session_ID = FString::Printf(TEXT("%s_%s"), *Prefix, *timestamp);
}

bool UniversalLoggingImpl::IsClusterMaster()
{
  IDisplayClusterClusterManager* Manager = IDisplayCluster::Get().GetClusterMgr();
  if (Manager == nullptr)
  {
    return true; // if we are not in cluster mode, we are always the master
  }
  return Manager->IsMaster();
}

FString UniversalLoggingImpl::GetNodeName()
{
  if (IDisplayCluster::Get().GetOperationMode() == EDisplayClusterOperationMode::Cluster)
    return IDisplayCluster::Get().GetClusterMgr()->GetNodeId();
  else 
    return FString(TEXT("Localhost"));
}

IMPLEMENT_MODULE(UniversalLoggingImpl, UniversalLogging)
