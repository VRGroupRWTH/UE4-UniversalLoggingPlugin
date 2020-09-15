#pragma once

#include "IDisplayCluster.h"
#include "Modules/ModuleManager.h"

#include "ILogStream.h"

class IUniversalLogging : public IModuleInterface
{
public:
  /**
  * Singleton-like access to this module's interface. This should not be necessary as the intended use is through the UniLog macro
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
  * Creates a new log stream with the name UniversalLogging_<StreamName>.log in <projectdir>/Saved/Logs.
  *
  * @return the newly created log stream object.
  */
  virtual ILogStream* NewLogStream(const FString StreamName) = 0;

  /**
  * Creates a new log stream with the name <filename> in <projectdir>/<filepath>.
  *
  * @return the newly created log stream object.
  */
  virtual ILogStream* NewLogStream(const FString StreamName, const FString Filepath, const FString Filename,
                                   bool bPer_Session = false, const bool bLogOnMaster = true,
                                   const bool bLogOnSlaves = false) = 0;

  /**
  * Getter for log streams.
  *
  * @return the requested log stream or nullptr if it does not exist.
  */
  virtual ILogStream* GetLogStream(const FString StreamName) = 0;

  /**
  * Getter for the default log streams (convenience). Equivalent to GetLogStream("").
  *
  * @return the default log stream.
  */
  virtual ILogStream* GetDefaultLogStream() = 0;

  /**
  * Get the current session identifier. It uniquely identifies each time the application starts.
  */
  virtual FString GetSessionIdentifier() = 0;

  /**
  * Logs the given text to the given log stream. If no log stream is specified, the default is used.
  */
  virtual void Log(const FString Text, const FString Stream = "", const bool bOmit_Newline = false) = 0;

  /**
  * Advanced logging with printf like syntax.
  * Note: Does not automatically add newline!
  */
  template<typename... TArgs>
  void LogF(const FString Stream, const FString Format, TArgs&&... Args)
  {
    const FString Out = FString::Printf(*Format, Args...);
    Log(Out, Stream, true);
  }
};

#define UniLog IUniversalLogging::Get()
