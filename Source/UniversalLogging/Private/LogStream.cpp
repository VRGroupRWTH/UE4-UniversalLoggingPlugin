#include "UniversalLoggingPrivatePCH.h"

#include "LogStream.h"
#include "UniversalLogging.h"

#include "HAL/PlatformFilemanager.h"
#include "Paths.h"
#include "IPlatformFileProfilerWrapper.h"

LogStreamImpl::LogStreamImpl(const FString Filepath, const FString Filename, const bool bPer_Session, const bool bLogOnMaster, const bool bLogOnSlaves)
  : Filepath(Filepath)
  , Filename(Filename)
  , bPer_Session(bPer_Session)
  , bOnScreen(false)
  , OnScreenColor(0, 0, 255, 255)
  , bLogOnMaster(bLogOnMaster)
  , bLogOnSlaves(bLogOnSlaves)
  , bLogOnScreenOnMaster(true)
  , bLogOnScreenOnSlaves(false)
  , bIs_Open(false)
  , bIs_Valid(false)
  , File_Handle(nullptr)
{
  if ((bLogOnMaster && UniversalLoggingImpl::IsClusterMaster()) || (bLogOnSlaves && !UniversalLoggingImpl::IsClusterMaster()))
  {
    Open();
    if (bIs_Open)
      bIs_Valid = true;
    Close();
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
  bLogOnScreenOnMaster = Val;
}

bool LogStreamImpl::GetLogOnScreenOnMaster() const
{
  return bLogOnScreenOnMaster;
}

void LogStreamImpl::SetLogOnScreenOnSlaves(const bool Val)
{
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
  IPlatformFile& platform_file = FPlatformFileManager::Get().GetPlatformFile();
  FString file_path = FPaths::ConvertRelativePathToFull(FPaths::ProjectDir() + Filepath);
  if(bPer_Session)
    file_path += "/" + UniLog.GetSessionIdentifier();
  platform_file.CreateDirectoryTree(*file_path);
  file_path += "/" + Filename;
  File_Handle = platform_file.OpenWrite(*file_path);
  if (!File_Handle)
  {
    UE_LOG(LogTemp, Error, TEXT("Universal Logging: The file %s could not be opened for writing."), *file_path);
    bIs_Open = false;
    bIs_Valid = false;
    return;
  }
  bIs_Open = true;
}

void LogStreamImpl::Close()
{
  delete File_Handle;
  File_Handle = nullptr;
  bIs_Open = false;
}

bool LogStreamImpl::GetIsOpen() const
{
  return bIs_Open;
}

void LogStreamImpl::Write(const FString Text)
{
  if (!bIs_Valid)
    return;
  if (!bIs_Open)
    Open();
  File_Handle->Write(reinterpret_cast<const uint8*>(TCHAR_TO_ANSI(*Text)), Text.Len());
}