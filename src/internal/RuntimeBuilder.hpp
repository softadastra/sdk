/**
 *
 *  @file RuntimeBuilder.hpp
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

#ifndef SOFTADASTRA_SDK_INTERNAL_RUNTIME_BUILDER_HPP
#define SOFTADASTRA_SDK_INTERNAL_RUNTIME_BUILDER_HPP

#include <softadastra/sdk/Error.hpp>
#include <softadastra/sdk/Result.hpp>

#include "Runtime.hpp"

namespace softadastra::sdk
{
  class ClientOptions;
}

namespace softadastra::sdk::internal
{
  /**
   * @brief Builds the internal SDK runtime from public client options.
   *
   * RuntimeBuilder is the only place where public SDK configuration is
   * translated into low-level Softadastra runtime modules.
   *
   * This keeps ClientImpl focused on behavior and keeps module construction
   * isolated from the public SDK API.
   */
  class RuntimeBuilder
  {
  public:
    /**
     * @brief Result returned when building a runtime.
     */
    using BuildResult = Result<Runtime, Error>;

    /**
     * @brief Builds a complete internal runtime from SDK client options.
     *
     * The builder always creates the required local runtime modules:
     * - store
     * - sync config
     * - sync context
     * - sync engine
     * - sync scheduler
     * - metadata service
     *
     * Optional modules are created only when enabled in ClientOptions:
     * - transport
     * - discovery
     *
     * @param options Public SDK client options.
     * @return Runtime on success, Error on failure.
     */
    [[nodiscard]] static BuildResult build(
        const ClientOptions &options);

  private:
    /**
     * @brief Creates the local store module.
     *
     * @param runtime Runtime being built.
     * @param options Public SDK client options.
     */
    static void build_store(
        Runtime &runtime,
        const ClientOptions &options);

    /**
     * @brief Creates the sync modules.
     *
     * @param runtime Runtime being built.
     * @param options Public SDK client options.
     */
    static void build_sync(
        Runtime &runtime,
        const ClientOptions &options);

    /**
     * @brief Creates transport modules when transport is enabled.
     *
     * @param runtime Runtime being built.
     * @param options Public SDK client options.
     */
    static void build_transport(
        Runtime &runtime,
        const ClientOptions &options);

    /**
     * @brief Creates discovery service when discovery is enabled.
     *
     * @param runtime Runtime being built.
     * @param options Public SDK client options.
     */
    static void build_discovery(
        Runtime &runtime,
        const ClientOptions &options);

    /**
     * @brief Creates metadata service.
     *
     * @param runtime Runtime being built.
     * @param options Public SDK client options.
     */
    static void build_metadata(
        Runtime &runtime,
        const ClientOptions &options);
  };

} // namespace softadastra::sdk::internal

#endif // SOFTADASTRA_SDK_INTERNAL_RUNTIME_BUILDER_HPP
