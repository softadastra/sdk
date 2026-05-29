/**
 *
 *  @file Client.cpp
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

#include <softadastra/sdk/Client.hpp>
#include "ClientImpl.hpp"

#include <utility>

namespace softadastra::sdk
{
  class Client::Impl final : public ClientImpl
  {
  public:
    using ClientImpl::ClientImpl;
  };

  Client::Client()
      : impl_(std::make_unique<Impl>())
  {
  }

  Client::Client(ClientOptions options)
      : impl_(std::make_unique<Impl>(std::move(options)))
  {
  }

  Client::~Client() = default;

  Client::Client(Client &&) noexcept = default;

  Client &Client::operator=(Client &&) noexcept = default;

  Client::VoidResult Client::open()
  {
    return impl_->open();
  }

  void Client::close() noexcept
  {
    impl_->close();
  }

  bool Client::is_open() const noexcept
  {
    return impl_->is_open();
  }

  bool Client::opened() const noexcept
  {
    return impl_->opened();
  }

  Client::VoidResult Client::put(
      const Key &key,
      const Value &value)
  {
    return impl_->put(key, value);
  }

  Client::VoidResult Client::put(
      std::string key,
      std::string value)
  {
    return impl_->put(
        std::move(key),
        std::move(value));
  }

  Client::ValueResult Client::get(const Key &key) const
  {
    return impl_->get(key);
  }

  Client::ValueResult Client::get(
      const std::string &key) const
  {
    return impl_->get(key);
  }

  Client::VoidResult Client::remove(const Key &key)
  {
    return impl_->remove(key);
  }

  Client::VoidResult Client::remove(const std::string &key)
  {
    return impl_->remove(key);
  }

  bool Client::contains(const Key &key) const
  {
    return impl_->contains(key);
  }

  bool Client::contains(const std::string &key) const
  {
    return impl_->contains(key);
  }

  std::size_t Client::size() const noexcept
  {
    return impl_->size();
  }

  bool Client::empty() const noexcept
  {
    return impl_->empty();
  }

  Client::TickStateResult Client::tick(bool prune_completed)
  {
    return impl_->tick(prune_completed);
  }

  Client::SyncStateResult Client::sync_state() const
  {
    return impl_->sync_state();
  }

  Result<std::size_t, Error> Client::retry_expired()
  {
    return impl_->retry_expired();
  }

  Result<std::size_t, Error> Client::prune_completed()
  {
    return impl_->prune_completed();
  }

  Result<std::size_t, Error> Client::prune_failed()
  {
    return impl_->prune_failed();
  }

  Client::VoidResult Client::start_transport()
  {
    return impl_->start_transport();
  }

  void Client::stop_transport() noexcept
  {
    impl_->stop_transport();
  }

  bool Client::transport_running() const noexcept
  {
    return impl_->transport_running();
  }

  Client::VoidResult Client::connect(const Peer &peer)
  {
    return impl_->connect(peer);
  }

  Client::VoidResult Client::disconnect(const Peer &peer)
  {
    return impl_->disconnect(peer);
  }

  Client::VoidResult Client::start_discovery()
  {
    return impl_->start_discovery();
  }

  void Client::stop_discovery() noexcept
  {
    impl_->stop_discovery();
  }

  bool Client::discovery_running() const noexcept
  {
    return impl_->discovery_running();
  }

  Client::PeersResult Client::peers() const
  {
    return impl_->peers();
  }

  Client::NodeInfoResult Client::node_info() const
  {
    return impl_->node_info();
  }

  Client::NodeInfoResult Client::refresh_node_info()
  {
    return impl_->refresh_node_info();
  }

  const ClientOptions &Client::options() const noexcept
  {
    return impl_->options();
  }

} // namespace softadastra::sdk
