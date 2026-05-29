/**
 *
 *  @file ErrorMapper.hpp
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

#ifndef SOFTADASTRA_SDK_INTERNAL_ERROR_MAPPER_HPP
#define SOFTADASTRA_SDK_INTERNAL_ERROR_MAPPER_HPP

#include <string>

#include <softadastra/sdk/Error.hpp>
#include <softadastra/core/errors/Error.hpp>
#include <softadastra/core/errors/ErrorCode.hpp>

namespace softadastra::sdk::internal
{
  /**
   * @brief Maps internal Softadastra errors to public SDK errors.
   *
   * ErrorMapper is an internal SDK boundary object.
   *
   * It prevents internal module error types from leaking into the public SDK
   * API. Store, sync, transport, discovery, metadata, and core errors are
   * converted to softadastra::sdk::Error before they reach SDK users.
   */
  class ErrorMapper
  {
  public:
    /**
     * @brief Converts a core Softadastra error to a public SDK error.
     *
     * @param error Internal core error.
     * @return Public SDK error.
     */
    [[nodiscard]] static Error from_core(
        const softadastra::core::errors::Error &error);

    /**
     * @brief Converts a core Softadastra error code to a public SDK error code.
     *
     * @param code Internal core error code.
     * @return Public SDK error code.
     */
    [[nodiscard]] static Error::Code from_core_code(
        softadastra::core::errors::ErrorCode code) noexcept;

    /**
     * @brief Creates a public SDK store error.
     *
     * @param message Developer-facing error message.
     * @param context Optional diagnostic context.
     * @return Public SDK error.
     */
    [[nodiscard]] static Error store(
        std::string message,
        std::string context = {});

    /**
     * @brief Creates a public SDK sync error.
     *
     * @param message Developer-facing error message.
     * @param context Optional diagnostic context.
     * @return Public SDK error.
     */
    [[nodiscard]] static Error sync(
        std::string message,
        std::string context = {});

    /**
     * @brief Creates a public SDK transport error.
     *
     * @param message Developer-facing error message.
     * @param context Optional diagnostic context.
     * @return Public SDK error.
     */
    [[nodiscard]] static Error transport(
        std::string message,
        std::string context = {});

    /**
     * @brief Creates a public SDK discovery error.
     *
     * @param message Developer-facing error message.
     * @param context Optional diagnostic context.
     * @return Public SDK error.
     */
    [[nodiscard]] static Error discovery(
        std::string message,
        std::string context = {});

    /**
     * @brief Creates a public SDK metadata error.
     *
     * @param message Developer-facing error message.
     * @param context Optional diagnostic context.
     * @return Public SDK error.
     */
    [[nodiscard]] static Error metadata(
        std::string message,
        std::string context = {});

    /**
     * @brief Creates a public SDK internal error.
     *
     * @param message Developer-facing error message.
     * @param context Optional diagnostic context.
     * @return Public SDK error.
     */
    [[nodiscard]] static Error internal(
        std::string message,
        std::string context = {});
  };

} // namespace softadastra::sdk::internal

#endif // SOFTADASTRA_SDK_INTERNAL_ERROR_MAPPER_HPP
