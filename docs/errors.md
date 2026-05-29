# Errors

The Softadastra C++ SDK uses explicit error handling.
SDK operations return `Result<T, Error>` instead of throwing exceptions for normal runtime failures.

This makes application code predictable and makes failure states easy to inspect.

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
```

## Header

Use the umbrella header:

```cpp
#include <softadastra/sdk.hpp>
```

Or include the error-related SDK types directly:

```cpp
#include <softadastra/sdk/Error.hpp>
#include <softadastra/sdk/Result.hpp>
```

## Public error type

The public SDK error type is:

```cpp
softadastra::sdk::Error
```

It contains:

```txt
error code
message
optional context
```

Example:

```cpp
Error error =
    Error::invalid_argument(
        "key cannot be empty",
        "key");
```

## Public result type

The public SDK result type is:

```cpp
softadastra::sdk::Result<T, Error>
```

Example:

```cpp
Result<Value, Error>
```

For operations that do not return a value, the SDK uses:

```cpp
Result<void, Error>
```

Example:

```cpp
const auto stored =
    client.put("name", "Softadastra");

if (stored.is_err())
{
  std::cerr << stored.error().message() << "\n";
}
```

## Error codes

The SDK exposes stable public error codes.

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

These codes are part of the public SDK API.

Internal Softadastra module errors are mapped to these SDK-level codes before they reach SDK users.

## Error fields

### code

```cpp
error.code();
```

Returns the machine-readable error code.

Example:

```cpp
if (error.code() == Error::Code::InvalidArgument)
{
  // invalid input
}
```

### message

```cpp
error.message();
```

Returns a developer-facing error message.

Example:

```cpp
std::cerr << error.message() << "\n";
```

### context

```cpp
error.context();
```

Returns optional context.

Example:

```cpp
Error error =
    Error::not_found(
        "key not found",
        "profile/name");

error.context(); // "profile/name"
```

### has_context

```cpp
error.has_context();
```

Returns `true` when the error has diagnostic context.

## Checking success or failure

### ok

```cpp
error.ok();
```

Returns `true` when the error code is `Error::Code::None`.

### has_error

```cpp
error.has_error();
```

Returns `true` when the error code is not `Error::Code::None`.

Example:

```cpp
Error error =
    Error::invalid_state("client is closed");

if (error.has_error())
{
  std::cerr << error.message() << "\n";
}
```

## Stable string names

Use `code_string()` to get a stable string representation of the error code.

```cpp
std::cout << error.code_string() << "\n";
```

Example output:

```txt
invalid_argument
```

You can also call:

```cpp
Error::to_string(Error::Code::InvalidArgument);
```

## Creating errors

### none

```cpp
Error error =
    Error::none();
```

Creates a success-like error.

### make

```cpp
Error error =
    Error::make(
        Error::Code::InvalidArgument,
        "invalid input",
        "field:name");
```

Creates an error from explicit fields.

### unknown

```cpp
Error error =
    Error::unknown("unknown failure");
```

### invalid_argument

```cpp
Error error =
    Error::invalid_argument(
        "key cannot be empty");
```

Use this when the caller passed invalid input.

### invalid_state

```cpp
Error error =
    Error::invalid_state(
        "SDK client is not open");
```

Use this when the object is in the wrong state for the requested operation.

### not_found

```cpp
Error error =
    Error::not_found(
        "key not found",
        "profile/name");
```

Use this when a requested item does not exist.

### already_exists

```cpp
Error error =
    Error::already_exists(
        "node already exists",
        "node-1");
```

### io_error

```cpp
Error error =
    Error::io_error(
        "failed to read WAL file",
        "data/sdk-store.wal");
```

Use this for file or system I/O failures.

### store_error

```cpp
Error error =
    Error::store_error(
        "store operation failed");
```

Use this for local store failures.

### sync_error

```cpp
Error error =
    Error::sync_error(
        "sync operation failed");
```

Use this for sync pipeline failures.

### transport_error

```cpp
Error error =
    Error::transport_error(
        "transport is disabled");
```

Use this for transport failures.

### discovery_error

```cpp
Error error =
    Error::discovery_error(
        "discovery is disabled");
```

Use this for discovery failures.

### metadata_error

```cpp
Error error =
    Error::metadata_error(
        "metadata service is not initialized");
```

Use this for metadata failures.

### internal_error

```cpp
Error error =
    Error::internal_error(
        "SDK runtime is not initialized");
```

Use this when the SDK reaches an unexpected internal state.

## Error code strings

| Code                           | String             |
| ------------------------------ | ------------------ |
| `Error::Code::None`            | `none`             |
| `Error::Code::Unknown`         | `unknown`          |
| `Error::Code::InvalidArgument` | `invalid_argument` |
| `Error::Code::InvalidState`    | `invalid_state`    |
| `Error::Code::NotFound`        | `not_found`        |
| `Error::Code::AlreadyExists`   | `already_exists`   |
| `Error::Code::IoError`         | `io_error`         |
| `Error::Code::StoreError`      | `store_error`      |
| `Error::Code::SyncError`       | `sync_error`       |
| `Error::Code::TransportError`  | `transport_error`  |
| `Error::Code::DiscoveryError`  | `discovery_error`  |
| `Error::Code::MetadataError`   | `metadata_error`   |
| `Error::Code::InternalError`   | `internal_error`   |

## Result usage

### Operation with no returned value

`put()` returns `Result<void, Error>`.

```cpp
const auto result =
    client.put("name", "Softadastra");

if (result.is_err())
{
  std::cerr << result.error().code_string()
            << ": "
            << result.error().message()
            << "\n";

  return 1;
}
```

### Operation with returned value

`get()` returns `Result<Value, Error>`.

```cpp
const auto result =
    client.get("name");

