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

#include <softadastra/store/engine/StoreEngine.hpp>

#include <softadastra/sync/core/SyncConfig.hpp>
#include <softadastra/sync/core/SyncContext.hpp>
#include <softadastra/sync/engine/SyncEngine.hpp>
#include <softadastra/sync/scheduler/SyncScheduler.hpp>

#include <softadastra/transport/backend/TcpTransportBackend.hpp>
#include <softadastra/transport/core/TransportConfig.hpp>
#include <softadastra/transport/core/TransportContext.hpp>
#include <softadastra/transport/engine/TransportEngine.hpp>

#include <softadastra/discovery/DiscoveryService.hpp>
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
   * Store, Sync, Transport, Discovery, and Metadata internally.
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
     * @brief Returns true if transport modules exist.
     *
     * @return true if transport is initialized.
     */
    [[nodiscard]] bool has_transport() const noexcept;

    /**
     * @brief Returns true if discovery service exists.
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

    using StoreEngine = softadastra::store::engine::StoreEngine;

    using SyncConfig = softadastra::sync::core::SyncConfig;
    using SyncContext = softadastra::sync::core::SyncContext;
    using SyncEngine = softadastra::sync::engine::SyncEngine;
    using SyncScheduler = softadastra::sync::scheduler::SyncScheduler;

    using TransportConfig = softadastra::transport::core::TransportConfig;
    using TransportContext = softadastra::transport::core::TransportContext;
    using TcpTransportBackend =
        softadastra::transport::backend::TcpTransportBackend;
    using TransportEngine = softadastra::transport::engine::TransportEngine;

    using DiscoveryService = softadastra::discovery::DiscoveryService;
    using MetadataService = softadastra::metadata::MetadataService;

    std::unique_ptr<StoreEngine> store{};

    std::unique_ptr<SyncConfig> sync_config{};
    std::unique_ptr<SyncContext> sync_context{};
    std::unique_ptr<SyncEngine> sync{};
    std::unique_ptr<SyncScheduler> scheduler{};

    std::unique_ptr<TransportConfig> transport_config{};
    std::unique_ptr<TransportContext> transport_context{};
    std::unique_ptr<TcpTransportBackend> transport_backend{};
    std::unique_ptr<TransportEngine> transport{};

    std::unique_ptr<DiscoveryService> discovery{};
    std::unique_ptr<MetadataService> metadata{};
  };

} // namespace softadastra::sdk::internal

#endif // SOFTADASTRA_SDK_INTERNAL_RUNTIME_HPP
