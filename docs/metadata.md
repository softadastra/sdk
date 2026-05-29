# Metadata

Metadata is the SDK layer that exposes information about the local Softadastra node.
It gives applications a stable public view of the current node without exposing internal metadata runtime objects.

The main public SDK type is:

```cpp
softadastra::sdk::NodeInfo
```

The main client methods are:

```cpp
client.node_info();
client.refresh_node_info();
```

## Header

Use the umbrella header:

```cpp
#include <softadastra/sdk.hpp>
```

Or include the metadata-related SDK types directly:

```cpp
#include <softadastra/sdk/Client.hpp>
#include <softadastra/sdk/ClientOptions.hpp>
#include <softadastra/sdk/NodeInfo.hpp>
#include <softadastra/sdk/Error.hpp>
```

## What metadata provides

Metadata describes the local node.

A `NodeInfo` can contain:

```txt
node id
display name
hostname
operating system name
runtime version
started timestamp
uptime
capabilities
```

This is useful for:

```txt
debugging
local node inspection
peer identity display
diagnostics
CLI output
runtime dashboards
Converdict reports
```

## Metadata is initialized by default

Unlike transport and discovery, metadata is not an optional service exposed through `ClientOptions`.

When the SDK client opens successfully, the metadata service is built internally.

```cpp
Client client{
    ClientOptions::memory_only("node-1")};

client.open();

const auto info =
    client.node_info();
```

## Set metadata options

Use `with_metadata()` to set the display name and version:

```cpp
ClientOptions options =
    ClientOptions::memory_only("node-1")
        .with_metadata("Local Node", "0.1.0");
```

Then create the client:

```cpp
Client client{options};
```

## Read node info

Use `node_info()`:

```cpp
const auto info =
    client.node_info();

if (info.is_ok())
{
  const auto &node =
      info.value();

  std::cout << node.node_id() << "\n";
  std::cout << node.display_name() << "\n";
  std::cout << node.hostname() << "\n";
  std::cout << node.os_name() << "\n";
  std::cout << node.version() << "\n";
}
```

## Refresh node info

Use `refresh_node_info()` when you want the SDK to refresh runtime metadata before returning it.

```cpp
const auto info =
    client.refresh_node_info();
```

Example:

```cpp
if (info.is_ok())
{
  std::cout << "node: "
            << info.value().label()
            << "\n";
}
```

## Complete metadata example

```cpp
#include <softadastra/sdk.hpp>

#include <iostream>

int main()
{
  using namespace softadastra::sdk;

  ClientOptions options =
      ClientOptions::memory_only("metadata-node")
          .with_metadata("Metadata Node", "0.1.0");

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

  const auto info =
      client.refresh_node_info();

  if (info.is_err())
  {
    std::cerr << info.error().code_string()
              << ": "
              << info.error().message()
              << "\n";

    client.close();

    return 1;
  }

  const auto &node =
      info.value();

  std::cout << "node_id      : "
            << node.node_id()
            << "\n";

  std::cout << "display_name : "
            << node.display_name()
            << "\n";

  std::cout << "label        : "
            << node.label()
            << "\n";

  std::cout << "hostname     : "
            << node.hostname()
            << "\n";

  std::cout << "os_name      : "
            << node.os_name()
            << "\n";

  std::cout << "version      : "
            << node.version()
            << "\n";

  std::cout << "started_at   : "
            << node.started_at_ms()
            << " ms\n";

  std::cout << "uptime       : "
            << node.uptime_ms()
            << " ms\n";

  std::cout << "capabilities : "
            << node.capabilities().size()
            << "\n";

  client.close();

  return 0;
}
```

## NodeInfo

`NodeInfo` is the public SDK representation of local node metadata.

```cpp
NodeInfo info{
    "node-1",
    "Local Node",
    "localhost",
    "linux",
    "0.1.0"};
```

## Node id

The node id is the logical identifier of the local node.

```cpp
node.node_id();
```

It comes from `ClientOptions`:

```cpp
ClientOptions::memory_only("node-1");
```

or:

```cpp
ClientOptions::persistent(
    "node-1",
    "data/sdk-store.wal");
```

## Display name

The display name is a human-readable node label.

```cpp
node.display_name();
```

Set it with:

```cpp
ClientOptions options =
    ClientOptions::memory_only("node-1")
        .with_metadata("My Laptop", "0.1.0");
```

## Label

`label()` returns the best display label.

```cpp
node.label();
```

If `display_name()` is not empty, `label()` returns it.

If `display_name()` is empty, `label()` returns `node_id()`.

