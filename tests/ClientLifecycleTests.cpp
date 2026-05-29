/**
 *
 *  @file ClientLifecycleTests.cpp
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

#include <utility>

namespace
{
  using softadastra::sdk::Client;
  using softadastra::sdk::ClientOptions;
  using softadastra::sdk::Error;
}

TEST(ClientLifecycleTests, DefaultClientStartsClosed)
{
  const Client client;

  EXPECT_FALSE(client.is_open());
  EXPECT_FALSE(client.opened());
  EXPECT_EQ(client.size(), 0U);
  EXPECT_TRUE(client.empty());
}

TEST(ClientLifecycleTests, OpenInitializesClient)
{
  Client client{ClientOptions::memory_only("node-lifecycle")};

  const auto opened = client.open();

  ASSERT_TRUE(opened.is_ok());
  EXPECT_TRUE(client.is_open());
  EXPECT_TRUE(client.opened());
}

TEST(ClientLifecycleTests, OpenTwiceIsOk)
{
  Client client{ClientOptions::memory_only("node-open-twice")};

  const auto first = client.open();
  const auto second = client.open();

  EXPECT_TRUE(first.is_ok());
  EXPECT_TRUE(second.is_ok());
  EXPECT_TRUE(client.is_open());
}

TEST(ClientLifecycleTests, CloseOnClosedClientIsSafe)
{
  Client client{ClientOptions::memory_only("node-close-safe")};

  EXPECT_FALSE(client.is_open());

  client.close();

  EXPECT_FALSE(client.is_open());
  EXPECT_FALSE(client.opened());
}

TEST(ClientLifecycleTests, CloseAfterOpenClosesClient)
{
  Client client{ClientOptions::memory_only("node-close-after-open")};

  ASSERT_TRUE(client.open().is_ok());
  ASSERT_TRUE(client.is_open());

  client.close();

  EXPECT_FALSE(client.is_open());
  EXPECT_FALSE(client.opened());
  EXPECT_EQ(client.size(), 0U);
  EXPECT_TRUE(client.empty());
}

TEST(ClientLifecycleTests, CanReopenAfterClose)
{
  Client client{ClientOptions::memory_only("node-reopen")};

  ASSERT_TRUE(client.open().is_ok());
  client.close();

  ASSERT_FALSE(client.is_open());

  const auto reopened = client.open();

  EXPECT_TRUE(reopened.is_ok());
  EXPECT_TRUE(client.is_open());
}

TEST(ClientLifecycleTests, OpenFailsWithInvalidOptions)
{
  ClientOptions options{"node-invalid"};

  options.set_node_id("");

  Client client{options};

  const auto opened = client.open();

  ASSERT_TRUE(opened.is_err());
  EXPECT_EQ(opened.error().code(), Error::Code::InvalidArgument);
  EXPECT_FALSE(client.is_open());
}

TEST(ClientLifecycleTests, OperationsFailWhenClientIsClosed)
{
  Client client{ClientOptions::memory_only("node-closed")};

  const auto put_result = client.put("name", "Softadastra");
  const auto get_result = client.get("name");
  const auto remove_result = client.remove("name");
  const auto sync_result = client.sync_state();
  const auto tick_result = client.tick();

  EXPECT_TRUE(put_result.is_err());
  EXPECT_TRUE(get_result.is_err());
  EXPECT_TRUE(remove_result.is_err());
  EXPECT_TRUE(sync_result.is_err());
  EXPECT_TRUE(tick_result.is_err());

  EXPECT_EQ(put_result.error().code(), Error::Code::InvalidState);
  EXPECT_EQ(get_result.error().code(), Error::Code::InvalidState);
  EXPECT_EQ(remove_result.error().code(), Error::Code::InvalidState);
  EXPECT_EQ(sync_result.error().code(), Error::Code::InvalidState);
  EXPECT_EQ(tick_result.error().code(), Error::Code::InvalidState);
}

TEST(ClientLifecycleTests, MoveConstructorTransfersOpenClient)
{
  Client source{ClientOptions::memory_only("node-move-source")};

  ASSERT_TRUE(source.open().is_ok());
  ASSERT_TRUE(source.is_open());

  Client target{std::move(source)};

  EXPECT_TRUE(target.is_open());
  EXPECT_FALSE(source.is_open());
}

TEST(ClientLifecycleTests, MoveAssignmentTransfersOpenClient)
{
  Client source{ClientOptions::memory_only("node-move-assign-source")};
  Client target{ClientOptions::memory_only("node-move-assign-target")};

  ASSERT_TRUE(source.open().is_ok());
  ASSERT_TRUE(source.is_open());

  target = std::move(source);

  EXPECT_TRUE(target.is_open());
  EXPECT_FALSE(source.is_open());
}

TEST(ClientLifecycleTests, OptionsArePreserved)
{
  const ClientOptions options =
      ClientOptions::memory_only("node-options")
          .with_metadata("Lifecycle Node", "0.1.0");

  Client client{options};

  EXPECT_EQ(client.options().node_id(), "node-options");
  EXPECT_EQ(client.options().display_name(), "Lifecycle Node");
  EXPECT_EQ(client.options().version(), "0.1.0");
}
