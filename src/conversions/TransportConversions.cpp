/**
 *
 *  @file TransportConversions.cpp
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

#include "TransportConversions.hpp"
#include <softadastra/sdk/ClientOptions.hpp>

namespace softadastra::sdk::internal
{
  softadastra::transport::core::TransportConfig to_transport_config(
      const ClientOptions &options)
  {
    return softadastra::transport::core::TransportConfig{
        options.transport_host(),
        options.transport_port()};
  }

  softadastra::transport::core::PeerInfo to_transport_peer(
      const Peer &peer)
  {
    return softadastra::transport::core::PeerInfo{
        peer.node_id(),
        peer.host(),
        peer.port()};
  }

  Peer from_transport_peer(
      const softadastra::transport::core::PeerInfo &peer)
  {
    return Peer{
        peer.node_id,
        peer.host,
        peer.port};
  }

} // namespace softadastra::sdk::internal
