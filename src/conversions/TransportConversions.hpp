/**
 *
 *  @file TransportConversions.hpp
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

#ifndef SOFTADASTRA_SDK_TRANSPORT_CONVERSIONS_HPP
#define SOFTADASTRA_SDK_TRANSPORT_CONVERSIONS_HPP

#include <softadastra/sdk/Peer.hpp>
#include <softadastra/transport/core/PeerInfo.hpp>
#include <softadastra/transport/core/TransportConfig.hpp>

namespace softadastra::sdk
{
  class ClientOptions;
}

namespace softadastra::sdk::internal
{
  /**
   * @brief Converts public SDK client options to an internal transport config.
   *
   * @param options Public SDK client options.
   * @return Internal transport configuration.
   */
  [[nodiscard]] softadastra::transport::core::TransportConfig
  to_transport_config(const ClientOptions &options);

  /**
   * @brief Converts a public SDK peer to an internal transport peer.
   *
   * @param peer Public SDK peer.
   * @return Internal transport peer info.
   */
  [[nodiscard]] softadastra::transport::core::PeerInfo to_transport_peer(
      const Peer &peer);

  /**
   * @brief Converts an internal transport peer to a public SDK peer.
   *
   * @param peer Internal transport peer info.
   * @return Public SDK peer.
   */
  [[nodiscard]] Peer from_transport_peer(
      const softadastra::transport::core::PeerInfo &peer);

} // namespace softadastra::sdk::internal

#endif // SOFTADASTRA_SDK_TRANSPORT_CONVERSIONS_HPP
