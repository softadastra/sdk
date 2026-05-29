/**
 *
 *  @file ClientRecoveryTests.cpp
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

#include <gtest/gtest.h>

#include <chrono>
#include <filesystem>
#include <string>

namespace
{
  using softadastra::sdk::Client;
  using softadastra::sdk::ClientOptions;

  [[nodiscard]] std::filesystem::path unique_wal_path(
      const std::string &name)
  {
    const auto now =
        std::chrono::steady_clock::now()
            .time_since_epoch()
            .count();

    auto path =
        std::filesystem::temp_directory_path() /
        "softadastra-sdk-tests" /
        (name + "-" + std::to_string(now) + ".wal");

    std::filesystem::create_directories(path.parent_path());

    return path;
  }

  void remove_if_exists(const std::filesystem::path &path)
  {
    std::error_code error;
    std::filesystem::remove(path, error);
  }
}

TEST(ClientRecoveryTests, PersistentClientRecoversValueAfterReopen)
{
  const auto wal_path =
      unique_wal_path("persistent-reopen");

  remove_if_exists(wal_path);

  {
    Client writer{
        ClientOptions::persistent(
            "node-recovery-writer",
            wal_path.string())};

    ASSERT_TRUE(writer.open().is_ok());
    ASSERT_TRUE(writer.put("name", "Softadastra").is_ok());

    writer.close();
  }

  {
    Client reader{
        ClientOptions::persistent(
            "node-recovery-reader",
            wal_path.string())};

    ASSERT_TRUE(reader.open().is_ok());

    const auto value =
        reader.get("name");

    ASSERT_TRUE(value.is_ok());
    EXPECT_EQ(value.value().to_string(), "Softadastra");

    reader.close();
  }

  remove_if_exists(wal_path);
}

TEST(ClientRecoveryTests, PersistentClientRecoversMultipleValues)
{
  const auto wal_path =
      unique_wal_path("persistent-multiple");

  remove_if_exists(wal_path);

  {
    Client writer{
        ClientOptions::persistent(
            "node-recovery-multiple-writer",
            wal_path.string())};

    ASSERT_TRUE(writer.open().is_ok());
    ASSERT_TRUE(writer.put("profile/name", "Ada").is_ok());
    ASSERT_TRUE(writer.put("profile/lang", "C++").is_ok());
    ASSERT_TRUE(writer.put("profile/runtime", "Softadastra").is_ok());

    writer.close();
  }

  {
    Client reader{
        ClientOptions::persistent(
            "node-recovery-multiple-reader",
            wal_path.string())};

    ASSERT_TRUE(reader.open().is_ok());

    const auto name =
        reader.get("profile/name");

    const auto language =
        reader.get("profile/lang");

    const auto runtime =
        reader.get("profile/runtime");

    ASSERT_TRUE(name.is_ok());
    ASSERT_TRUE(language.is_ok());
    ASSERT_TRUE(runtime.is_ok());

    EXPECT_EQ(name.value().to_string(), "Ada");
    EXPECT_EQ(language.value().to_string(), "C++");
    EXPECT_EQ(runtime.value().to_string(), "Softadastra");

    reader.close();
  }

  remove_if_exists(wal_path);
}

TEST(ClientRecoveryTests, PersistentClientRecoversOverwrittenValue)
{
  const auto wal_path =
      unique_wal_path("persistent-overwrite");

  remove_if_exists(wal_path);

  {
    Client writer{
        ClientOptions::persistent(
            "node-recovery-overwrite-writer",
            wal_path.string())};

    ASSERT_TRUE(writer.open().is_ok());
    ASSERT_TRUE(writer.put("name", "first").is_ok());
    ASSERT_TRUE(writer.put("name", "second").is_ok());

    writer.close();
  }

  {
    Client reader{
        ClientOptions::persistent(
            "node-recovery-overwrite-reader",
            wal_path.string())};

    ASSERT_TRUE(reader.open().is_ok());

    const auto value =
        reader.get("name");

    ASSERT_TRUE(value.is_ok());
    EXPECT_EQ(value.value().to_string(), "second");

    reader.close();
  }

  remove_if_exists(wal_path);
}

TEST(ClientRecoveryTests, PersistentClientRecoversRemovedValue)
{
  const auto wal_path =
      unique_wal_path("persistent-remove");

  remove_if_exists(wal_path);

  {
    Client writer{
        ClientOptions::persistent(
            "node-recovery-remove-writer",
            wal_path.string())};

    ASSERT_TRUE(writer.open().is_ok());
    ASSERT_TRUE(writer.put("name", "Softadastra").is_ok());
    ASSERT_TRUE(writer.remove("name").is_ok());

    writer.close();
  }

  {
    Client reader{
        ClientOptions::persistent(
            "node-recovery-remove-reader",
            wal_path.string())};

    ASSERT_TRUE(reader.open().is_ok());

    EXPECT_FALSE(reader.contains("name"));

    const auto value =
        reader.get("name");

    EXPECT_TRUE(value.is_err());

    reader.close();
  }

  remove_if_exists(wal_path);
}

TEST(ClientRecoveryTests, MemoryOnlyClientDoesNotRecoverAfterReopen)
{
  Client writer{
      ClientOptions::memory_only("node-memory-recovery-writer")};

  ASSERT_TRUE(writer.open().is_ok());
  ASSERT_TRUE(writer.put("name", "Softadastra").is_ok());
  ASSERT_TRUE(writer.contains("name"));

  writer.close();

  Client reader{
      ClientOptions::memory_only("node-memory-recovery-reader")};

  ASSERT_TRUE(reader.open().is_ok());

  EXPECT_FALSE(reader.contains("name"));

  const auto value =
      reader.get("name");

  EXPECT_TRUE(value.is_err());
}
