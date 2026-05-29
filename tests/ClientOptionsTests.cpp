/**
 *
 *  @file ClientOptionsTests.cpp
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

#include <softadastra/sdk/ClientOptions.hpp>

#include <gtest/gtest.h>

#include <cstdint>
#include <string>

namespace
{
  using softadastra::sdk::ClientOptions;
}

TEST(ClientOptionsTests, DefaultOptionsAreValidAndDurable)
{
  const ClientOptions options;

  EXPECT_TRUE(options.is_valid());
  EXPECT_TRUE(options.valid());

  EXPECT_EQ(options.node_id(), "node-1");
  EXPECT_EQ(options.store_wal_path(), "data/sdk-store.wal");
  EXPECT_TRUE(options.store_wal_enabled());
  EXPECT_TRUE(options.auto_flush());
  EXPECT_EQ(options.initial_store_capacity(), 1024U);

  EXPECT_EQ(options.sync_batch_size(), 64U);
  EXPECT_EQ(options.max_sync_retries(), 5U);
  EXPECT_TRUE(options.require_ack());
  EXPECT_TRUE(options.auto_queue());

  EXPECT_FALSE(options.transport_enabled());
  EXPECT_EQ(options.transport_host(), "0.0.0.0");
  EXPECT_EQ(options.transport_port(), 0U);

  EXPECT_FALSE(options.discovery_enabled());
  EXPECT_EQ(options.discovery_host(), "0.0.0.0");
  EXPECT_EQ(options.discovery_port(), 0U);

  EXPECT_TRUE(options.display_name().empty());
  EXPECT_EQ(options.version(), "0.1.0");
}

TEST(ClientOptionsTests, ConstructorSetsNodeIdAndDisplayName)
{
  const ClientOptions options{"node-alpha"};

  EXPECT_EQ(options.node_id(), "node-alpha");
  EXPECT_EQ(options.display_name(), "node-alpha");
  EXPECT_TRUE(options.is_valid());
}

TEST(ClientOptionsTests, MemoryOnlyDisablesWalAndAck)
{
  const ClientOptions options =
      ClientOptions::memory_only("node-memory");

  EXPECT_EQ(options.node_id(), "node-memory");
  EXPECT_FALSE(options.store_wal_enabled());
  EXPECT_FALSE(options.auto_flush());
  EXPECT_FALSE(options.require_ack());
  EXPECT_TRUE(options.auto_queue());
  EXPECT_TRUE(options.is_valid());
}

TEST(ClientOptionsTests, LocalIsAliasForMemoryOnly)
{
  const ClientOptions options =
      ClientOptions::local("node-local");

  EXPECT_EQ(options.node_id(), "node-local");
  EXPECT_FALSE(options.store_wal_enabled());
  EXPECT_FALSE(options.auto_flush());
  EXPECT_FALSE(options.require_ack());
  EXPECT_TRUE(options.is_valid());
}

TEST(ClientOptionsTests, PersistentEnablesDurableDefaults)
{
  const ClientOptions options =
      ClientOptions::persistent(
          "node-persistent",
          "data/custom.wal");

  EXPECT_EQ(options.node_id(), "node-persistent");
  EXPECT_EQ(options.store_wal_path(), "data/custom.wal");
  EXPECT_TRUE(options.store_wal_enabled());
  EXPECT_TRUE(options.auto_flush());
  EXPECT_EQ(options.sync_batch_size(), 64U);
  EXPECT_EQ(options.max_sync_retries(), 5U);
  EXPECT_TRUE(options.require_ack());
  EXPECT_TRUE(options.auto_queue());
  EXPECT_TRUE(options.is_valid());
}

TEST(ClientOptionsTests, DurableIsAliasForPersistent)
{
  const ClientOptions options =
      ClientOptions::durable(
          "node-durable",
          "data/durable.wal");

  EXPECT_EQ(options.node_id(), "node-durable");
  EXPECT_EQ(options.store_wal_path(), "data/durable.wal");
  EXPECT_TRUE(options.store_wal_enabled());
  EXPECT_TRUE(options.auto_flush());
  EXPECT_TRUE(options.require_ack());
  EXPECT_TRUE(options.is_valid());
}

TEST(ClientOptionsTests, FastUsesWalWithoutAutoFlush)
{
  const ClientOptions options =
      ClientOptions::fast(
          "node-fast",
          "data/fast.wal");

  EXPECT_EQ(options.node_id(), "node-fast");
  EXPECT_EQ(options.store_wal_path(), "data/fast.wal");
  EXPECT_TRUE(options.store_wal_enabled());
  EXPECT_FALSE(options.auto_flush());
  EXPECT_EQ(options.sync_batch_size(), 128U);
  EXPECT_EQ(options.max_sync_retries(), 2U);
  EXPECT_FALSE(options.require_ack());
  EXPECT_TRUE(options.auto_queue());
  EXPECT_TRUE(options.is_valid());
}

TEST(ClientOptionsTests, SettersUpdateValues)
{
  ClientOptions options;

  options.set_node_id("node-custom");
  options.set_store_wal_path("data/custom.wal");
  options.set_store_wal_enabled(false);
  options.set_auto_flush(false);
  options.set_initial_store_capacity(2048);
  options.set_sync_batch_size(32);
  options.set_max_sync_retries(3);
  options.set_require_ack(false);
  options.set_auto_queue(false);
  options.set_metadata("Display Node", "0.2.0");

  EXPECT_EQ(options.node_id(), "node-custom");
  EXPECT_EQ(options.store_wal_path(), "data/custom.wal");
  EXPECT_FALSE(options.store_wal_enabled());
  EXPECT_FALSE(options.auto_flush());
  EXPECT_EQ(options.initial_store_capacity(), 2048U);
  EXPECT_EQ(options.sync_batch_size(), 32U);
  EXPECT_EQ(options.max_sync_retries(), 3U);
  EXPECT_FALSE(options.require_ack());
  EXPECT_FALSE(options.auto_queue());
  EXPECT_EQ(options.display_name(), "Display Node");
  EXPECT_EQ(options.version(), "0.2.0");
  EXPECT_TRUE(options.is_valid());
}

TEST(ClientOptionsTests, SetTransportEnablesTransport)
{
  ClientOptions options;

  options.set_transport("127.0.0.1", 9000);

  EXPECT_TRUE(options.transport_enabled());
  EXPECT_EQ(options.transport_host(), "127.0.0.1");
  EXPECT_EQ(options.transport_port(), 9000U);
  EXPECT_TRUE(options.is_valid());
}

TEST(ClientOptionsTests, DisableTransportResetsTransport)
{
  ClientOptions options =
      ClientOptions{"node-1"}.with_transport(
          "127.0.0.1",
          9000);

  ASSERT_TRUE(options.transport_enabled());

  options.disable_transport();

  EXPECT_FALSE(options.transport_enabled());
  EXPECT_EQ(options.transport_host(), "0.0.0.0");
  EXPECT_EQ(options.transport_port(), 0U);
  EXPECT_TRUE(options.is_valid());
}

TEST(ClientOptionsTests, SetDiscoveryEnablesDiscovery)
{
  ClientOptions options;

  options.set_discovery("127.0.0.1", 5050);

  EXPECT_TRUE(options.discovery_enabled());
  EXPECT_EQ(options.discovery_host(), "127.0.0.1");
  EXPECT_EQ(options.discovery_port(), 5050U);
  EXPECT_TRUE(options.is_valid());
}

TEST(ClientOptionsTests, DisableDiscoveryResetsDiscovery)
{
  ClientOptions options =
      ClientOptions{"node-1"}.with_discovery(
          "127.0.0.1",
          5050);

  ASSERT_TRUE(options.discovery_enabled());

  options.disable_discovery();

  EXPECT_FALSE(options.discovery_enabled());
  EXPECT_EQ(options.discovery_host(), "0.0.0.0");
  EXPECT_EQ(options.discovery_port(), 0U);
  EXPECT_TRUE(options.is_valid());
}

TEST(ClientOptionsTests, WithTransportReturnsModifiedCopy)
{
  const ClientOptions base{"node-base"};

  const ClientOptions options =
      base.with_transport("127.0.0.1", 7000);

  EXPECT_FALSE(base.transport_enabled());
  EXPECT_TRUE(options.transport_enabled());
  EXPECT_EQ(options.transport_host(), "127.0.0.1");
  EXPECT_EQ(options.transport_port(), 7000U);
}

TEST(ClientOptionsTests, WithLocalTransportUsesLocalhost)
{
  const ClientOptions options =
      ClientOptions{"node-local-transport"}
          .with_local_transport(7001);

  EXPECT_TRUE(options.transport_enabled());
  EXPECT_EQ(options.transport_host(), "127.0.0.1");
  EXPECT_EQ(options.transport_port(), 7001U);
}

TEST(ClientOptionsTests, WithDiscoveryReturnsModifiedCopy)
{
  const ClientOptions base{"node-base"};

  const ClientOptions options =
      base.with_discovery("127.0.0.1", 5051);

  EXPECT_FALSE(base.discovery_enabled());
  EXPECT_TRUE(options.discovery_enabled());
  EXPECT_EQ(options.discovery_host(), "127.0.0.1");
  EXPECT_EQ(options.discovery_port(), 5051U);
}

TEST(ClientOptionsTests, WithLocalDiscoveryUsesLocalhost)
{
  const ClientOptions options =
      ClientOptions{"node-local-discovery"}
          .with_local_discovery(5052);

  EXPECT_TRUE(options.discovery_enabled());
  EXPECT_EQ(options.discovery_host(), "127.0.0.1");
  EXPECT_EQ(options.discovery_port(), 5052U);
}

TEST(ClientOptionsTests, WithMetadataReturnsModifiedCopy)
{
  const ClientOptions base{"node-base"};

  const ClientOptions options =
      base.with_metadata("Softadastra Node", "1.0.0");

  EXPECT_TRUE(base.display_name().empty() ||
              base.display_name() == "node-base");
  EXPECT_EQ(options.display_name(), "Softadastra Node");
  EXPECT_EQ(options.version(), "1.0.0");
}

TEST(ClientOptionsTests, InvalidWhenNodeIdIsEmpty)
{
  ClientOptions options;

  options.set_node_id("");

  EXPECT_FALSE(options.is_valid());
  EXPECT_FALSE(options.valid());
}

TEST(ClientOptionsTests, InvalidWhenWalEnabledAndPathIsEmpty)
{
  ClientOptions options;

  options.set_store_wal_enabled(true);
  options.set_store_wal_path("");

  EXPECT_FALSE(options.is_valid());
}

TEST(ClientOptionsTests, ValidWhenWalDisabledAndPathIsEmpty)
{
  ClientOptions options;

  options.set_store_wal_enabled(false);
  options.set_store_wal_path("");

  EXPECT_TRUE(options.is_valid());
}

TEST(ClientOptionsTests, InvalidWhenInitialCapacityIsZero)
{
  ClientOptions options;

  options.set_initial_store_capacity(0);

  EXPECT_FALSE(options.is_valid());
}

TEST(ClientOptionsTests, InvalidWhenSyncBatchSizeIsZero)
{
  ClientOptions options;

  options.set_sync_batch_size(0);

  EXPECT_FALSE(options.is_valid());
}

TEST(ClientOptionsTests, InvalidWhenTransportEnabledWithEmptyHost)
{
  ClientOptions options;

  options.set_transport("", 9000);

  EXPECT_FALSE(options.is_valid());
}

TEST(ClientOptionsTests, InvalidWhenTransportEnabledWithZeroPort)
{
  ClientOptions options;

  options.set_transport("127.0.0.1", 0);

  EXPECT_FALSE(options.is_valid());
}

TEST(ClientOptionsTests, InvalidWhenDiscoveryEnabledWithEmptyHost)
{
  ClientOptions options;

  options.set_discovery("", 5050);

  EXPECT_FALSE(options.is_valid());
}

TEST(ClientOptionsTests, InvalidWhenDiscoveryEnabledWithZeroPort)
{
  ClientOptions options;

  options.set_discovery("127.0.0.1", 0);

  EXPECT_FALSE(options.is_valid());
}

TEST(ClientOptionsTests, InvalidWhenVersionIsEmpty)
{
  ClientOptions options;

  options.set_metadata("node", "");

  EXPECT_FALSE(options.is_valid());
}
