/**
 *
 *  @file DiscoveryConversions.cpp
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

#include "DiscoveryConversions.hpp"
#include <softadastra/sdk/ClientOptions.hpp>

namespace softadastra::sdk::internal
{
  softadastra::discovery::DiscoveryOptions to_discovery_options(
      const ClientOptions &options)
  {
    softadastra::discovery::DiscoveryOptions result;

    result.node_id = options.node_id();
    result.bind_host = options.discovery_host();
    result.bind_port = options.discovery_port();

    return result;
  }

  Peer from_discovery_peer(
      const softadastra::discovery::Peer &peer)
  {
    return Peer{
        peer.node_id,
        peer.host,
        peer.port};
  }

  softadastra::discovery::Peer to_discovery_peer(
      const Peer &peer)
  {
    return softadastra::discovery::Peer{
        peer.node_id(),
        peer.host(),
        peer.port()};
  }

} // namespace softadastra::sdk::internal
