/**
 *
 *  @file NodeInfo.cpp
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

#include <softadastra/sdk/NodeInfo.hpp>
#include <algorithm>
#include <utility>

namespace softadastra::sdk
{
  NodeInfo::NodeInfo(
      std::string node_id,
      std::string display_name,
      std::string hostname,
      std::string os_name,
      std::string version)
      : node_id_(std::move(node_id)),
        display_name_(std::move(display_name)),
        hostname_(std::move(hostname)),
        os_name_(std::move(os_name)),
        version_(std::move(version))
  {
  }

  const std::string &NodeInfo::node_id() const noexcept
  {
    return node_id_;
  }

  const std::string &NodeInfo::display_name() const noexcept
  {
    return display_name_;
  }

  const std::string &NodeInfo::hostname() const noexcept
  {
    return hostname_;
  }

  const std::string &NodeInfo::os_name() const noexcept
  {
    return os_name_;
  }

  const std::string &NodeInfo::version() const noexcept
  {
    return version_;
  }

  std::int64_t NodeInfo::started_at_ms() const noexcept
  {
    return started_at_ms_;
  }

  std::int64_t NodeInfo::uptime_ms() const noexcept
  {
    return uptime_ms_;
  }

  const std::vector<std::string> &NodeInfo::capabilities() const noexcept
  {
    return capabilities_;
  }

  void NodeInfo::set_node_id(std::string value)
  {
    node_id_ = std::move(value);
  }

  void NodeInfo::set_display_name(std::string value)
  {
    display_name_ = std::move(value);
  }

  void NodeInfo::set_hostname(std::string value)
  {
    hostname_ = std::move(value);
  }

  void NodeInfo::set_os_name(std::string value)
  {
    os_name_ = std::move(value);
  }

  void NodeInfo::set_version(std::string value)
  {
    version_ = std::move(value);
  }

  void NodeInfo::set_started_at_ms(std::int64_t value) noexcept
  {
    started_at_ms_ = value;
  }

  void NodeInfo::set_uptime_ms(std::int64_t value) noexcept
  {
    uptime_ms_ = value;
  }

  void NodeInfo::set_capabilities(std::vector<std::string> values)
  {
    capabilities_ = std::move(values);
  }

  void NodeInfo::add_capability(std::string value)
  {
    if (!value.empty())
    {
      capabilities_.push_back(std::move(value));
    }
  }

  const std::string &NodeInfo::label() const noexcept
  {
    return display_name_.empty() ? node_id_ : display_name_;
  }

  bool NodeInfo::has_capability(const std::string &value) const noexcept
  {
    return std::find(
               capabilities_.begin(),
               capabilities_.end(),
               value) != capabilities_.end();
  }

  bool NodeInfo::is_valid() const noexcept
  {
    return !node_id_.empty() &&
           !hostname_.empty() &&
           !os_name_.empty() &&
           !version_.empty();
  }

  bool NodeInfo::valid() const noexcept
  {
    return is_valid();
  }

  void NodeInfo::clear() noexcept
  {
    node_id_.clear();
    display_name_.clear();
    hostname_.clear();
    os_name_.clear();
    version_.clear();
    started_at_ms_ = 0;
    uptime_ms_ = 0;
    capabilities_.clear();
  }

} // namespace softadastra::sdk
