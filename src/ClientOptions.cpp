/**
 *
 *  @file ClientOptions.cpp
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

#include <softadastra/sdk/ClientOptions.hpp>

#include <utility>

namespace softadastra::sdk
{
  ClientOptions::ClientOptions() = default;

  ClientOptions::ClientOptions(std::string node_id)
      : node_id_(std::move(node_id))
  {
    if (display_name_.empty())
    {
      display_name_ = node_id_;
    }
  }

  ClientOptions ClientOptions::memory_only(std::string node_id)
  {
    ClientOptions options{std::move(node_id)};

    options.store_wal_enabled_ = false;
    options.auto_flush_ = false;
    options.initial_store_capacity_ = 1024;

    options.sync_batch_size_ = 64;
    options.max_sync_retries_ = 2;
    options.require_ack_ = false;
    options.auto_queue_ = true;

    options.transport_backend_ = TransportBackend::AsyncTcp;

    return options;
  }

  ClientOptions ClientOptions::local(std::string node_id)
  {
    return memory_only(std::move(node_id));
  }

  ClientOptions ClientOptions::persistent(
      std::string node_id,
      std::string wal_path)
  {
    ClientOptions options{std::move(node_id)};

    options.store_wal_path_ = std::move(wal_path);
    options.store_wal_enabled_ = true;
    options.auto_flush_ = true;
    options.initial_store_capacity_ = 1024;

    options.sync_batch_size_ = 64;
    options.max_sync_retries_ = 5;
    options.require_ack_ = true;
    options.auto_queue_ = true;

    options.transport_backend_ = TransportBackend::AsyncTcp;

    return options;
  }

  ClientOptions ClientOptions::durable(
      std::string node_id,
      std::string wal_path)
  {
    return persistent(
        std::move(node_id),
        std::move(wal_path));
  }

  ClientOptions ClientOptions::fast(
      std::string node_id,
      std::string wal_path)
  {
    ClientOptions options{std::move(node_id)};

    options.store_wal_path_ = std::move(wal_path);
    options.store_wal_enabled_ = true;
    options.auto_flush_ = false;
    options.initial_store_capacity_ = 1024;

    options.sync_batch_size_ = 128;
    options.max_sync_retries_ = 2;
    options.require_ack_ = false;
    options.auto_queue_ = true;

    options.transport_backend_ = TransportBackend::AsyncTcp;

    return options;
  }

  const std::string &ClientOptions::node_id() const noexcept
  {
    return node_id_;
  }

  const std::string &ClientOptions::store_wal_path() const noexcept
  {
    return store_wal_path_;
  }

  bool ClientOptions::store_wal_enabled() const noexcept
  {
    return store_wal_enabled_;
  }

  bool ClientOptions::auto_flush() const noexcept
  {
    return auto_flush_;
  }

  std::size_t ClientOptions::initial_store_capacity() const noexcept
  {
    return initial_store_capacity_;
  }

  std::size_t ClientOptions::sync_batch_size() const noexcept
  {
    return sync_batch_size_;
  }

  std::uint32_t ClientOptions::max_sync_retries() const noexcept
  {
    return max_sync_retries_;
  }

  bool ClientOptions::require_ack() const noexcept
  {
    return require_ack_;
  }

  bool ClientOptions::auto_queue() const noexcept
  {
    return auto_queue_;
  }

  bool ClientOptions::transport_enabled() const noexcept
  {
    return transport_enabled_;
  }

  ClientOptions::TransportBackend
  ClientOptions::transport_backend() const noexcept
  {
    return transport_backend_;
  }

  const std::string &ClientOptions::transport_host() const noexcept
  {
    return transport_host_;
  }

  std::uint16_t ClientOptions::transport_port() const noexcept
  {
    return transport_port_;
  }

  bool ClientOptions::discovery_enabled() const noexcept
  {
    return discovery_enabled_;
  }

  const std::string &ClientOptions::discovery_host() const noexcept
  {
    return discovery_host_;
  }

  std::uint16_t ClientOptions::discovery_port() const noexcept
  {
    return discovery_port_;
  }

  const std::string &ClientOptions::display_name() const noexcept
  {
    return display_name_;
  }

  const std::string &ClientOptions::version() const noexcept
  {
    return version_;
  }

  void ClientOptions::set_node_id(std::string value)
  {
    node_id_ = std::move(value);

    if (display_name_.empty())
    {
      display_name_ = node_id_;
    }
  }

  void ClientOptions::set_store_wal_path(std::string value)
  {
    store_wal_path_ = std::move(value);
  }

  void ClientOptions::set_store_wal_enabled(bool value) noexcept
  {
    store_wal_enabled_ = value;
  }

  void ClientOptions::set_auto_flush(bool value) noexcept
  {
    auto_flush_ = value;
  }

  void ClientOptions::set_initial_store_capacity(
      std::size_t value) noexcept
  {
    initial_store_capacity_ = value;
  }

  void ClientOptions::set_sync_batch_size(std::size_t value) noexcept
  {
    sync_batch_size_ = value;
  }

  void ClientOptions::set_max_sync_retries(
      std::uint32_t value) noexcept
  {
    max_sync_retries_ = value;
  }

  void ClientOptions::set_require_ack(bool value) noexcept
  {
    require_ack_ = value;
  }

  void ClientOptions::set_auto_queue(bool value) noexcept
  {
    auto_queue_ = value;
  }

  void ClientOptions::set_transport_backend(
      TransportBackend value) noexcept
  {
    transport_backend_ = value;
  }

  void ClientOptions::set_transport(
      std::string host,
      std::uint16_t port)
  {
    set_transport(
        std::move(host),
        port,
        TransportBackend::AsyncTcp);
  }

  void ClientOptions::set_transport(
      std::string host,
      std::uint16_t port,
      TransportBackend backend)
  {
    transport_enabled_ = true;
    transport_backend_ = backend;
    transport_host_ = std::move(host);
    transport_port_ = port;
  }

  void ClientOptions::disable_transport() noexcept
  {
    transport_enabled_ = false;
    transport_backend_ = TransportBackend::AsyncTcp;
    transport_host_ = "0.0.0.0";
    transport_port_ = 0;
  }

  void ClientOptions::set_discovery(
      std::string host,
      std::uint16_t port)
  {
    discovery_enabled_ = true;
    discovery_host_ = std::move(host);
    discovery_port_ = port;
  }

  void ClientOptions::disable_discovery() noexcept
  {
    discovery_enabled_ = false;
    discovery_host_ = "0.0.0.0";
    discovery_port_ = 0;
  }

  void ClientOptions::set_metadata(
      std::string name,
      std::string runtime_version)
  {
    display_name_ = std::move(name);
    version_ = std::move(runtime_version);
  }

  ClientOptions ClientOptions::with_transport(
      std::string host,
      std::uint16_t port) const
  {
    return with_transport(
        std::move(host),
        port,
        TransportBackend::AsyncTcp);
  }

  ClientOptions ClientOptions::with_transport(
      std::string host,
      std::uint16_t port,
      TransportBackend backend) const
  {
    ClientOptions options = *this;
    options.set_transport(
        std::move(host),
        port,
        backend);

    return options;
  }

  ClientOptions ClientOptions::with_local_transport(
      std::uint16_t port) const
  {
    return with_local_transport(
        port,
        TransportBackend::AsyncTcp);
  }

  ClientOptions ClientOptions::with_local_transport(
      std::uint16_t port,
      TransportBackend backend) const
  {
    return with_transport(
        "127.0.0.1",
        port,
        backend);
  }

  ClientOptions ClientOptions::with_discovery(
      std::string host,
      std::uint16_t port) const
  {
    ClientOptions options = *this;
    options.set_discovery(std::move(host), port);
    return options;
  }

  ClientOptions ClientOptions::with_local_discovery(
      std::uint16_t port) const
  {
    return with_discovery("127.0.0.1", port);
  }

  ClientOptions ClientOptions::with_metadata(
      std::string name,
      std::string runtime_version) const
  {
    ClientOptions options = *this;
    options.set_metadata(
        std::move(name),
        std::move(runtime_version));

    return options;
  }

  bool ClientOptions::is_valid() const noexcept
  {
    if (node_id_.empty())
    {
      return false;
    }

    if (store_wal_enabled_ && store_wal_path_.empty())
    {
      return false;
    }

    if (initial_store_capacity_ == 0)
    {
      return false;
    }

    if (sync_batch_size_ == 0)
    {
      return false;
    }

    if (transport_enabled_)
    {
      if (transport_host_.empty() || transport_port_ == 0)
      {
        return false;
      }
    }

    if (discovery_enabled_)
    {
      if (discovery_host_.empty() || discovery_port_ == 0)
      {
        return false;
      }
    }

    if (version_.empty())
    {
      return false;
    }

    return true;
  }

  bool ClientOptions::valid() const noexcept
  {
    return is_valid();
  }

} // namespace softadastra::sdk
