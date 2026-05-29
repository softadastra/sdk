# Changelog

All notable changes to the Softadastra C++ SDK will be documented in this file.

The format follows a simple release-oriented structure:

```txt
Added
Changed
Fixed
Removed
```

## v0.1.0

### Added

- Added the first stable public C++ SDK foundation for Softadastra.
- Added the public umbrella header:

```cpp
#include <softadastra/sdk.hpp>
```

- Added public SDK version helpers:

```cpp
softadastra::sdk::sdk_version()
softadastra::sdk::sdk_version_major()
softadastra::sdk::sdk_version_minor()
softadastra::sdk::sdk_version_patch()
```

- Added stable public SDK types:

```cpp
softadastra::sdk::Client
softadastra::sdk::ClientOptions
softadastra::sdk::Error
softadastra::sdk::Result
softadastra::sdk::Key
softadastra::sdk::Value
softadastra::sdk::Peer
softadastra::sdk::NodeInfo
softadastra::sdk::SyncState
softadastra::sdk::TickResult
```

- Added `Client` as the main public SDK facade.
- Added `ClientOptions` for memory-only, persistent, durable, and fast runtime modes.
- Added local store operations:

```cpp
client.put(...)
client.get(...)
client.remove(...)
client.contains(...)
client.size()
client.empty()
```

- Added sync inspection and manual sync control:

```cpp
client.sync_state()
client.tick()
client.retry_expired()
client.prune_completed()
client.prune_failed()
```

- Added optional transport support:

```cpp
client.start_transport()
client.stop_transport()
client.transport_running()
client.connect(...)
client.disconnect(...)
```

- Added optional discovery support:

```cpp
client.start_discovery()
client.stop_discovery()
client.discovery_running()
client.peers()
```

- Added metadata support:

```cpp
client.node_info()
client.refresh_node_info()
```

- Added public error model with stable SDK error codes:

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

- Added internal SDK runtime boundary:

```txt
src/internal/Runtime.*
src/internal/RuntimeBuilder.*
src/internal/RuntimeGuards.*
src/internal/ErrorMapper.*
```

- Added internal conversion layer:

```txt
src/conversions/StoreConversions.*
src/conversions/SyncConversions.*
src/conversions/TransportConversions.*
src/conversions/DiscoveryConversions.*
src/conversions/MetadataConversions.*
```

- Added `ClientImpl` to keep the public `Client.hpp` stable and hide internal runtime modules.
- Added examples:

```txt
examples/01_local_store.cpp
examples/02_persistent_store.cpp
examples/03_restart_recovery.cpp
examples/04_sync_state.cpp
examples/05_tick.cpp
examples/06_transport.cpp
examples/07_discovery.cpp
examples/08_metadata.cpp
```

- Added unit tests for public SDK types and client behavior:

```txt
ErrorTests.cpp
KeyTests.cpp
ValueTests.cpp
ClientOptionsTests.cpp
ClientLifecycleTests.cpp
ClientStoreTests.cpp
ClientRecoveryTests.cpp
ClientSyncTests.cpp
ClientTransportTests.cpp
ClientDiscoveryTests.cpp
ClientMetadataTests.cpp
```

- Added documentation:

```txt
docs/getting-started.md
docs/client.md
docs/local-store.md
docs/persistence.md
docs/sync.md
docs/transport.md
docs/discovery.md
docs/metadata.md
docs/errors.md
```

- Added CMake install and package export support.
- Added CMake package config template:

```txt
cmake/sdk-cpp-config.cmake.in
```

### Changed

- Reorganized the SDK around a clean public/private boundary.
- Moved internal Softadastra runtime usage behind `ClientImpl`.
- Kept public headers focused on stable SDK types.
- Removed direct exposure of internal runtime modules from the public `Client` interface.
- Renamed the public sync snapshot concept to `SyncState` instead of `SyncResult`.
- Aligned examples with the new SDK structure and naming.
- Updated CMake sources to include the new public types, internal runtime files, conversion files, examples, and tests.
- Added the `softadastra::sdk` CMake alias target in addition to `sdk-cpp::sdk-cpp`.

### Fixed

- Fixed the SDK architecture so Converdict can depend on the SDK instead of depending directly on internal Softadastra modules.
- Fixed public API stability by hiding `StoreEngine`, `SyncEngine`, `SyncScheduler`, `TransportEngine`, `DiscoveryService`, and `MetadataService` behind the SDK facade.
- Fixed error boundaries by mapping internal errors to stable SDK-level errors.
- Fixed the example list to match the new example files.
- Fixed test organization to match the stabilized SDK API.

### Removed

- Removed the old design where public SDK headers could expose too many internal Softadastra runtime details.
- Removed old example naming from the CMake configuration:

```txt
03_remove_value
04_basic_sync
05_tcp_peer_sync
06_discovery
07_node_metadata
```

- Removed the old `SyncResult` public naming in favor of `SyncState`.
