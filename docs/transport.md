# Transport

Transport is the SDK layer used to communicate with other Softadastra nodes.

The SDK transport is now based on the async TCP backend.

This means transport is event-driven:

```txt
async TCP backend
↓
produces transport events
↓
SDK processes events
↓
TransportEngine updates peer state and dispatches messages
```

Transport is optional.

A Softadastra application can still write locally, persist data, and inspect sync state without starting transport.

```txt
local store works without transport
persistence works without transport
sync tracking works without transport
transport is only needed to communicate with peers
```

## Header

Use the umbrella header:

```cpp
#include <softadastra/sdk.hpp>
```

Or include the transport-related SDK types directly:

```cpp
#include <softadastra/sdk/Client.hpp>
#include <softadastra/sdk/ClientOptions.hpp>
#include <softadastra/sdk/Peer.hpp>
#include <softadastra/sdk/Error.hpp>
```

## What transport does

Transport is responsible for peer communication.

It allows a local SDK client to:

```txt
start a local async transport service
connect to a peer
disconnect from a peer
process transport events
check whether transport is running
```

The public SDK API is:

```cpp
client.start_transport();
client.stop_transport();
client.transport_running();
client.process_transport_events();
client.connect(peer);
client.disconnect(peer);
```

The public peer type is:

```cpp
softadastra::sdk::Peer
```

## Async transport model

The SDK hides the internal async backend.

Application code does not use:

```txt
AsyncTcpTransportBackend
TransportEvent
TransportEngine
io_context
```

directly.

Instead, the public API stays simple:

```cpp
client.start_transport();
client.process_transport_events();
```

The internal flow is:

```txt
Client
↓
ClientImpl
↓
Runtime
↓
vix::async::core::io_context
↓
AsyncTcpTransportBackend
↓
TransportEngine
```

The backend performs async network work.

The SDK exposes `process_transport_events()` so the application can explicitly advance event handling.

## Transport is disabled by default

A default client does not start transport.

```cpp
Client client{
    ClientOptions::memory_only("node-1")};

client.open();

const auto started =
    client.start_transport();

// started.error().code() == Error::Code::TransportError
```

This is intentional.

Local-first behavior must not require networking.

## Enable transport

Enable transport through `ClientOptions`:

```cpp
ClientOptions options =
    ClientOptions::memory_only("node-transport")
        .with_local_transport(9100);
```

This enables async TCP transport and binds it to localhost:

```txt
127.0.0.1:9100
```

Then create and open the client:

```cpp
Client client{options};

const auto opened =
    client.open();

if (opened.is_err())
{
  return 1;
}
```

## Transport backend

The SDK currently uses async TCP transport as its public default.

```cpp
ClientOptions::TransportBackend::AsyncTcp
```

You normally do not need to set it manually.

This:

```cpp
ClientOptions options =
    ClientOptions::memory_only("node-a")
        .with_local_transport(9100);
```

is equivalent to:

```cpp
ClientOptions options =
    ClientOptions::memory_only("node-a")
        .with_local_transport(
            9100,
            ClientOptions::TransportBackend::AsyncTcp);
```

You can also set it explicitly:

```cpp
ClientOptions options =
    ClientOptions::memory_only("node-a");

options.set_transport(
    "127.0.0.1",
    9100,
    ClientOptions::TransportBackend::AsyncTcp);
```

## Start transport

After opening the client, start transport:

```cpp
const auto started =
    client.start_transport();

if (started.is_err())
{
  std::cerr << started.error().code_string()
            << ": "
            << started.error().message()
            << "\n";

  return 1;
}
```

Check the running state:

```cpp
if (client.transport_running())
{
  std::cout << "transport is running\n";
}
```

## Process transport events

Because the transport backend is async and event-driven, applications can process queued backend events manually:

```cpp
const auto processed =
    client.process_transport_events();

if (processed.is_ok())
{
  std::cout << "handled events: "
            << processed.value()
            << "\n";
}
```

