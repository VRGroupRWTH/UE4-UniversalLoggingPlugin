#pragma once

class LogStreamImpl : public ILogStream
{
public:
  LogStreamImpl(const FString filepath= "Saved/Logs", const FString filename = "UniversalLogging.log");
  virtual ~LogStreamImpl() = default;

  FString GetFilepath() override;
  FString GetFilename() override;

private:
  const FString _filepath;
  const FString _filename;
};