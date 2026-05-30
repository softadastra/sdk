# Sync

The sync layer is the part of the Softadastra C++ SDK that tracks local operations so they can be delivered later.

A local write is not only applied to the local store. It is also submitted to the synchronization pipeline.

```txt
local write
↓
local store
↓
sync outbox
↓
sync queue
↓
transport batch when peers are available
↓
async transport events
↓
event processing
```

The important rule is:

```txt
sync is not required for local correctness
```

The network can be unavailable. Local writes still work.

## Header

Use the umbrella header:

```cpp
#include <softadastra/sdk.hpp>
```

Or include the sync-related SDK types directly:

```cpp
#include <softadastra/sdk/Client.hpp>
#include <softadastra/sdk/ClientOptions.hpp>
#include <softadastra/sdk/SyncState.hpp>
#include <softadastra/sdk/TickResult.hpp>
```

## What sync means in the SDK

The SDK exposes a compact sync API:

```cpp
client.sync_state();
client.tick();
client.retry_expired();
client.prune_completed();
client.prune_failed();
```

When transport is enabled, the SDK also exposes:

```cpp
client.process_transport_events();
```

Sync tracks operations.

Transport sends and receives data.

Transport events update peer state and dispatch inbound messages.

## Local writes are tracked

When you call:

```cpp
client.put("message", "hello");
```

the SDK submits the operation to the internal sync pipeline.

You can inspect that pipeline with:

```cpp
const auto state =
    client.sync_state();
```

Example:

```cpp
#include <softadastra/sdk.hpp>

#include <iostream>

int main()
{
  using namespace softadastra::sdk;

  Client client{
      ClientOptions::memory_only("sync-node")};

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

    std::cout << "queued: "
              << state.value().queued_count()
              << "\n";

    std::cout << "last submitted version: "
              << state.value().last_submitted_version()
              << "\n";
  }

  client.close();

  return 0;
}
```

## SyncState

`SyncState` is a public snapshot of the synchronization pipeline.

It exposes counters and version markers only.

```cpp
const auto state =
    client.sync_state();

if (state.is_ok())
{
  const auto &sync =
      state.value();

  sync.outbox_size();
  sync.queued_count();
  sync.in_flight_count();
  sync.acknowledged_count();
  sync.failed_count();
  sync.last_submitted_version();
  sync.last_applied_remote_version();
  sync.total_retries();
}
```

## SyncState fields

| Method                          | Meaning                                          |
| ------------------------------- | ------------------------------------------------ |
| `outbox_size()`                 | Number of operations tracked in the outbox       |
| `queued_count()`                | Number of operations waiting to be sent          |
| `in_flight_count()`             | Number of operations currently being delivered   |
| `acknowledged_count()`          | Number of operations acknowledged as delivered   |
| `failed_count()`                | Number of operations marked as failed            |
| `last_submitted_version()`      | Last local operation version submitted to sync   |
| `last_applied_remote_version()` | Last remote operation version applied locally    |
| `total_retries()`               | Total retry attempts performed by the sync layer |

## SyncState helpers

```cpp
state.value().has_queued();
state.value().has_in_flight();
state.value().has_failed();
state.value().has_work();
state.value().empty();
state.value().total_tracked();
```

Example:

```cpp
const auto state =
    client.sync_state();

if (state.is_ok() && state.value().has_work())
{
  std::cout << "sync has pending work\n";
}
```

## Tick

`tick()` advances the sync pipeline once.

```cpp
const auto tick =
    client.tick();
```

A tick can:

```txt
retry expired operations
collect outbound work
optionally prune completed entries
process a small batch of async transport events when transport is enabled
```

Example:

```cpp
#include <softadastra/sdk.hpp>

#include <iostream>

int main()
{
  using namespace softadastra::sdk;

  Client client{
      ClientOptions::memory_only("tick-node")};

  if (client.open().is_err())
  {
    return 1;
  }

  client.put("message", "hello");

  const auto tick =
      client.tick();

  if (tick.is_ok())
  {
    std::cout << "retried: "
              << tick.value().retried_count()
              << "\n";

    std::cout << "pruned: "
              << tick.value().pruned_count()
              << "\n";

    std::cout << "batch: "
              << tick.value().batch_size()
              << "\n";
  }

  client.close();

  return 0;
}
```

## TickResult

`TickResult` describes what happened during one manual sync tick.

```cpp
const auto tick =
    client.tick();

if (tick.is_ok())
{
  const auto &result =
      tick.value();

  result.retried_count();
  result.pruned_count();
  result.batch_size();
  result.has_work();
}
```

## TickResult fields

| Method            | Meaning                                         |
| ----------------- | ----------------------------------------------- |
| `retried_count()` | Number of expired operations requeued for retry |
| `pruned_count()`  | Number of completed or failed entries pruned    |
| `batch_size()`    | Number of outbound sync items produced          |

## TickResult helpers

```cpp
tick.value().has_work();
tick.value().retried();
tick.value().pruned();
tick.value().produced_batch();
tick.value().empty();
```

## Tick with pruning

By default, `tick()` does not prune completed entries.

