# Getting Started

This guide shows how to start using the Softadastra C++ SDK.
The SDK gives applications a simple public API over the Softadastra local-first runtime.
You do not need to manually wire the internal store, WAL, sync, transport, discovery, or metadata modules.

## Install or build the SDK

Clone the SDK repository:

```bash
git clone https://github.com/softadastra/sdk.git
cd sdk
```

Build with Vix:

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

You can also build with CMake directly:

```bash
cmake -S . -B build \
  -DSDK_CPP_BUILD_EXAMPLES=ON \
  -DSDK_CPP_BUILD_TESTS=ON

cmake --build build
```

## Include the SDK

Use the umbrella header:

```cpp
#include <softadastra/sdk.hpp>
```

Then use the SDK namespace:

```cpp
using namespace softadastra::sdk;
```

## Your first client

Create a memory-only client:

```cpp
#include <softadastra/sdk.hpp>

#include <iostream>

int main()
{
  using namespace softadastra::sdk;

  Client client{ClientOptions::memory_only("node-1")};

  const auto opened = client.open();

  if (opened.is_err())
  {
    std::cerr << opened.error().code_string()
              << ": "
              << opened.error().message()
              << "\n";

    return 1;
  }

  client.put("hello", "world");

  const auto value =
      client.get("hello");

  if (value.is_ok())
  {
    std::cout << value.value().to_string() << "\n";
  }

  client.close();

  return 0;
}
```

Expected output:

```txt
world
```

## What happened?

This example does four things:

```txt
1. creates a local SDK client
2. opens the Softadastra runtime
3. writes a value locally
4. reads the value from the local store
```

The important point:

```txt
client.put("hello", "world");
```

does not need the network.

Softadastra accepts the write locally first.

## Memory-only mode

Memory-only mode is useful for examples, tests, and temporary local state.

```cpp
ClientOptions options =
    ClientOptions::memory_only("node-memory");
```

In memory-only mode, data does not survive restart.

Use it when you want a simple local runtime without persistence.

## Persistent mode

Use persistent mode when local writes must survive restart.

```cpp
ClientOptions options =
    ClientOptions::persistent(
        "node-persistent",
        "data/sdk-store.wal");
```

Example:

```cpp
#include <softadastra/sdk.hpp>

#include <iostream>

int main()
{
  using namespace softadastra::sdk;

  Client client{
      ClientOptions::persistent(
          "node-persistent",
          "data/sdk-store.wal")};

  if (client.open().is_err())
  {
    return 1;
  }

  client.put("profile/name", "Ada");

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

Persistent mode uses a WAL path so accepted local writes can be recovered later.

## Restart recovery

A persistent client can recover data after closing and reopening.

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

Expected output:

```txt
saved-before-restart
```

This is the basic Softadastra guarantee:

```txt
A write accepted locally should remain durable.
```

## Store operations

The SDK exposes simple local store operations:

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

## Error handling

SDK operations return `Result<T, Error>`.

Example with `get()`:

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

Example with `put()`:

```cpp
const auto stored =
    client.put("name", "Softadastra");

if (stored.is_err())
{
  std::cerr << stored.error().code_string()
            << ": "
            << stored.error().message()
            << "\n";

  return 1;
}
```

Common error codes:

```cpp
Error::Code::InvalidArgument
Error::Code::InvalidState
Error::Code::NotFound
Error::Code::StoreError
Error::Code::SyncError
Error::Code::TransportError
Error::Code::DiscoveryError
Error::Code::MetadataError
Error::Code::InternalError
```

## Sync state

Every local write is submitted to the sync pipeline.

You can inspect the current sync state:

```cpp
const auto state =
    client.sync_state();

if (state.is_ok())
{
  std::cout << "outbox: "
            << state.value().outbox_size()
            << "\n";

  std::cout << "queued: "
            << state.value().queued_count()
            << "\n";
}
```

You can also advance the sync pipeline manually:

```cpp
const auto tick =
    client.tick();

if (tick.is_ok())
{
  std::cout << "batch: "
            << tick.value().batch_size()
            << "\n";
}
```

## Transport

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

## Discovery

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

## Metadata

You can expose node metadata:

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

  std::cout << node.node_id() << "\n";
  std::cout << node.display_name() << "\n";
  std::cout << node.hostname() << "\n";
  std::cout << node.os_name() << "\n";
  std::cout << node.version() << "\n";
}
```

## Run the examples

After building with examples enabled:

```bash
./build/sdk_cpp_01_local_store
./build/sdk_cpp_02_persistent_store
./build/sdk_cpp_03_restart_recovery
./build/sdk_cpp_04_sync_state
./build/sdk_cpp_05_tick
./build/sdk_cpp_06_transport
./build/sdk_cpp_07_discovery
./build/sdk_cpp_08_metadata
```

With a Ninja build directory, the path may be:

```bash
./build-ninja/sdk_cpp_01_local_store
```

## Next step

Read the dedicated guides:

```txt
docs/client.md
docs/local-store.md
docs/persistence.md
docs/sync.md
docs/transport.md
docs/discovery.md
docs/metadata.md
docs/errors.md
```
