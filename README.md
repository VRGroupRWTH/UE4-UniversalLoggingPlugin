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
ILogStream* stream = UniLog.NewLogStream("StreamName", "path/to/file", "filename.log");
```

The path provided is still relative to your project directory. As soon as you created the `LogStream`, the complete filepath and the file are attempted to be created. You can check if it was successful using:

```cpp
ILogStream* UniLog.GetLogStream("StreamName");
if (stream.GetIsValid()) {...}
```

After your stream was created, you can log to it like before using `"StreamName"` to identify it:

```cpp
UniLog.Log("Message", "StreamName");
UniLog.LogF("StreamName", "Message %s", *variable);
```

### Per Session Logging

You might want to have log files that don't get overwritten each time your application starts. You can specify this when creating the `LogStream`:

```cpp
UniLog.NewLogStream("StreamName", "path/to/file", "filename.log", true);
```

This will put a session identifier folder between the path and the filename. A session starts with every run of the application (like clicking "Play" in the editor). 
The identifieris of the form: `<Play|PlayInPreview|PlayInEditor|Stopped>_MM-DD-YY_HH-MM-SS`