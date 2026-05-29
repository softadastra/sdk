/**
 *
 *  @file ClientStoreTests.cpp
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
#include <softadastra/sdk/Key.hpp>
#include <softadastra/sdk/Value.hpp>

#include <gtest/gtest.h>

#include <cstdint>
#include <string>
#include <vector>

namespace
{
  using softadastra::sdk::Client;
  using softadastra::sdk::ClientOptions;
  using softadastra::sdk::Error;
  using softadastra::sdk::Key;
  using softadastra::sdk::Value;
}

TEST(ClientStoreTests, PutAndGetStringValue)
{
  Client client{ClientOptions::memory_only("node-store-put-get")};

  ASSERT_TRUE(client.open().is_ok());

  const auto put_result =
      client.put("name", "Softadastra");

  ASSERT_TRUE(put_result.is_ok());

  const auto value_result =
      client.get("name");

  ASSERT_TRUE(value_result.is_ok());
  EXPECT_EQ(value_result.value().to_string(), "Softadastra");
}

TEST(ClientStoreTests, PutAndGetSdkKeyValue)
{
  Client client{ClientOptions::memory_only("node-store-sdk-types")};

  ASSERT_TRUE(client.open().is_ok());

  const Key key{"profile/name"};
  const Value value{"Gaspard"};

  const auto put_result =
      client.put(key, value);

  ASSERT_TRUE(put_result.is_ok());

  const auto value_result =
      client.get(key);

  ASSERT_TRUE(value_result.is_ok());
  EXPECT_EQ(value_result.value(), value);
}

TEST(ClientStoreTests, PutSupportsBinaryValue)
{
  Client client{ClientOptions::memory_only("node-store-binary")};

  ASSERT_TRUE(client.open().is_ok());

  const Value value{
      Value::Container{
          static_cast<std::uint8_t>('a'),
          static_cast<std::uint8_t>(0),
          static_cast<std::uint8_t>('b')}};

  ASSERT_TRUE(client.put(Key{"binary"}, value).is_ok());

  const auto value_result =
      client.get("binary");

  ASSERT_TRUE(value_result.is_ok());
  ASSERT_EQ(value_result.value().size(), 3U);
  EXPECT_EQ(value_result.value().bytes()[0], static_cast<std::uint8_t>('a'));
  EXPECT_EQ(value_result.value().bytes()[1], 0U);
  EXPECT_EQ(value_result.value().bytes()[2], static_cast<std::uint8_t>('b'));
}

TEST(ClientStoreTests, PutOverwritesExistingValue)
{
  Client client{ClientOptions::memory_only("node-store-overwrite")};

  ASSERT_TRUE(client.open().is_ok());

  ASSERT_TRUE(client.put("name", "first").is_ok());
  ASSERT_TRUE(client.put("name", "second").is_ok());

  const auto value_result =
      client.get("name");

  ASSERT_TRUE(value_result.is_ok());
  EXPECT_EQ(value_result.value().to_string(), "second");
}

TEST(ClientStoreTests, GetMissingKeyReturnsNotFound)
{
  Client client{ClientOptions::memory_only("node-store-missing")};

  ASSERT_TRUE(client.open().is_ok());

  const auto value_result =
      client.get("missing");

  ASSERT_TRUE(value_result.is_err());
  EXPECT_EQ(value_result.error().code(), Error::Code::NotFound);
  EXPECT_EQ(value_result.error().context(), "missing");
}

TEST(ClientStoreTests, PutRejectsEmptyKey)
{
  Client client{ClientOptions::memory_only("node-store-empty-key")};

  ASSERT_TRUE(client.open().is_ok());

  const auto result =
      client.put("", "value");

  ASSERT_TRUE(result.is_err());
  EXPECT_EQ(result.error().code(), Error::Code::InvalidArgument);
}

TEST(ClientStoreTests, GetRejectsEmptyKey)
{
  Client client{ClientOptions::memory_only("node-store-get-empty-key")};

  ASSERT_TRUE(client.open().is_ok());

  const auto result =
      client.get("");

  ASSERT_TRUE(result.is_err());
  EXPECT_EQ(result.error().code(), Error::Code::InvalidArgument);
}

TEST(ClientStoreTests, RemoveRejectsEmptyKey)
{
  Client client{ClientOptions::memory_only("node-store-remove-empty-key")};

  ASSERT_TRUE(client.open().is_ok());

  const auto result =
      client.remove("");

  ASSERT_TRUE(result.is_err());
  EXPECT_EQ(result.error().code(), Error::Code::InvalidArgument);
}

TEST(ClientStoreTests, ContainsReturnsFalseForMissingKey)
{
  Client client{ClientOptions::memory_only("node-store-contains-missing")};

  ASSERT_TRUE(client.open().is_ok());

  EXPECT_FALSE(client.contains("missing"));
}

TEST(ClientStoreTests, ContainsReturnsTrueForExistingKey)
{
  Client client{ClientOptions::memory_only("node-store-contains-existing")};

  ASSERT_TRUE(client.open().is_ok());
  ASSERT_TRUE(client.put("name", "Softadastra").is_ok());

  EXPECT_TRUE(client.contains("name"));
}

TEST(ClientStoreTests, ContainsReturnsFalseForEmptyKey)
{
  Client client{ClientOptions::memory_only("node-store-contains-empty")};

  ASSERT_TRUE(client.open().is_ok());

  EXPECT_FALSE(client.contains(""));
}

TEST(ClientStoreTests, SizeAndEmptyReflectStoreState)
{
  Client client{ClientOptions::memory_only("node-store-size")};

  ASSERT_TRUE(client.open().is_ok());

  EXPECT_TRUE(client.empty());
  EXPECT_EQ(client.size(), 0U);

  ASSERT_TRUE(client.put("one", "1").is_ok());

  EXPECT_FALSE(client.empty());
  EXPECT_EQ(client.size(), 1U);

  ASSERT_TRUE(client.put("two", "2").is_ok());

  EXPECT_FALSE(client.empty());
  EXPECT_EQ(client.size(), 2U);
}

TEST(ClientStoreTests, RemoveDeletesExistingKey)
{
  Client client{ClientOptions::memory_only("node-store-remove")};

  ASSERT_TRUE(client.open().is_ok());

  ASSERT_TRUE(client.put("name", "Softadastra").is_ok());
  ASSERT_TRUE(client.contains("name"));

  const auto removed =
      client.remove("name");

  ASSERT_TRUE(removed.is_ok());
  EXPECT_FALSE(client.contains("name"));

  const auto value_result =
      client.get("name");

  ASSERT_TRUE(value_result.is_err());
  EXPECT_EQ(value_result.error().code(), Error::Code::NotFound);
}

TEST(ClientStoreTests, RemoveMissingKeyStillReturnsOk)
{
  Client client{ClientOptions::memory_only("node-store-remove-missing")};

  ASSERT_TRUE(client.open().is_ok());

  const auto removed =
      client.remove("missing");

  EXPECT_TRUE(removed.is_ok());
}

TEST(ClientStoreTests, ClosedClientDoesNotExposeStoreState)
{
  Client client{ClientOptions::memory_only("node-store-closed")};

  ASSERT_TRUE(client.open().is_ok());
  ASSERT_TRUE(client.put("name", "Softadastra").is_ok());

  ASSERT_EQ(client.size(), 1U);
  ASSERT_TRUE(client.contains("name"));

  client.close();

  EXPECT_EQ(client.size(), 0U);
  EXPECT_TRUE(client.empty());
  EXPECT_FALSE(client.contains("name"));
}
