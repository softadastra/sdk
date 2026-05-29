# Discovery

Discovery is the SDK layer used to find other Softadastra nodes.

Discovery is optional.

A Softadastra application can write locally, persist data, inspect sync state, and even use transport with known peers without enabling discovery.

```txt
local store works without discovery
persistence works without discovery
sync tracking works without discovery
transport can work without discovery when peers are known
discovery is only needed to find peers automatically
```

## Header

Use the umbrella header:

```cpp
#include <softadastra/sdk.hpp>
```

Or include the discovery-related SDK types directly:

```cpp
#include <softadastra/sdk/Client.hpp>
#include <softadastra/sdk/ClientOptions.hpp>
#include <softadastra/sdk/Peer.hpp>
#include <softadastra/sdk/Error.hpp>
```

## What discovery does

Discovery helps a local node find other nodes.

The SDK exposes a small public discovery API:

```cpp
client.start_discovery();
client.stop_discovery();
client.discovery_running();
client.peers();
```

The public peer type is:

```cpp
softadastra::sdk::Peer
```

Internal discovery objects are not exposed through the SDK.

## Discovery is disabled by default

A default client does not start discovery.

```cpp
Client client{
    ClientOptions::memory_only("node-1")};

client.open();

const auto started =
    client.start_discovery();

// started.error().code() == Error::Code::DiscoveryError
```

This is intentional.

Local-first behavior must not require peer discovery.

## Enable discovery

Enable discovery through `ClientOptions`:

```cpp
ClientOptions options =
    ClientOptions::memory_only("node-discovery")
        .with_local_discovery(5051);
```

This enables discovery on localhost:

```txt
127.0.0.1:5051
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

## Start discovery

After opening the client, start discovery:

```cpp
const auto started =
    client.start_discovery();

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
if (client.discovery_running())
{
  std::cout << "discovery is running\n";
}
```

## Stop discovery

```cpp
client.stop_discovery();
```

`stop_discovery()` is safe to call even when discovery is not running.

It is also safe when discovery is disabled.

## Complete discovery start example

```cpp
#include <softadastra/sdk.hpp>

#include <iostream>

int main()
{
  using namespace softadastra::sdk;

  ClientOptions options =
      ClientOptions::memory_only("discovery-node")
          .with_local_discovery(5051);

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
      client.start_discovery();

  if (started.is_err())
  {
    std::cerr << started.error().code_string()
              << ": "
              << started.error().message()
              << "\n";

    return 1;
  }

  std::cout << "discovery running: "
            << (client.discovery_running() ? "yes" : "no")
            << "\n";

  client.stop_discovery();
  client.close();

  return 0;
}
```

## Listing discovered peers

Use `peers()` to read the current discovered peer list:

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

If no peers are discovered, the result is valid and the vector is empty.

```cpp
if (peers.is_ok() && peers.value().empty())
{
  std::cout << "no peers discovered\n";
}
```

## Complete peers example

```cpp
#include <softadastra/sdk.hpp>

#include <iostream>

int main()
{
  using namespace softadastra::sdk;

  ClientOptions options =
      ClientOptions::memory_only("node-discovery")
          .with_local_discovery(5051);

  Client client{options};

  if (client.open().is_err())
  {
    return 1;
  }

  const auto started =
      client.start_discovery();

  if (started.is_err())
  {
    std::cerr << started.error().message() << "\n";
    return 1;
  }

  const auto peers =
      client.peers();

  if (peers.is_err())
  {
    std::cerr << peers.error().message() << "\n";

    client.stop_discovery();
    client.close();

    return 1;
  }

  std::cout << "peers: "
            << peers.value().size()
            << "\n";

  for (const auto &peer : peers.value())
  {
    std::cout << peer.node_id()
              << " "
              << peer.host()
              << ":"
              << peer.port()
              << "\n";
  }

  client.stop_discovery();
  client.close();

  return 0;
}
```

## Peer

`Peer` is the public SDK representation of another node.

A peer has:

```txt
node id
host
port
```

Create a peer:

```cpp
Peer peer{
    "node-b",
    "127.0.0.1",
    9101};