You can request pruning:

```cpp
const auto tick =
    client.tick(true);
```

This asks the sync scheduler to prune completed entries during the tick.

## Retry expired operations

Use `retry_expired()` to retry expired operations manually:

```cpp
const auto retried =
    client.retry_expired();

if (retried.is_ok())
{
  std::cout << "retried: "
            << retried.value()
            << "\n";
}
```

This returns the number of operations requeued for delivery.

## Prune completed entries

Use `prune_completed()` to remove completed sync entries:

```cpp
const auto pruned =
    client.prune_completed();

if (pruned.is_ok())
{
  std::cout << "pruned completed: "
            << pruned.value()
            << "\n";
}
```

## Prune failed entries

Use `prune_failed()` to remove failed sync entries:

```cpp
const auto pruned =
    client.prune_failed();

if (pruned.is_ok())
{
  std::cout << "pruned failed: "
            << pruned.value()
            << "\n";
}
```

## Async transport events

When transport is enabled, the SDK uses the async TCP backend internally.

The backend produces transport events such as:

```txt
peer connected
peer disconnected
envelope received
send completed
send failed
backend error
```

The SDK exposes one public method to process those events:

```cpp
client.process_transport_events();
```

You can control the maximum number of events:

```cpp
client.process_transport_events(128);
```

This method lets the SDK consume queued backend events, update transport peer state, and dispatch inbound messages.

## Sync with async transport

A typical manual sync and transport loop looks like this:

```cpp
while (running)
{
  client.tick();
  client.process_transport_events(64);

  // application work here
}
```

In a small CLI or test, you can call it manually after important operations:

```cpp
client.start_transport();
client.process_transport_events();

client.connect(peer);
client.process_transport_events();

client.put("message", "hello");

client.tick();
client.process_transport_events();
```

## Sync without transport

Sync still works without transport.

This is valid:

```cpp
Client client{
    ClientOptions::memory_only("node-sync")};

client.open();

client.put("message", "hello");

const auto state =
    client.sync_state();

const auto tick =
    client.tick();

client.close();
```

In this case:

```txt
local write is accepted
store is updated
sync state is updated
no peer delivery happens
```

Transport is not required.

## Sync with transport enabled

Enable transport when the node must communicate with peers:

```cpp
ClientOptions options =
    ClientOptions::memory_only("node-a")
        .with_local_transport(9100);

Client client{options};

client.open();
client.start_transport();
client.process_transport_events();
```

Connect to a known peer:

```cpp
Peer peer =
    Peer::local("node-b", 9101);

client.connect(peer);
client.process_transport_events();
```

Then write and tick:

```cpp
client.put("message", "hello");

const auto tick =
    client.tick();

client.process_transport_events();
```

## Complete sync plus async transport example

```cpp
#include <softadastra/sdk.hpp>

#include <iostream>

int main()
{
  using namespace softadastra::sdk;

  ClientOptions options =
      ClientOptions::memory_only("node-a")
          .with_local_transport(9100);

  Client client{options};

  const auto opened =
      client.open();

  if (opened.is_err())
  {
    std::cerr << opened.error().message() << "\n";
    return 1;
  }

  const auto started =
      client.start_transport();

  if (started.is_err())
  {
    std::cerr << started.error().message() << "\n";
    client.close();
    return 1;
  }

  client.process_transport_events();

  client.put("message", "hello async sync");

  const auto before =
      client.sync_state();

  if (before.is_ok())
  {
    std::cout << "outbox before tick: "
              << before.value().outbox_size()
              << "\n";
  }

  const auto tick =
      client.tick();

  if (tick.is_ok())
  {
    std::cout << "batch size: "
              << tick.value().batch_size()
              << "\n";
  }

  const auto handled =
      client.process_transport_events();

  if (handled.is_ok())
  {
    std::cout << "handled transport events: "
              << handled.value()
              << "\n";
  }

  client.stop_transport();
  client.close();

  return 0;
}
```

## Closed client behavior

Sync methods require an open client.

```cpp
Client client{
    ClientOptions::memory_only("closed-sync")};

const auto state =
    client.sync_state();

if (state.is_err())
{
  // Error::Code::InvalidState
}
```

The following methods return `InvalidState` when the client is closed:

```txt
sync_state()
tick()
retry_expired()
prune_completed()
prune_failed()
```

`process_transport_events()` also returns `InvalidState` when the client is closed.

## Sync and persistence

Sync and persistence are different.

Persistence answers:

```txt
Can the local write survive restart?
```

Sync answers:

```txt
Can the local operation be delivered later?
```

A persistent write can survive restart even when no peer is available.

```cpp
Client client{
    ClientOptions::persistent(
        "node-1",
        "data/sdk-store.wal")};

client.open();
client.put("message", "hello");
client.close();
```

The local state is persisted.

The sync layer tracks the operation for later delivery.

## Sync and transport

Sync does not automatically mean transport is running.

Transport is optional.

```cpp
ClientOptions options =
    ClientOptions::memory_only("node-transport")
        .with_local_transport(9100);
```

Then:

