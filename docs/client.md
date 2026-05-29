# Client

`softadastra::sdk::Client` is the main entry point of the Softadastra C++ SDK.

It gives applications a small public API for local writes, persistent storage, sync inspection, manual sync ticks, optional transport, optional discovery, and node metadata.

```cpp
#include <softadastra/sdk.hpp>

int main()
{
  using namespace softadastra::sdk;

  Client client{ClientOptions::memory_only("node-1")};
  if (client.open().is_err())
  {
    return 1;
  }

  client.put("hello", "world");
  const auto value = client.get("hello");
  client.close();

  return value.is_ok() ? 0 : 1;
}
```

## Purpose

`Client` hides the internal Softadastra runtime modules.

Applications do not need to manually create:

```txt
StoreEngine
SyncEngine
SyncScheduler
TransportEngine
DiscoveryService
MetadataService
```

The SDK client builds and owns those modules internally.

The public user works with:

```cpp
softadastra::sdk::Client
```

## Header

Use the umbrella header:

```cpp
#include <softadastra/sdk.hpp>
```

Or include the client directly:

```cpp
#include <softadastra/sdk/Client.hpp>
```

## Namespace

```cpp
softadastra::sdk
```

Example:

```cpp
using namespace softadastra::sdk;
```

## Creating a client

### Default client

```cpp
Client client;
```

The default client uses default `ClientOptions`.

### Client with explicit options

```cpp
Client client{
    ClientOptions::memory_only("node-1")};
```

Persistent client:

```cpp
Client client{
    ClientOptions::persistent(
        "node-1",
        "data/sdk-store.wal")};
```

## Opening the client

Before using store, sync, transport, discovery, or metadata operations, open the client:

```cpp
const auto opened =
    client.open();

if (opened.is_err())
{
  return 1;
}
```

`open()` builds the internal runtime from `ClientOptions`.

It initializes:

```txt
local store
sync config
sync context
sync engine
sync scheduler
metadata service
optional transport
optional discovery
```

## Closing the client

```cpp
client.close();
```

`close()` stops optional services and releases internal runtime resources.

It is safe to call `close()` more than once.

The destructor also closes the client.

## Checking lifecycle state

```cpp
if (client.is_open())
{
  // client is ready
}
```

Backward-compatible alias:

```cpp
client.opened();
```

## Local store operations

### put

```cpp
const auto result =
    client.put("name", "Softadastra");
```

With SDK types:

```cpp
Key key{"profile/name"};
Value value{"Ada"};

client.put(key, value);
```

`put()` writes locally. Network access is not required.

### get

```cpp
const auto value =
    client.get("name");

if (value.is_ok())
{
  std::cout << value.value().to_string() << "\n";
}
```

When the key does not exist, `get()` returns `Error::Code::NotFound`.

### remove

```cpp
const auto removed =
    client.remove("name");
```

### contains

```cpp
if (client.contains("name"))
{
  // key exists
}
```

`contains()` returns `false` when the client is closed.

### size

```cpp
const std::size_t count =
    client.size();
```

`size()` returns `0` when the client is closed.

### empty

```cpp
if (client.empty())
{
  // store has no entries
}
```

## Full local store example

```cpp
#include <softadastra/sdk.hpp>

#include <iostream>

int main()
{
  using namespace softadastra::sdk;

  Client client{
      ClientOptions::memory_only("client-local-store")};

  const auto opened =
      client.open();

  if (opened.is_err())
  {
    std::cerr << opened.error().message() << "\n";
    return 1;
  }

  const auto stored =
      client.put("language", "C++");

  if (stored.is_err())
  {
    std::cerr << stored.error().message() << "\n";
    return 1;
  }

  const auto value =
      client.get("language");

  if (value.is_err())
  {
    std::cerr << value.error().message() << "\n";
    return 1;
  }

  std::cout << value.value().to_string() << "\n";

  client.close();

  return 0;
}
```

## Sync operations

Every accepted local write is submitted to the sync pipeline.

### sync_state

```cpp
const auto state =
    client.sync_state();

if (state.is_ok())
{
  std::cout << state.value().outbox_size() << "\n";
}
```

`sync_state()` returns a `SyncState`.

It exposes:

```cpp
outbox_size()
queued_count()
in_flight_count()
acknowledged_count()
failed_count()
last_submitted_version()
last_applied_remote_version()
total_retries()
```

### tick

```cpp
const auto tick =
    client.tick();
```

`tick()` advances the sync pipeline once.

It returns a `TickResult`:

```cpp
retried_count()
pruned_count()
batch_size()
has_work()
```

