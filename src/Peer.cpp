/**
 *
 *  @file Peer.cpp
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

#include <softadastra/sdk/Peer.hpp>
#include <utility>

namespace softadastra::sdk
{
  Peer::Peer(
      std::string node_id,
      std::string host,
      std::uint16_t port)
      : node_id_(std::move(node_id)),
        host_(std::move(host)),
        port_(port)
  {
  }

  Peer Peer::local(
      std::string node_id,
      std::uint16_t port)
  {
    return Peer{
        std::move(node_id),
        "127.0.0.1",
        port};
  }

  const std::string &Peer::node_id() const noexcept
  {
    return node_id_;
  }

  const std::string &Peer::host() const noexcept
  {
    return host_;
  }

  std::uint16_t Peer::port() const noexcept
  {
    return port_;
  }

  void Peer::set_node_id(std::string value)
  {
    node_id_ = std::move(value);
  }

  void Peer::set_host(std::string value)
  {
    host_ = std::move(value);
  }

  void Peer::set_port(std::uint16_t value) noexcept
  {
    port_ = value;
  }

  bool Peer::is_localhost() const noexcept
  {
    return host_ == "127.0.0.1" ||
           host_ == "localhost";
  }

  bool Peer::is_valid() const noexcept
  {
    return !node_id_.empty() &&
           !host_.empty() &&
           port_ != 0;
  }

  bool Peer::valid() const noexcept
  {
    return is_valid();
  }

  void Peer::clear() noexcept
  {
    node_id_.clear();
    host_.clear();
    port_ = 0;
  }

} // namespace softadastra::sdk
