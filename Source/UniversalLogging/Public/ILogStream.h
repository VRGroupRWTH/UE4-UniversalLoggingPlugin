#pragma once

class ILogStream
{
public:
  virtual FString GetFilepath() = 0;
  virtual FString GetFilename() = 0;
  virtual bool GetIsValid() = 0;
};