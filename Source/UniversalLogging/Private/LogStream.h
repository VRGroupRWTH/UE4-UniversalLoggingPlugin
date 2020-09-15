#pragma once
#include "Math/Color.h"

class LogStreamImpl : public ILogStream
{
public:
  LogStreamImpl(const FString Filepath = "Saved/Logs", const FString Filename = "UniversalLogging.log",
                const bool bPer_Session = false, const bool bLogOnMaster = true, const bool bLogOnSlaves = false);
  virtual ~LogStreamImpl();

  FString GetFilepath() override;
  FString GetFilename() override;

  void SetOnScreen(const bool Val) override;
  bool GetOnScreen() const override;
  void SetOnScreenColor(const FColor Color) override;
  FColor GetOnScreenColor() const override;

  bool GetLogOnMaster() const override;
  bool GetLogOnSlaves() const override;

  void SetLogOnScreenOnMaster(const bool Val) override;
  bool GetLogOnScreenOnMaster() const override;
  void SetLogOnScreenOnSlaves(const bool Val) override;
  bool GetLogOnScreenOnSlaves() const override;

  bool GetIsValid() override;

  void Open();
  void Close();
  bool GetIsOpen() const;
  void Write(const FString Text);

private:
  const FString Filepath;
  const FString Filename;
  bool bPer_Session;

  bool bOnScreen;
  FColor OnScreenColor;

  bool bLogOnMaster;
  bool bLogOnSlaves;

  bool bLogOnScreenOnMaster;
  bool bLogOnScreenOnSlaves;

  bool bIs_Open;
  bool bIs_Valid;

  IFileHandle* File_Handle;
};
