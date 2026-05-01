# sdk-cpp

> Official C++ SDK for Softadastra.

`sdk-cpp` provides a small, stable, developer-facing C++ API over the Softadastra runtime foundation.

It is designed to make Softadastra usable from applications without requiring developers to manually wire the internal modules.

```cpp
#include <softadastra/sdk.hpp>

int main()
{
    using namespace softadastra::sdk;

    Client client{
        ClientOptions::persistent(
            "node-1",
            "data/sdk.wal")};

    auto opened = client.open();

    if (opened.is_err())
    {
        return 1;
    }

    client.put("hello", "world");

    auto value = client.get("hello");

    client.close();

    return value.is_ok() ? 0 : 1;
}
```

## What is Softadastra?

Softadastra is an offline-first and local-first foundation for building reliable software under real-world network conditions.

It is built around: local writes, durable storage, write-ahead logging, sync tracking, safe retries, peer transport, peer discovery, node metadata, and eventual convergence.

The internal Softadastra repository contains low-level modules: `core`, `fs`, `wal`, `store`, `sync`, `transport`, `discovery`, `metadata`, `cli`.

`sdk-cpp` is the public C++ SDK that wraps these modules into a clean API.

## Why sdk-cpp?

The internal Softadastra modules are powerful, but they are low-level. A developer should not need to manually create and connect `StoreEngine`, `SyncEngine`, `SyncScheduler`, `TransportEngine`, `DiscoveryService`, and `MetadataService`.

The SDK provides one main entry point instead: `softadastra::sdk::Client`.

## Repository name

The repository is named `sdk-cpp` — intentionally without the `softadastra-` prefix. Inside C++ code, the namespace remains `softadastra::sdk`.

## Status

Current version: **v0.1.0**

This is the initial SDK foundation release. The goal of `v0.1.0` is to provide the first stable public SDK layer over the existing Softadastra modules.

## Features

### Local store

```cpp
client.put("key", "value");
auto value = client.get("key");
client.remove("key");
```

### Persistent store

```cpp
ClientOptions options = ClientOptions::persistent(
    "node-1",
    "data/sdk.wal");
```

### Sync

```cpp
auto state = client.sync_state();
auto tick  = client.tick();
```

### Transport

```cpp
client.start_transport();
client.connect(peer);
client.stop_transport();
```

### Discovery

```cpp
client.start_discovery();
auto peers = client.peers();
client.stop_discovery();
```

### Metadata

```cpp
auto info     = client.node_info();
auto refreshed = client.refresh_node_info();
```

## Public API

Main types:

- `softadastra::sdk::Client`
- `softadastra::sdk::ClientOptions`
- `softadastra::sdk::Result<T, E>`
- `softadastra::sdk::Error`
- `softadastra::sdk::Key`
- `softadastra::sdk::Value`
- `softadastra::sdk::Peer`
- `softadastra::sdk::NodeInfo`
- `softadastra::sdk::SyncResult`
- `softadastra::sdk::TickResult`

### Include

Umbrella header:

```cpp
#include <softadastra/sdk.hpp>
```

Individual headers:

```cpp
#include <softadastra/sdk/Client.hpp>
#include <softadastra/sdk/ClientOptions.hpp>
#include <softadastra/sdk/Result.hpp>
#include <softadastra/sdk/Error.hpp>
```

## Project structure

```
sdk-cpp/
├── CHANGELOG.md
├── CMakeLists.txt
├── CMakePresets.json
├── LICENSE
├── README.md
├── vix.json
├── cmake/
│   └── sdk-cpp-config.cmake.in
├── docs/
│   ├── getting-started.md
│   ├── local-store.md
│   ├── sync.md
│   ├── discovery.md
│   └── metadata.md
├── examples/
│   ├── 01_local_store.cpp
│   ├── 02_persistent_store.cpp
│   ├── 03_remove_value.cpp
│   ├── 04_basic_sync.cpp
│   ├── 05_tcp_peer_sync.cpp
│   ├── 06_discovery.cpp
│   └── 07_node_metadata.cpp
├── include/
│   └── softadastra/
│       ├── sdk.hpp
│       └── sdk/
│           ├── Client.hpp
│           ├── ClientOptions.hpp
│           ├── Error.hpp
│           ├── Key.hpp
│           ├── NodeInfo.hpp
│           ├── Peer.hpp
│           ├── Result.hpp
│           ├── SyncResult.hpp
│           ├── TickResult.hpp
│           └── Value.hpp
├── src/
│   ├── Client.cpp
│   ├── ClientOptions.cpp
│   ├── Error.cpp
│   └── conversions/
│       ├── DiscoveryConversions.cpp
│       ├── MetadataConversions.cpp
│       ├── StoreConversions.cpp
│       ├── SyncConversions.cpp
│       └── TransportConversions.cpp
└── tests/
    ├── CMakeLists.txt
    ├── ClientOptionsTests.cpp
    ├── ClientTests.cpp
    └── ResultTests.cpp
```

