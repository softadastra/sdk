/**
 *
 *  @file Client.hpp
 *  @author Softadastra
 *
 *  Copyright 2026, Softadastra.
 *  All rights reserved.
 *  https://github.com/softadastra/sdk.git
 *
 *  Licensed under the Apache License, Version 2.0.
 *
 *  Softadastra C++ SDK
 *
 */

#ifndef SOFTADASTRA_SDK_CLIENT_HPP
#define SOFTADASTRA_SDK_CLIENT_HPP

#include <cstddef>
#include <memory>
#include <string>
#include <vector>

#include <softadastra/sdk/ClientOptions.hpp>
#include <softadastra/sdk/Error.hpp>
#include <softadastra/sdk/Key.hpp>
#include <softadastra/sdk/NodeInfo.hpp>
#include <softadastra/sdk/Peer.hpp>
#include <softadastra/sdk/Result.hpp>
#include <softadastra/sdk/SyncState.hpp>
#include <softadastra/sdk/TickResult.hpp>
#include <softadastra/sdk/Value.hpp>

namespace softadastra::sdk
{
  /**
   * @brief High-level Softadastra C++ SDK client.
   *
   * Client is the main public entry point of the Softadastra C++ SDK.
   *
   * It exposes a small, stable developer-facing API for:
   * - local key/value writes
   * - WAL-backed persistence
   * - restart recovery
   * - sync state inspection
   * - manual sync ticks
   * - optional async transport
   * - optional discovery
   * - node metadata
   *
   * Internal Softadastra modules such as StoreEngine, SyncEngine,
   * SyncScheduler, TransportEngine, AsyncTcpTransportBackend,
   * DiscoveryService, and MetadataService are hidden behind the SDK
   * implementation.
   */
  class Client
  {
  public:
    /**
     * @brief Result returned by operations without a value.
     */
    using VoidResult = Result<void, Error>;

    /**
     * @brief Result returned by value lookup operations.
     */
    using ValueResult = Result<Value, Error>;

    /**
     * @brief Result returned by sync state inspection.
     */
    using SyncStateResult = Result<SyncState, Error>;

    /**
     * @brief Result returned by sync tick operations.
     */
    using TickStateResult = Result<TickResult, Error>;

    /**
     * @brief Result returned by node metadata operations.
     */
    using NodeInfoResult = Result<NodeInfo, Error>;

    /**
     * @brief Result returned by peer listing operations.
     */
    using PeersResult = Result<std::vector<Peer>, Error>;

    /**
     * @brief Result returned by async transport event processing.
     */
    using TransportEventsResult = Result<std::size_t, Error>;

    /**
     * @brief Creates a client with default options.
     */
    Client();

    /**
     * @brief Creates a client with explicit options.
     *
     * @param options SDK client options.
     */
    explicit Client(ClientOptions options);

    /**
     * @brief Stops owned services before destruction.
     */
    ~Client();

    Client(const Client &) = delete;
    Client &operator=(const Client &) = delete;

    /**
     * @brief Moves a client.
     */
    Client(Client &&) noexcept;

    /**
     * @brief Move-assigns a client.
     */
    Client &operator=(Client &&) noexcept;

    /**
     * @brief Opens and initializes the SDK client.
     *
     * This builds the internal Softadastra runtime according to ClientOptions.
     *
     * @return ok on success, error on failure.
     */
    [[nodiscard]] VoidResult open();

    /**
     * @brief Closes the SDK client and stops optional services.
     */
    void close() noexcept;

    /**
     * @brief Returns true if the client is open.
     *
     * @return true if open.
     */
    [[nodiscard]] bool is_open() const noexcept;

    /**
     * @brief Backward-compatible alias for is_open().
     *
     * @return true if open.
     */
    [[nodiscard]] bool opened() const noexcept;

    /**
     * @brief Stores a value for a key.
     *
     * The write is accepted locally through the SDK runtime. Network access is
     * not required for local correctness.
     *
     * @param key SDK key.
     * @param value SDK value.
     * @return ok on success, error on failure.
     */
    [[nodiscard]] VoidResult put(
        const Key &key,
        const Value &value);

    /**
     * @brief Stores a string value for a string key.
     *
     * Convenience overload for text payloads.
     *
     * @param key Key string.
     * @param value Value string.
     * @return ok on success, error on failure.
     */
    [[nodiscard]] VoidResult put(
        std::string key,
        std::string value);

    /**
     * @brief Reads a value from the local store.
     *
     * @param key SDK key.
     * @return value on success, error on failure.
     */
    [[nodiscard]] ValueResult get(const Key &key) const;

    /**
     * @brief Reads a value from the local store by string key.
     *
     * @param key Key string.
     * @return value on success, error on failure.
     */
    [[nodiscard]] ValueResult get(const std::string &key) const;

    /**
     * @brief Removes a value from the local store.
     *
     * @param key SDK key.
     * @return ok on success, error on failure.
     */
    [[nodiscard]] VoidResult remove(const Key &key);

