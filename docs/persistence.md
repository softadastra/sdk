# Persistence

Persistence is the part of the Softadastra C++ SDK that allows local writes to survive restart.
In Softadastra, persistence is not an optional detail for serious applications. It is one of the core guarantees.

```txt
accepted local write
↓
persisted through WAL
↓
applied to local store
↓
available after restart
```

The main SDK entry point for persistence is:

```cpp
ClientOptions::persistent(...)
```

## Why persistence matters

Many systems treat local state as temporary.
Softadastra does the opposite.
If an application accepts a write locally, that write should not disappear just because:

```txt
the process stops
the machine restarts
the network is unavailable
the remote server cannot be reached
sync is delayed
```

Persistent mode makes local writes recoverable.

## Basic persistent client

```cpp
#include <softadastra/sdk.hpp>

int main()
{
  using namespace softadastra::sdk;

  Client client{
      ClientOptions::persistent(
          "node-1",
          "data/sdk-store.wal")};

  if (client.open().is_err())
  {
    return 1;
  }

  client.put("hello", "world");

  client.close();

  return 0;
}
```

The important part is:

```cpp
ClientOptions::persistent(
    "node-1",
    "data/sdk-store.wal")
```

This enables WAL-backed persistence for the local store.

## Persistent mode

Persistent mode creates a client that stores accepted local writes in a WAL-backed store.

```cpp
ClientOptions options =
    ClientOptions::persistent(
        "node-persistent",
        "data/sdk-store.wal");
```

The first argument is the local node id:

```txt
node-persistent
```

The second argument is the WAL path:

```txt
data/sdk-store.wal
```

## Durable mode

`durable()` is an explicit alias for `persistent()`.

```cpp
ClientOptions options =
    ClientOptions::durable(
        "node-durable",
        "data/sdk-store.wal");
```

Use `persistent()` in examples and application code.

Use `durable()` when you want to emphasize the durability guarantee.

## Fast mode

Fast mode keeps WAL persistence enabled but disables automatic flushing after every write.

```cpp
ClientOptions options =
    ClientOptions::fast(
        "node-fast",
        "data/sdk-store.wal");
```

Fast mode is useful for:

```txt
benchmarks
controlled tests
performance experiments
high-throughput local workloads
```

It is not the safest default for critical data.

For critical data, use:

```cpp
ClientOptions::persistent(...)
```

## Memory-only mode is not persistent

Memory-only mode does not use a WAL file.

```cpp
ClientOptions options =
    ClientOptions::memory_only("node-memory");
```

When the client closes, the data disappears.

```cpp
Client writer{
    ClientOptions::memory_only("node-memory")};

writer.open();
writer.put("name", "Softadastra");
writer.close();

Client reader{
    ClientOptions::memory_only("node-memory")};

reader.open();

const auto value =
    reader.get("name");

// value is an error because memory-only data was not persisted
```

Use memory-only mode for:

```txt
tests
examples
temporary state
simple demos
```

Do not use memory-only mode when data must survive restart.

## Restart recovery example

This example writes a value, closes the client, opens a new client with the same WAL path, and reads the value back.

```cpp
#include <softadastra/sdk.hpp>

#include <iostream>
#include <string>

int main()
{
  using namespace softadastra::sdk;

  const std::string wal_path =
      "data/restart-recovery.wal";

  {
    Client writer{
        ClientOptions::persistent(
            "node-writer",
            wal_path)};

    const auto opened =
        writer.open();

    if (opened.is_err())
    {
      std::cerr << opened.error().code_string()
                << ": "
                << opened.error().message()
                << "\n";

      return 1;
    }

    const auto stored =
        writer.put("status", "saved-before-restart");

    if (stored.is_err())
    {
      std::cerr << stored.error().code_string()
                << ": "
                << stored.error().message()
                << "\n";

      return 1;
    }

    writer.close();
  }

  {
    Client reader{
        ClientOptions::persistent(
            "node-reader",
            wal_path)};

    const auto opened =
        reader.open();

    if (opened.is_err())
    {
      std::cerr << opened.error().code_string()
                << ": "
                << opened.error().message()
                << "\n";

      return 1;
    }

    const auto value =
        reader.get("status");

    if (value.is_err())
    {
      std::cerr << value.error().code_string()
                << ": "
                << value.error().message()
                << "\n";

      return 1;
    }

    std::cout << value.value().to_string() << "\n";

    reader.close();
  }

  return 0;
}
```

Expected output:

```txt
saved-before-restart
```

## What the WAL path means

The WAL path identifies where persistent local operations are stored.

```cpp
ClientOptions::persistent(
    "node-1",
    "data/sdk-store.wal")
```

The path should be stable across restarts.

If you use a different WAL path, the new client will not recover the previous data.

Good:

```cpp
ClientOptions::persistent(
    "node-1",
    "data/app.wal")
```

Bad for recovery:

```cpp
ClientOptions::persistent(
    "node-1",
    "data/app-1.wal")

ClientOptions::persistent(
    "node-1",
    "data/app-2.wal")
```

Those are two different WAL files.

