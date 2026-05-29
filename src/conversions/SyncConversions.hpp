/**
 *
 *  @file SyncConversions.hpp
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

#ifndef SOFTADASTRA_SDK_SYNC_CONVERSIONS_HPP
#define SOFTADASTRA_SDK_SYNC_CONVERSIONS_HPP

#include <softadastra/sdk/SyncState.hpp>
#include <softadastra/sdk/TickResult.hpp>

#include <softadastra/sync/core/SyncConfig.hpp>
#include <softadastra/sync/core/SyncState.hpp>
#include <softadastra/sync/scheduler/SyncScheduler.hpp>

namespace softadastra::sdk
{
  class ClientOptions;
}

namespace softadastra::sdk::internal
{
  /**
   * @brief Converts public SDK client options to an internal sync config.
   *
   * @param options Public SDK client options.
   * @return Internal sync configuration.
   */
  [[nodiscard]] softadastra::sync::core::SyncConfig to_sync_config(
      const ClientOptions &options);

  /**
   * @brief Converts an internal sync state to a public SDK sync state.
   *
   * @param state Internal sync state.
   * @return Public SDK sync state.
   */
  [[nodiscard]] SyncState from_sync_state(
      const softadastra::sync::core::SyncState &state);

  /**
   * @brief Converts a public SDK sync state to an internal sync state.
   *
   * @param state Public SDK sync state.
   * @return Internal sync state.
   */
  [[nodiscard]] softadastra::sync::core::SyncState to_sync_state(
      const SyncState &state);

  /**
   * @brief Converts an internal scheduler tick result to a public SDK tick result.
   *
   * @param result Internal scheduler tick result.
   * @return Public SDK tick result.
   */
  [[nodiscard]] TickResult from_scheduler_tick_result(
      const softadastra::sync::scheduler::SyncScheduler::TickResult &result);

} // namespace softadastra::sdk::internal

#endif // SOFTADASTRA_SDK_SYNC_CONVERSIONS_HPP
