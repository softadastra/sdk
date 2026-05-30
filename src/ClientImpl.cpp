/**
 *
 *  @file ClientImpl.cpp
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

#include "ClientImpl.hpp"

#include "conversions/DiscoveryConversions.hpp"
#include "conversions/MetadataConversions.hpp"
#include "conversions/StoreConversions.hpp"
#include "conversions/SyncConversions.hpp"
#include "conversions/TransportConversions.hpp"

#include "internal/ErrorMapper.hpp"
#include "internal/RuntimeBuilder.hpp"
#include "internal/RuntimeGuards.hpp"

#include <softadastra/store/core/Operation.hpp>

#include <utility>

namespace softadastra::sdk
{
  ClientImpl::ClientImpl()
      : ClientImpl(ClientOptions{})
  {
  }

  ClientImpl::ClientImpl(ClientOptions options)
      : options_(std::move(options))
  {
  }

  ClientImpl::~ClientImpl()
  {
    close();
  }

  ClientImpl::ClientImpl(ClientImpl &&other) noexcept
      : options_(std::move(other.options_)),
        runtime_(std::move(other.runtime_)),
        open_(other.open_)
  {
    other.open_ = false;
  }

  ClientImpl &ClientImpl::operator=(ClientImpl &&other) noexcept
  {
    if (this != &other)
    {
      close();

      options_ = std::move(other.options_);
      runtime_ = std::move(other.runtime_);
      open_ = other.open_;

      other.open_ = false;
    }

    return *this;
  }

  ClientImpl::VoidResult ClientImpl::open()
  {
    if (open_)
    {
      return VoidResult::ok();
    }

    if (!options_.is_valid())
    {
      return VoidResult::err(
          Error::invalid_argument(
              "invalid SDK client options"));
    }

    auto built =
        internal::RuntimeBuilder::build(options_);

    if (built.is_err())
    {
      return VoidResult::err(built.error());
    }

    runtime_ = std::move(built.value());
    open_ = true;

    return VoidResult::ok();
  }

  void ClientImpl::close() noexcept
  {
    runtime_.reset();
    open_ = false;
  }

  bool ClientImpl::is_open() const noexcept
  {
    return open_;
  }

  bool ClientImpl::opened() const noexcept
  {
    return is_open();
  }

  ClientImpl::VoidResult ClientImpl::put(
      const Key &key,
      const Value &value)
  {
    const auto open_result =
        internal::RuntimeGuards::require_open(
            open_,
            runtime_);

    if (open_result.is_err())
    {
      return open_result;
    }

    if (!key.is_valid())
    {
      return VoidResult::err(
          Error::invalid_argument(
              "key cannot be empty"));
    }

    softadastra::store::core::Operation operation =
        softadastra::store::core::Operation::put(
            internal::to_store_key(key),
            internal::to_store_value(value));

    const auto submitted =
        runtime_.sync->submit_local_operation(operation);

    if (submitted.is_err())
    {
      return VoidResult::err(
          internal::ErrorMapper::from_core(
              submitted.error()));
    }

    return VoidResult::ok();
  }

  ClientImpl::VoidResult ClientImpl::put(
      std::string key,
      std::string value)
  {
    return put(
        Key{std::move(key)},
        Value::from_string(value));
  }

  ClientImpl::ValueResult ClientImpl::get(const Key &key) const
  {
    const auto open_result =
        internal::RuntimeGuards::require_open(
            open_,
            runtime_);

    if (open_result.is_err())
    {
      return ValueResult::err(open_result.error());
    }

    if (!key.is_valid())
    {
      return ValueResult::err(
          Error::invalid_argument(
              "key cannot be empty"));
    }

    const auto entry =
        runtime_.store->get(
            internal::to_store_key(key));

    if (!entry.has_value())
    {
      return ValueResult::err(
          Error::not_found(
              "key not found",
              key.str()));
    }

    return ValueResult::ok(
        internal::from_store_value(entry->value));
  }

  ClientImpl::ValueResult ClientImpl::get(
      const std::string &key) const
  {
    return get(Key{key});
  }

  ClientImpl::VoidResult ClientImpl::remove(const Key &key)
  {
    const auto open_result =
        internal::RuntimeGuards::require_open(
            open_,
            runtime_);

    if (open_result.is_err())
    {
      return open_result;
    }

    if (!key.is_valid())
    {
      return VoidResult::err(
          Error::invalid_argument(
              "key cannot be empty"));
    }

    softadastra::store::core::Operation operation =
        softadastra::store::core::Operation::remove(
            internal::to_store_key(key));

    const auto submitted =
        runtime_.sync->submit_local_operation(operation);

    if (submitted.is_err())
    {
      return VoidResult::err(
          internal::ErrorMapper::from_core(
              submitted.error()));
    }

    return VoidResult::ok();
  }

  ClientImpl::VoidResult ClientImpl::remove(
      const std::string &key)
  {
    return remove(Key{key});
  }

  bool ClientImpl::contains(const Key &key) const
  {
    if (!open_ || !runtime_.store || !key.is_valid())
    {
      return false;
    }

    return runtime_.store->contains(
        internal::to_store_key(key));
  }

  bool ClientImpl::contains(const std::string &key) const
  {
    return contains(Key{key});
  }

  std::size_t ClientImpl::size() const noexcept
  {
    if (!open_ || !runtime_.store)
    {
      return 0;
    }

    return runtime_.store->size();
  }

  bool ClientImpl::empty() const noexcept
  {
    return size() == 0;
  }

  ClientImpl::TickStateResult ClientImpl::tick(
      bool prune_completed)
  {
    const auto open_result =
        internal::RuntimeGuards::require_open(
            open_,
            runtime_);

    if (open_result.is_err())
    {
      return TickStateResult::err(open_result.error());
    }

    const auto result =
        runtime_.scheduler->tick(prune_completed);

    if (options_.transport_enabled() &&
        runtime_.has_transport())
    {
      runtime_.process_transport_events(64);
    }

    return TickStateResult::ok(
        internal::from_scheduler_tick_result(result));
  }

  ClientImpl::SyncStateResult ClientImpl::sync_state() const
  {
    const auto open_result =
        internal::RuntimeGuards::require_open(
            open_,
            runtime_);

    if (open_result.is_err())
    {
      return SyncStateResult::err(open_result.error());
    }

    return SyncStateResult::ok(
        internal::from_sync_state(
            runtime_.sync->state()));
  }

  Result<std::size_t, Error> ClientImpl::retry_expired()
  {
    const auto open_result =
        internal::RuntimeGuards::require_open(
            open_,
            runtime_);

    if (open_result.is_err())
    {
      return Result<std::size_t, Error>::err(
          open_result.error());
    }

    return Result<std::size_t, Error>::ok(
        runtime_.scheduler->retry_only());
  }

  Result<std::size_t, Error> ClientImpl::prune_completed()
  {
    const auto open_result =
        internal::RuntimeGuards::require_open(
            open_,
            runtime_);

    if (open_result.is_err())
    {
      return Result<std::size_t, Error>::err(
          open_result.error());
    }

    return Result<std::size_t, Error>::ok(
        runtime_.scheduler->prune_completed());
  }

  Result<std::size_t, Error> ClientImpl::prune_failed()
  {
    const auto open_result =
        internal::RuntimeGuards::require_open(
            open_,
            runtime_);

    if (open_result.is_err())
    {
      return Result<std::size_t, Error>::err(
          open_result.error());
    }

    return Result<std::size_t, Error>::ok(
        runtime_.scheduler->prune_failed());
  }

  ClientImpl::VoidResult ClientImpl::start_transport()
  {
    const auto transport_result =
        internal::RuntimeGuards::require_transport(
            open_,
            options_.transport_enabled(),
            runtime_);

    if (transport_result.is_err())
    {
      return transport_result;
    }

    if (runtime_.transport->is_running())
    {
      runtime_.process_transport_events(64);
      return VoidResult::ok();
    }

    if (!runtime_.transport->start())
    {
      return VoidResult::err(
          internal::ErrorMapper::transport(
              "failed to start transport"));
    }

    runtime_.process_transport_events(64);

    return VoidResult::ok();
  }

  void ClientImpl::stop_transport() noexcept
  {
    if (runtime_.transport)
    {
      runtime_.transport->stop();
    }

    if (runtime_.async_transport_backend)
    {
      runtime_.async_transport_backend->shutdown();
    }

    runtime_.process_transport_events(256);
  }

  bool ClientImpl::transport_running() const noexcept
  {
    return runtime_.transport &&
           runtime_.transport->is_running();
  }

  ClientImpl::TransportEventsResult
  ClientImpl::process_transport_events(std::size_t max_events)
  {
    const auto transport_result =
        internal::RuntimeGuards::require_transport(
            open_,
            options_.transport_enabled(),
            runtime_);

    if (transport_result.is_err())
    {
      return TransportEventsResult::err(
          transport_result.error());
    }

    return TransportEventsResult::ok(
        runtime_.process_transport_events(max_events));
  }

  ClientImpl::VoidResult ClientImpl::connect(const Peer &peer)
  {
    const auto transport_result =
        internal::RuntimeGuards::require_transport(
            open_,
            options_.transport_enabled(),
            runtime_);

    if (transport_result.is_err())
    {
      return transport_result;
    }

    if (!peer.is_valid())
    {
      return VoidResult::err(
          Error::invalid_argument(
              "invalid peer"));
    }

    if (!runtime_.transport->connect_peer(
            internal::to_transport_peer(peer)))
    {
      runtime_.process_transport_events(64);

      return VoidResult::err(
          internal::ErrorMapper::transport(
              "failed to connect to peer",
              peer.node_id()));
    }

    runtime_.process_transport_events(64);

    return VoidResult::ok();
  }

  ClientImpl::VoidResult ClientImpl::disconnect(const Peer &peer)
  {
    const auto transport_result =
        internal::RuntimeGuards::require_transport(
            open_,
            options_.transport_enabled(),
            runtime_);

    if (transport_result.is_err())
    {
      return transport_result;
    }

    if (!peer.is_valid())
    {
      return VoidResult::err(
          Error::invalid_argument(
              "invalid peer"));
    }

    if (!runtime_.transport->disconnect_peer(
            internal::to_transport_peer(peer)))
    {
      runtime_.process_transport_events(64);

      return VoidResult::err(
          internal::ErrorMapper::transport(
              "failed to disconnect from peer",
              peer.node_id()));
    }

    runtime_.process_transport_events(64);

    return VoidResult::ok();
  }

  ClientImpl::VoidResult ClientImpl::start_discovery()
  {
    const auto discovery_result =
        internal::RuntimeGuards::require_discovery(
            open_,
            options_.discovery_enabled(),
            runtime_);

    if (discovery_result.is_err())
    {
      return discovery_result;
    }

    if (runtime_.discovery->is_running())
    {
      return VoidResult::ok();
    }

    if (!runtime_.discovery->options().is_valid())
    {
      return VoidResult::err(
          internal::ErrorMapper::discovery(
              "invalid discovery options"));
    }

    if (!runtime_.discovery->start())
    {
      return VoidResult::err(
          internal::ErrorMapper::discovery(
              "failed to start discovery"));
    }

    return VoidResult::ok();
  }

  void ClientImpl::stop_discovery() noexcept
  {
    if (runtime_.discovery)
    {
      runtime_.discovery->stop();
    }
  }

  bool ClientImpl::discovery_running() const noexcept
  {
    return runtime_.discovery &&
           runtime_.discovery->is_running();
  }

  ClientImpl::PeersResult ClientImpl::peers() const
  {
    const auto discovery_result =
        internal::RuntimeGuards::require_discovery(
            open_,
            options_.discovery_enabled(),
            runtime_);

    if (discovery_result.is_err())
    {
      return PeersResult::err(discovery_result.error());
    }

    std::vector<Peer> result;

    for (const auto &peer : runtime_.discovery->peers())
    {
      result.push_back(
          internal::from_discovery_peer(peer));
    }

    return PeersResult::ok(std::move(result));
  }

  ClientImpl::NodeInfoResult ClientImpl::node_info() const
  {
    const auto metadata_result =
        internal::RuntimeGuards::require_metadata(
            open_,
            runtime_);

    if (metadata_result.is_err())
    {
      return NodeInfoResult::err(metadata_result.error());
    }

    const auto metadata =
        runtime_.metadata->local_or_refresh();

    if (!metadata.has_value())
    {
      return NodeInfoResult::err(
          internal::ErrorMapper::metadata(
              "local node metadata is not available"));
    }

    return NodeInfoResult::ok(
        internal::from_node_metadata(*metadata));
  }

  ClientImpl::NodeInfoResult ClientImpl::refresh_node_info()
  {
    const auto metadata_result =
        internal::RuntimeGuards::require_metadata(
            open_,
            runtime_);

    if (metadata_result.is_err())
    {
      return NodeInfoResult::err(metadata_result.error());
    }

    const auto metadata =
        runtime_.metadata->refresh();

    if (!metadata.has_value())
    {
      return NodeInfoResult::err(
          internal::ErrorMapper::metadata(
              "failed to refresh local node metadata"));
    }

    return NodeInfoResult::ok(
        internal::from_node_metadata(*metadata));
  }

  const ClientOptions &ClientImpl::options() const noexcept
  {
    return options_;
  }

} // namespace softadastra::sdk