## Build

```bash
vix build

#cmake
# Configure
cmake --preset dev-ninja

# Build
cmake --build --preset build-ninja

# MSVC
cmake --preset dev-msvc
cmake --build --preset build-msvc
```

## Run examples

```bash
./build-ninja/examples/01_local_store
./build-ninja/examples/02_persistent_store
./build-ninja/examples/03_remove_value
```

## Examples

### Minimal

```cpp
#include <iostream>
#include <softadastra/sdk.hpp>

int main()
{
    using namespace softadastra::sdk;

    Client client{ClientOptions::memory_only("node-memory")};

    auto opened = client.open();

    if (opened.is_err())
    {
        std::cerr << opened.error().message() << "\n";
        return 1;
    }

    auto put_result = client.put("name", "Softadastra");

    if (put_result.is_err())
    {
        std::cerr << put_result.error().message() << "\n";
        return 1;
    }

    auto value_result = client.get("name");

    if (value_result.is_ok())
    {
        std::cout << value_result.value().to_string() << "\n";
    }

    client.close();

    return 0;
}
```

### Persistent store

```cpp
#include <iostream>
#include <softadastra/sdk.hpp>

int main()
{
    using namespace softadastra::sdk;

    ClientOptions options = ClientOptions::persistent(
        "node-persistent",
        "data/sdk-store.wal");

    Client client{options};

    auto opened = client.open();

    if (opened.is_err())
    {
        std::cerr << opened.error().message() << "\n";
        return 1;
    }

    client.put("profile/name",     "Ada");
    client.put("profile/language", "C++");

    auto name = client.get("profile/name");

    if (name.is_ok())
    {
        std::cout << name.value().to_string() << "\n";
    }

    client.close();

    return 0;
}
```

### Sync

```cpp
#include <iostream>
#include <softadastra/sdk.hpp>

int main()
{
    using namespace softadastra::sdk;

    ClientOptions options = ClientOptions::persistent(
        "node-sync",
        "data/sync.wal");

    Client client{options};

    if (client.open().is_err())
    {
        return 1;
    }

    client.put("message", "hello");

    auto state = client.sync_state();

    if (state.is_ok())
    {
        std::cout << "queued: " << state.value().queued_count << "\n";
    }

    auto tick = client.tick();

    if (tick.is_ok())
    {
        std::cout << "batch: " << tick.value().batch_size << "\n";
    }

    client.close();

    return 0;
}
```

### Discovery

```cpp
#include <iostream>
#include <softadastra/sdk.hpp>

int main()
{
    using namespace softadastra::sdk;

    ClientOptions options = ClientOptions::local("node-discovery");

    options.enable_discovery         = true;
    options.discovery_host           = "127.0.0.1";
    options.discovery_port           = 5051;
    options.discovery_broadcast_host = "127.0.0.1";
    options.discovery_broadcast_port = 5052;

    Client client{options};

    if (client.open().is_err())
    {
        return 1;
    }

    auto started = client.start_discovery();

    if (started.is_err())
    {
        std::cerr << started.error().message() << "\n";
        return 1;
    }

    auto peers = client.peers();

    if (peers.is_ok())
    {
        for (const auto &peer : peers.value())
        {
            std::cout << peer.node_id << " "
                      << peer.host   << ":"
                      << peer.port   << "\n";
        }
    }

    client.stop_discovery();
    client.close();

    return 0;
}
```

