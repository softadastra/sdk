/**
 *
 *  @file Error.hpp
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

#ifndef SOFTADASTRA_SDK_ERROR_HPP
#define SOFTADASTRA_SDK_ERROR_HPP

#include <string>
#include <string_view>

namespace softadastra::sdk
{
  /**
   * @brief Public SDK error object.
   *
   * Error is the stable developer-facing error type returned by SDK
   * operations.
   *
   * The SDK does not expose internal Softadastra module errors directly.
   * Internal errors are converted to this public type before they reach SDK
   * users.
   */
  class Error
  {
  public:
    /**
     * @brief Stable SDK error code.
     *
     * These codes form the public error surface of the SDK.
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
     * @param code Stable SDK error code.
     * @param message Developer-facing error message.
     * @param context Optional diagnostic context.
     */
    Error(
        Code code,
        std::string message,
        std::string context = {});

    /**
     * @brief Creates a success-like SDK error.
     *
     * @return Error with Code::None.
     */
    [[nodiscard]] static Error none();

    /**
     * @brief Creates an SDK error from explicit fields.
     *
     * @param code Stable SDK error code.
     * @param message Developer-facing error message.
     * @param context Optional diagnostic context.
     * @return SDK error.
     */
    [[nodiscard]] static Error make(
        Code code,
        std::string message,
        std::string context = {});

    /**
     * @brief Creates an unknown error.
     *
     * @param message Developer-facing error message.
     * @param context Optional diagnostic context.
     * @return SDK error.
     */
    [[nodiscard]] static Error unknown(
        std::string message,
        std::string context = {});

    /**
     * @brief Creates an invalid argument error.
     *
     * @param message Developer-facing error message.
     * @param context Optional diagnostic context.
     * @return SDK error.
     */
    [[nodiscard]] static Error invalid_argument(
        std::string message,
        std::string context = {});

    /**
     * @brief Creates an invalid state error.
     *
     * @param message Developer-facing error message.
     * @param context Optional diagnostic context.
     * @return SDK error.
     */
    [[nodiscard]] static Error invalid_state(
        std::string message,
        std::string context = {});

    /**
     * @brief Creates a not found error.
     *
     * @param message Developer-facing error message.
     * @param context Optional diagnostic context.
     * @return SDK error.
     */
    [[nodiscard]] static Error not_found(
        std::string message,
        std::string context = {});

    /**
     * @brief Creates an already exists error.
     *
     * @param message Developer-facing error message.
     * @param context Optional diagnostic context.
     * @return SDK error.
     */
    [[nodiscard]] static Error already_exists(
        std::string message,
        std::string context = {});

    /**
     * @brief Creates an I/O error.
     *
     * @param message Developer-facing error message.
     * @param context Optional diagnostic context.
     * @return SDK error.
     */
    [[nodiscard]] static Error io_error(
        std::string message,
        std::string context = {});

    /**
     * @brief Creates a store error.
     *
     * @param message Developer-facing error message.
     * @param context Optional diagnostic context.
     * @return SDK error.
     */
    [[nodiscard]] static Error store_error(
        std::string message,
        std::string context = {});

    /**
     * @brief Creates a sync error.
     *
     * @param message Developer-facing error message.
     * @param context Optional diagnostic context.
     * @return SDK error.
     */
    [[nodiscard]] static Error sync_error(
        std::string message,
        std::string context = {});

    /**
     * @brief Creates a transport error.
     *
     * @param message Developer-facing error message.
     * @param context Optional diagnostic context.
     * @return SDK error.
     */
    [[nodiscard]] static Error transport_error(
        std::string message,
        std::string context = {});

    /**
     * @brief Creates a discovery error.
     *
     * @param message Developer-facing error message.
     * @param context Optional diagnostic context.
     * @return SDK error.
     */
    [[nodiscard]] static Error discovery_error(
        std::string message,
        std::string context = {});

    /**
     * @brief Creates a metadata error.
     *
     * @param message Developer-facing error message.
     * @param context Optional diagnostic context.
     * @return SDK error.
     */
    [[nodiscard]] static Error metadata_error(
        std::string message,
        std::string context = {});

    /**
     * @brief Creates an internal SDK error.
     *
     * @param message Developer-facing error message.
     * @param context Optional diagnostic context.
     * @return SDK error.
     */
    [[nodiscard]] static Error internal_error(
        std::string message,
        std::string context = {});

    /**
     * @brief Returns the stable SDK error code.
     *
     * @return SDK error code.
     */
    [[nodiscard]] Code code() const noexcept;

    /**
     * @brief Returns the developer-facing error message.
     *
     * @return Error message.
     */
    [[nodiscard]] const std::string &message() const noexcept;

    /**
     * @brief Returns the optional diagnostic context.
     *
     * @return Error context.
     */
    [[nodiscard]] const std::string &context() const noexcept;

    /**
     * @brief Returns true when this object represents success.
     *
     * @return true if the error code is Code::None.
     */
    [[nodiscard]] bool ok() const noexcept;

    /**
     * @brief Returns true when this object represents an error.
     *
     * @return true if the error code is not Code::None.
     */
    [[nodiscard]] bool has_error() const noexcept;

    /**
     * @brief Returns true when diagnostic context is available.
     *
     * @return true if the context string is not empty.
     */
    [[nodiscard]] bool has_context() const noexcept;

    /**
     * @brief Clears the error and resets it to Code::None.
     */
    void clear() noexcept;

    /**
     * @brief Returns the stable string representation of an SDK error code.
     *
     * @param code SDK error code.
     * @return Stable error code string.
     */
    [[nodiscard]] static std::string_view to_string(Code code) noexcept;

    /**
     * @brief Returns the stable string representation of this error code.
     *
     * @return Stable error code string.
     */
    [[nodiscard]] std::string_view code_string() const noexcept;

    /**
     * @brief Compares two errors for equality.
     *
     * @param left Left error.
     * @param right Right error.
     * @return true if both errors contain the same code, message, and context.
     */
    [[nodiscard]] friend bool operator==(
        const Error &left,
        const Error &right) noexcept
    {
      return left.code_ == right.code_ &&
             left.message_ == right.message_ &&
             left.context_ == right.context_;
    }

    /**
     * @brief Compares two errors for inequality.
     *
     * @param left Left error.
     * @param right Right error.
     * @return true if the errors are different.
     */
    [[nodiscard]] friend bool operator!=(
        const Error &left,
        const Error &right) noexcept
    {
      return !(left == right);
    }

  private:
    Code code_{Code::None};
    std::string message_{};
    std::string context_{};
  };

} // namespace softadastra::sdk

#endif // SOFTADASTRA_SDK_ERROR_HPP
