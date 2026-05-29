/**
 *
 *  @file DiscoveryConversions.hpp
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

#ifndef SOFTADASTRA_SDK_DISCOVERY_CONVERSIONS_HPP
#define SOFTADASTRA_SDK_DISCOVERY_CONVERSIONS_HPP

#include <softadastra/sdk/Peer.hpp>
#include <softadastra/discovery/DiscoveryOptions.hpp>
#include <softadastra/discovery/Peer.hpp>

namespace softadastra::sdk
{
  class ClientOptions;
}

namespace softadastra::sdk::internal
{
  /**
   * @brief Converts public SDK client options to internal discovery options.
   *
   * @param options Public SDK client options.
   * @return Internal discovery options.
   */
  [[nodiscard]] softadastra::discovery::DiscoveryOptions
  to_discovery_options(const ClientOptions &options);

  /**
   * @brief Converts an internal discovery peer to a public SDK peer.
   *
   * @param peer Internal discovery peer.
   * @return Public SDK peer.
   */
  [[nodiscard]] Peer from_discovery_peer(
      const softadastra::discovery::Peer &peer);

  /**
   * @brief Converts a public SDK peer to an internal discovery peer.
   *
   * @param peer Public SDK peer.
   * @return Internal discovery peer.
   */
  [[nodiscard]] softadastra::discovery::Peer to_discovery_peer(
      const Peer &peer);

} // namespace softadastra::sdk::internal

#endif // SOFTADASTRA_SDK_DISCOVERY_CONVERSIONS_HPP
