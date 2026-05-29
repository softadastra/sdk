# sdk-cpp

Official C++ SDK for Softadastra.

`sdk-cpp` provides a small, stable, developer-facing C++ API over the Softadastra runtime foundation.

It is designed to make Softadastra usable from applications without requiring developers to manually wire internal modules such as store, WAL, sync, transport, discovery, or metadata.

```cpp
#include <softadastra/sdk.hpp>

int main()
{
  using namespace softadastra::sdk;

  Client client{
      ClientOptions::persistent("node-1","data/sdk-store.wal")
  };

  const auto opened = client.open();
  if (opened.is_err())
  {
    return 1;
  }

  client.put("hello", "world");
  const auto value = client.get("hello");
  client.close();

  return value.is_ok() ? 0 : 1;
}
```

## What is Softadastra?

Softadastra is a local-first and offline-capable synchronization runtime built to keep data durable, synchronized, and recoverable under real-world network conditions.

It is built around:

- local writes
- durable storage
- write-ahead logging
- sync tracking
- safe retries
- peer transport
- peer discovery
- node metadata
- eventual convergence

The internal Softadastra runtime contains low-level modules such as:

- `core`
- `wal`
- `store`
- `sync`
- `transport`
- `discovery`
- `metadata`

`sdk-cpp` is the public C++ SDK that wraps these modules into a clean API.

## Why sdk-cpp exists

Softadastra’s internal modules are powerful, but they are low-level.

A developer should not need to manually create and connect:

- `StoreEngine`
- `SyncEngine`
- `SyncScheduler`
- `TransportEngine`
- `DiscoveryService`
- `MetadataService`

The SDK provides one main entry point instead:

```cpp
softadastra::sdk::Client
```

This keeps application code simple while preserving Softadastra’s local-first guarantees.

## Current version

```txt
0.1.0
```

This release initializes the public C++ SDK foundation.

The goal of `v0.1.0` is to provide a stable first API surface over the existing Softadastra runtime.

## Repository

```txt
https://github.com/softadastra/sdk.git
```

The repository is named:

```txt
sdk
```

The C++ namespace is:

```cpp
softadastra::sdk
```

## Public API

Main public types:

```cpp
softadastra::sdk::Client
softadastra::sdk::ClientOptions
softadastra::sdk::Error
softadastra::sdk::Result
softadastra::sdk::Key
softadastra::sdk::Value
softadastra::sdk::Peer
softadastra::sdk::NodeInfo
softadastra::sdk::SyncState
softadastra::sdk::TickResult
```

Umbrella include:

```cpp
#include <softadastra/sdk.hpp>
```

Individual includes:

```cpp
#include <softadastra/sdk/Client.hpp>
#include <softadastra/sdk/ClientOptions.hpp>
#include <softadastra/sdk/Error.hpp>
#include <softadastra/sdk/Result.hpp>
#include <softadastra/sdk/Key.hpp>
#include <softadastra/sdk/Value.hpp>
#include <softadastra/sdk/Peer.hpp>
#include <softadastra/sdk/NodeInfo.hpp>
#include <softadastra/sdk/SyncState.hpp>
#include <softadastra/sdk/TickResult.hpp>
#include <softadastra/sdk/Version.hpp>
```

## Core model

Softadastra follows this model:

```txt
write locally
persist locally
track operation
sync when possible
retry when needed
converge later
```

A local operation should not depend on the network.

This must work even when transport and discovery are disabled:

```cpp
client.put("key", "value");
```

Transport and discovery are optional layers used to exchange operations with peers.

## Basic usage

### Memory-only local store

```cpp
#include <softadastra/sdk.hpp>

#include <iostream>

int main()
{
  using namespace softadastra::sdk;

  Client client{
      ClientOptions::memory_only("node-memory")};

  const auto opened = client.open();

  if (opened.is_err())
  {
    std::cerr << opened.error().message() << "\n";
    return 1;
  }

  const auto stored =
      client.put("name", "Softadastra");

  if (stored.is_err())
  {
    std::cerr << stored.error().message() << "\n";
    return 1;
  }

  const auto value =
      client.get("name");

  if (value.is_ok())
  {
    std::cout << value.value().to_string() << "\n";
  }

  client.close();

  return 0;
}
```

### Persistent WAL-backed store

```cpp
#include <softadastra/sdk.hpp>

#include <iostream>

int main()
{
  using namespace softadastra::sdk;

  ClientOptions options =
      ClientOptions::persistent(
          "node-persistent",
          "data/sdk-store.wal");

  Client client{options};

  if (client.open().is_err())
  {
    return 1;
  }

  client.put("profile/name", "Ada");
  client.put("profile/language", "C++");

  const auto name =
      client.get("profile/name");

  if (name.is_ok())
  {
    std::cout << name.value().to_string() << "\n";
  }

  client.close();

  return 0;
}
```

