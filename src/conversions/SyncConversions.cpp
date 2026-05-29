/**
 *
 *  @file SyncConversions.cpp
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

#include "SyncConversions.hpp"
#include <softadastra/sdk/ClientOptions.hpp>

namespace softadastra::sdk::internal
{
  softadastra::sync::core::SyncConfig to_sync_config(
      const ClientOptions &options)
  {
    auto config =
        softadastra::sync::core::SyncConfig::durable(
            options.node_id());

    config.batch_size = options.sync_batch_size();
    config.max_retries = options.max_sync_retries();
    config.require_ack = options.require_ack();
    config.auto_queue = options.auto_queue();

    return config;
  }

  SyncState from_sync_state(
      const softadastra::sync::core::SyncState &state)
  {
    SyncState result;

    result.set_outbox_size(state.outbox_size);
    result.set_queued_count(state.queued_count);
    result.set_in_flight_count(state.in_flight_count);
    result.set_acknowledged_count(state.acknowledged_count);
    result.set_failed_count(state.failed_count);
    result.set_last_submitted_version(state.last_submitted_version);
    result.set_last_applied_remote_version(
        state.last_applied_remote_version);
    result.set_total_retries(state.total_retries);

    return result;
  }

  softadastra::sync::core::SyncState to_sync_state(
      const SyncState &state)
  {
    softadastra::sync::core::SyncState result;

    result.outbox_size = state.outbox_size();
    result.queued_count = state.queued_count();
    result.in_flight_count = state.in_flight_count();
    result.acknowledged_count = state.acknowledged_count();
    result.failed_count = state.failed_count();
    result.last_submitted_version = state.last_submitted_version();
    result.last_applied_remote_version =
        state.last_applied_remote_version();
    result.total_retries = state.total_retries();

    return result;
  }

  TickResult from_scheduler_tick_result(
      const softadastra::sync::scheduler::SyncScheduler::TickResult &result)
  {
    return TickResult{
        result.retried_count,
        result.pruned_count,
        result.batch_size()};
  }

} // namespace softadastra::sdk::internal
