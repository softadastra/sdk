/**
 *
 *  @file ClientMetadataTests.cpp
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
#include <softadastra/sdk/NodeInfo.hpp>

#include <gtest/gtest.h>

namespace
{
  using softadastra::sdk::Client;
  using softadastra::sdk::ClientOptions;
  using softadastra::sdk::Error;
  using softadastra::sdk::NodeInfo;
}

TEST(ClientMetadataTests, NodeInfoFailsWhenClientIsClosed)
{
  Client client{
      ClientOptions::memory_only("node-metadata-closed")};

  const auto info =
      client.node_info();

  ASSERT_TRUE(info.is_err());
  EXPECT_EQ(info.error().code(), Error::Code::InvalidState);
}

TEST(ClientMetadataTests, RefreshNodeInfoFailsWhenClientIsClosed)
{
  Client client{
      ClientOptions::memory_only("node-metadata-refresh-closed")};

  const auto info =
      client.refresh_node_info();

  ASSERT_TRUE(info.is_err());
  EXPECT_EQ(info.error().code(), Error::Code::InvalidState);
}

TEST(ClientMetadataTests, NodeInfoReturnsMetadataAfterOpen)
{
  const ClientOptions options =
      ClientOptions::memory_only("node-metadata-info")
          .with_metadata("Metadata Node", "0.1.0");

  Client client{options};

  ASSERT_TRUE(client.open().is_ok());

  const auto info =
      client.node_info();

  ASSERT_TRUE(info.is_ok());

  EXPECT_EQ(info.value().node_id(), "node-metadata-info");
  EXPECT_EQ(info.value().display_name(), "Metadata Node");
  EXPECT_EQ(info.value().version(), "0.1.0");
  EXPECT_FALSE(info.value().hostname().empty());
  EXPECT_FALSE(info.value().os_name().empty());
  EXPECT_TRUE(info.value().is_valid());
}

TEST(ClientMetadataTests, RefreshNodeInfoReturnsMetadataAfterOpen)
{
  const ClientOptions options =
      ClientOptions::memory_only("node-metadata-refresh")
          .with_metadata("Refresh Node", "0.1.0");

  Client client{options};

  ASSERT_TRUE(client.open().is_ok());

  const auto info =
      client.refresh_node_info();

  ASSERT_TRUE(info.is_ok());

  EXPECT_EQ(info.value().node_id(), "node-metadata-refresh");
  EXPECT_EQ(info.value().display_name(), "Refresh Node");
  EXPECT_EQ(info.value().version(), "0.1.0");
  EXPECT_FALSE(info.value().hostname().empty());
  EXPECT_FALSE(info.value().os_name().empty());
  EXPECT_TRUE(info.value().is_valid());
}

TEST(ClientMetadataTests, NodeInfoUsesNodeIdAsLabelWhenDisplayNameIsEmpty)
{
  NodeInfo info{
      "node-label",
      "",
      "localhost",
      "linux",
      "0.1.0"};

  EXPECT_EQ(info.label(), "node-label");
  EXPECT_TRUE(info.is_valid());
}

TEST(ClientMetadataTests, NodeInfoUsesDisplayNameAsLabelWhenPresent)
{
  NodeInfo info{
      "node-label",
      "Display Name",
      "localhost",
      "linux",
      "0.1.0"};

  EXPECT_EQ(info.label(), "Display Name");
  EXPECT_TRUE(info.is_valid());
}

TEST(ClientMetadataTests, NodeInfoSettersUpdateFields)
{
  NodeInfo info;

  info.set_node_id("node-custom");
  info.set_display_name("Custom Node");
  info.set_hostname("host.local");
  info.set_os_name("linux");
  info.set_version("0.2.0");
  info.set_started_at_ms(1000);
  info.set_uptime_ms(2000);
  info.add_capability("store");
  info.add_capability("sync");

  EXPECT_EQ(info.node_id(), "node-custom");
  EXPECT_EQ(info.display_name(), "Custom Node");
  EXPECT_EQ(info.hostname(), "host.local");
  EXPECT_EQ(info.os_name(), "linux");
  EXPECT_EQ(info.version(), "0.2.0");
  EXPECT_EQ(info.started_at_ms(), 1000);
  EXPECT_EQ(info.uptime_ms(), 2000);
  EXPECT_TRUE(info.has_capability("store"));
  EXPECT_TRUE(info.has_capability("sync"));
  EXPECT_FALSE(info.has_capability("transport"));
  EXPECT_TRUE(info.is_valid());
}

TEST(ClientMetadataTests, NodeInfoSetCapabilitiesReplacesCapabilities)
{
  NodeInfo info;

  info.set_capabilities({"store", "sync"});

  EXPECT_TRUE(info.has_capability("store"));
  EXPECT_TRUE(info.has_capability("sync"));
  EXPECT_FALSE(info.has_capability("transport"));
}

TEST(ClientMetadataTests, NodeInfoIgnoresEmptyCapability)
{
  NodeInfo info;

  info.add_capability("");

  EXPECT_TRUE(info.capabilities().empty());
}

TEST(ClientMetadataTests, NodeInfoClearResetsFields)
{
  NodeInfo info{
      "node-clear",
      "Clear Node",
      "localhost",
      "linux",
      "0.1.0"};

  info.set_started_at_ms(1000);
  info.set_uptime_ms(2000);
  info.add_capability("store");

  ASSERT_TRUE(info.is_valid());

  info.clear();

  EXPECT_TRUE(info.node_id().empty());
  EXPECT_TRUE(info.display_name().empty());
  EXPECT_TRUE(info.hostname().empty());
  EXPECT_TRUE(info.os_name().empty());
  EXPECT_TRUE(info.version().empty());
  EXPECT_EQ(info.started_at_ms(), 0);
  EXPECT_EQ(info.uptime_ms(), 0);
  EXPECT_TRUE(info.capabilities().empty());
  EXPECT_FALSE(info.is_valid());
}

TEST(ClientMetadataTests, NodeInfoEqualityComparesAllFields)
{
  NodeInfo first{
      "node-1",
      "Node",
      "localhost",
      "linux",
      "0.1.0"};

  first.set_started_at_ms(1000);
  first.set_uptime_ms(2000);
  first.add_capability("store");

  NodeInfo second{
      "node-1",
      "Node",
      "localhost",
      "linux",
      "0.1.0"};

  second.set_started_at_ms(1000);
  second.set_uptime_ms(2000);
  second.add_capability("store");

  NodeInfo third{
      "node-2",
      "Node",
      "localhost",
      "linux",
      "0.1.0"};

  EXPECT_EQ(first, second);
  EXPECT_NE(first, third);
}
