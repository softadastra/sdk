/**
 *
 *  @file Error.cpp
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

#include <softadastra/sdk/Error.hpp>
#include <utility>

namespace softadastra::sdk
{
  Error::Error(
      Code code,
      std::string message,
      std::string context)
      : code_(code),
        message_(std::move(message)),
        context_(std::move(context))
  {
  }

  Error Error::none()
  {
    return Error{};
  }

  Error Error::make(
      Code code,
      std::string message,
      std::string context)
  {
    return Error{
        code,
        std::move(message),
        std::move(context)};
  }

  Error Error::unknown(
      std::string message,
      std::string context)
  {
    return Error{
        Code::Unknown,
        std::move(message),
        std::move(context)};
  }

  Error Error::invalid_argument(
      std::string message,
      std::string context)
  {
    return Error{
        Code::InvalidArgument,
        std::move(message),
        std::move(context)};
  }

  Error Error::invalid_state(
      std::string message,
      std::string context)
  {
    return Error{
        Code::InvalidState,
        std::move(message),
        std::move(context)};
  }

  Error Error::not_found(
      std::string message,
      std::string context)
  {
    return Error{
        Code::NotFound,
        std::move(message),
        std::move(context)};
  }

  Error Error::already_exists(
      std::string message,
      std::string context)
  {
    return Error{
        Code::AlreadyExists,
        std::move(message),
        std::move(context)};
  }

  Error Error::io_error(
      std::string message,
      std::string context)
  {
    return Error{
        Code::IoError,
        std::move(message),
        std::move(context)};
  }

  Error Error::store_error(
      std::string message,
      std::string context)
  {
    return Error{
        Code::StoreError,
        std::move(message),
        std::move(context)};
  }

  Error Error::sync_error(
      std::string message,
      std::string context)
  {
    return Error{
        Code::SyncError,
        std::move(message),
        std::move(context)};
  }

  Error Error::transport_error(
      std::string message,
      std::string context)
  {
    return Error{
        Code::TransportError,
        std::move(message),
        std::move(context)};
  }

  Error Error::discovery_error(
      std::string message,
      std::string context)
  {
    return Error{
        Code::DiscoveryError,
        std::move(message),
        std::move(context)};
  }

  Error Error::metadata_error(
      std::string message,
      std::string context)
  {
    return Error{
        Code::MetadataError,
        std::move(message),
        std::move(context)};
  }

  Error Error::internal_error(
      std::string message,
      std::string context)
  {
    return Error{
        Code::InternalError,
        std::move(message),
        std::move(context)};
  }

  Error::Code Error::code() const noexcept
  {
    return code_;
  }

  const std::string &Error::message() const noexcept
  {
    return message_;
  }

  const std::string &Error::context() const noexcept
  {
    return context_;
  }

  bool Error::ok() const noexcept
  {
    return code_ == Code::None;
  }

  bool Error::has_error() const noexcept
  {
    return code_ != Code::None;
  }

  bool Error::has_context() const noexcept
  {
    return !context_.empty();
  }

  void Error::clear() noexcept
  {
    code_ = Code::None;
    message_.clear();
    context_.clear();
  }

  std::string_view Error::to_string(Code code) noexcept
  {
    switch (code)
    {
    case Code::None:
      return "none";

    case Code::Unknown:
      return "unknown";

    case Code::InvalidArgument:
      return "invalid_argument";

    case Code::InvalidState:
      return "invalid_state";

    case Code::NotFound:
      return "not_found";

    case Code::AlreadyExists:
      return "already_exists";

    case Code::IoError:
      return "io_error";

    case Code::StoreError:
      return "store_error";

    case Code::SyncError:
      return "sync_error";

    case Code::TransportError:
      return "transport_error";

    case Code::DiscoveryError:
      return "discovery_error";

    case Code::MetadataError:
      return "metadata_error";

    case Code::InternalError:
      return "internal_error";
    }

    return "unknown";
  }

  std::string_view Error::code_string() const noexcept
  {
    return to_string(code_);
  }

} // namespace softadastra::sdk