    /**
     * @brief Removes a value from the local store by string key.
     *
     * @param key Key string.
     * @return ok on success, error on failure.
     */
    [[nodiscard]] VoidResult remove(const std::string &key);

    /**
     * @brief Returns true if the local store contains a key.
     *
     * This method returns false when the client is closed.
     *
     * @param key SDK key.
     * @return true if present.
     */
    [[nodiscard]] bool contains(const Key &key) const;

    /**
     * @brief Returns true if the local store contains a key.
     *
     * This method returns false when the client is closed.
     *
     * @param key Key string.
     * @return true if present.
     */
    [[nodiscard]] bool contains(const std::string &key) const;

    /**
     * @brief Returns the number of entries in the local store.
     *
     * This method returns zero when the client is closed.
     *
     * @return Entry count.
     */
    [[nodiscard]] std::size_t size() const noexcept;

    /**
     * @brief Returns true if the local store is empty.
     *
     * @return true if empty.
     */
    [[nodiscard]] bool empty() const noexcept;

    /**
     * @brief Advances the sync pipeline once.
     *
     * This is deterministic and manual. It retries expired operations, collects
     * a batch ready for transport, optionally prunes completed entries, and
     * processes queued async transport events when transport is enabled.
     *
     * @param prune_completed true to prune completed entries.
     * @return tick result on success, error on failure.
     */
    [[nodiscard]] TickStateResult tick(
        bool prune_completed = false);

    /**
     * @brief Returns a compact synchronization state.
     *
     * @return sync state on success, error on failure.
     */
    [[nodiscard]] SyncStateResult sync_state() const;

    /**
     * @brief Retries expired sync operations.
     *
     * @return number of retried entries on success, error on failure.
     */
    [[nodiscard]] Result<std::size_t, Error> retry_expired();

    /**
     * @brief Prunes completed sync entries.
     *
     * @return number of pruned entries on success, error on failure.
     */
    [[nodiscard]] Result<std::size_t, Error> prune_completed();

    /**
     * @brief Prunes failed sync entries.
     *
     * @return number of pruned entries on success, error on failure.
     */
    [[nodiscard]] Result<std::size_t, Error> prune_failed();

    /**
     * @brief Starts optional async transport support.
     *
     * Requires ClientOptions transport support to be enabled.
     *
     * @return ok on success, error on failure.
     */
    [[nodiscard]] VoidResult start_transport();

    /**
     * @brief Stops transport if it is available.
     */
    void stop_transport() noexcept;

    /**
     * @brief Returns true if transport is enabled and running.
     *
     * @return true if transport is running.
     */
    [[nodiscard]] bool transport_running() const noexcept;

    /**
     * @brief Processes queued async transport backend events.
     *
     * The async TCP transport backend produces transport events. The SDK keeps
     * those backend details hidden and exposes this method so applications can
     * explicitly advance event processing when they run transport manually.
     *
     * This method is useful after:
     * - start_transport()
     * - connect()
     * - disconnect()
     * - tick()
     * - any application-level wait or loop
     *
     * @param max_events Maximum number of backend events to process.
     * @return number of handled events on success, error on failure.
     */
    [[nodiscard]] TransportEventsResult process_transport_events(
        std::size_t max_events = 64);

    /**
     * @brief Connects to a peer through transport.
     *
     * Requires transport support to be enabled.
     *
     * @param peer Remote peer.
     * @return ok on success, error on failure.
     */
    [[nodiscard]] VoidResult connect(const Peer &peer);

    /**
     * @brief Disconnects from a peer through transport.
     *
     * Requires transport support to be enabled.
     *
     * @param peer Remote peer.
     * @return ok on success, error on failure.
     */
    [[nodiscard]] VoidResult disconnect(const Peer &peer);

    /**
     * @brief Starts optional discovery support.
     *
     * Requires ClientOptions discovery support to be enabled.
     *
     * @return ok on success, error on failure.
     */
    [[nodiscard]] VoidResult start_discovery();

    /**
     * @brief Stops discovery if it is available.
     */
    void stop_discovery() noexcept;

    /**
     * @brief Returns true if discovery is enabled and running.
     *
     * @return true if discovery is running.
     */
    [[nodiscard]] bool discovery_running() const noexcept;

    /**
     * @brief Returns discovered peers.
     *
     * Requires discovery support to be enabled.
     *
     * @return peer list on success, error on failure.
     */
    [[nodiscard]] PeersResult peers() const;

    /**
     * @brief Returns local node metadata.
     *
     * @return node info on success, error on failure.
     */
    [[nodiscard]] NodeInfoResult node_info() const;

    /**
     * @brief Refreshes local node metadata and returns it.
     *
     * @return node info on success, error on failure.
     */
    [[nodiscard]] NodeInfoResult refresh_node_info();

    /**
     * @brief Returns the SDK options used by this client.
     *
     * @return client options.
     */
    [[nodiscard]] const ClientOptions &options() const noexcept;

  private:
    class Impl;

    std::unique_ptr<Impl> impl_{};
  };

} // namespace softadastra::sdk

#endif // SOFTADASTRA_SDK_CLIENT_HPP
