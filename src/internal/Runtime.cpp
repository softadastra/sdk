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
      : store(std::move(other.store)),
        sync_config(std::move(other.sync_config)),
        sync_context(std::move(other.sync_context)),
        sync(std::move(other.sync)),
        scheduler(std::move(other.scheduler)),
        transport_config(std::move(other.transport_config)),
        transport_context(std::move(other.transport_context)),
        transport_backend(std::move(other.transport_backend)),
        transport(std::move(other.transport)),
        discovery(std::move(other.discovery)),
        metadata(std::move(other.metadata))
  {
  }

  Runtime &Runtime::operator=(Runtime &&other) noexcept
  {
    if (this != &other)
    {
      reset();

      store = std::move(other.store);

      sync_config = std::move(other.sync_config);
      sync_context = std::move(other.sync_context);
      sync = std::move(other.sync);
      scheduler = std::move(other.scheduler);

      transport_config = std::move(other.transport_config);
      transport_context = std::move(other.transport_context);
      transport_backend = std::move(other.transport_backend);
      transport = std::move(other.transport);

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

    transport.reset();
    transport_backend.reset();
    transport_context.reset();
    transport_config.reset();

    scheduler.reset();
    sync.reset();
    sync_context.reset();
    sync_config.reset();

    store.reset();
  }

  void Runtime::stop_services() noexcept
  {
    if (discovery)
    {
      discovery->stop();
    }

    if (transport)
    {
      transport->stop();
    }
  }

  bool Runtime::has_local_runtime() const noexcept
  {
    return store &&
           sync_config &&
           sync_context &&
           sync &&
           scheduler;
  }

  bool Runtime::has_transport() const noexcept
  {
    return transport_config &&
           transport_context &&
           transport_backend &&
           transport;
  }

  bool Runtime::has_discovery() const noexcept
  {
    return static_cast<bool>(discovery);
  }

  bool Runtime::has_metadata() const noexcept
  {
    return static_cast<bool>(metadata);
  }

} // namespace softadastra::sdk::internal
