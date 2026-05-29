/**
 *
 *  @file Key.cpp
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

#include <softadastra/sdk/Key.hpp>

#include <utility>

namespace softadastra::sdk
{
  Key::Key(std::string value)
      : value_(std::move(value))
  {
  }

  Key::Key(std::string_view value)
      : value_(value)
  {
  }

  Key::Key(const char *value)
      : value_(value == nullptr ? "" : value)
  {
  }

  Key Key::from(std::string value)
  {
    return Key{std::move(value)};
  }

  Key Key::from_view(std::string_view value)
  {
    return Key{value};
  }

  const std::string &Key::str() const noexcept
  {
    return value_;
  }

  const std::string &Key::value() const noexcept
  {
    return value_;
  }

  bool Key::empty() const noexcept
  {
    return value_.empty();
  }

  bool Key::is_valid() const noexcept
  {
    return !value_.empty();
  }

  bool Key::valid() const noexcept
  {
    return is_valid();
  }

  void Key::clear() noexcept
  {
    value_.clear();
  }

} // namespace softadastra::sdk
