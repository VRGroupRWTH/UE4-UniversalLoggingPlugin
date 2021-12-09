#include "UniversalLogging.h"

#include "LogStream.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/HUD.h"
#if PLATFORM_WINDOWS || PLATFORM_LINUX
#include "IDisplayCluster.h"
#include "Cluster/IDisplayClusterClusterManager.h"
#endif
#include "Editor.h"
#include "Misc/CommandLine.h"

LogFileManager UniversalLoggingImpl::Log_File_Manager{};

void UniversalLoggingImpl::StartupModule()
{
  Streams.Add("", MakeUnique<LogStreamImpl>());

  FWorldDelegates::OnPostWorldInitialization.AddRaw(this, &UniversalLoggingImpl::OnWorldStart);
  FWorldDelegates::OnWorldPostActorTick.AddRaw(this, &UniversalLoggingImpl::OnPostActorTick);

#if WITH_EDITOR
	FEditorDelegates::BeginPIE.AddRaw(this, &UniversalLoggingImpl::OnSessionStart);
	FEditorDelegates::EndPIE.AddRaw(this, &UniversalLoggingImpl::OnSessionEnd);
#endif

	Session_ID = "";
}

void UniversalLoggingImpl::ShutdownModule()
{
}

void UniversalLoggingImpl::OnWorldStart(UWorld* World, const UWorld::InitializationValues)
{
  if (!World->IsGameWorld())
    return;

  On_Screen_Log_Actor = dynamic_cast<AOnScreenLog*>(World->SpawnActor(AOnScreenLog::StaticClass()));

	//only set Session_ID on the first world of this session
	if(Session_ID != "")
		return;

	if (World->IsPlayInEditor())
    ResetSessionId("PlayInEditor");
  else if (World->IsPlayInPreview())
    ResetSessionId("PlayInPreview");
  else
    ResetSessionId("Play");
}

void UniversalLoggingImpl::OnSessionStart(const bool)
{
  Session_ID = "";
}

void UniversalLoggingImpl::OnSessionEnd(const bool)
{
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
  if (HUD)
  {
    HUD->AddPostRenderedActor(On_Screen_Log_Actor); // Doing this every tick seems excessive (AddPostRenderActor checks for duplicates, though)
    HUD->bShowOverlays = true; // Yuck, but necessary as otherwise the Actor's PostRenderFor method is not called.  
  }
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
    UE_LOG(LogUni, Log, TEXT("[%s] %s"), *Stream, *Full_Text);
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
#if PLATFORM_WINDOWS || PLATFORM_LINUX
  if (!IDisplayCluster::IsAvailable()) 
  {
     return true;
  }
  IDisplayClusterClusterManager* Manager = IDisplayCluster::Get().GetClusterMgr();
  if (Manager == nullptr)
  {
    return true; // if we are not in cluster mode, we are always the master
  }
  return Manager->IsMaster() || !Manager->IsSlave();
#else
  return true;
#endif
}

FString UniversalLoggingImpl::GetNodeName()
{
#if PLATFORM_WINDOWS || PLATFORM_LINUX
  if (IDisplayCluster::Get().GetOperationMode() == EDisplayClusterOperationMode::Cluster)
    return IDisplayCluster::Get().GetClusterMgr()->GetNodeId();
  else 
    return FString(TEXT("Localhost"));
#else
	return FString(TEXT("Localhost"));
#endif
}

LogFileManager* UniversalLoggingImpl::GetLogFileManager()
{
  return &Log_File_Manager;
}

IMPLEMENT_MODULE(UniversalLoggingImpl, UniversalLogging)