```cpp
client.start_transport();
client.process_transport_events();
```

Without transport, local operations can still be tracked.

They simply cannot be sent to peers yet.

## Sync and discovery

Discovery is optional.

Discovery helps find peers.

Sync tracks what must be delivered.

Transport sends data.

```txt
discovery finds peers
transport connects peers
sync tracks operations
```

These layers are separate.

## Manual sync flow

A simple manual sync flow looks like this:

```cpp
Client client{
    ClientOptions::memory_only("node-sync")};

client.open();

client.put("message", "hello");

const auto before =
    client.sync_state();

const auto tick =
    client.tick();

const auto after =
    client.sync_state();

client.close();
```

With transport enabled:

```cpp
Client client{
    ClientOptions::memory_only("node-sync-transport")
        .with_local_transport(9100)};

client.open();
client.start_transport();

client.put("message", "hello");

client.tick();
client.process_transport_events();

client.stop_transport();
client.close();
```

## Complete sync state example

```cpp
#include <softadastra/sdk.hpp>

#include <iostream>

namespace
{
  void print_state(const softadastra::sdk::SyncState &state)
  {
    std::cout << "outbox_size: "
              << state.outbox_size()
              << "\n";

    std::cout << "queued_count: "
              << state.queued_count()
              << "\n";

    std::cout << "in_flight_count: "
              << state.in_flight_count()
              << "\n";

    std::cout << "acknowledged_count: "
              << state.acknowledged_count()
              << "\n";

    std::cout << "failed_count: "
              << state.failed_count()
              << "\n";

    std::cout << "last_submitted_version: "
              << state.last_submitted_version()
              << "\n";

    std::cout << "last_applied_remote_version: "
              << state.last_applied_remote_version()
              << "\n";

    std::cout << "total_retries: "
              << state.total_retries()
              << "\n";
  }
}

int main()
{
  using namespace softadastra::sdk;

  Client client{
      ClientOptions::memory_only("sync-state-example")};

  if (client.open().is_err())
  {
    return 1;
  }

  std::cout << "Initial state\n";

  const auto initial =
      client.sync_state();

  if (initial.is_ok())
  {
    print_state(initial.value());
  }

  client.put("message", "hello");

  std::cout << "\nAfter write\n";

  const auto after_write =
      client.sync_state();

  if (after_write.is_ok())
  {
    print_state(after_write.value());
  }

  client.close();

  return 0;
}
```

## Complete tick example

```cpp
#include <softadastra/sdk.hpp>

#include <iostream>

namespace
{
  void print_tick(const softadastra::sdk::TickResult &tick)
  {
    std::cout << "retried_count: "
              << tick.retried_count()
              << "\n";

    std::cout << "pruned_count: "
              << tick.pruned_count()
              << "\n";

    std::cout << "batch_size: "
              << tick.batch_size()
              << "\n";

    std::cout << "has_work: "
              << (tick.has_work() ? "yes" : "no")
              << "\n";
  }
}

int main()
{
  using namespace softadastra::sdk;

  Client client{
      ClientOptions::memory_only("tick-example")};

  if (client.open().is_err())
  {
    return 1;
  }

  client.put("message", "hello");

  const auto result =
      client.tick();

  if (result.is_ok())
  {
    print_tick(result.value());
  }

  client.close();

  return 0;
}
```

## Error behavior

Common sync errors:

| Situation                                              | Error code                                               |
| ------------------------------------------------------ | -------------------------------------------------------- |
| Client is closed                                       | `Error::Code::InvalidState`                              |
| Runtime is incomplete                                  | `Error::Code::InternalError`                             |
| Internal sync failure                                  | `Error::Code::SyncError` or `Error::Code::InternalError` |
| Transport events processed while transport is disabled | `Error::Code::TransportError`                            |

Example:

```cpp
const auto tick =
    client.tick();

if (tick.is_err())
{
  std::cerr << tick.error().code_string()
            << ": "
            << tick.error().message()
            << "\n";
}
```

## Recommended pattern

Without transport:

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
    client.put("message", "hello");

if (stored.is_err())
{
  client.close();
  return 1;
}

const auto state =
    client.sync_state();

if (state.is_ok() && state.value().has_work())
{
  client.tick();
}

client.close();
```

With transport:

```cpp
Client client{
    ClientOptions::persistent(
        "node-1",
        "data/sdk-store.wal")
        .with_local_transport(9100)};

const auto opened =
    client.open();

if (opened.is_err())
{
  return 1;
}

const auto started =
    client.start_transport();

if (started.is_err())
{
  client.close();
  return 1;
}

client.put("message", "hello");

client.tick();
client.process_transport_events();

client.stop_transport();
client.close();
```

## Summary

Use the sync API when you need to inspect or manually advance synchronization work.

```txt
put()                       -> accepts local operation
sync_state()                -> inspects sync counters
tick()                      -> advances sync once
retry_expired()             -> retries expired work
prune_completed()           -> prunes completed work
prune_failed()              -> prunes failed work
process_transport_events()  -> processes async transport events
```

The sync layer makes local operations deliverable later.

It does not make the network required for local writes.
