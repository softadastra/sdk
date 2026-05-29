/**
 *
 *  @file ClientOptions.hpp
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

#ifndef SOFTADASTRA_SDK_CLIENT_OPTIONS_HPP
#define SOFTADASTRA_SDK_CLIENT_OPTIONS_HPP

#include <cstddef>
#include <cstdint>
#include <string>

namespace softadastra::sdk
{
  /**
   * @brief Public SDK client configuration.
   *
   * ClientOptions describes how a Softadastra SDK client should be initialized.
   *
   * This type is part of the stable public SDK API. It intentionally avoids
   * exposing internal Softadastra runtime configuration objects such as store,
   * sync, transport, discovery, or metadata configs.
   *
   * Internal configuration objects are built from ClientOptions inside the SDK
   * implementation.
   */
  class ClientOptions
  {
  public:
    /**
     * @brief Creates default durable client options.
     */
    ClientOptions();

    /**
     * @brief Creates client options for a local node.
     *
     * @param node_id Local logical node identifier.
     */
    explicit ClientOptions(std::string node_id);

    /**
     * @brief Creates memory-only client options.
     *
     * Memory-only mode does not persist the local store to a WAL file.
     *
     * @param node_id Local logical node identifier.
     * @return SDK client options.
     */
    [[nodiscard]] static ClientOptions memory_only(std::string node_id);

    /**
     * @brief Creates local client options.
     *
     * This is a developer-friendly alias for memory_only().
     *
     * @param node_id Local logical node identifier.
     * @return SDK client options.
     */
    [[nodiscard]] static ClientOptions local(std::string node_id);

    /**
     * @brief Creates persistent WAL-backed client options.
     *
     * Persistent mode is the default production-oriented mode. Local writes are
     * backed by a WAL path so they can be recovered after restart.
     *
     * @param node_id Local logical node identifier.
     * @param wal_path Store WAL path.
     * @return SDK client options.
     */
    [[nodiscard]] static ClientOptions persistent(
        std::string node_id,
        std::string wal_path = "data/sdk-store.wal");

    /**
     * @brief Creates durable WAL-backed client options.
     *
     * This is an explicit alias for persistent().
     *
     * @param node_id Local logical node identifier.
     * @param wal_path Store WAL path.
     * @return SDK client options.
     */
    [[nodiscard]] static ClientOptions durable(
        std::string node_id,
        std::string wal_path = "data/sdk-store.wal");

    /**
     * @brief Creates fast WAL-backed client options.
     *
     * Fast mode keeps WAL persistence enabled but disables automatic flushing
     * after every write. It is useful for benchmarks, tests, and controlled
     * environments where throughput matters more than maximum durability.
     *
     * @param node_id Local logical node identifier.
     * @param wal_path Store WAL path.
     * @return SDK client options.
     */
    [[nodiscard]] static ClientOptions fast(
        std::string node_id,
        std::string wal_path = "data/sdk-store.wal");

    /**
     * @brief Returns the local logical node identifier.
     *
     * @return Node id.
     */
    [[nodiscard]] const std::string &node_id() const noexcept;

    /**
     * @brief Returns the store WAL path.
     *
     * @return Store WAL path.
     */
    [[nodiscard]] const std::string &store_wal_path() const noexcept;

    /**
     * @brief Returns true if the local store WAL is enabled.
     *
     * @return true when WAL persistence is enabled.
     */
    [[nodiscard]] bool store_wal_enabled() const noexcept;

    /**
     * @brief Returns true if WAL writes are flushed automatically.
     *
     * @return true when auto flush is enabled.
     */
    [[nodiscard]] bool auto_flush() const noexcept;

    /**
     * @brief Returns the initial local store capacity hint.
     *
     * @return Initial store capacity.
     */
    [[nodiscard]] std::size_t initial_store_capacity() const noexcept;

    /**
     * @brief Returns the maximum number of sync operations per batch.
     *
     * @return Sync batch size.
     */
    [[nodiscard]] std::size_t sync_batch_size() const noexcept;

    /**
     * @brief Returns the maximum number of sync retries.
     *
     * @return Maximum retry count.
     */
    [[nodiscard]] std::uint32_t max_sync_retries() const noexcept;

    /**
     * @brief Returns true if outbound operations require acknowledgements.
     *
     * @return true when acknowledgements are required.
     */
    [[nodiscard]] bool require_ack() const noexcept;

    /**
     * @brief Returns true if local operations are queued automatically.
     *
     * @return true when auto queue is enabled.
     */
    [[nodiscard]] bool auto_queue() const noexcept;

    /**
     * @brief Returns true if transport support is enabled.
     *
     * @return true when transport is enabled.
     */
    [[nodiscard]] bool transport_enabled() const noexcept;

    /**
     * @brief Returns the transport bind host.
     *
     * @return Transport host.
     */
    [[nodiscard]] const std::string &transport_host() const noexcept;

    /**
     * @brief Returns the transport bind port.
     *
     * @return Transport port.
     */
    [[nodiscard]] std::uint16_t transport_port() const noexcept;

    /**
     * @brief Returns true if discovery support is enabled.
     *
     * @return true when discovery is enabled.
     */
    [[nodiscard]] bool discovery_enabled() const noexcept;

    /**
     * @brief Returns the discovery bind host.
     *
     * @return Discovery host.
     */
    [[nodiscard]] const std::string &discovery_host() const noexcept;

    /**
     * @brief Returns the discovery bind port.
     *
     * @return Discovery port.
     */
    [[nodiscard]] std::uint16_t discovery_port() const noexcept;

    /**
     * @brief Returns the human-readable node display name.
     *
     * @return Display name.
     */
    [[nodiscard]] const std::string &display_name() const noexcept;

    /**
     * @brief Returns the runtime or product version exposed through metadata.
     *
     * @return Version string.
     */
    [[nodiscard]] const std::string &version() const noexcept;

    /**
     * @brief Sets the local logical node identifier.
     *
     * @param value Node id.
     */
    void set_node_id(std::string value);

    /**
     * @brief Sets the store WAL path.
     *
     * @param value WAL path.
     */
    void set_store_wal_path(std::string value);

    /**
     * @brief Enables or disables the local store WAL.
     *
     * @param value true to enable WAL persistence.
     */
    void set_store_wal_enabled(bool value) noexcept;

    /**
     * @brief Enables or disables automatic WAL flushing.
     *
     * @param value true to flush after every accepted local write.
     */
    void set_auto_flush(bool value) noexcept;

    /**
     * @brief Sets the initial local store capacity hint.
     *
     * @param value Initial store capacity.
     */
    void set_initial_store_capacity(std::size_t value) noexcept;

    /**
     * @brief Sets the maximum number of sync operations per batch.
     *
     * @param value Sync batch size.
     */
    void set_sync_batch_size(std::size_t value) noexcept;

    /**
     * @brief Sets the maximum number of sync retries.
     *
     * @param value Maximum retry count.
     */
    void set_max_sync_retries(std::uint32_t value) noexcept;

    /**
     * @brief Enables or disables acknowledgement requirements.
     *
     * @param value true to require acknowledgements.
     */
    void set_require_ack(bool value) noexcept;

    /**
     * @brief Enables or disables automatic queueing of local operations.
     *
     * @param value true to queue local operations automatically.
     */
    void set_auto_queue(bool value) noexcept;

    /**
     * @brief Sets the transport configuration.
     *
     * This also enables transport support.
     *
     * @param host Transport bind host.
     * @param port Transport bind port.
     */
    void set_transport(std::string host, std::uint16_t port);

    /**
     * @brief Disables transport support.
     */
    void disable_transport() noexcept;

    /**
     * @brief Sets the discovery configuration.
     *
     * This also enables discovery support.
     *
     * @param host Discovery bind host.
     * @param port Discovery bind port.
     */
    void set_discovery(std::string host, std::uint16_t port);

    /**
     * @brief Disables discovery support.
     */
    void disable_discovery() noexcept;

    /**
     * @brief Sets the metadata display fields.
     *
     * @param name Human-readable node display name.
     * @param runtime_version Runtime or product version.
     */
    void set_metadata(
        std::string name,
        std::string runtime_version = "0.1.0");

    /**
     * @brief Returns a copy with transport enabled.
     *
     * @param host Transport bind host.
     * @param port Transport bind port.
     * @return Modified client options.
     */
    [[nodiscard]] ClientOptions with_transport(
        std::string host,
        std::uint16_t port) const;

    /**
     * @brief Returns a copy with localhost transport enabled.
     *
     * @param port Transport bind port.
     * @return Modified client options.
     */
    [[nodiscard]] ClientOptions with_local_transport(
        std::uint16_t port) const;

    /**
     * @brief Returns a copy with discovery enabled.
     *
     * @param host Discovery bind host.
     * @param port Discovery bind port.
     * @return Modified client options.
     */
    [[nodiscard]] ClientOptions with_discovery(
        std::string host,
        std::uint16_t port) const;

    /**
     * @brief Returns a copy with localhost discovery enabled.
     *
     * @param port Discovery bind port.
     * @return Modified client options.
     */
    [[nodiscard]] ClientOptions with_local_discovery(
        std::uint16_t port) const;

    /**
     * @brief Returns a copy with metadata fields set.
     *
     * @param name Human-readable node display name.
     * @param runtime_version Runtime or product version.
     * @return Modified client options.
     */
    [[nodiscard]] ClientOptions with_metadata(
        std::string name,
        std::string runtime_version = "0.1.0") const;

    /**
     * @brief Returns true if the options can be used to open a client.
     *
     * @return true if the options are valid.
     */
    [[nodiscard]] bool is_valid() const noexcept;

    /**
     * @brief Backward-compatible alias for is_valid().
     *
     * @return true if the options are valid.
     */
    [[nodiscard]] bool valid() const noexcept;

  private:
    std::string node_id_{"node-1"};

    std::string store_wal_path_{"data/sdk-store.wal"};
    bool store_wal_enabled_{true};
    bool auto_flush_{true};
    std::size_t initial_store_capacity_{1024};

    std::size_t sync_batch_size_{64};
    std::uint32_t max_sync_retries_{5};
    bool require_ack_{true};
    bool auto_queue_{true};

    bool transport_enabled_{false};
    std::string transport_host_{"0.0.0.0"};
    std::uint16_t transport_port_{0};

    bool discovery_enabled_{false};
    std::string discovery_host_{"0.0.0.0"};
    std::uint16_t discovery_port_{0};

    std::string display_name_{};
    std::string version_{"0.1.0"};
  };

} // namespace softadastra::sdk

#endif // SOFTADASTRA_SDK_CLIENT_OPTIONS_HPP
