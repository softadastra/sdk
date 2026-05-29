/**
 *
 *  @file ClientSyncTests.cpp
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

#include <softadastra/sdk/Client.hpp>
#include <softadastra/sdk/ClientOptions.hpp>
#include <softadastra/sdk/Error.hpp>
#include <softadastra/sdk/SyncState.hpp>
#include <softadastra/sdk/TickResult.hpp>

#include <gtest/gtest.h>

#include <cstddef>

namespace
{
  using softadastra::sdk::Client;
  using softadastra::sdk::ClientOptions;
  using softadastra::sdk::Error;
  using softadastra::sdk::SyncState;
  using softadastra::sdk::TickResult;
}

TEST(ClientSyncTests, SyncStateFailsWhenClientIsClosed)
{
  Client client{
      ClientOptions::memory_only("node-sync-closed")};

  const auto state =
      client.sync_state();

  ASSERT_TRUE(state.is_err());
  EXPECT_EQ(state.error().code(), Error::Code::InvalidState);
}

TEST(ClientSyncTests, TickFailsWhenClientIsClosed)
{
  Client client{
      ClientOptions::memory_only("node-sync-tick-closed")};

  const auto result =
      client.tick();

  ASSERT_TRUE(result.is_err());
  EXPECT_EQ(result.error().code(), Error::Code::InvalidState);
}

TEST(ClientSyncTests, InitialSyncStateIsAvailable)
{
  Client client{
      ClientOptions::memory_only("node-sync-initial")};

  ASSERT_TRUE(client.open().is_ok());

  const auto state =
      client.sync_state();

  ASSERT_TRUE(state.is_ok());

  EXPECT_EQ(state.value().outbox_size(), 0U);
  EXPECT_EQ(state.value().queued_count(), 0U);
  EXPECT_EQ(state.value().in_flight_count(), 0U);
  EXPECT_EQ(state.value().acknowledged_count(), 0U);
  EXPECT_EQ(state.value().failed_count(), 0U);
  EXPECT_EQ(state.value().last_submitted_version(), 0U);
  EXPECT_EQ(state.value().last_applied_remote_version(), 0U);
  EXPECT_EQ(state.value().total_retries(), 0U);
  EXPECT_TRUE(state.value().empty());
}

TEST(ClientSyncTests, PutUpdatesSyncState)
{
  Client client{
      ClientOptions::memory_only("node-sync-put")};

  ASSERT_TRUE(client.open().is_ok());

  ASSERT_TRUE(client.put("message", "hello").is_ok());

  const auto state =
      client.sync_state();

  ASSERT_TRUE(state.is_ok());

  EXPECT_GT(state.value().outbox_size(), 0U);
  EXPECT_GT(state.value().last_submitted_version(), 0U);
}

TEST(ClientSyncTests, TickReturnsResult)
{
  Client client{
      ClientOptions::memory_only("node-sync-tick")};

  ASSERT_TRUE(client.open().is_ok());

  const auto tick =
      client.tick();

  ASSERT_TRUE(tick.is_ok());

  EXPECT_EQ(tick.value().retried_count(), 0U);
  EXPECT_EQ(tick.value().pruned_count(), 0U);
}

TEST(ClientSyncTests, TickAfterPutIsValid)
{
  Client client{
      ClientOptions::memory_only("node-sync-tick-after-put")};

  ASSERT_TRUE(client.open().is_ok());
  ASSERT_TRUE(client.put("message", "hello").is_ok());

  const auto tick =
      client.tick();

  ASSERT_TRUE(tick.is_ok());

  EXPECT_GE(tick.value().batch_size(), 0U);
}

TEST(ClientSyncTests, RetryExpiredReturnsCount)
{
  Client client{
      ClientOptions::memory_only("node-sync-retry")};

  ASSERT_TRUE(client.open().is_ok());

  const auto result =
      client.retry_expired();

  ASSERT_TRUE(result.is_ok());
  EXPECT_EQ(result.value(), 0U);
}

TEST(ClientSyncTests, PruneCompletedReturnsCount)
{
  Client client{
      ClientOptions::memory_only("node-sync-prune-completed")};

  ASSERT_TRUE(client.open().is_ok());

  const auto result =
      client.prune_completed();

  ASSERT_TRUE(result.is_ok());
  EXPECT_EQ(result.value(), 0U);
}

TEST(ClientSyncTests, PruneFailedReturnsCount)
{
  Client client{
      ClientOptions::memory_only("node-sync-prune-failed")};

  ASSERT_TRUE(client.open().is_ok());

  const auto result =
      client.prune_failed();

  ASSERT_TRUE(result.is_ok());
  EXPECT_EQ(result.value(), 0U);
}

TEST(ClientSyncTests, SyncStateHelpersWork)
{
  SyncState state;

  EXPECT_TRUE(state.empty());
  EXPECT_FALSE(state.has_queued());
  EXPECT_FALSE(state.has_in_flight());
  EXPECT_FALSE(state.has_failed());
  EXPECT_FALSE(state.has_work());
  EXPECT_EQ(state.total_tracked(), 0U);

  state.set_queued_count(2);
  state.set_in_flight_count(1);
  state.set_acknowledged_count(3);
  state.set_failed_count(1);

  EXPECT_FALSE(state.empty());
  EXPECT_TRUE(state.has_queued());
  EXPECT_TRUE(state.has_in_flight());
  EXPECT_TRUE(state.has_failed());
  EXPECT_TRUE(state.has_work());
  EXPECT_EQ(state.total_tracked(), 7U);

  state.clear();

  EXPECT_TRUE(state.empty());
}

TEST(ClientSyncTests, TickResultHelpersWork)
{
  TickResult result;

  EXPECT_TRUE(result.empty());
  EXPECT_FALSE(result.has_work());
  EXPECT_FALSE(result.retried());
  EXPECT_FALSE(result.pruned());
  EXPECT_FALSE(result.produced_batch());

  result.set_retried_count(1);
  result.set_pruned_count(2);
  result.set_batch_size(3);

  EXPECT_FALSE(result.empty());
  EXPECT_TRUE(result.has_work());
  EXPECT_TRUE(result.retried());
  EXPECT_TRUE(result.pruned());
  EXPECT_TRUE(result.produced_batch());

  result.clear();

  EXPECT_TRUE(result.empty());
}