if (result.is_err())
{
  std::cerr << result.error().code_string()
            << ": "
            << result.error().message()
            << "\n";

  return 1;
}

std::cout << result.value().to_string() << "\n";
```

### Operation with counter result

`prune_completed()` returns `Result<std::size_t, Error>`.

```cpp
const auto pruned =
    client.prune_completed();

if (pruned.is_ok())
{
  std::cout << "pruned: "
            << pruned.value()
            << "\n";
}
```

## Common SDK errors

| Situation                              | Error code                     |
| -------------------------------------- | ------------------------------ |
| Client used before `open()`            | `Error::Code::InvalidState`    |
| Empty key passed to `put()`            | `Error::Code::InvalidArgument` |
| Empty key passed to `get()`            | `Error::Code::InvalidArgument` |
| Missing key passed to `get()`          | `Error::Code::NotFound`        |
| Transport method called while disabled | `Error::Code::TransportError`  |
| Discovery method called while disabled | `Error::Code::DiscoveryError`  |
| Metadata service unavailable           | `Error::Code::MetadataError`   |
| Runtime object missing unexpectedly    | `Error::Code::InternalError`   |

## Closed client example

```cpp
#include <softadastra/sdk.hpp>

#include <iostream>

int main()
{
  using namespace softadastra::sdk;

  Client client;

  const auto result =
      client.put("name", "Softadastra");

  if (result.is_err())
  {
    std::cout << result.error().code_string()
              << ": "
              << result.error().message()
              << "\n";
  }

  return 0;
}
```

Expected output:

```txt
invalid_state: SDK client is not open
```

## Missing key example

```cpp
#include <softadastra/sdk.hpp>

#include <iostream>

int main()
{
  using namespace softadastra::sdk;

  Client client{
      ClientOptions::memory_only("node-errors")};

  if (client.open().is_err())
  {
    return 1;
  }

  const auto value =
      client.get("missing");

  if (value.is_err())
  {
    std::cout << value.error().code_string()
              << ": "
              << value.error().message();

    if (value.error().has_context())
    {
      std::cout << " ("
                << value.error().context()
                << ")";
    }

    std::cout << "\n";
  }

  client.close();

  return 0;
}
```

Expected output:

```txt
not_found: key not found (missing)
```

## Empty key example

```cpp
#include <softadastra/sdk.hpp>

#include <iostream>

int main()
{
  using namespace softadastra::sdk;

  Client client{
      ClientOptions::memory_only("node-empty-key")};

  if (client.open().is_err())
  {
    return 1;
  }

  const auto result =
      client.put("", "value");

  if (result.is_err())
  {
    std::cout << result.error().code_string()
              << ": "
              << result.error().message()
              << "\n";
  }

  client.close();

  return 0;
}
```

Expected output:

```txt
invalid_argument: key cannot be empty
```

## Transport disabled example

```cpp
#include <softadastra/sdk.hpp>

#include <iostream>

int main()
{
  using namespace softadastra::sdk;

  Client client{
      ClientOptions::memory_only("node-no-transport")};

  if (client.open().is_err())
  {
    return 1;
  }

  const auto result =
      client.start_transport();

  if (result.is_err())
  {
    std::cout << result.error().code_string()
              << ": "
              << result.error().message()
              << "\n";
  }

  client.close();

  return 0;
}
```

Expected output:

```txt
transport_error: transport is disabled
```

## Discovery disabled example

```cpp
#include <softadastra/sdk.hpp>

#include <iostream>

int main()
{
  using namespace softadastra::sdk;

  Client client{
      ClientOptions::memory_only("node-no-discovery")};

  if (client.open().is_err())
  {
    return 1;
  }

  const auto result =
      client.start_discovery();

  if (result.is_err())
  {
    std::cout << result.error().code_string()
              << ": "
              << result.error().message()
              << "\n";
  }

  client.close();

  return 0;
}
```

Expected output:

```txt
discovery_error: discovery is disabled
```

## Clearing errors

An error can be reset:

```cpp
Error error =
    Error::invalid_state("client is closed");

error.clear();

error.ok(); // true
```

After `clear()`:

```txt
code    = none
message = empty
context = empty
```

## Comparing errors

Errors compare by:

```txt
code
message
context
```

Example:

```cpp
Error a =
    Error::invalid_state("client is closed");

Error b =
    Error::invalid_state("client is closed");

if (a == b)
{
  // same error
}
```

## Recommended pattern

For operations without a return value:

```cpp
const auto result =
    client.put("key", "value");

if (result.is_err())
{
  std::cerr << result.error().code_string()
            << ": "
            << result.error().message()
            << "\n";

  return 1;
}
```

For operations with a return value:

```cpp
const auto result =
    client.get("key");

if (result.is_err())
{
  std::cerr << result.error().code_string()
            << ": "
            << result.error().message()
            << "\n";

  return 1;
}

const auto &value =
    result.value();
```

For optional context:

```cpp
if (result.error().has_context())
{
  std::cerr << "context: "
            << result.error().context()
            << "\n";
}
```

## Why the SDK uses explicit errors

The SDK is built for local-first and distributed systems.

In those systems, failure is normal:

```txt
files may be missing
the client may be closed
the network may be unavailable
peers may be unreachable
discovery may find no nodes
sync may need retries
```

Explicit errors make those states visible.

The SDK does not hide them.

## Summary

The SDK error model is simple:

```txt
Result<T, Error>       -> operation can succeed or fail
Result<void, Error>    -> operation can fail without returning a value
Error::Code            -> stable machine-readable code
message()              -> developer-facing explanation
context()              -> optional diagnostic detail
code_string()          -> stable string form
```

Use this model everywhere in SDK-based applications and tools such as Converdict.
