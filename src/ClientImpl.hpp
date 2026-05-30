/**
 *
 *  @file ClientImpl.hpp
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

#ifndef SOFTADASTRA_SDK_CLIENT_IMPL_HPP
#define SOFTADASTRA_SDK_CLIENT_IMPL_HPP

#include <cstddef>
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

#include "internal/Runtime.hpp"

namespace softadastra::sdk
{
  /**
   * @brief Private implementation of the public SDK client.
   *
   * ClientImpl owns the real SDK behavior and talks to the internal
   * Softadastra runtime modules.
   *
   * This class is intentionally not part of the installed public SDK API.
   * Public users should only include and use softadastra::sdk::Client.
   */
  class ClientImpl
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
     * @brief Result returned by transport event processing.
     */
    using TransportEventsResult = Result<std::size_t, Error>;

    /**
     * @brief Creates an implementation with default options.
     */
    ClientImpl();

    /**
     * @brief Creates an implementation with explicit options.
     *
     * @param options SDK client options.
     */
    explicit ClientImpl(ClientOptions options);

    /**
     * @brief Stops services before destruction.
     */
    ~ClientImpl();

    ClientImpl(const ClientImpl &) = delete;
    ClientImpl &operator=(const ClientImpl &) = delete;

    /**
     * @brief Moves a client implementation.
     */
    ClientImpl(ClientImpl &&) noexcept;

    /**
     * @brief Move-assigns a client implementation.
     */
    ClientImpl &operator=(ClientImpl &&) noexcept;

    /**
     * @brief Opens the SDK runtime.
     *
     * @return ok on success, error on failure.
     */
    [[nodiscard]] VoidResult open();

    /**
     * @brief Closes the SDK runtime and stops optional services.
     */
    void close() noexcept;

    /**
     * @brief Returns true if the SDK runtime is open.
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
     * @brief Reads a value from the local store.
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
     * @brief Removes a value from the local store.
     *
     * @param key Key string.
     * @return ok on success, error on failure.
     */
    [[nodiscard]] VoidResult remove(const std::string &key);

    /**
     * @brief Returns true if the local store contains a key.
     *
     * @param key SDK key.
     * @return true if present.
     */
    [[nodiscard]] bool contains(const Key &key) const;

    /**
     * @brief Returns true if the local store contains a key.
     *
     * @param key Key string.
     * @return true if present.
     */
    [[nodiscard]] bool contains(const std::string &key) const;

    /**
     * @brief Returns the local store size.
     *
     * @return Number of entries.
     */
    [[nodiscard]] std::size_t size() const noexcept;

    /**
     * @brief Returns true if the local store is empty.
     *
     * @return true if empty.
     */
    [[nodiscard]] bool empty() const noexcept;

    /**
     * @brief Runs one manual synchronization tick.
     *
     * @param prune_completed true to prune completed entries.
     * @return tick result on success, error on failure.
     */
    [[nodiscard]] TickStateResult tick(
        bool prune_completed = false);

    /**
     * @brief Returns the current synchronization state.
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
     * @brief Starts transport support.
     *
     * @return ok on success, error on failure.
     */
    [[nodiscard]] VoidResult start_transport();

    /**
     * @brief Stops transport support.
     */
    void stop_transport() noexcept;

    /**
     * @brief Returns true if transport is running.
     *
     * @return true if running.
     */
    [[nodiscard]] bool transport_running() const noexcept;

    /**
     * @brief Processes queued async transport backend events.
     *
     * The async TCP transport backend produces TransportEvent objects.
     * TransportEngine remains responsible for consuming those events, updating
     * peer state, and dispatching inbound messages.
     *
     * @param max_events Maximum number of events to process.
     * @return number of handled events on success, error on failure.
     */
    [[nodiscard]] TransportEventsResult process_transport_events(
        std::size_t max_events = 64);

    /**
     * @brief Connects to a peer.
     *
     * @param peer Remote peer.
     * @return ok on success, error on failure.
     */
    [[nodiscard]] VoidResult connect(const Peer &peer);

    /**
     * @brief Disconnects from a peer.
     *
     * @param peer Remote peer.
     * @return ok on success, error on failure.
     */
    [[nodiscard]] VoidResult disconnect(const Peer &peer);

    /**
     * @brief Starts discovery support.
     *
     * @return ok on success, error on failure.
     */
    [[nodiscard]] VoidResult start_discovery();

    /**
     * @brief Stops discovery support.
     */
    void stop_discovery() noexcept;

    /**
     * @brief Returns true if discovery is running.
     *
     * @return true if running.
     */
    [[nodiscard]] bool discovery_running() const noexcept;

    /**
     * @brief Returns discovered peers.
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
     * @brief Refreshes and returns local node metadata.
     *
     * @return node info on success, error on failure.
     */
    [[nodiscard]] NodeInfoResult refresh_node_info();

    /**
     * @brief Returns the options used by the client.
     *
     * @return client options.
     */
    [[nodiscard]] const ClientOptions &options() const noexcept;

  private:
    /**
     * @brief SDK options.
     */
    ClientOptions options_{};

    /**
     * @brief Internal runtime owned by the SDK implementation.
     */
    internal::Runtime runtime_{};

    /**
     * @brief Whether the client is currently open.
     */
    bool open_{false};
  };

} // namespace softadastra::sdk

#endif // SOFTADASTRA_SDK_CLIENT_IMPL_HPP
