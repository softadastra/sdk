/**
 *
 *  @file MetadataConversions.hpp
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

#ifndef SOFTADASTRA_SDK_METADATA_CONVERSIONS_HPP
#define SOFTADASTRA_SDK_METADATA_CONVERSIONS_HPP

#include <softadastra/sdk/NodeInfo.hpp>
#include <softadastra/metadata/MetadataOptions.hpp>
#include <softadastra/metadata/core/NodeMetadata.hpp>

namespace softadastra::sdk
{
  class ClientOptions;
}

namespace softadastra::sdk::internal
{
  /**
   * @brief Converts public SDK client options to internal metadata options.
   *
   * @param options Public SDK client options.
   * @return Internal metadata options.
   */
  [[nodiscard]] softadastra::metadata::MetadataOptions
  to_metadata_options(const ClientOptions &options);

  /**
   * @brief Converts internal node metadata to public SDK node information.
   *
   * @param metadata Internal node metadata.
   * @return Public SDK node information.
   */
  [[nodiscard]] NodeInfo from_node_metadata(
      const softadastra::metadata::core::NodeMetadata &metadata);

} // namespace softadastra::sdk::internal

#endif // SOFTADASTRA_SDK_METADATA_CONVERSIONS_HPP
