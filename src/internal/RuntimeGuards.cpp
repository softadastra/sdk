/**
 *
 *  @file RuntimeGuards.cpp
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

#include "RuntimeGuards.hpp"
#include "ErrorMapper.hpp"

namespace softadastra::sdk::internal
{
  RuntimeGuards::GuardResult RuntimeGuards::require_open(
      bool is_open,
      const Runtime &runtime)
  {
    if (!is_open)
    {
      return GuardResult::err(
          Error::invalid_state(
              "SDK client is not open"));
    }

    if (!runtime.has_local_runtime())
    {
      return GuardResult::err(
          ErrorMapper::internal(
              "SDK local runtime is not initialized"));
    }

    return GuardResult::ok();
  }

  RuntimeGuards::GuardResult RuntimeGuards::require_transport(
      bool is_open,
      bool transport_enabled,
      const Runtime &runtime)
  {
    const auto open_result = require_open(is_open, runtime);

    if (open_result.is_err())
    {
      return open_result;
    }

    if (!transport_enabled)
    {
      return GuardResult::err(
          ErrorMapper::transport(
              "transport is disabled"));
    }

    if (!runtime.has_transport())
    {
      return GuardResult::err(
          ErrorMapper::transport(
              "transport is not initialized"));
    }

    return GuardResult::ok();
  }

  RuntimeGuards::GuardResult RuntimeGuards::require_discovery(
      bool is_open,
      bool discovery_enabled,
      const Runtime &runtime)
  {
    const auto open_result = require_open(is_open, runtime);

    if (open_result.is_err())
    {
      return open_result;
    }

    if (!discovery_enabled)
    {
      return GuardResult::err(
          ErrorMapper::discovery(
              "discovery is disabled"));
    }

    if (!runtime.has_discovery())
    {
      return GuardResult::err(
          ErrorMapper::discovery(
              "discovery is not initialized"));
    }

    return GuardResult::ok();
  }

  RuntimeGuards::GuardResult RuntimeGuards::require_metadata(
      bool is_open,
      const Runtime &runtime)
  {
    const auto open_result = require_open(is_open, runtime);

    if (open_result.is_err())
    {
      return open_result;
    }

    if (!runtime.has_metadata())
    {
      return GuardResult::err(
          ErrorMapper::metadata(
              "metadata service is not initialized"));
    }

    return GuardResult::ok();
  }

} // namespace softadastra::sdk::internal