### Metadata

```cpp
#include <iostream>
#include <softadastra/sdk.hpp>

int main()
{
    using namespace softadastra::sdk;

    ClientOptions options = ClientOptions::local("node-info");
    options.display_name  = "Node Info Demo";
    options.version       = "0.1.0";

    Client client{options};

    if (client.open().is_err())
    {
        return 1;
    }

    auto info = client.refresh_node_info();

    if (info.is_ok())
    {
        const auto &node = info.value();

        std::cout << "node id : " << node.node_id  << "\n";
        std::cout << "label   : " << node.label()  << "\n";
        std::cout << "host    : " << node.hostname  << "\n";
        std::cout << "os      : " << node.os_name   << "\n";
        std::cout << "version : " << node.version   << "\n";
        std::cout << "uptime  : " << node.uptime_ms << "ms\n";
    }

    client.close();

    return 0;
}
```

## Design principles

- Small public API
- Stable user-facing types
- Explicit error handling
- No hidden magic
- Local-first behavior
- Network as an optional layer
- Deterministic sync flow
- Internal modules hidden behind the SDK facade

## Offline-first model

```
write locally
persist locally
track operation
sync when possible
retry when needed
converge later
```

A local operation should not depend on the network. This should work even when transport or discovery is disabled:

```cpp
client.put("key", "value");
```

Transport and discovery are optional layers used to exchange operations with peers.

## Error handling

```cpp
// With return value
auto result = client.get("name");

if (result.is_err())
{
    std::cerr << result.error().message() << "\n";
    return 1;
}

std::cout << result.value().to_string() << "\n";

// Without return value
auto result = client.put("key", "value");

if (result.is_err())
{
    std::cerr << result.error().code_string()
              << ": "
              << result.error().message()
              << "\n";
}
```

## Relationship with Softadastra modules

`sdk-cpp` is a facade over the internal modules, not a replacement.

| Internal module | Public SDK API |
|----------------|----------------|
| `softadastra::store::engine::StoreEngine` | `client.put()` / `client.get()` |
| `softadastra::sync::engine::SyncEngine` | `client.sync_state()` / `client.tick()` |
| `softadastra::discovery::DiscoveryService` | `client.start_discovery()` / `client.peers()` |

## Documentation

| File | Description |
|------|-------------|
| `docs/getting-started.md` | First steps with the SDK |
| `docs/local-store.md` | Local key-value store |
| `docs/sync.md` | Sync pipeline |
| `docs/discovery.md` | Peer discovery |
| `docs/metadata.md` | Node metadata |

## Examples

| File | Description |
|------|-------------|
| `examples/01_local_store.cpp` | In-memory local store |
| `examples/02_persistent_store.cpp` | WAL-backed persistent store |
| `examples/03_remove_value.cpp` | Removing keys |
| `examples/04_basic_sync.cpp` | Local sync pipeline |
| `examples/05_tcp_peer_sync.cpp` | TCP peer sync |
| `examples/06_discovery.cpp` | Peer discovery |
| `examples/07_node_metadata.cpp` | Node metadata |

## Tests

```bash
ctest --test-dir build-ninja
```

| File | Description |
|------|-------------|
| `tests/ResultTests.cpp` | Result type tests |
| `tests/ClientOptionsTests.cpp` | ClientOptions tests |
| `tests/ClientTests.cpp` | Client integration tests |

## Install

The SDK is prepared for CMake package installation.

```cmake
find_package(sdk-cpp REQUIRED)

target_link_libraries(my_app PRIVATE softadastra::sdk-cpp)
# or
target_link_libraries(my_app PRIVATE softadastra::sdk)
```

## Roadmap

- [ ] Stabilize the public `Client` API
- [ ] Complete internal module conversions
- [ ] Add stronger examples
- [ ] Add transport and discovery integration examples
- [ ] Add metadata output examples
- [ ] Add more tests around error handling
- [ ] Add install/export validation
- [ ] Add package manager integration through Vix registry

## Versioning

Current release: **v0.1.0**

This release initializes the C++ SDK foundation. Future versions should keep the public API stable and evolve carefully.

## License

Licensed under the Apache License, Version 2.0. See `LICENSE`.
