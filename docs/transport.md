# Transport

Transport is the SDK layer used to start local peer communication and connect to other Softadastra nodes.

Transport is optional.

A Softadastra application can write locally, persist data, and inspect sync state without starting transport.

```txt
local store works without transport
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
start a local transport service
connect to a peer
disconnect from a peer
report whether transport is running
```

The public SDK API is:

```cpp
client.start_transport();
client.stop_transport();
client.transport_running();
client.connect(peer);
client.disconnect(peer);
```

The public peer type is:

```cpp
softadastra::sdk::Peer
```

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

Local-first behavior should not require networking.

## Enable transport

Enable transport through `ClientOptions`:

```cpp
ClientOptions options =
    ClientOptions::memory_only("node-transport")
        .with_local_transport(9100);
```

This enables transport and binds it to localhost:

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

## Stop transport

```cpp
client.stop_transport();
```

`stop_transport()` is safe to call even when transport is not running.

It is also safe when transport is disabled.

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

  Peer peer =
      Peer::local("node-b", 9101);

  const auto connected =
      client.connect(peer);

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
    std::cout << "connected to "
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
```

## Transport and discovery

Transport connects and communicates with peers.

Discovery finds peers.

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

  Peer peer =
      Peer::local("peer-1", 9101);

  client.connect(peer);

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
```

Use transport only when the SDK client must communicate with other nodes.