```cpp
NodeInfo info{
    "node-1",
    "",
    "localhost",
    "linux",
    "0.1.0"};

info.label(); // "node-1"
```

## Hostname

`hostname()` returns the local machine hostname reported by the metadata layer.

```cpp
node.hostname();
```

This is useful for diagnostics and CLI output.

## OS name

`os_name()` returns the operating system name reported by the metadata layer.

```cpp
node.os_name();
```

Example values can include:

```txt
linux
windows
macos
unknown
```

The exact value depends on the runtime implementation.

## Version

`version()` returns the runtime or product version exposed through metadata.

```cpp
node.version();
```

Set it with:

```cpp
ClientOptions options =
    ClientOptions::memory_only("node-1")
        .with_metadata("Local Node", "0.1.0");
```

## Started timestamp

`started_at_ms()` returns the node start timestamp in milliseconds.

```cpp
node.started_at_ms();
```

This value is useful for diagnostics and reporting.

## Uptime

`uptime_ms()` returns the current node uptime in milliseconds.

```cpp
node.uptime_ms();
```

Use it to show how long the local node has been running.

## Capabilities

Capabilities describe what the node can do.

```cpp
node.capabilities();
```

Check a capability:

```cpp
if (node.has_capability("store"))
{
  // node has store capability
}
```

Add a capability manually when building `NodeInfo` values in tests or reports:

```cpp
NodeInfo info;

info.add_capability("store");
info.add_capability("sync");
```

Set all capabilities:

```cpp
info.set_capabilities({"store", "sync", "transport"});
```

## NodeInfo setters

`NodeInfo` can be built manually for tests, examples, and report generation.

```cpp
NodeInfo info;

info.set_node_id("node-1");
info.set_display_name("Local Node");
info.set_hostname("localhost");
info.set_os_name("linux");
info.set_version("0.1.0");
info.set_started_at_ms(1000);
info.set_uptime_ms(2000);
info.add_capability("store");
```

## Valid node info

A valid `NodeInfo` must have:

```txt
non-empty node id
non-empty hostname
non-empty OS name
non-empty version
```

Check validity:

```cpp
if (info.is_valid())
{
  // usable node info
}
```

Backward-compatible alias:

```cpp
info.valid();
```

## Clear node info

```cpp
info.clear();
```

After clearing, the object becomes invalid.

```cpp
NodeInfo info{
    "node-1",
    "Local Node",
    "localhost",
    "linux",
    "0.1.0"};

info.clear();

info.is_valid(); // false
```

## Closed client behavior

Metadata operations require an open client.

```cpp
Client client{
    ClientOptions::memory_only("node-1")};

const auto info =
    client.node_info();

if (info.is_err())
{
  // Error::Code::InvalidState
}
```

The same rule applies to:

```cpp
client.refresh_node_info();
```

## Metadata and local-first behavior

Metadata is not required for local writes, but it helps identify the local runtime.

This works because the store is local-first:

```cpp
client.put("key", "value");
```

Metadata adds context around that node:

```txt
which node accepted the write?
what machine was it?
what runtime version was running?
what capabilities were available?
```

For Converdict, this is especially useful because reports should identify the node that executed a scenario.

## Metadata and Converdict

Converdict can use SDK metadata to enrich reports.

Example report fields:

```txt
node_id
display_name
hostname
os_name
version
started_at_ms
uptime_ms
capabilities
```

This allows a Converdict run to explain not only what happened, but also where it happened.

## Error behavior

Common metadata errors:

| Situation                | Error code                   |
| ------------------------ | ---------------------------- |
| Client is closed         | `Error::Code::InvalidState`  |
| Metadata service missing | `Error::Code::MetadataError` |
| Metadata refresh failed  | `Error::Code::MetadataError` |
| Runtime is incomplete    | `Error::Code::InternalError` |

Example:

```cpp
const auto info =
    client.refresh_node_info();

if (info.is_err())
{
  std::cerr << info.error().code_string()
            << ": "
            << info.error().message()
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
          .with_metadata("Local Node", "0.1.0");

  Client client{options};

  const auto opened =
      client.open();

  if (opened.is_err())
  {
    return 1;
  }

  const auto info =
      client.refresh_node_info();

  if (info.is_ok())
  {
    const auto &node =
        info.value();

    std::cout << node.label()
              << " running on "
              << node.os_name()
              << "\n";
  }

  client.close();

  return 0;
}
```

## Summary

Metadata gives the SDK a stable public view of the local node.

```txt
node_info()          -> read current metadata
refresh_node_info()  -> refresh and read metadata
NodeInfo             -> public node metadata object
```

Use metadata when you need diagnostics, reports, runtime display, node identity, or Converdict scenario context.