### Restart recovery

```cpp
#include <softadastra/sdk.hpp>

#include <iostream>
#include <string>

int main()
{
  using namespace softadastra::sdk;

  const std::string wal_path =
      "data/recovery.wal";

  {
    Client writer{
        ClientOptions::persistent(
            "node-writer",
            wal_path)};

    if (writer.open().is_err())
    {
      return 1;
    }

    writer.put("status", "saved-before-restart");
    writer.close();
  }

  {
    Client reader{
        ClientOptions::persistent(
            "node-reader",
            wal_path)};

    if (reader.open().is_err())
    {
      return 1;
    }

    const auto value =
        reader.get("status");

    if (value.is_ok())
    {
      std::cout << value.value().to_string() << "\n";
    }

    reader.close();
  }

  return 0;
}
```

## Client options

### Memory-only mode

```cpp
ClientOptions options =
    ClientOptions::memory_only("node-1");
```

Memory-only mode does not persist the local store to a WAL file.

Use it for:

- tests
- examples
- temporary local state
- simple demos

### Persistent mode

```cpp
ClientOptions options =
    ClientOptions::persistent(
        "node-1",
        "data/sdk-store.wal");
```

Persistent mode enables WAL-backed storage.

Use it when local writes must survive restart.

### Fast mode

```cpp
ClientOptions options =
    ClientOptions::fast(
        "node-1",
        "data/sdk-store.wal");
```

Fast mode keeps WAL persistence enabled but disables automatic flushing after every write.

Use it for:

- benchmarks
- controlled test environments
- performance experiments

## Local store API

```cpp
client.put("key", "value");

const auto value =
    client.get("key");

client.remove("key");

const bool exists =
    client.contains("key");

const std::size_t count =
    client.size();
```

`Value` is binary-safe.

```cpp
Value value =
    Value::from_bytes({0x01, 0x02, 0x03});

client.put(Key{"binary"}, value);
```

## Sync API

```cpp
const auto state =
    client.sync_state();

const auto tick =
    client.tick();

const auto retried =
    client.retry_expired();

const auto pruned =
    client.prune_completed();
```

`SyncState` exposes a stable view of the sync pipeline:

```cpp
state.value().outbox_size();
state.value().queued_count();
state.value().in_flight_count();
state.value().acknowledged_count();
state.value().failed_count();
state.value().last_submitted_version();
state.value().last_applied_remote_version();
state.value().total_retries();
```

`TickResult` describes what happened during one manual sync tick:

```cpp
tick.value().retried_count();
tick.value().pruned_count();
tick.value().batch_size();
tick.value().has_work();
```

## Transport API

Transport is optional.

Enable it through `ClientOptions`:

```cpp
ClientOptions options =
    ClientOptions::memory_only("node-transport")
        .with_local_transport(9100);

Client client{options};

client.open();
client.start_transport();
```

Connect to a peer:

```cpp
Peer peer =
    Peer::local("peer-1", 9101);

client.connect(peer);
```

Stop transport:

```cpp
client.stop_transport();
```

## Discovery API

Discovery is optional.

Enable it through `ClientOptions`:

```cpp
ClientOptions options =
    ClientOptions::memory_only("node-discovery")
        .with_local_discovery(5051);

Client client{options};

client.open();
client.start_discovery();

const auto peers =
    client.peers();

client.stop_discovery();
```

## Metadata API

```cpp
ClientOptions options =
    ClientOptions::memory_only("node-info")
        .with_metadata("Local Node", "0.1.0");

Client client{options};

client.open();

const auto info =
    client.refresh_node_info();

if (info.is_ok())
{
  const auto &node = info.value();

  node.node_id();
  node.display_name();
  node.hostname();
  node.os_name();
  node.version();
  node.uptime_ms();
}
```

## Error handling

SDK operations return `Result<T, Error>`.

```cpp
const auto value =
    client.get("name");

if (value.is_err())
{
  std::cerr << value.error().code_string()
            << ": "
            << value.error().message()
            << "\n";

  return 1;
}

std::cout << value.value().to_string() << "\n";
```

Operations without a return value use `Result<void, Error>`.

```cpp
const auto stored =
    client.put("name", "Softadastra");

if (stored.is_err())
{
  std::cerr << stored.error().message() << "\n";
}
```

Stable SDK error codes:

```cpp
Error::Code::None
Error::Code::Unknown
Error::Code::InvalidArgument
Error::Code::InvalidState
Error::Code::NotFound
Error::Code::AlreadyExists
Error::Code::IoError
Error::Code::StoreError
Error::Code::SyncError
Error::Code::TransportError
Error::Code::DiscoveryError
Error::Code::MetadataError
Error::Code::InternalError
```

## Project structure

