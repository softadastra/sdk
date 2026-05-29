/**
 *
 *  @file ClientDiscoveryTests.cpp
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

#include <gtest/gtest.h>

namespace
{
  using softadastra::sdk::Client;
  using softadastra::sdk::ClientOptions;
  using softadastra::sdk::Error;
}

TEST(ClientDiscoveryTests, StartDiscoveryFailsWhenClientIsClosed)
{
  Client client{
      ClientOptions::memory_only("node-discovery-closed")
          .with_local_discovery(5051)};

  const auto result =
      client.start_discovery();

  ASSERT_TRUE(result.is_err());
  EXPECT_EQ(result.error().code(), Error::Code::InvalidState);
}

TEST(ClientDiscoveryTests, StartDiscoveryFailsWhenDiscoveryIsDisabled)
{
  Client client{
      ClientOptions::memory_only("node-discovery-disabled")};

  ASSERT_TRUE(client.open().is_ok());

  const auto result =
      client.start_discovery();

  ASSERT_TRUE(result.is_err());
  EXPECT_EQ(result.error().code(), Error::Code::DiscoveryError);
  EXPECT_FALSE(client.discovery_running());
}

TEST(ClientDiscoveryTests, StartDiscoveryWorksWhenEnabled)
{
  Client client{
      ClientOptions::memory_only("node-discovery-enabled")
          .with_local_discovery(5052)};

  ASSERT_TRUE(client.open().is_ok());

  const auto result =
      client.start_discovery();

  ASSERT_TRUE(result.is_ok());
  EXPECT_TRUE(client.discovery_running());

  client.stop_discovery();

  EXPECT_FALSE(client.discovery_running());
}

TEST(ClientDiscoveryTests, StartDiscoveryTwiceIsOk)
{
  Client client{
      ClientOptions::memory_only("node-discovery-start-twice")
          .with_local_discovery(5053)};

  ASSERT_TRUE(client.open().is_ok());

  const auto first =
      client.start_discovery();

  const auto second =
      client.start_discovery();

  EXPECT_TRUE(first.is_ok());
  EXPECT_TRUE(second.is_ok());
  EXPECT_TRUE(client.discovery_running());

  client.stop_discovery();
}

TEST(ClientDiscoveryTests, StopDiscoveryIsSafeWhenDisabled)
{
  Client client{
      ClientOptions::memory_only("node-discovery-stop-disabled")};

  ASSERT_TRUE(client.open().is_ok());

  EXPECT_FALSE(client.discovery_running());

  client.stop_discovery();

  EXPECT_FALSE(client.discovery_running());
}

TEST(ClientDiscoveryTests, StopDiscoveryIsSafeWhenClosed)
{
  Client client{
      ClientOptions::memory_only("node-discovery-stop-closed")
          .with_local_discovery(5054)};

  EXPECT_FALSE(client.is_open());

  client.stop_discovery();

  EXPECT_FALSE(client.discovery_running());
}

TEST(ClientDiscoveryTests, PeersFailsWhenClientIsClosed)
{
  Client client{
      ClientOptions::memory_only("node-discovery-peers-closed")
          .with_local_discovery(5055)};

  const auto peers =
      client.peers();

  ASSERT_TRUE(peers.is_err());
  EXPECT_EQ(peers.error().code(), Error::Code::InvalidState);
}

TEST(ClientDiscoveryTests, PeersFailsWhenDiscoveryIsDisabled)
{
  Client client{
      ClientOptions::memory_only("node-discovery-peers-disabled")};

  ASSERT_TRUE(client.open().is_ok());

  const auto peers =
      client.peers();

  ASSERT_TRUE(peers.is_err());
  EXPECT_EQ(peers.error().code(), Error::Code::DiscoveryError);
}

TEST(ClientDiscoveryTests, PeersReturnsListWhenDiscoveryIsEnabled)
{
  Client client{
      ClientOptions::memory_only("node-discovery-peers-enabled")
          .with_local_discovery(5056)};

  ASSERT_TRUE(client.open().is_ok());

  const auto peers =
      client.peers();

  ASSERT_TRUE(peers.is_ok());
  EXPECT_TRUE(peers.value().empty());
}

TEST(ClientDiscoveryTests, DiscoveryRunningIsFalseBeforeStart)
{
  Client client{
      ClientOptions::memory_only("node-discovery-before-start")
          .with_local_discovery(5057)};

  ASSERT_TRUE(client.open().is_ok());

  EXPECT_FALSE(client.discovery_running());
}

TEST(ClientDiscoveryTests, DiscoveryRunningIsFalseAfterClose)
{
  Client client{
      ClientOptions::memory_only("node-discovery-after-close")
          .with_local_discovery(5058)};

  ASSERT_TRUE(client.open().is_ok());
  ASSERT_TRUE(client.start_discovery().is_ok());
  ASSERT_TRUE(client.discovery_running());

  client.close();

  EXPECT_FALSE(client.discovery_running());
}
