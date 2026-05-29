/**
 *
 *  @file Value.cpp
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

#include <softadastra/sdk/Value.hpp>

#include <utility>

namespace softadastra::sdk
{
  Value::Value(Container bytes)
      : data_(std::move(bytes))
  {
  }

  Value::Value(std::span<const std::uint8_t> bytes)
      : data_(bytes.begin(), bytes.end())
  {
  }

  Value::Value(std::string_view text)
      : data_(text.begin(), text.end())
  {
  }

  Value::Value(const char *text)
      : Value(text == nullptr ? std::string_view{} : std::string_view{text})
  {
  }

  Value Value::from_bytes(Container bytes)
  {
    return Value{std::move(bytes)};
  }

  Value Value::from_span(std::span<const std::uint8_t> bytes)
  {
    return Value{bytes};
  }

  Value Value::from_string(std::string_view text)
  {
    return Value{text};
  }

  const Value::Container &Value::bytes() const noexcept
  {
    return data_;
  }

  const Value::Container &Value::data() const noexcept
  {
    return data_;
  }

  Value::Container &Value::data() noexcept
  {
    return data_;
  }

  std::span<const std::uint8_t> Value::span() const noexcept
  {
    return std::span<const std::uint8_t>{
        data_.data(),
        data_.size()};
  }

  bool Value::empty() const noexcept
  {
    return data_.empty();
  }

  std::size_t Value::size() const noexcept
  {
    return data_.size();
  }

  void Value::clear() noexcept
  {
    data_.clear();
  }

  std::string Value::to_string() const
  {
    return std::string{
        data_.begin(),
        data_.end()};
  }

} // namespace softadastra::sdk
