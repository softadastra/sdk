/**
 *
 *  @file Version.hpp
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

#ifndef SOFTADASTRA_SDK_VERSION_HPP
#define SOFTADASTRA_SDK_VERSION_HPP

#include <cstdint>
#include <string_view>

namespace softadastra::sdk
{
  /**
   * @brief Major version number of the Softadastra C++ SDK.
   *
   * The major version changes when the SDK introduces breaking changes in the
   * public API.
   */
  inline constexpr std::uint32_t version_major = 0;

  /**
   * @brief Minor version number of the Softadastra C++ SDK.
   *
   * The minor version changes when the SDK adds new public features while
   * keeping backward compatibility.
   */
  inline constexpr std::uint32_t version_minor = 1;

  /**
   * @brief Patch version number of the Softadastra C++ SDK.
   *
   * The patch version changes when the SDK receives bug fixes or internal
   * improvements that do not change the public API.
   */
  inline constexpr std::uint32_t version_patch = 0;

  /**
   * @brief Complete semantic version string of the Softadastra C++ SDK.
   */
  inline constexpr std::string_view version_string = "0.1.0";

  /**
   * @brief Returns the major version number.
   *
   * @return SDK major version.
   */
  [[nodiscard]] std::uint32_t sdk_version_major() noexcept;

  /**
   * @brief Returns the minor version number.
   *
   * @return SDK minor version.
   */
  [[nodiscard]] std::uint32_t sdk_version_minor() noexcept;

  /**
   * @brief Returns the patch version number.
   *
   * @return SDK patch version.
   */
  [[nodiscard]] std::uint32_t sdk_version_patch() noexcept;

  /**
   * @brief Returns the complete SDK semantic version string.
   *
   * @return SDK version string.
   */
  [[nodiscard]] std::string_view sdk_version() noexcept;

} // namespace softadastra::sdk

#endif // SOFTADASTRA_SDK_VERSION_HPP
