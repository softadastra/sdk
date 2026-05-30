/**
 *
 *  @file Runtime.cpp
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

#include "Runtime.hpp"

#include <utility>

namespace softadastra::sdk::internal
{
  Runtime::~Runtime()
  {
    reset();
  }

  Runtime::Runtime(Runtime &&other) noexcept
      : io_context(std::move(other.io_context)),
        store(std::move(other.store)),
        sync_config(std::move(other.sync_config)),
        sync_context(std::move(other.sync_context)),
        sync(std::move(other.sync)),
        scheduler(std::move(other.scheduler)),
        transport_config(std::move(other.transport_config)),
        transport_context(std::move(other.transport_context)),
        async_transport_backend(std::move(other.async_transport_backend)),
        transport(std::move(other.transport)),
        discovery_config(std::move(other.discovery_config)),
        discovery_context(std::move(other.discovery_context)),
        discovery_backend(std::move(other.discovery_backend)),
        discovery_engine(std::move(other.discovery_engine)),
        discovery(std::move(other.discovery)),
        metadata(std::move(other.metadata))
  {
  }

  Runtime &Runtime::operator=(Runtime &&other) noexcept
  {
    if (this != &other)
    {
      reset();

      io_context = std::move(other.io_context);

      store = std::move(other.store);

      sync_config = std::move(other.sync_config);
      sync_context = std::move(other.sync_context);
      sync = std::move(other.sync);
      scheduler = std::move(other.scheduler);

      transport_config = std::move(other.transport_config);
      transport_context = std::move(other.transport_context);
      async_transport_backend =
          std::move(other.async_transport_backend);
      transport = std::move(other.transport);

      discovery_config = std::move(other.discovery_config);
      discovery_context = std::move(other.discovery_context);
      discovery_backend = std::move(other.discovery_backend);
      discovery_engine = std::move(other.discovery_engine);
      discovery = std::move(other.discovery);
      metadata = std::move(other.metadata);
    }

    return *this;
  }

  void Runtime::reset() noexcept
  {
    stop_services();

    metadata.reset();

    discovery.reset();
    discovery_engine.reset();
    discovery_backend.reset();
    discovery_context.reset();
    discovery_config.reset();

    transport.reset();
    async_transport_backend.reset();
    transport_context.reset();
    transport_config.reset();

    scheduler.reset();
    sync.reset();
    sync_context.reset();
    sync_config.reset();

    store.reset();

    io_context.reset();
  }

  void Runtime::stop_services() noexcept
  {
    if (discovery)
    {
      discovery->stop();
    }

    if (async_transport_backend)
    {
      async_transport_backend->shutdown();
    }

    if (transport && async_transport_backend)
    {
      transport->process_backend_shutdown_events(
          *async_transport_backend,
          256);
    }

    if (transport)
    {
      transport->stop();
    }

    if (io_context)
    {
      io_context->stop();
    }
  }

  std::size_t Runtime::process_transport_events(
      std::size_t max_events)
  {
    if (!transport || !async_transport_backend)
    {
      return 0;
    }

    return transport->process_events(
        async_transport_backend->drain_events(max_events));
  }

  bool Runtime::has_local_runtime() const noexcept
  {
    return store &&
           sync_config &&
           sync_context &&
           sync &&
           scheduler;
  }

  bool Runtime::has_async_runtime() const noexcept
  {
    return static_cast<bool>(io_context);
  }

  bool Runtime::has_transport() const noexcept
  {
    return io_context &&
           transport_config &&
           transport_context &&
           async_transport_backend &&
           transport;
  }

  bool Runtime::has_discovery() const noexcept
  {
    return discovery_config &&
           discovery_context &&
           discovery_backend &&
           discovery_engine &&
           discovery;
  }

  bool Runtime::has_metadata() const noexcept
  {
    return static_cast<bool>(metadata);
  }

} // namespace softadastra::sdk::internal