### retry_expired

```cpp
const auto retried =
    client.retry_expired();
```

Returns the number of expired operations requeued for retry.

### prune_completed

```cpp
const auto pruned =
    client.prune_completed();
```

Returns the number of completed sync entries removed.

### prune_failed

```cpp
const auto pruned =
    client.prune_failed();
```

Returns the number of failed sync entries removed.

## Sync example

```cpp
#include <softadastra/sdk.hpp>

#include <iostream>

int main()
{
  using namespace softadastra::sdk;

  Client client{
      ClientOptions::memory_only("client-sync")};

  if (client.open().is_err())
  {
    return 1;
  }

  client.put("message", "hello");

  const auto state =
      client.sync_state();

  if (state.is_ok())
  {
    std::cout << "outbox: "
              << state.value().outbox_size()
              << "\n";
  }

  const auto tick =
      client.tick();

  if (tick.is_ok())
  {
    std::cout << "batch: "
              << tick.value().batch_size()
              << "\n";
  }

  client.close();

  return 0;
}
```

## Transport operations

Transport is optional.

It must be enabled through `ClientOptions`.

```cpp
ClientOptions options =
    ClientOptions::memory_only("node-transport")
        .with_local_transport(9100);

Client client{options};
```

### start_transport

```cpp
client.start_transport();
```

### stop_transport

```cpp
client.stop_transport();
```

### transport_running

```cpp
if (client.transport_running())
{
  // transport is running
}
```

### connect

```cpp
Peer peer =
    Peer::local("peer-1", 9101);

client.connect(peer);
```

### disconnect

```cpp
client.disconnect(peer);
```

If transport is disabled, transport operations return `Error::Code::TransportError`.

## Discovery operations

Discovery is optional.

It must be enabled through `ClientOptions`.

```cpp
ClientOptions options =
    ClientOptions::memory_only("node-discovery")
        .with_local_discovery(5051);

Client client{options};
```

### start_discovery

```cpp
client.start_discovery();
```

### stop_discovery

```cpp
client.stop_discovery();
```

### discovery_running

```cpp
if (client.discovery_running())
{
  // discovery is running
}
```

### peers

```cpp
const auto peers =
    client.peers();

if (peers.is_ok())
{
  for (const auto &peer : peers.value())
  {
    std::cout << peer.node_id()
              << " "
              << peer.host()
              << ":"
              << peer.port()
              << "\n";
  }
}
```

If discovery is disabled, discovery operations return `Error::Code::DiscoveryError`.

## Metadata operations

Metadata is initialized by default when the client opens.

### node_info

```cpp
const auto info =
    client.node_info();
```

### refresh_node_info

```cpp
const auto info =
    client.refresh_node_info();
```

Example:

```cpp
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

## Options access

You can inspect the options used by the client:

```cpp
const ClientOptions &options =
    client.options();

std::cout << options.node_id() << "\n";
```

## Move-only type

`Client` is move-only.

Allowed:

```cpp
Client a{
    ClientOptions::memory_only("node-a")};

Client b{
    std::move(a)};
```

Not allowed:

```cpp
Client a;
Client b = a;
```

Copying is disabled because the client owns runtime services and internal state.

## Error behavior

Operations return `Result<T, Error>`.

Operations without a value return:

```cpp
Result<void, Error>
```

Example:

```cpp
const auto result =
    client.put("name", "Softadastra");

if (result.is_err())
{
  std::cerr << result.error().code_string()
            << ": "
            << result.error().message()
            << "\n";
}
```

Common lifecycle errors:

```txt
closed client       -> invalid_state
empty key           -> invalid_argument
missing key         -> not_found
disabled transport  -> transport_error
disabled discovery  -> discovery_error
```

## Client lifecycle rule

The client must be opened before use:

```cpp
Client client{
    ClientOptions::memory_only("node-1")};

client.open();

client.put("key", "value");

client.close();
```

Calling store, sync, transport, discovery, or metadata operations before `open()` returns an error.

## Recommended pattern

```cpp
Client client{
    ClientOptions::persistent(
        "node-1",
        "data/sdk-store.wal")};

const auto opened =
    client.open();

if (opened.is_err())
{
  return 1;
}

const auto stored =
    client.put("hello", "world");

if (stored.is_err())
{
  client.close();
  return 1;
}

client.close();

return 0;
```

## When to use Client

Use `Client` when you want:

```txt
local writes
persistent local state
restart recovery
sync state inspection
manual sync ticks
optional peer transport
optional peer discovery
node metadata
```

Use the lower-level Softadastra runtime modules only if you are developing Softadastra itself.
