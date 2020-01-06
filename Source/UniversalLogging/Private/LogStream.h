#pragma once

class LogStreamImpl : public ILogStream
{
public:
  LogStreamImpl(const FString Filepath= "Saved/Logs", const FString Filename = "UniversalLogging.log", const bool bPer_Session = false);
  virtual ~LogStreamImpl();

  FString GetFilepath() override;
  FString GetFilename() override;

  bool GetIsValid() override;

  void Open();
  void Close();
  bool GetIsOpen() const;
  void Write(const FString text);

private:
  const FString Filepath;
  const FString Filename;
  bool bPer_Session;

  bool bIs_Open;
  bool bIs_Valid;

  IFileHandle* File_Handle;
};