#pragma once

class ILogStream
{
public:
  virtual FString GetFilepath() = 0;
  virtual FString GetFilename() = 0;
  virtual bool GetIsValid() = 0;

  virtual void SetOnScreen(const bool Val) = 0;
  virtual bool GetOnScreen() const = 0;
  virtual void SetOnScreenColor(const FColor Color) = 0;
  virtual FColor GetOnScreenColor() const = 0;

  virtual bool GetLogOnMaster() const = 0;
  virtual bool GetLogOnSlaves() const = 0;

  virtual void SetLogOnScreenOnMaster(const bool Val) = 0;
  virtual bool GetLogOnScreenOnMaster() const = 0;
  virtual void SetLogOnScreenOnSlaves(const bool Val) = 0;
  virtual bool GetLogOnScreenOnSlaves() const = 0;
};