#pragma once

#include "ModuleManager.h"

#include "ILogStream.h"

// #include "fmt/printf.h"

class IUniversalLogging : public IModuleInterface
{
public:
  /**
  * Singleton-like access to this module's interface.  This is just for convenience!
  * Beware of calling this during the shutdown phase, though.  Your module might have been unloaded already.
  *
  * @return Returns singleton instance, loading the module on demand if needed.
  */
  static inline IUniversalLogging& Get()
  {
    return FModuleManager::LoadModuleChecked< IUniversalLogging >("UniversalLogging");
  }

  /**
  * Checks to see if this module is loaded and ready.  It is only valid to call Get() if IsAvailable() returns true.
  *
  * @return True if the module is loaded and ready to use.
  */
  static inline bool IsAvailable()
  {
    return FModuleManager::Get().IsModuleLoaded("UniversalLogging");
  }

  /**
  * Creates a new log stream with the name UniversalLogging_<streamname>.log in <projectdir>/Saved/Logs.
  *
  * @return the newly created log stream object.
  */
  virtual ILogStream* NewLogStream(const FString streamname) = 0;

  /**
  * Creates a new log stream with the name <filename> in <projectdir>/<filepath>.
  *
  * @return the newly created log stream object.
  */
  virtual ILogStream* NewLogStream(const FString streamname, const FString filepath, const FString filename) = 0;

  /**
  * Getter for log streams.
  *
  * @return the requested log stream or nullptr if it does not exist.
  */
  virtual ILogStream* GetLogStream(const FString streamname) = 0;

  /**
  * Getter for the default log streams (convenience). Equivalent to GetLogStream("").
  *
  * @return the default log stream.
  */
  virtual ILogStream* GetDefaultLogStream() = 0;

  /**
  * Logs the given text to the given log stream. If no log stream is specified, the default is used.
  */
  virtual void Log(const FString text, const FString stream = "", bool omit_newline = false) = 0;

  /**
  * Advanced logging with printf like syntax.
  * Note: Does not automatically add newline!
  */
  template<typename... TArgs>
  void LogF(FString stream, FString format, TArgs&&... args)
  {
    FString out = FString::Printf(*format, args...);
    Log(out, stream, true);
  }
};

#define UniLog IUniversalLogging::Get()