```txt
sdk/
├── include/
│   └── softadastra/
│       ├── sdk.hpp
│       └── sdk/
│           ├── Client.hpp
│           ├── ClientOptions.hpp
│           ├── Error.hpp
│           ├── Result.hpp
│           ├── Key.hpp
│           ├── Value.hpp
│           ├── Peer.hpp
│           ├── NodeInfo.hpp
│           ├── SyncState.hpp
│           ├── TickResult.hpp
│           └── Version.hpp
│
├── src/
│   ├── Client.cpp
│   ├── ClientImpl.hpp
│   ├── ClientImpl.cpp
│   ├── ClientOptions.cpp
│   ├── Error.cpp
│   ├── Key.cpp
│   ├── Value.cpp
│   ├── Peer.cpp
│   ├── NodeInfo.cpp
│   ├── SyncState.cpp
│   ├── TickResult.cpp
│   ├── Version.cpp
│   │
│   ├── internal/
│   │   ├── Runtime.hpp
│   │   ├── Runtime.cpp
│   │   ├── RuntimeBuilder.hpp
│   │   ├── RuntimeBuilder.cpp
│   │   ├── RuntimeGuards.hpp
│   │   ├── RuntimeGuards.cpp
│   │   ├── ErrorMapper.hpp
│   │   └── ErrorMapper.cpp
│   │
│   └── conversions/
│       ├── StoreConversions.hpp
│       ├── StoreConversions.cpp
│       ├── SyncConversions.hpp
│       ├── SyncConversions.cpp
│       ├── TransportConversions.hpp
│       ├── TransportConversions.cpp
│       ├── DiscoveryConversions.hpp
│       ├── DiscoveryConversions.cpp
│       ├── MetadataConversions.hpp
│       └── MetadataConversions.cpp
│
├── examples/
├── tests/
├── docs/
├── cmake/
├── CMakeLists.txt
├── CMakePresets.json
├── vix.json
├── README.md
├── CHANGELOG.md
├── LICENSE
└── cmd.md
```

## Build

Recommended:

```bash
vix build
```

Build with examples:

```bash
vix build -- -DSDK_CPP_BUILD_EXAMPLES=ON
```

Build with tests:

```bash
vix build -- -DSDK_CPP_BUILD_TESTS=ON
```

Build with CMake directly:

```bash
cmake -S . -B build \
  -DSDK_CPP_BUILD_EXAMPLES=ON \
  -DSDK_CPP_BUILD_TESTS=ON

cmake --build build
```

## Run examples

```bash
./build/examples/sdk_cpp_01_local_store
./build/examples/sdk_cpp_02_persistent_store
./build/examples/sdk_cpp_03_restart_recovery
./build/examples/sdk_cpp_04_sync_state
./build/examples/sdk_cpp_05_tick
./build/examples/sdk_cpp_06_transport
./build/examples/sdk_cpp_07_discovery
./build/examples/sdk_cpp_08_metadata
```

Depending on the generator and build directory, executable paths may differ.

For Ninja builds:

```bash
./build-ninja/sdk_cpp_01_local_store
```

## Run tests

```bash
ctest --test-dir build
```

Or with a Ninja build directory:

```bash
ctest --test-dir build-ninja
```

## Install

```bash
cmake --install build
```

Use from another CMake project:

```cmake
find_package(sdk-cpp REQUIRED)

target_link_libraries(my_app PRIVATE sdk-cpp::sdk-cpp)
```

Alias target:

```cmake
target_link_libraries(my_app PRIVATE softadastra::sdk)
```

## Design principles

- Small public API
- Stable user-facing types
- Explicit error handling
- No hidden network dependency
- Local-first behavior
- Network as an optional layer
- Deterministic sync flow
- Internal modules hidden behind the SDK facade
- Clean separation between public headers and internal runtime
- Safe foundation for products such as Converdict

## Relationship with Softadastra

`sdk-cpp` is a facade over Softadastra runtime modules.

It does not replace Softadastra internals.

It exposes a stable API for applications and tools that need to use Softadastra without depending directly on internal module types.

## Relationship with Converdict

Converdict should use Softadastra through `sdk-cpp`.

That keeps Converdict independent from internal runtime details and allows the SDK to become the stable bridge between the product layer and the Softadastra foundation.

## Roadmap

- [x] Public SDK types
- [x] Client facade
- [x] ClientOptions
- [x] Error model
- [x] Local store API
- [x] Persistent store API
- [x] Restart recovery example
- [x] Sync state API
- [x] Tick API
- [x] Transport API
- [x] Discovery API
- [x] Metadata API
- [x] Tests
- [x] Examples
- [ ] Validate install/export package
- [ ] Add stronger multi-node sync example
- [ ] Add Converdict integration scenario
- [ ] Publish through Vix registry

## License

Licensed under the Apache License, Version 2.0.

See `LICENSE` for details.
