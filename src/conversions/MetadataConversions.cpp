/**
 *
 *  @file MetadataConversions.cpp
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

#include "MetadataConversions.hpp"
#include <softadastra/sdk/ClientOptions.hpp>

#include <string>
#include <type_traits>

namespace
{
  [[nodiscard]] std::string capability_to_string(
      softadastra::metadata::types::CapabilityType capability)
  {
    using CapabilityValue =
        std::underlying_type_t<
            softadastra::metadata::types::CapabilityType>;

    return "capability:" +
           std::to_string(
               static_cast<CapabilityValue>(capability));
  }
}

namespace softadastra::sdk::internal
{
  softadastra::metadata::MetadataOptions to_metadata_options(
      const ClientOptions &options)
  {
    softadastra::metadata::MetadataOptions result;

    result.node_id = options.node_id();
    result.display_name =
        options.display_name().empty()
            ? options.node_id()
            : options.display_name();
    result.version = options.version();

    return result;
  }

  NodeInfo from_node_metadata(
      const softadastra::metadata::core::NodeMetadata &metadata)
  {
    NodeInfo result{
        metadata.identity.node_id,
        metadata.identity.display_name,
        metadata.runtime.hostname,
        metadata.runtime.os_name,
        metadata.runtime.version};

    result.set_uptime_ms(metadata.runtime.uptime.millis());

    for (const auto capability : metadata.capabilities.values)
    {
      result.add_capability(capability_to_string(capability));
    }

    return result;
  }

} // namespace softadastra::sdk::internal
