# Local Store

The local store is the simplest part of the Softadastra C++ SDK.
It lets an application write, read, remove, and inspect values locally through `softadastra::sdk::Client`.
The important rule is:

```txt
local writes do not require the network
```

A Softadastra application can accept data locally even when transport and discovery are disabled.

## Header

Use the umbrella header:

```cpp
#include <softadastra/sdk.hpp>
```

Or include the store-related SDK types directly:

```cpp
#include <softadastra/sdk/Client.hpp>
#include <softadastra/sdk/ClientOptions.hpp>
#include <softadastra/sdk/Key.hpp>
#include <softadastra/sdk/Value.hpp>
```

## Basic store example

```cpp
#include <softadastra/sdk.hpp>

#include <iostream>

int main()
{
  using namespace softadastra::sdk;

  Client client{
      ClientOptions::memory_only("local-store-node")};

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
      client.put("hello", "world");

  if (stored.is_err())
  {
    std::cerr << stored.error().code_string()
              << ": "
              << stored.error().message()
              << "\n";

    return 1;
  }

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

## Store operations

The SDK exposes these local store operations:

```cpp
client.put("key", "value");
client.get("key");
client.remove("key");
client.contains("key");
client.size();
client.empty();
```

These methods operate on the local store through the SDK client.

## Opening the client first

A client must be opened before store operations are used.

```cpp
Client client{
    ClientOptions::memory_only("node-1")};

const auto opened =
    client.open();

if (opened.is_err())
{
  return 1;
}
```

Calling `put`, `get`, or `remove` before `open()` returns an `InvalidState` error.

```cpp
Client client;

const auto result =
    client.put("name", "Softadastra");

if (result.is_err())
{
  // Error::Code::InvalidState
}
```

## Writing values

Use `put()` to write a key/value pair.

```cpp
const auto result =
    client.put("name", "Softadastra");

if (result.is_err())
{
  std::cerr << result.error().message() << "\n";
}
```

The key must not be empty.

```cpp
const auto result =
    client.put("", "value");

// result.error().code() == Error::Code::InvalidArgument
```

## Reading values

Use `get()` to read a value from the local store.

```cpp
const auto value =
    client.get("name");

if (value.is_ok())
{
  std::cout << value.value().to_string() << "\n";
}
```

If the key does not exist, the SDK returns `Error::Code::NotFound`.

```cpp
const auto value =
    client.get("missing");

if (value.is_err())
{
  // value.error().code() == Error::Code::NotFound
}
```

## Removing values

Use `remove()` to delete a key from the local store.

```cpp
const auto removed =
    client.remove("name");

if (removed.is_err())
{
  std::cerr << removed.error().message() << "\n";
}
```

Removing a missing key is allowed.

The operation is still submitted to the local runtime as a local remove operation.

## Checking if a key exists

Use `contains()`:

```cpp
if (client.contains("name"))
{
  std::cout << "name exists\n";
}
```

`contains()` returns `false` when:

```txt
the client is closed
the key is empty
the key does not exist
```

## Counting entries

Use `size()`:

```cpp
const std::size_t count =
    client.size();
```

Use `empty()`:

```cpp
if (client.empty())
{
  std::cout << "store is empty\n";
}
```

When the client is closed:

```txt
size()  returns 0
empty() returns true
```

## Using Key and Value

The string overloads are convenient:

```cpp
client.put("language", "C++");
```

For explicit SDK types, use `Key` and `Value`:

```cpp
Key key{"profile/name"};
Value value{"Ada"};

const auto stored =
    client.put(key, value);
```

Then read it back:

```cpp
const auto result =
    client.get(key);

if (result.is_ok())
{
  std::cout << result.value().to_string() << "\n";
}
```

## Key rules

A `Key` is a stable string-based identifier.

```cpp
Key key{"profile/name"};
```

A valid key must not be empty:

```cpp
Key empty_key{};

if (!empty_key.is_valid())
{
  // invalid key
}
```

Useful methods:

```cpp
key.str();
key.value();
key.empty();
key.is_valid();
key.clear();
```

## Value rules

A `Value` is binary-safe.

It stores raw bytes.

```cpp
Value value{"hello"};
```

Text is stored as bytes:

```cpp
Value text =
    Value::from_string("Softadastra");
