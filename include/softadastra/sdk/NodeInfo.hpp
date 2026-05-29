/**
 *
 *  @file NodeInfo.hpp
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

#ifndef SOFTADASTRA_SDK_NODE_INFO_HPP
#define SOFTADASTRA_SDK_NODE_INFO_HPP

#include <cstdint>
#include <string>
#include <vector>

namespace softadastra::sdk
{
  /**
   * @brief Public SDK node information.
   *
   * NodeInfo exposes a stable developer-facing view of a Softadastra node.
   *
   * It is intentionally independent from the internal metadata module. The SDK
   * converts internal metadata objects to this public representation before
   * returning them to users.
   */
  class NodeInfo
  {
  public:
    /**
     * @brief Creates an empty invalid node info object.
     */
    NodeInfo() = default;

    /**
     * @brief Creates node information.
     *
     * @param node_id Logical node identifier.
     * @param display_name Human-readable node name.
     * @param hostname Local hostname.
     * @param os_name Operating system name.
     * @param version Runtime or product version.
     */
    NodeInfo(
        std::string node_id,
        std::string display_name,
        std::string hostname,
        std::string os_name,
        std::string version);

    /**
     * @brief Returns the logical node identifier.
     *
     * @return Node id.
     */
    [[nodiscard]] const std::string &node_id() const noexcept;

    /**
     * @brief Returns the human-readable node name.
     *
     * @return Display name.
     */
    [[nodiscard]] const std::string &display_name() const noexcept;

    /**
     * @brief Returns the local hostname.
     *
     * @return Hostname.
     */
    [[nodiscard]] const std::string &hostname() const noexcept;

    /**
     * @brief Returns the operating system name.
     *
     * @return Operating system name.
     */
    [[nodiscard]] const std::string &os_name() const noexcept;

    /**
     * @brief Returns the runtime or product version.
     *
     * @return Version string.
     */
    [[nodiscard]] const std::string &version() const noexcept;

    /**
     * @brief Returns the timestamp at which the node started.
     *
     * The value is expressed in milliseconds since the Unix epoch.
     *
     * @return Start timestamp in milliseconds.
     */
    [[nodiscard]] std::int64_t started_at_ms() const noexcept;

    /**
     * @brief Returns the node uptime.
     *
     * The value is expressed in milliseconds.
     *
     * @return Uptime in milliseconds.
     */
    [[nodiscard]] std::int64_t uptime_ms() const noexcept;

    /**
     * @brief Returns the list of public capability names.
     *
     * @return Capability names.
     */
    [[nodiscard]] const std::vector<std::string> &capabilities() const noexcept;

    /**
     * @brief Sets the logical node identifier.
     *
     * @param value New node id.
     */
    void set_node_id(std::string value);

    /**
     * @brief Sets the human-readable node name.
     *
     * @param value New display name.
     */
    void set_display_name(std::string value);

    /**
     * @brief Sets the local hostname.
     *
     * @param value New hostname.
     */
    void set_hostname(std::string value);

    /**
     * @brief Sets the operating system name.
     *
     * @param value New operating system name.
     */
    void set_os_name(std::string value);

    /**
     * @brief Sets the runtime or product version.
     *
     * @param value New version.
     */
    void set_version(std::string value);

    /**
     * @brief Sets the node start timestamp.
     *
     * The value must be expressed in milliseconds since the Unix epoch.
     *
     * @param value Start timestamp in milliseconds.
     */
    void set_started_at_ms(std::int64_t value) noexcept;

    /**
     * @brief Sets the node uptime.
     *
     * The value must be expressed in milliseconds.
     *
     * @param value Uptime in milliseconds.
     */
    void set_uptime_ms(std::int64_t value) noexcept;

    /**
     * @brief Sets public capability names.
     *
     * @param values Capability names.
     */
    void set_capabilities(std::vector<std::string> values);

    /**
     * @brief Adds one public capability name.
     *
     * Empty capability names are ignored.
     *
     * @param value Capability name.
     */
    void add_capability(std::string value);

    /**
     * @brief Returns the best display label.
     *
     * The display name is preferred. If it is empty, the node id is returned.
     *
     * @return Display label.
     */
    [[nodiscard]] const std::string &label() const noexcept;

    /**
     * @brief Returns true if the node has a capability.
     *
     * @param value Capability name.
     * @return true if the capability exists.
     */
    [[nodiscard]] bool has_capability(const std::string &value) const noexcept;

    /**
     * @brief Returns true if the node info can be used by SDK users.
     *
     * A valid NodeInfo must contain:
     * - node id
     * - hostname
     * - operating system name
     * - version
     *
     * @return true if the node info is valid.
     */
    [[nodiscard]] bool is_valid() const noexcept;

    /**
     * @brief Backward-compatible alias for is_valid().
     *
     * @return true if the node info is valid.
     */
    [[nodiscard]] bool valid() const noexcept;

    /**
     * @brief Clears all node information.
     */
    void clear() noexcept;

    /**
     * @brief Compares two node info objects for equality.
     *
     * @param left Left node info.
     * @param right Right node info.
     * @return true if both objects contain the same values.
     */
    [[nodiscard]] friend bool operator==(
        const NodeInfo &left,
        const NodeInfo &right) noexcept
    {
      return left.node_id_ == right.node_id_ &&
             left.display_name_ == right.display_name_ &&
             left.hostname_ == right.hostname_ &&
             left.os_name_ == right.os_name_ &&
             left.version_ == right.version_ &&
             left.started_at_ms_ == right.started_at_ms_ &&
             left.uptime_ms_ == right.uptime_ms_ &&
             left.capabilities_ == right.capabilities_;
    }

    /**
     * @brief Compares two node info objects for inequality.
     *
     * @param left Left node info.
     * @param right Right node info.
     * @return true if the objects are different.
     */
    [[nodiscard]] friend bool operator!=(
        const NodeInfo &left,
        const NodeInfo &right) noexcept
    {
      return !(left == right);
    }

  private:
    std::string node_id_{};
    std::string display_name_{};
    std::string hostname_{};
    std::string os_name_{};
    std::string version_{};
    std::int64_t started_at_ms_{0};
    std::int64_t uptime_ms_{0};
    std::vector<std::string> capabilities_{};
  };

} // namespace softadastra::sdk

#endif // SOFTADASTRA_SDK_NODE_INFO_HPP
