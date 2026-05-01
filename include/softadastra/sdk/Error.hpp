/**
 *
 *  @file Error.hpp
 *  @author Gaspard Kirira
 *
 *  Copyright 2026, Softadastra.
 *  All rights reserved.
 *  https://github.com/softadastra/sdk-cpp
 *
 *  Licensed under the Apache License, Version 2.0.
 *
 *  Softadastra C++ SDK
 *
 */

#ifndef SOFTADASTRA_SDK_ERROR_HPP
#define SOFTADASTRA_SDK_ERROR_HPP

#include <string>
#include <string_view>

#include <softadastra/core/errors/Error.hpp>
#include <softadastra/core/errors/ErrorCode.hpp>
#include <softadastra/core/errors/ErrorContext.hpp>

namespace softadastra::sdk
{
  namespace core_errors = softadastra::core::errors;

  /**
   * @brief Public SDK error object.
   *
   * Error is the developer-facing error type returned by SDK operations.
   *
   * It wraps the stable Softadastra core error model while keeping the SDK API
   * small, clear, and independent from internal module details.
   */
  class Error
  {
  public:
    /**
     * @brief SDK-level error code.
     *
     * This enum gives the SDK a stable public error surface while allowing
     * internal Softadastra modules to evolve independently.
     */
    enum class Code
    {
      None,
      Unknown,
      InvalidArgument,
      InvalidState,
      NotFound,
      AlreadyExists,
      IoError,
      StoreError,
      SyncError,
      TransportError,
      DiscoveryError,
      MetadataError,
      InternalError
    };

    /**
     * @brief Creates an empty success-like error.
     */
    Error() = default;

    /**
     * @brief Creates an SDK error.
     *
     * @param code Machine-readable SDK error code.
     * @param message Developer-facing diagnostic message.
     * @param context Optional diagnostic context.
     */
    Error(
        Code code,
        std::string message,
        std::string context = {});

    /**
     * @brief Creates an empty success-like error.
     *
     * @return Success-like SDK error.
     */
    [[nodiscard]] static Error none();

    /**
     * @brief Creates an SDK error from SDK fields.
     *
     * @param code Machine-readable SDK error code.
     * @param message Developer-facing diagnostic message.
     * @param context Optional diagnostic context.
     * @return SDK error.
     */
    [[nodiscard]] static Error make(
        Code code,
        std::string message,
        std::string context = {});

    /**
     * @brief Creates an invalid argument error.
     *
     * @param message Developer-facing diagnostic message.
     * @param context Optional diagnostic context.
     * @return SDK error.
     */
    [[nodiscard]] static Error invalid_argument(
        std::string message,
        std::string context = {});

    /**
     * @brief Creates an invalid state error.
     *
     * @param message Developer-facing diagnostic message.
     * @param context Optional diagnostic context.
     * @return SDK error.
     */
    [[nodiscard]] static Error invalid_state(
        std::string message,
        std::string context = {});

    /**
     * @brief Creates a not found error.
     *
     * @param message Developer-facing diagnostic message.
     * @param context Optional diagnostic context.
     * @return SDK error.
     */
    [[nodiscard]] static Error not_found(
        std::string message,
        std::string context = {});

    /**
     * @brief Creates an I/O error.
     *
     * @param message Developer-facing diagnostic message.
     * @param context Optional diagnostic context.
     * @return SDK error.
     */
    [[nodiscard]] static Error io_error(
        std::string message,
        std::string context = {});

    /**
     * @brief Creates a sync error.
     *
     * @param message Developer-facing diagnostic message.
     * @param context Optional diagnostic context.
     * @return SDK error.
     */
    [[nodiscard]] static Error sync_error(
        std::string message,
        std::string context = {});

    /**
     * @brief Creates a transport error.
     *
     * @param message Developer-facing diagnostic message.
     * @param context Optional diagnostic context.
     * @return SDK error.
     */
    [[nodiscard]] static Error transport_error(
        std::string message,
        std::string context = {});

    /**
     * @brief Creates a discovery error.
     *
     * @param message Developer-facing diagnostic message.
     * @param context Optional diagnostic context.
     * @return SDK error.
     */
    [[nodiscard]] static Error discovery_error(
        std::string message,
        std::string context = {});

    /**
     * @brief Creates a metadata error.
     *
     * @param message Developer-facing diagnostic message.
     * @param context Optional diagnostic context.
     * @return SDK error.
     */
    [[nodiscard]] static Error metadata_error(
        std::string message,
        std::string context = {});

    /**
     * @brief Creates an internal SDK error.
     *
     * @param message Developer-facing diagnostic message.
     * @param context Optional diagnostic context.
     * @return SDK error.
     */
    [[nodiscard]] static Error internal_error(
        std::string message,
        std::string context = {});

    /**
     * @brief Creates an SDK error from a core Softadastra error.
     *
     * @param error Core Softadastra error.
     * @return SDK error.
     */
    [[nodiscard]] static Error from_core(
        const core_errors::Error &error);

    /**
     * @brief Converts the SDK error back to a core Softadastra error.
     *
     * @return Core Softadastra error.
     */
    [[nodiscard]] core_errors::Error to_core() const;

    /**
     * @brief Returns the machine-readable SDK error code.
     *
     * @return SDK error code.
     */
    [[nodiscard]] Code code() const noexcept;

    /**
     * @brief Returns the developer-facing diagnostic message.
     *
     * @return Error message.
     */
    [[nodiscard]] const std::string &message() const noexcept;

    /**
     * @brief Returns optional diagnostic context.
     *
     * @return Error context.
     */
    [[nodiscard]] const std::string &context() const noexcept;

    /**
     * @brief Returns true when diagnostic context is available.
     *
     * @return True if context is not empty.
     */
    [[nodiscard]] bool has_context() const noexcept;

    /**
     * @brief Returns true if this object represents success.
     *
     * @return True if the error code is Code::None.
     */
    [[nodiscard]] bool ok() const noexcept;

    /**
     * @brief Returns true if this object represents an error.
     *
     * @return True if the error code is not Code::None.
     */
    [[nodiscard]] bool has_error() const noexcept;

    /**
     * @brief Clears the error and resets it to success.
     */
    void clear() noexcept;

    /**
     * @brief Returns the stable string representation of an SDK error code.
     *
     * @param code SDK error code.
     * @return Stable string representation.
     */
    [[nodiscard]] static std::string_view to_string(Code code) noexcept;

    /**
     * @brief Returns the stable string representation of the current error code.
     *
     * @return Stable string representation.
     */
    [[nodiscard]] std::string_view code_string() const noexcept;

  private:
    /**
     * @brief Converts a core Softadastra error code to an SDK error code.
     *
     * @param code Core Softadastra error code.
     * @return SDK error code.
     */
    [[nodiscard]] static Code from_core_code(
        core_errors::ErrorCode code) noexcept;

    /**
     * @brief Converts an SDK error code to a core Softadastra error code.
     *
     * @param code SDK error code.
     * @return Core Softadastra error code.
     */
    [[nodiscard]] static core_errors::ErrorCode to_core_code(
        Code code) noexcept;

  private:
    Code code_{Code::None};
    std::string message_{};
    std::string context_{};
  };

} // namespace softadastra::sdk

#endif // SOFTADASTRA_SDK_ERROR_HPP
