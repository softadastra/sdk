/**
 *
 *  @file Version.cpp
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

#include <softadastra/sdk/Version.hpp>

namespace softadastra::sdk
{
  std::uint32_t sdk_version_major() noexcept
  {
    return version_major;
  }

  std::uint32_t sdk_version_minor() noexcept
  {
    return version_minor;
  }

  std::uint32_t sdk_version_patch() noexcept
  {
    return version_patch;
  }

  std::string_view sdk_version() noexcept
  {
    return version_string;
  }

} // namespace softadastra::sdk