```

Binary data is supported:

```cpp
Value binary =
    Value::from_bytes({
        static_cast<std::uint8_t>(0x01),
        static_cast<std::uint8_t>(0x02),
        static_cast<std::uint8_t>(0x03)});
```

Useful methods:

```cpp
value.bytes();
value.data();
value.span();
value.size();
value.empty();
value.to_string();
value.clear();
```

`to_string()` does not validate UTF-8. It simply creates a `std::string` from the stored bytes.

## Empty values

Empty values are allowed.

```cpp
Value empty_value;

client.put("empty", empty_value);
```

An empty value is different from a missing key.

```txt
empty value = key exists, value has zero bytes
missing key = key does not exist
```

## Binary-safe example

```cpp
#include <softadastra/sdk.hpp>

#include <cstdint>
#include <iostream>

int main()
{
  using namespace softadastra::sdk;

  Client client{
      ClientOptions::memory_only("binary-node")};

  if (client.open().is_err())
  {
    return 1;
  }

  Value value{
      Value::Container{
          static_cast<std::uint8_t>('a'),
          static_cast<std::uint8_t>(0),
          static_cast<std::uint8_t>('b')}};

  client.put(Key{"binary"}, value);

  const auto result =
      client.get("binary");

  if (result.is_ok())
  {
    std::cout << "size: "
              << result.value().size()
              << "\n";
  }

  client.close();

  return 0;
}
```

Expected output:

```txt
size: 3
```

## Overwriting values

Calling `put()` on an existing key replaces the previous value.

```cpp
client.put("name", "first");
client.put("name", "second");

const auto value =
    client.get("name");

// value == "second"
```

## Local-first behavior

A local store write does not wait for a remote server.

```cpp
client.put("message", "hello");
```

This operation is accepted locally first.

In persistent mode, the write is also backed by the local WAL.

In both modes, the write is submitted to the sync pipeline so it can be synchronized later when peers are available.

## Memory-only store

Memory-only mode is useful for examples and tests:

```cpp
Client client{
    ClientOptions::memory_only("node-memory")};
```

In memory-only mode:

```txt
writes are local
writes are not persisted to a WAL file
data disappears after close/restart
sync state can still track local operations
```

## Persistent store

Persistent mode uses a WAL path:

```cpp
Client client{
    ClientOptions::persistent(
        "node-persistent",
        "data/sdk-store.wal")};
```

In persistent mode:

```txt
writes are local
writes are persisted through the WAL
data can be recovered after restart
sync state can still track local operations
```

## Restart recovery example

```cpp
#include <softadastra/sdk.hpp>

#include <iostream>
#include <string>

int main()
{
  using namespace softadastra::sdk;

  const std::string wal_path =
      "data/local-store-recovery.wal";

  {
    Client writer{
        ClientOptions::persistent(
            "writer-node",
            wal_path)};

    if (writer.open().is_err())
    {
      return 1;
    }

    writer.put("status", "saved");
    writer.close();
  }

  {
    Client reader{
        ClientOptions::persistent(
            "reader-node",
            wal_path)};

    if (reader.open().is_err())
    {
      return 1;
    }

    const auto status =
        reader.get("status");

    if (status.is_ok())
    {
      std::cout << status.value().to_string() << "\n";
    }

    reader.close();
  }

  return 0;
}
```

Expected output:

```txt
saved
```

## Error behavior

Common local store errors:

| Situation              | Error code                                                |
| ---------------------- | --------------------------------------------------------- |
| Client is closed       | `Error::Code::InvalidState`                               |
| Key is empty           | `Error::Code::InvalidArgument`                            |
| Key does not exist     | `Error::Code::NotFound`                                   |
| Internal store failure | `Error::Code::StoreError` or `Error::Code::InternalError` |

Example:

```cpp
const auto value =
    client.get("missing");

if (value.is_err())
{
  std::cerr << value.error().code_string()
            << ": "
            << value.error().message()
            << "\n";
}
```

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
    client.put("key", "value");

if (stored.is_err())
{
  client.close();
  return 1;
}

const auto value =
    client.get("key");

if (value.is_ok())
{
  std::cout << value.value().to_string() << "\n";
}

client.close();
```

## When to use the local store

Use the local store when an application needs:

```txt
offline writes
local state
restart recovery
simple key/value persistence
syncable local operations
binary-safe values
```

The local store is the first layer that makes Softadastra useful without any network.
