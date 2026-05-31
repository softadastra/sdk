# Softadastra C++ SDK

The official C++ SDK for Softadastra.
Softadastra helps applications save data locally, keep working offline, and synchronize when the connection comes back.
This SDK gives developers a simple C++ API to use Softadastra inside their applications.

## What this SDK is for

Use this SDK when you want your C++ application to:

- save data locally
- read data back later
- keep important data after restart
- work without depending on a constant internet connection
- prepare for synchronization with other nodes

The SDK hides the internal Softadastra runtime behind one main API:

```cpp
softadastra::sdk::Client
```

## Install

Linux and macOS:

```bash
curl -fsSL https://softadastra.com/install.sh | bash
```

Windows PowerShell:

```powershell
irm https://softadastra.com/install.ps1 | iex
```

## Example

```cpp
#include <softadastra/sdk.hpp>
#include <iostream>

int main()
{
    using namespace softadastra::sdk;

    Client client{
        ClientOptions::persistent(
            "my-app",
            "data/app.wal"
        )
    };

    const auto opened = client.open();

    if (opened.is_err()){
        std::cerr << opened.error().message() << "\n";
        return 1;
    }

    const auto saved = client.put("user:name", "Ada");

    if (saved.is_err()){
        std::cerr << saved.error().message() << "\n";
        return 1;
    }

    const auto name = client.get("user:name");

    if (name.is_ok()){
        std::cout << name.value().to_string() << "\n";
    }

    client.close();

    return 0;
}
```

This example saves a value locally and reads it back.
Because it uses a persistent store, the data can be recovered after the application restarts.

## Use with CMake

```cmake
find_package(sdk-cpp REQUIRED)
target_link_libraries(my_app PRIVATE softadastra::sdk)
```

## Main include

```cpp
#include <softadastra/sdk.hpp>
```

## Main API

```cpp
Client client{ClientOptions::persistent("my-app", "data/app.wal")};

client.open();
client.put("key", "value");
client.get("key");
client.remove("key");
client.close();
```

## Repository role

This repository contains the developer-facing C++ SDK.
The main Softadastra repository contains the runtime and command-line tool.

## Links

- Softadastra: [github.com/softadastra/softadastra](https://github.com/softadastra/softadastra)
- Website: [softadastra.com](https://softadastra.com)
- Documentation: [docs.softadastra.com](https://docs.softadastra.com)

## License

Licensed under the Apache License, Version 2.0. \
See [LICENSE](LICENSE) for details.
