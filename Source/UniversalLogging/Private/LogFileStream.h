#pragma once

class LogFileStream
{
public:
  LogFileStream(const FString Filepath, const FString Filename);
  virtual ~LogFileStream();

  void Open();
  void Close();
  bool GetIsOpen() const;
  void Write(const FString Text);

private:
  const FString Filepath;
  const FString Filename;
  bool bIs_Open;
  IFileHandle* File_Handle;
};