/**
 *
 *  @file ErrorMapper.cpp
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

#include "ErrorMapper.hpp"

#include <utility>

namespace softadastra::sdk::internal
{
  Error ErrorMapper::from_core(
      const softadastra::core::errors::Error &error)
  {
    return Error::make(
        from_core_code(error.code()),
        error.message(),
        error.context().message());
  }

  Error::Code ErrorMapper::from_core_code(
      softadastra::core::errors::ErrorCode code) noexcept
  {
    namespace core_errors = softadastra::core::errors;

    switch (code)
    {
    case core_errors::ErrorCode::None:
      return Error::Code::None;

    case core_errors::ErrorCode::Unknown:
      return Error::Code::Unknown;

    case core_errors::ErrorCode::InvalidArgument:
      return Error::Code::InvalidArgument;

    case core_errors::ErrorCode::InvalidState:
      return Error::Code::InvalidState;

    case core_errors::ErrorCode::NotFound:
      return Error::Code::NotFound;

    case core_errors::ErrorCode::AlreadyExists:
      return Error::Code::AlreadyExists;

    case core_errors::ErrorCode::FileNotFound:
    case core_errors::ErrorCode::FileAlreadyExists:
    case core_errors::ErrorCode::FileReadError:
    case core_errors::ErrorCode::FileWriteError:
    case core_errors::ErrorCode::PermissionDenied:
      return Error::Code::IoError;

    case core_errors::ErrorCode::ConfigMissing:
    case core_errors::ErrorCode::ConfigInvalid:
      return Error::Code::InvalidArgument;

    case core_errors::ErrorCode::HashError:
    case core_errors::ErrorCode::TimeError:
    case core_errors::ErrorCode::InternalError:
      return Error::Code::InternalError;

    default:
      return Error::Code::Unknown;
    }
  }

  Error ErrorMapper::store(
      std::string message,
      std::string context)
  {
    return Error::store_error(
        std::move(message),
        std::move(context));
  }

  Error ErrorMapper::sync(
      std::string message,
      std::string context)
  {
    return Error::sync_error(
        std::move(message),
        std::move(context));
  }

  Error ErrorMapper::transport(
      std::string message,
      std::string context)
  {
    return Error::transport_error(
        std::move(message),
        std::move(context));
  }

  Error ErrorMapper::discovery(
      std::string message,
      std::string context)
  {
    return Error::discovery_error(
        std::move(message),
        std::move(context));
  }

  Error ErrorMapper::metadata(
      std::string message,
      std::string context)
  {
    return Error::metadata_error(
        std::move(message),
        std::move(context));
  }

  Error ErrorMapper::internal(
      std::string message,
      std::string context)
  {
    return Error::internal_error(
        std::move(message),
        std::move(context));
  }

} // namespace softadastra::sdk::internal