You can also control the maximum number of events:

```cpp
client.process_transport_events(128);
```

Use this after operations such as:

```txt
start_transport()
connect()
disconnect()
tick()
application wait loop
```

The SDK already processes a small batch of transport events after common transport operations, but explicit calls are useful in application loops.

## Recommended application loop

A simple manual loop can look like this:

```cpp
while (running)
{
  client.tick();
  client.process_transport_events(64);

  // application work here
}
```

For a CLI, test, or small app, you can call it only after transport operations:

```cpp
client.start_transport();
client.process_transport_events();

client.connect(peer);
client.process_transport_events();
```

## Stop transport

```cpp
client.stop_transport();
```

`stop_transport()` is safe to call even when transport is not running.

It is also safe when transport is disabled.

When transport stops, the SDK shuts down the async backend and processes shutdown events internally.

## Complete transport start example

```cpp
#include <softadastra/sdk.hpp>

#include <iostream>

int main()
{
  using namespace softadastra::sdk;

  ClientOptions options =
      ClientOptions::memory_only("transport-node")
          .with_local_transport(9100);

  Client client{options};

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

  const auto started =
      client.start_transport();

  if (started.is_err())
  {
    std::cerr << started.error().code_string()
              << ": "
              << started.error().message()
              << "\n";

    return 1;
  }

  client.process_transport_events();

  std::cout << "transport running: "
            << (client.transport_running() ? "yes" : "no")
            << "\n";

  client.stop_transport();
  client.close();

  return 0;
}
```

## Peer

`Peer` describes another Softadastra node.

A peer has:

```txt
node id
host
port
```

Create a peer manually:

```cpp
Peer peer{
    "peer-1",
    "127.0.0.1",
    9101};
```

Create a local peer:

```cpp
Peer peer =
    Peer::local("peer-1", 9101);
```

## Peer methods

```cpp
peer.node_id();
peer.host();
peer.port();
peer.is_localhost();
peer.is_valid();
peer.clear();
```

A valid peer must have:

```txt
non-empty node id
non-empty host
non-zero port
```

Example:

```cpp
Peer peer =
    Peer::local("peer-1", 9101);

if (peer.is_valid())
{
  std::cout << peer.node_id()
            << " "
            << peer.host()
            << ":"
            << peer.port()
            << "\n";
}
```

## Connect to a peer

Transport must be enabled and the client must be open.

```cpp
ClientOptions options =
    ClientOptions::memory_only("node-a")
        .with_local_transport(9100);

Client client{options};

client.open();
client.start_transport();

Peer peer =
    Peer::local("node-b", 9101);

const auto connected =
    client.connect(peer);

client.process_transport_events();
```

Handle failure:

```cpp
if (connected.is_err())
{
  std::cerr << connected.error().code_string()
            << ": "
            << connected.error().message();

  if (connected.error().has_context())
  {
    std::cerr << " ("
              << connected.error().context()
              << ")";
  }

  std::cerr << "\n";
}
```

## Disconnect from a peer

```cpp
const auto disconnected =
    client.disconnect(peer);

client.process_transport_events();

if (disconnected.is_err())
{
  std::cerr << disconnected.error().code_string()
            << ": "
            << disconnected.error().message()
            << "\n";
}
```

## Complete peer connection example

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

  if (client.open().is_err())
  {
    return 1;
  }

  const auto started =
      client.start_transport();

  if (started.is_err())
  {
    std::cerr << started.error().message() << "\n";
    return 1;
  }

  client.process_transport_events();

  Peer peer =
      Peer::local("node-b", 9101);

  const auto connected =
      client.connect(peer);

  client.process_transport_events();

  if (connected.is_err())
  {
    std::cerr << "connect failed: "
              << connected.error().code_string()
              << ": "
              << connected.error().message()
              << "\n";
  }
  else
  {
    std::cout << "connect request accepted for "
              << peer.node_id()
              << "\n";
  }

  client.stop_transport();
  client.close();

  return 0;
}
```

## Transport and sync

Transport does not replace sync.

The roles are different:

```txt
sync tracks operations
transport sends data between peers
transport events update peer state and dispatch inbound messages
```

A local write can exist in sync state even when transport is stopped.

```cpp
client.put("message", "hello");