```

Create a localhost peer:

```cpp
Peer peer =
    Peer::local("node-b", 9101);
```

A valid peer must have:

```txt
non-empty node id
non-empty host
non-zero port
```

## Discovery and transport

Discovery finds peers.

Transport connects to peers.

Sync tracks operations.

```txt
discovery finds peers
transport connects peers
sync tracks operations
```

Discovery alone does not deliver sync operations.

A typical flow is:

```cpp
ClientOptions options =
    ClientOptions::memory_only("node-a")
        .with_local_discovery(5051)
        .with_local_transport(9100);

Client client{options};

client.open();

client.start_discovery();
client.start_transport();

const auto peers =
    client.peers();

if (peers.is_ok())
{
  for (const auto &peer : peers.value())
  {
    client.connect(peer);
  }
}
```

## Discovery and local-first behavior

Discovery is not required for local correctness.

This still works when discovery is disabled:

```cpp
client.put("key", "value");
```

This also works when discovery has found no peers:

```cpp
const auto state =
    client.sync_state();
```

Discovery is only needed when the application wants to find other nodes automatically.

## Known peers without discovery

If your application already knows peer addresses, you can skip discovery.

```cpp
ClientOptions options =
    ClientOptions::memory_only("node-a")
        .with_local_transport(9100);

Client client{options};

client.open();
client.start_transport();

Peer peer{
    "node-b",
    "192.168.1.20",
    9101};

client.connect(peer);
```

In this case, discovery is not needed.

## Lifecycle rules

Discovery operations require:

```txt
client opened
discovery enabled in ClientOptions
discovery runtime initialized
```

This means:

```cpp
Client client{
    ClientOptions::memory_only("node-1")};

client.start_discovery();
```

returns `Error::Code::InvalidState`, because the client is closed.

This:

```cpp
Client client{
    ClientOptions::memory_only("node-1")};

client.open();
client.start_discovery();
```

returns `Error::Code::DiscoveryError`, because discovery is disabled.

Correct:

```cpp
Client client{
    ClientOptions::memory_only("node-1")
        .with_local_discovery(5051)};

client.open();
client.start_discovery();
```

## Error behavior

Common discovery errors:

| Situation                 | Error code                    |
| ------------------------- | ----------------------------- |
| Client is closed          | `Error::Code::InvalidState`   |
| Discovery is disabled     | `Error::Code::DiscoveryError` |
| Discovery failed to start | `Error::Code::DiscoveryError` |
| Runtime is incomplete     | `Error::Code::InternalError`  |

Example:

```cpp
const auto peers =
    client.peers();

if (peers.is_err())
{
  std::cerr << peers.error().code_string()
            << ": "
            << peers.error().message()
            << "\n";
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
          .with_local_discovery(5051)
          .with_local_transport(9100);

  Client client{options};

  const auto opened =
      client.open();

  if (opened.is_err())
  {
    return 1;
  }

  const auto discovery_started =
      client.start_discovery();

  if (discovery_started.is_err())
  {
    client.close();
    return 1;
  }

  const auto transport_started =
      client.start_transport();

  if (transport_started.is_err())
  {
    client.stop_discovery();
    client.close();
    return 1;
  }

  const auto peers =
      client.peers();

  if (peers.is_ok())
  {
    for (const auto &peer : peers.value())
    {
      client.connect(peer);
    }
  }

  client.stop_discovery();
  client.stop_transport();
  client.close();

  return 0;
}
```

## When to use discovery

Use discovery when your application needs:

```txt
automatic peer discovery
local network peer lookup
node discovery before transport connection
dynamic peer lists
multi-node local-first systems
```

Do not use discovery just to write local data.

For local writes, the store API is enough:

```cpp
client.put("key", "value");
```

## Summary

Discovery is optional.

```txt
local store does not require discovery
persistence does not require discovery
sync tracking does not require discovery
transport can work without discovery
automatic peer lookup requires discovery
```

Use discovery when the SDK client needs to find other nodes automatically.
