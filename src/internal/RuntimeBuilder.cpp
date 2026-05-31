/**
 *
 *  @file RuntimeBuilder.cpp
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

#include "RuntimeBuilder.hpp"

#include "ErrorMapper.hpp"

#include "../conversions/DiscoveryConversions.hpp"
#include "../conversions/MetadataConversions.hpp"
#include "../conversions/SyncConversions.hpp"
#include "../conversions/TransportConversions.hpp"

#include <softadastra/sdk/ClientOptions.hpp>

#include <memory>
#include <utility>

#include <softadastra/store/core/StoreConfig.hpp>

namespace softadastra::sdk::internal
{
  RuntimeBuilder::BuildResult RuntimeBuilder::build(
      const ClientOptions &options)
  {
    if (!options.is_valid())
    {
      return BuildResult::err(
          Error::invalid_argument(
              "invalid SDK client options"));
    }

    Runtime runtime;

    build_store(runtime, options);
    build_sync(runtime, options);
    build_async_runtime(runtime);
    build_transport(runtime, options);
    build_discovery(runtime, options);
    build_metadata(runtime, options);

    if (!runtime.has_local_runtime())
    {
      return BuildResult::err(
          ErrorMapper::internal(
              "failed to build local SDK runtime"));
    }

    if (options.transport_enabled() &&
        !runtime.has_async_runtime())
    {
      return BuildResult::err(
          ErrorMapper::transport(
              "failed to build SDK async runtime"));
    }

    if (options.transport_enabled() &&
        !runtime.has_transport())
    {
      return BuildResult::err(
          ErrorMapper::transport(
              "failed to build SDK async transport runtime"));
    }

    return BuildResult::ok(std::move(runtime));
  }

  void RuntimeBuilder::build_store(
      Runtime &runtime,
      const ClientOptions &options)
  {
    softadastra::store::core::StoreConfig config;

    if (options.store_wal_enabled())
    {
      config =
          softadastra::store::core::StoreConfig::durable(
              options.store_wal_path());

      config.enable_wal = true;
      config.auto_flush = options.auto_flush();
      config.initial_capacity = options.initial_store_capacity();
    }
    else
    {
      config =
          softadastra::store::core::StoreConfig::memory_only();

      config.initial_capacity = options.initial_store_capacity();
    }

    runtime.store =
        std::make_unique<Runtime::StoreEngine>(
            std::move(config));
  }

  void RuntimeBuilder::build_sync(
      Runtime &runtime,
      const ClientOptions &options)
  {
    runtime.sync_config =
        std::make_unique<Runtime::SyncConfig>(
            to_sync_config(options));

    runtime.sync_context =
        std::make_unique<Runtime::SyncContext>(
            *runtime.store,
            *runtime.sync_config);

    runtime.sync =
        std::make_unique<Runtime::SyncEngine>(
            *runtime.sync_context);

    runtime.scheduler =
        std::make_unique<Runtime::SyncScheduler>(
            *runtime.sync);
  }

  void RuntimeBuilder::build_async_runtime(Runtime &runtime)
  {
    if (runtime.io_context)
    {
      return;
    }

    runtime.io_context =
        std::make_unique<Runtime::IoContext>();
  }

  void RuntimeBuilder::build_transport(
      Runtime &runtime,
      const ClientOptions &options)
  {
    if (!options.transport_enabled() &&
        !options.discovery_enabled())
    {
      return;
    }

    build_async_runtime(runtime);

    runtime.transport_config =
        std::make_unique<Runtime::TransportConfig>(
            to_transport_config(options));

    runtime.transport_context =
        std::make_unique<Runtime::TransportContext>(
            *runtime.transport_config,
            *runtime.sync);

    switch (options.transport_backend())
    {
    case ClientOptions::TransportBackend::AsyncTcp:
      runtime.async_transport_backend =
          std::make_unique<Runtime::AsyncTcpTransportBackend>(
              *runtime.io_context,
              *runtime.transport_config);

      runtime.transport =
          std::make_unique<Runtime::TransportEngine>(
              *runtime.transport_context,
              *runtime.async_transport_backend);

      break;
    }
  }

  void RuntimeBuilder::build_discovery(
      Runtime &runtime,
      const ClientOptions &options)
  {
    if (!options.discovery_enabled())
    {
      return;
    }

    if (!runtime.transport)
    {
      return;
    }

    const auto discovery_options =
        to_discovery_options(options);

    runtime.discovery_config =
        std::make_unique<Runtime::DiscoveryConfig>(
            discovery_options.to_config());

    runtime.discovery_context =
        std::make_unique<Runtime::DiscoveryContext>(
            *runtime.discovery_config,
            *runtime.transport);

    runtime.discovery_backend =
        std::make_unique<Runtime::UdpDiscoveryBackend>(
            *runtime.discovery_config);

    runtime.discovery_engine =
        std::make_unique<Runtime::DiscoveryEngine>(
            *runtime.discovery_context,
            *runtime.discovery_backend);

    runtime.discovery =
        std::make_unique<Runtime::DiscoveryService>(
            discovery_options,
            *runtime.transport);
  }

  void RuntimeBuilder::build_metadata(
      Runtime &runtime,
      const ClientOptions &options)
  {
    if (runtime.discovery_engine)
    {
      runtime.metadata =
          std::make_unique<Runtime::MetadataService>(
              to_metadata_options(options),
              *runtime.discovery_engine);

      runtime.metadata->start();
      return;
    }

    runtime.metadata =
        std::make_unique<Runtime::MetadataService>(
            to_metadata_options(options));

    runtime.metadata->start();
  }
} // namespace softadastra::sdk::internal
