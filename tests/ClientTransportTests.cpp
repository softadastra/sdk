/**
 *
 *  @file ClientTransportTests.cpp
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
#include <softadastra/sdk/Peer.hpp>

#include <gtest/gtest.h>

namespace
{
  using softadastra::sdk::Client;
  using softadastra::sdk::ClientOptions;
  using softadastra::sdk::Error;
  using softadastra::sdk::Peer;
}

TEST(ClientTransportTests, StartTransportFailsWhenClientIsClosed)
{
  Client client{
      ClientOptions::memory_only("node-transport-closed")
          .with_local_transport(9100)};

  const auto result =
      client.start_transport();

  ASSERT_TRUE(result.is_err());
  EXPECT_EQ(result.error().code(), Error::Code::InvalidState);
}

TEST(ClientTransportTests, StartTransportFailsWhenTransportIsDisabled)
{
  Client client{
      ClientOptions::memory_only("node-transport-disabled")};

  ASSERT_TRUE(client.open().is_ok());

  const auto result =
      client.start_transport();

  ASSERT_TRUE(result.is_err());
  EXPECT_EQ(result.error().code(), Error::Code::TransportError);
  EXPECT_FALSE(client.transport_running());
}

TEST(ClientTransportTests, StartTransportWorksWhenEnabled)
{
  Client client{
      ClientOptions::memory_only("node-transport-enabled")
          .with_local_transport(9101)};

  ASSERT_TRUE(client.open().is_ok());

  const auto result =
      client.start_transport();

  ASSERT_TRUE(result.is_ok());
  EXPECT_TRUE(client.transport_running());

  client.stop_transport();

  EXPECT_FALSE(client.transport_running());
}

TEST(ClientTransportTests, StartTransportTwiceIsOk)
{
  Client client{
      ClientOptions::memory_only("node-transport-start-twice")
          .with_local_transport(9102)};

  ASSERT_TRUE(client.open().is_ok());

  const auto first =
      client.start_transport();

  const auto second =
      client.start_transport();

  EXPECT_TRUE(first.is_ok());
  EXPECT_TRUE(second.is_ok());
  EXPECT_TRUE(client.transport_running());

  client.stop_transport();
}

TEST(ClientTransportTests, StopTransportIsSafeWhenDisabled)
{
  Client client{
      ClientOptions::memory_only("node-transport-stop-disabled")};

  ASSERT_TRUE(client.open().is_ok());

  EXPECT_FALSE(client.transport_running());

  client.stop_transport();

  EXPECT_FALSE(client.transport_running());
}

TEST(ClientTransportTests, StopTransportIsSafeWhenClosed)
{
  Client client{
      ClientOptions::memory_only("node-transport-stop-closed")
          .with_local_transport(9103)};

  EXPECT_FALSE(client.is_open());

  client.stop_transport();

  EXPECT_FALSE(client.transport_running());
}

TEST(ClientTransportTests, ConnectFailsWhenClientIsClosed)
{
  Client client{
      ClientOptions::memory_only("node-transport-connect-closed")
          .with_local_transport(9104)};

  const Peer peer =
      Peer::local("peer-1", 9200);

  const auto result =
      client.connect(peer);

  ASSERT_TRUE(result.is_err());
  EXPECT_EQ(result.error().code(), Error::Code::InvalidState);
}

TEST(ClientTransportTests, ConnectFailsWhenTransportIsDisabled)
{
  Client client{
      ClientOptions::memory_only("node-transport-connect-disabled")};

  ASSERT_TRUE(client.open().is_ok());

  const Peer peer =
      Peer::local("peer-1", 9201);

  const auto result =
      client.connect(peer);

  ASSERT_TRUE(result.is_err());
  EXPECT_EQ(result.error().code(), Error::Code::TransportError);
}

TEST(ClientTransportTests, ConnectRejectsInvalidPeer)
{
  Client client{
      ClientOptions::memory_only("node-transport-invalid-peer")
          .with_local_transport(9105)};

  ASSERT_TRUE(client.open().is_ok());

  const Peer peer;

  const auto result =
      client.connect(peer);

  ASSERT_TRUE(result.is_err());
  EXPECT_EQ(result.error().code(), Error::Code::InvalidArgument);
}

TEST(ClientTransportTests, DisconnectFailsWhenClientIsClosed)
{
  Client client{
      ClientOptions::memory_only("node-transport-disconnect-closed")
          .with_local_transport(9106)};

  const Peer peer =
      Peer::local("peer-1", 9202);

  const auto result =
      client.disconnect(peer);

  ASSERT_TRUE(result.is_err());
  EXPECT_EQ(result.error().code(), Error::Code::InvalidState);
}

TEST(ClientTransportTests, DisconnectFailsWhenTransportIsDisabled)
{
  Client client{
      ClientOptions::memory_only("node-transport-disconnect-disabled")};

  ASSERT_TRUE(client.open().is_ok());

  const Peer peer =
      Peer::local("peer-1", 9203);

  const auto result =
      client.disconnect(peer);

  ASSERT_TRUE(result.is_err());
  EXPECT_EQ(result.error().code(), Error::Code::TransportError);
}

TEST(ClientTransportTests, DisconnectRejectsInvalidPeer)
{
  Client client{
      ClientOptions::memory_only("node-transport-disconnect-invalid-peer")
          .with_local_transport(9107)};

  ASSERT_TRUE(client.open().is_ok());

  const Peer peer;

  const auto result =
      client.disconnect(peer);

  ASSERT_TRUE(result.is_err());
  EXPECT_EQ(result.error().code(), Error::Code::InvalidArgument);
}

TEST(ClientTransportTests, PeerLocalCreatesValidLocalhostPeer)
{
  const Peer peer =
      Peer::local("peer-local", 9300);

  EXPECT_EQ(peer.node_id(), "peer-local");
  EXPECT_EQ(peer.host(), "127.0.0.1");
  EXPECT_EQ(peer.port(), 9300U);
  EXPECT_TRUE(peer.is_localhost());
  EXPECT_TRUE(peer.is_valid());
  EXPECT_TRUE(peer.valid());
}

TEST(ClientTransportTests, PeerSettersUpdateFields)
{
  Peer peer;

  peer.set_node_id("peer-updated");
  peer.set_host("localhost");
  peer.set_port(9400);

  EXPECT_EQ(peer.node_id(), "peer-updated");
  EXPECT_EQ(peer.host(), "localhost");
  EXPECT_EQ(peer.port(), 9400U);
  EXPECT_TRUE(peer.is_localhost());
  EXPECT_TRUE(peer.is_valid());
}

TEST(ClientTransportTests, PeerClearMakesPeerInvalid)
{
  Peer peer =
      Peer::local("peer-clear", 9500);

  ASSERT_TRUE(peer.is_valid());

  peer.clear();

  EXPECT_TRUE(peer.node_id().empty());
  EXPECT_TRUE(peer.host().empty());
  EXPECT_EQ(peer.port(), 0U);
  EXPECT_FALSE(peer.is_valid());
}

TEST(ClientTransportTests, PeerEqualityComparesFields)
{
  const Peer first{
      "peer-1",
      "127.0.0.1",
      9600};

  const Peer second{
      "peer-1",
      "127.0.0.1",
      9600};

  const Peer third{
      "peer-2",
      "127.0.0.1",
      9600};

  EXPECT_EQ(first, second);
  EXPECT_NE(first, third);
}
