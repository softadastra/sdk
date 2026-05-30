/**
 *
 *  @file Runtime.hpp
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

#ifndef SOFTADASTRA_SDK_INTERNAL_RUNTIME_HPP
#define SOFTADASTRA_SDK_INTERNAL_RUNTIME_HPP

#include <memory>

#include <vix/async/core/io_context.hpp>

#include <softadastra/store/engine/StoreEngine.hpp>

#include <softadastra/sync/core/SyncConfig.hpp>
#include <softadastra/sync/core/SyncContext.hpp>
#include <softadastra/sync/engine/SyncEngine.hpp>
#include <softadastra/sync/scheduler/SyncScheduler.hpp>

#include <softadastra/transport/backend/AsyncTcpTransportBackend.hpp>
#include <softadastra/transport/core/TransportConfig.hpp>
#include <softadastra/transport/core/TransportContext.hpp>
#include <softadastra/transport/engine/TransportEngine.hpp>

#include <softadastra/discovery/DiscoveryService.hpp>
#include <softadastra/discovery/backend/UdpDiscoveryBackend.hpp>
#include <softadastra/discovery/core/DiscoveryConfig.hpp>
#include <softadastra/discovery/core/DiscoveryContext.hpp>
#include <softadastra/discovery/engine/DiscoveryEngine.hpp>

#include <softadastra/metadata/MetadataService.hpp>

namespace softadastra::sdk::internal
{
  /**
   * @brief Internal SDK runtime container.
   *
   * Runtime owns the low-level Softadastra modules used by the public SDK
   * client.
   *
   * This type must never be exposed through public SDK headers. It exists to
   * keep Client.hpp stable while allowing the SDK implementation to compose
   * Store, Sync, Transport, Discovery, Metadata, and the async runtime
   * internally.
   */
  class Runtime
  {
  public:
    /**
     * @brief Creates an empty runtime.
     */
    Runtime() = default;

    /**
     * @brief Stops services and releases owned modules.
     */
    ~Runtime();

    Runtime(const Runtime &) = delete;
    Runtime &operator=(const Runtime &) = delete;

    /**
     * @brief Moves a runtime.
     */
    Runtime(Runtime &&other) noexcept;

    /**
     * @brief Move-assigns a runtime.
     */
    Runtime &operator=(Runtime &&other) noexcept;

    /**
     * @brief Resets all runtime modules.
     */
    void reset() noexcept;

    /**
     * @brief Stops optional runtime services.
     */
    void stop_services() noexcept;

    /**
     * @brief Processes queued async transport backend events.
     *
     * Async transport backends produce TransportEvent objects. TransportEngine
     * remains responsible for consuming those events and updating peer/session
     * state.
     *
     * @param max_events Maximum number of events to process.
     * @return Number of events handled by the transport engine.
     */
    [[nodiscard]] std::size_t process_transport_events(
        std::size_t max_events);

    /**
     * @brief Returns true if the required local runtime modules exist.
     *
     * Required modules are:
     * - store
     * - sync config
     * - sync context
     * - sync engine
     * - sync scheduler
     *
     * @return true if the local runtime is complete.
     */
    [[nodiscard]] bool has_local_runtime() const noexcept;

    /**
     * @brief Returns true if the async runtime exists.
     *
     * @return true if async runtime is initialized.
     */
    [[nodiscard]] bool has_async_runtime() const noexcept;

    /**
     * @brief Returns true if transport modules exist.
     *
     * @return true if transport is initialized.
     */
    [[nodiscard]] bool has_transport() const noexcept;

    /**
     * @brief Returns true if discovery modules exist.
     *
     * Required discovery modules are:
     * - discovery config
     * - discovery context
     * - UDP discovery backend
     * - discovery engine
     * - discovery service
     *
     * @return true if discovery is initialized.
     */
    [[nodiscard]] bool has_discovery() const noexcept;

    /**
     * @brief Returns true if metadata service exists.
     *
     * @return true if metadata is initialized.
     */
    [[nodiscard]] bool has_metadata() const noexcept;

    using IoContext = vix::async::core::io_context;

    using StoreEngine = softadastra::store::engine::StoreEngine;

    using SyncConfig = softadastra::sync::core::SyncConfig;
    using SyncContext = softadastra::sync::core::SyncContext;
    using SyncEngine = softadastra::sync::engine::SyncEngine;
    using SyncScheduler = softadastra::sync::scheduler::SyncScheduler;

    using TransportConfig = softadastra::transport::core::TransportConfig;
    using TransportContext = softadastra::transport::core::TransportContext;
    using AsyncTcpTransportBackend =
        softadastra::transport::backend::AsyncTcpTransportBackend;
    using TransportEngine = softadastra::transport::engine::TransportEngine;

    using DiscoveryConfig = softadastra::discovery::core::DiscoveryConfig;
    using DiscoveryContext = softadastra::discovery::core::DiscoveryContext;
    using UdpDiscoveryBackend =
        softadastra::discovery::backend::UdpDiscoveryBackend;
    using DiscoveryEngine = softadastra::discovery::engine::DiscoveryEngine;
    using DiscoveryService = softadastra::discovery::DiscoveryService;

    using MetadataService = softadastra::metadata::MetadataService;

    /**
     * @brief Vix async runtime context.
     */
    std::unique_ptr<IoContext> io_context{};

    /**
     * @brief Local WAL-backed or memory-only store engine.
     */
    std::unique_ptr<StoreEngine> store{};

    /**
     * @brief Sync configuration.
     */
    std::unique_ptr<SyncConfig> sync_config{};

    /**
     * @brief Sync context binding store and sync configuration.
     */
    std::unique_ptr<SyncContext> sync_context{};

    /**
     * @brief Sync engine.
     */
    std::unique_ptr<SyncEngine> sync{};

    /**
     * @brief Sync scheduler.
     */
    std::unique_ptr<SyncScheduler> scheduler{};

    /**
     * @brief Transport configuration.
     */
    std::unique_ptr<TransportConfig> transport_config{};

    /**
     * @brief Transport context binding transport and sync.
     */
    std::unique_ptr<TransportContext> transport_context{};

    /**
     * @brief Async TCP transport backend.
     */
    std::unique_ptr<AsyncTcpTransportBackend> async_transport_backend{};

    /**
     * @brief Transport engine.
     */
    std::unique_ptr<TransportEngine> transport{};

    /**
     * @brief Discovery configuration.
     */
    std::unique_ptr<DiscoveryConfig> discovery_config{};

    /**
     * @brief Discovery context binding discovery configuration and transport.
     */
    std::unique_ptr<DiscoveryContext> discovery_context{};

    /**
     * @brief UDP discovery backend.
     */
    std::unique_ptr<UdpDiscoveryBackend> discovery_backend{};

    /**
     * @brief Discovery engine.
     */
    std::unique_ptr<DiscoveryEngine> discovery_engine{};

    /**
     * @brief Discovery service.
     */
    std::unique_ptr<DiscoveryService> discovery{};

    /**
     * @brief Metadata service.
     */
    std::unique_ptr<MetadataService> metadata{};
  };

} // namespace softadastra::sdk::internal

#endif // SOFTADASTRA_SDK_INTERNAL_RUNTIME_HPP
