# Unreal Universal Logging Plugin

This plugin was developed to have a unified way for logging that can be used in shipping builds.

## Usage

This should be all the documentation you need to use the Universal Logging library

### Easy Mode

In the simplest case, you can just do:

```cpp
#include "IUniversalLogging.h"

...

UniLog.Log("Message");
```

This will log `Message` to `<your project dir>/Saved/Logs/UniversalLogging.log`.

There are other levels of control, though:

```cpp
UniLog.Log("Message", "Name");

UniLog.Log("Message", "Name", true);
```

This will log `Message` to `<your project dir>/Saved/Logs/UniversalLogging_Name.log` and in the latter case will omit the newline which would otherwise be automatically added.

### Use Logging in Blueprints

UniversalLogging is also available from blueprints. Look for the `Log`, `NewLogStream` and `ModifyLogStream` nodes.

### Formatting Strings

The `LogF` function allows you to log using a `printf`-like syntax. The only thing to keep in mind is that `FString` variables have to be passed as pointers.

```cpp
FString variable = "Hello!";
UniLog.LogF("Name", "Message %s", *variable);
```

Note that the `LogF` function does not automatically add a newline at the end.

### Using LogStreams

You might not want to have more control over your logs and, e.g., set where they are saved. Settings are provided through `LogStream` objects. Create them like this:

```cpp
ILogStream* stream = UniLog.NewLogStream(StreamName, PathToFile, Filename, PerSession, LogOnMaster, LogOnSlaves);
```

Parameters:
 * StreamName (FString): The name of the newly created LogStream which will be used as a handle to retrieve it in the future.
 * PathToFile (FString): The path where you want your log(s) to be saved, relative to the project path. (Empty string for project root)
 * Filename (FString): The name of the log file. You have to specify a file extension.
 * PerSession (bool): If false (default) the log is overwritten each time the application is started. If true, a new folder is created at every launch, so the file is saved in `<Project Path>/<PathToFile>/<SessionID>/<Filename>`.
 * LogOnMaster (bool): For cluster mode (e.g. for the aixCAVE), specify if the master node writes the log (default) or not.
 * LogOnSlaves (bool): For cluster mode (e.g. for the aixCAVE), specify if the slave nodes write the log or not (default). The log file will be located in `<Project Path>/<PathToFile>/slavelogs/<Slave ID>_<Filename>`.

The path provided is still relative to your project directory. As soon as you created the `LogStream`, the complete filepath and the file are attempted to be created. You can check if it was successful using:

```cpp
ILogStream* stream = UniLog.GetLogStream("StreamName");
if (stream->GetIsValid()) {...}
```

After your stream was created, you can log to it like before using `"StreamName"` to identify it:

```cpp
UniLog.Log("Message", "StreamName");
UniLog.LogF("StreamName", "Message %s", *variable);
```

### On Screen Logging

You can also display the log messages on screen. This can even be enabled/disabled during runtime. To do this, use the ILogStream interface:

```cpp
ILogStream* stream = UniLog.GetLogStream("StreamName");
stream->SetOnScreen(true);
```

You can also change the color of the messages, to distinguish multiple streams that are logging to the screen at the same time:

```cpp
stream->SetOnScreenColor(FColor(255, 0, 255, 255)); // FColor parameters are int8 RGBA
```

By default, the on screen messages are only shown on the master in cluster mode. You can change this behavior using:

```cpp
stream->SetLogOnScreenOnMaster(false);
stream->SetLogOnScreenOnSlaves(true);
```

### Per Session Logging

You might want to have log files that don't get overwritten each time your application starts. You can specify this when creating the `LogStream`:

```cpp
UniLog.NewLogStream("StreamName", "path/to/file", "filename.log", true);
```

This will put a session identifier folder between the path and the filename. A session starts with every run of the application (like clicking "Play" in the editor). 

### Session Identifiers

The SessionID is automatically generated at the start of each session. You may access it using:

```cpp
FString SessionID = UniLog.GetSessionIdentifier();
```

Usually, it is of the form: `<Play|PlayInPreview|PlayInEditor|Stopped>_MM-DD-YY_hh-mm-ss`. However, it can be changed using the command line argument `-SessionID=<Custom ID>`. If this argument is present, it is always used. This is used for the aixCAVE, since the SessionID will not be the same on all slaves otherwise.