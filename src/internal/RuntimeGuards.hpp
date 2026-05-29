/**
 *
 *  @file RuntimeGuards.hpp
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

#ifndef SOFTADASTRA_SDK_INTERNAL_RUNTIME_GUARDS_HPP
#define SOFTADASTRA_SDK_INTERNAL_RUNTIME_GUARDS_HPP

#include <softadastra/sdk/Error.hpp>
#include <softadastra/sdk/Result.hpp>

#include "Runtime.hpp"

namespace softadastra::sdk::internal
{
  /**
   * @brief Internal guard helpers for SDK runtime access.
   *
   * RuntimeGuards centralizes runtime validation checks used by ClientImpl.
   *
   * It keeps ClientImpl methods small and ensures every operation returns
   * consistent SDK errors when the runtime is closed, incomplete, or when an
   * optional service is disabled.
   */
  class RuntimeGuards
  {
  public:
    /**
     * @brief Result returned by guard checks.
     */
    using GuardResult = Result<void, Error>;

    /**
     * @brief Ensures that the SDK client is open and has a local runtime.
     *
     * @param is_open Current client open state.
     * @param runtime Internal SDK runtime.
     * @return ok on success, error on failure.
     */
    [[nodiscard]] static GuardResult require_open(
        bool is_open,
        const Runtime &runtime);

    /**
     * @brief Ensures that transport support is available.
     *
     * @param is_open Current client open state.
     * @param transport_enabled true if transport is enabled by options.
     * @param runtime Internal SDK runtime.
     * @return ok on success, error on failure.
     */
    [[nodiscard]] static GuardResult require_transport(
        bool is_open,
        bool transport_enabled,
        const Runtime &runtime);

    /**
     * @brief Ensures that discovery support is available.
     *
     * @param is_open Current client open state.
     * @param discovery_enabled true if discovery is enabled by options.
     * @param runtime Internal SDK runtime.
     * @return ok on success, error on failure.
     */
    [[nodiscard]] static GuardResult require_discovery(
        bool is_open,
        bool discovery_enabled,
        const Runtime &runtime);

    /**
     * @brief Ensures that metadata support is available.
     *
     * @param is_open Current client open state.
     * @param runtime Internal SDK runtime.
     * @return ok on success, error on failure.
     */
    [[nodiscard]] static GuardResult require_metadata(
        bool is_open,
        const Runtime &runtime);
  };

} // namespace softadastra::sdk::internal

#endif // SOFTADASTRA_SDK_INTERNAL_RUNTIME_GUARDS_HPP
