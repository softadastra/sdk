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
    build_transport(runtime, options);
    build_discovery(runtime, options);
    build_metadata(runtime, options);

    if (!runtime.has_local_runtime())
    {
      return BuildResult::err(
          ErrorMapper::internal(
              "failed to build local SDK runtime"));
    }

    if (!runtime.has_metadata())
    {
      return BuildResult::err(
          ErrorMapper::metadata(
              "failed to build SDK metadata service"));
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

  void RuntimeBuilder::build_transport(
      Runtime &runtime,
      const ClientOptions &options)
  {
    if (!options.transport_enabled())
    {
      return;
    }

    runtime.transport_config =
        std::make_unique<Runtime::TransportConfig>(
            to_transport_config(options));

    runtime.transport_context =
        std::make_unique<Runtime::TransportContext>(
            *runtime.transport_config);

    runtime.transport_backend =
        std::make_unique<Runtime::TcpTransportBackend>(
            *runtime.transport_context);

    runtime.transport =
        std::make_unique<Runtime::TransportEngine>(
            *runtime.transport_context,
            *runtime.transport_backend);
  }

  void RuntimeBuilder::build_discovery(
      Runtime &runtime,
      const ClientOptions &options)
  {
    if (!options.discovery_enabled())
    {
      return;
    }

    runtime.discovery =
        std::make_unique<Runtime::DiscoveryService>(
            to_discovery_options(options));
  }

  void RuntimeBuilder::build_metadata(
      Runtime &runtime,
      const ClientOptions &options)
  {
    runtime.metadata =
        std::make_unique<Runtime::MetadataService>(
            to_metadata_options(options));
  }

} // namespace softadastra::sdk::internal
