/**
 *
 *  @file Peer.hpp
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

#ifndef SOFTADASTRA_SDK_PEER_HPP
#define SOFTADASTRA_SDK_PEER_HPP

#include <cstdint>
#include <string>

namespace softadastra::sdk
{
  /**
   * @brief Public SDK peer representation.
   *
   * Peer describes a remote or local Softadastra node that can be used by
   * transport, discovery, sync, or higher-level SDK features.
   *
   * The SDK intentionally exposes only stable public fields:
   * - node identifier
   * - host
   * - port
   *
   * Internal transport and discovery peer types are not exposed through this
   * public header.
   */
  class Peer
  {
  public:
    /**
     * @brief Creates an empty invalid peer.
     */
    Peer() = default;

    /**
     * @brief Creates a peer.
     *
     * @param node_id Logical peer node identifier.
     * @param host Peer host or IP address.
     * @param port Peer transport port.
     */
    Peer(
        std::string node_id,
        std::string host,
        std::uint16_t port);

    /**
     * @brief Creates a localhost peer.
     *
     * @param node_id Logical peer node identifier.
     * @param port Peer transport port.
     * @return SDK peer.
     */
    [[nodiscard]] static Peer local(
        std::string node_id,
        std::uint16_t port);

    /**
     * @brief Returns the logical peer node identifier.
     *
     * @return Peer node id.
     */
    [[nodiscard]] const std::string &node_id() const noexcept;

    /**
     * @brief Returns the peer host.
     *
     * @return Peer host or IP address.
     */
    [[nodiscard]] const std::string &host() const noexcept;

    /**
     * @brief Returns the peer transport port.
     *
     * @return Peer port.
     */
    [[nodiscard]] std::uint16_t port() const noexcept;

    /**
     * @brief Sets the logical peer node identifier.
     *
     * @param value New node id.
     */
    void set_node_id(std::string value);

    /**
     * @brief Sets the peer host.
     *
     * @param value New host.
     */
    void set_host(std::string value);

    /**
     * @brief Sets the peer transport port.
     *
     * @param value New port.
     */
    void set_port(std::uint16_t value) noexcept;

    /**
     * @brief Returns true if this peer points to localhost.
     *
     * @return true if the host is localhost or 127.0.0.1.
     */
    [[nodiscard]] bool is_localhost() const noexcept;

    /**
     * @brief Returns true if the peer can be used by the SDK.
     *
     * A valid peer must have:
     * - a non-empty node id
     * - a non-empty host
     * - a non-zero port
     *
     * @return true if the peer is valid.
     */
    [[nodiscard]] bool is_valid() const noexcept;

    /**
     * @brief Backward-compatible alias for is_valid().
     *
     * @return true if the peer is valid.
     */
    [[nodiscard]] bool valid() const noexcept;

    /**
     * @brief Clears the peer and makes it invalid.
     */
    void clear() noexcept;

    /**
     * @brief Compares two peers for equality.
     *
     * @param left Left peer.
     * @param right Right peer.
     * @return true if both peers contain the same node id, host, and port.
     */
    [[nodiscard]] friend bool operator==(
        const Peer &left,
        const Peer &right) noexcept
    {
      return left.node_id_ == right.node_id_ &&
             left.host_ == right.host_ &&
             left.port_ == right.port_;
    }

    /**
     * @brief Compares two peers for inequality.
     *
     * @param left Left peer.
     * @param right Right peer.
     * @return true if peers are different.
     */
    [[nodiscard]] friend bool operator!=(
        const Peer &left,
        const Peer &right) noexcept
    {
      return !(left == right);
    }

  private:
    std::string node_id_{};
    std::string host_{"127.0.0.1"};
    std::uint16_t port_{0};
  };

} // namespace softadastra::sdk

#endif // SOFTADASTRA_SDK_PEER_HPP
