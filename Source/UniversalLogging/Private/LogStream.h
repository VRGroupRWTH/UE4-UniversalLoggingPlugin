#pragma once

class LogStreamImpl : public ILogStream
{
public:
  LogStreamImpl(const FString filepath= "Saved/Logs", const FString filename = "UniversalLogging.log");
  virtual ~LogStreamImpl();

  FString GetFilepath() override;
  FString GetFilename() override;

  bool GetIsValid() override;

  void Open();
  void Close();
  bool GetIsOpen();
  void Write(const FString text);

private:
  const FString _filepath;
  const FString _filename;

  bool _is_open;
  bool _is_valid;

  IFileHandle* _file_handle;
};