const auto state =
    client.sync_state();
```

Transport becomes useful when there is a peer to send work to.

```cpp
client.start_transport();
client.connect(peer);
client.tick();
client.process_transport_events();
```

## Transport and discovery

Transport connects and communicates with peers.

Discovery finds peers.

Sync tracks operations.

```txt
discovery finds peers
transport connects peers
sync tracks operations
```

You can use transport without discovery if you already know the peer address.

```cpp
Peer peer{
    "node-b",
    "192.168.1.20",
    9101};

client.connect(peer);
```

You can use discovery to find peers first, then connect to them.

## Lifecycle rules

Transport operations require:

```txt
client opened
transport enabled in ClientOptions
transport runtime initialized
```

This means:

```cpp
Client client{
    ClientOptions::memory_only("node-1")};

client.start_transport();
```

returns `Error::Code::InvalidState`, because the client is closed.

This:

```cpp
Client client{
    ClientOptions::memory_only("node-1")};

client.open();
client.start_transport();
```

returns `Error::Code::TransportError`, because transport is disabled.

Correct:

```cpp
Client client{
    ClientOptions::memory_only("node-1")
        .with_local_transport(9100)};

client.open();
client.start_transport();
```

## Event processing lifecycle

The async backend can produce events during:

```txt
start
connect
disconnect
send
receive
shutdown
backend failure
```

Those events are processed through:

```cpp
client.process_transport_events();
```

The returned value is the number of events successfully handled by the transport engine.

```cpp
const auto result =
    client.process_transport_events(64);

if (result.is_ok())
{
  std::cout << "handled: "
            << result.value()
            << "\n";
}
```

## Error behavior

Common transport errors:

| Situation                 | Error code                     |
| ------------------------- | ------------------------------ |
| Client is closed          | `Error::Code::InvalidState`    |
| Transport is disabled     | `Error::Code::TransportError`  |
| Transport failed to start | `Error::Code::TransportError`  |
| Peer is invalid           | `Error::Code::InvalidArgument` |
| Connection failed         | `Error::Code::TransportError`  |
| Runtime is incomplete     | `Error::Code::InternalError`   |

Example:

```cpp
const auto result =
    client.connect(Peer{});

if (result.is_err())
{
  // result.error().code() == Error::Code::InvalidArgument
}
```

Calling `process_transport_events()` while transport is disabled returns `TransportError`.

Calling it while the client is closed returns `InvalidState`.

## Recommended pattern

```cpp
#include <softadastra/sdk.hpp>

#include <iostream>

int main()
{
  using namespace softadastra::sdk;

  ClientOptions options =
      ClientOptions::persistent(
          "node-1",
          "data/sdk-store.wal")
          .with_local_transport(9100);

  Client client{options};

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

  client.process_transport_events();

  Peer peer =
      Peer::local("peer-1", 9101);

  client.connect(peer);
  client.process_transport_events();

  client.tick();
  client.process_transport_events();

  client.stop_transport();
  client.close();

  return 0;
}
```

## When to use transport

Use transport when your application needs:

```txt
peer-to-peer communication
manual peer connection
sync delivery between known nodes
local network communication
node-to-node data exchange
event-driven transport processing
```

Do not use transport just to write local data.

For local writes, the store API is enough:

```cpp
client.put("key", "value");
```

## Summary

Transport is optional.

```txt
local store does not require transport
persistence does not require transport
sync tracking does not require transport
peer communication requires transport
async transport events should be processed
```

Use transport when the SDK client must communicate with other nodes.

Use `process_transport_events()` when your application wants to explicitly advance async transport event handling.