## The node id and the WAL path

The node id identifies the local logical node.

The WAL path identifies the durable local store.

```cpp
ClientOptions::persistent(
    "node-laptop",
    "data/laptop.wal")
```

For simple applications, keep both stable.

```txt
same node id
same WAL path
same local state
```

## Overwritten values are recovered

If a key is written more than once, the latest value should be recovered.

```cpp
{
  Client writer{
      ClientOptions::persistent(
          "node-writer",
          "data/overwrite.wal")};

  writer.open();

  writer.put("name", "first");
  writer.put("name", "second");

  writer.close();
}

{
  Client reader{
      ClientOptions::persistent(
          "node-reader",
          "data/overwrite.wal")};

  reader.open();

  const auto value =
      reader.get("name");

  // value == "second"

  reader.close();
}
```

Persistence must replay the final state correctly.

## Removed values are recovered as removed

Remove operations are also part of the local operation history.

```cpp
{
  Client writer{
      ClientOptions::persistent(
          "node-writer",
          "data/remove.wal")};

  writer.open();

  writer.put("name", "Softadastra");
  writer.remove("name");

  writer.close();
}

{
  Client reader{
      ClientOptions::persistent(
          "node-reader",
          "data/remove.wal")};

  reader.open();

  const auto value =
      reader.get("name");

  // value is Error::Code::NotFound

  reader.close();
}
```

A restart should not bring back a removed value.

## Persistence and sync

Persistence and sync are related, but they are not the same thing.

Persistence answers:

```txt
Can my local data survive restart?
```

Sync answers:

```txt
Can my local operations reach other nodes later?
```

A local write can be persisted even when no peer is connected.

```cpp
client.put("message", "hello");
```

That write can be:

```txt
stored locally
tracked for sync
sent later when peers are available
```

The network is not required for the write to be accepted locally.

## Inspecting sync state after a persistent write

```cpp
Client client{
    ClientOptions::persistent(
        "node-sync-persistent",
        "data/sync-persistent.wal")};

client.open();

client.put("message", "hello");

const auto state =
    client.sync_state();

if (state.is_ok())
{
  std::cout << "outbox: "
            << state.value().outbox_size()
            << "\n";
}

client.close();
```

Persistence protects the local store.

The sync state tracks delivery work.

## Error handling

Persistent mode can fail if the WAL path is invalid or cannot be created.

```cpp
Client client{
    ClientOptions::persistent(
        "node-1",
        "")};

const auto opened =
    client.open();

if (opened.is_err())
{
  // Error::Code::InvalidArgument
}
```

Common persistence-related errors:

| Situation                       | Error code                                                |
| ------------------------------- | --------------------------------------------------------- |
| Empty node id                   | `Error::Code::InvalidArgument`                            |
| Empty WAL path with WAL enabled | `Error::Code::InvalidArgument`                            |
| File read or write failure      | `Error::Code::IoError`                                    |
| Internal store failure          | `Error::Code::StoreError` or `Error::Code::InternalError` |
| Client used before open         | `Error::Code::InvalidState`                               |

## Recommended persistent layout

For an application, use a stable local data directory.

```txt
data/
├── sdk-store.wal
└── logs/
```

Example:

```cpp
ClientOptions options =
    ClientOptions::persistent(
        "user-device-1",
        "data/sdk-store.wal");
```

For tests, use a temporary WAL path so tests do not share state.

```cpp
ClientOptions options =
    ClientOptions::persistent(
        "test-node",
        temporary_wal_path);
```

## Production default

For production-like usage, prefer:

```cpp
ClientOptions options =
    ClientOptions::persistent(
        "node-1",
        "data/sdk-store.wal");
```

This enables:

```txt
WAL persistence
automatic flush
sync tracking
acknowledgement requirement
safe retry defaults
```

## What persistence does not do

Persistence does not automatically mean:

```txt
data is synced to another node
data is uploaded to a cloud service
data is encrypted
data has application-level conflict resolution
data has been acknowledged by a peer
```

Persistence means the local accepted write is recoverable locally.

Transport, discovery, and sync delivery are separate layers.

## Recommended pattern

```cpp
#include <softadastra/sdk.hpp>

#include <iostream>

int main()
{
  using namespace softadastra::sdk;

  Client client{
      ClientOptions::persistent(
          "node-1",
          "data/sdk-store.wal")};

  const auto opened =
      client.open();

  if (opened.is_err())
  {
    std::cerr << opened.error().code_string()
              << ": "
              << opened.error().message()
              << "\n";

    return 1;
  }

  const auto stored =
      client.put("key", "value");

  if (stored.is_err())
  {
    std::cerr << stored.error().code_string()
              << ": "
              << stored.error().message()
              << "\n";

    client.close();

    return 1;
  }

  client.close();

  return 0;
}
```

## Summary

Use persistence when data must survive restart.

```txt
memory_only()  -> temporary local state
persistent()   -> WAL-backed local durability
durable()      -> explicit alias for persistent()
fast()         -> WAL-backed but optimized for speed
```

The safest default is:

```cpp
ClientOptions::persistent(...)
```
