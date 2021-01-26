#include "LogStream.h"
#include "UniversalLogging.h"

#include "Misc/Paths.h"

LogStreamImpl::LogStreamImpl(const FString Filepath, const FString Filename, const bool bPer_Session, const bool bLogOnMaster, const bool bLogOnSlaves)
  : Filepath(Filepath)
  , Filename(Filename)
  , bPer_Session(bPer_Session)
  , bOnScreen(false)
  , OnScreenColor(255, 255, 255, 255)
  , OnScreenBackgroundColor(0, 0, 0, 128)
  , OnScreenSize(1.0)
  , OnScreenDuration(5.0)
  , bLogToDefaultLog(false)
  , bLogOnMaster(bLogOnMaster)
  , bLogOnSlaves(bLogOnSlaves)
  , bLogOnScreenOnMaster(true)
  , bLogOnScreenOnSlaves(false)
  , bIs_Valid(false)
  , Log_File_Stream(nullptr)
{
  if ((bLogOnMaster && UniversalLoggingImpl::IsClusterMaster()) || (bLogOnSlaves && !UniversalLoggingImpl::IsClusterMaster()))
  {
    Open();
    if (GetIsOpen())
      bIs_Valid = true;
    Close();
  }
  else
  {
    bIs_Valid = true;
  }
}

LogStreamImpl::~LogStreamImpl()
{
  Close();
}

FString LogStreamImpl::GetFilepath()
{
  return Filepath;
}

FString LogStreamImpl::GetFilename()
{
  return Filename;
}

void LogStreamImpl::SetPrefix(FString Prefix)
{
  MessagePrefix = Prefix;
}

FString LogStreamImpl::GetPrefix() const
{
  return MessagePrefix;
}

void LogStreamImpl::SetOnScreen(const bool Val)
{
  bOnScreen = Val;
}

bool LogStreamImpl::GetOnScreen() const
{
  return bOnScreen;
}

void LogStreamImpl::SetOnScreenColor(const FColor Color)
{
  OnScreenColor = Color;
}

FColor LogStreamImpl::GetOnScreenColor() const
{
  return OnScreenColor;
}

void LogStreamImpl::SetOnScreenBackgroundColor(const FColor Color)
{
  OnScreenBackgroundColor = Color;
}

FColor LogStreamImpl::GetOnScreenBackgroundColor() const
{
  return OnScreenBackgroundColor;
}

void LogStreamImpl::SetOnScreenSize(const float Scale)
{
  OnScreenSize = Scale;
}

float LogStreamImpl::GetOnScreenSize() const
{
  return OnScreenSize;
}

void LogStreamImpl::SetOnScreenDuration(const float Seconds)
{
  OnScreenDuration = Seconds;
}

float LogStreamImpl::GetOnScreenDuration() const
{
  return OnScreenDuration;
}

void LogStreamImpl::SetLogToDefaultLog(const bool Val)
{
  bLogToDefaultLog = Val;
}

bool LogStreamImpl::GetLogToDefaultLog() const
{
  return bLogToDefaultLog;
}

bool LogStreamImpl::GetLogOnMaster() const
{
  return bLogOnMaster;
}

bool LogStreamImpl::GetLogOnSlaves() const
{
  return bLogOnSlaves;
}

void LogStreamImpl::SetLogOnScreenOnMaster(const bool Val)
{
  if(Val) // to avoid confusion, this also enables logging
    SetOnScreen(true); 
  bLogOnScreenOnMaster = Val;
}

bool LogStreamImpl::GetLogOnScreenOnMaster() const
{
  return bLogOnScreenOnMaster;
}

void LogStreamImpl::SetLogOnScreenOnSlaves(const bool Val)
{
  if (Val) // to avoid confusion, this also enables logging
    SetOnScreen(true);
  bLogOnScreenOnSlaves = Val;
}

bool LogStreamImpl::GetLogOnScreenOnSlaves() const
{
  return bLogOnScreenOnSlaves;
}

bool LogStreamImpl::GetIsValid()
{
  return bIs_Valid;
}

void LogStreamImpl::Open()
{
  FString File_Path = Filepath;
  if(bPer_Session)
    File_Path = FPaths::Combine(File_Path, UniLog.GetSessionIdentifier());
  Log_File_Stream = UniversalLoggingImpl::GetLogFileManager()->GetLogFileStream(File_Path, Filename); // this will not stay like this, as LogFileStreams will be managed by LogFileManager
  Log_File_Stream->Open();
  if (!Log_File_Stream->GetIsOpen())
  {
    bIs_Valid = false;
    return;
  }
}

void LogStreamImpl::Close()
{
  Log_File_Stream->Close();
}

bool LogStreamImpl::GetIsOpen() const
{
  return Log_File_Stream->GetIsOpen();
}

void LogStreamImpl::Write(const FString Text)
{
  if (!bIs_Valid)
    return;
  if (!Log_File_Stream->GetIsOpen())
    Open();
  Log_File_Stream->Write(MessagePrefix + Text);
}