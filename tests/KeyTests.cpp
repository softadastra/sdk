/**
 *
 *  @file KeyTests.cpp
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

#include <softadastra/sdk/Key.hpp>
#include <gtest/gtest.h>

#include <string>
#include <string_view>

namespace
{
  using softadastra::sdk::Key;
}

TEST(KeyTests, DefaultKeyIsEmptyAndInvalid)
{
  const Key key;

  EXPECT_TRUE(key.empty());
  EXPECT_FALSE(key.is_valid());
  EXPECT_FALSE(key.valid());
  EXPECT_TRUE(key.str().empty());
  EXPECT_TRUE(key.value().empty());
}

TEST(KeyTests, CreatesKeyFromString)
{
  std::string text{"profile/name"};

  const Key key{text};

  EXPECT_EQ(key.str(), "profile/name");
  EXPECT_EQ(key.value(), "profile/name");
  EXPECT_TRUE(key.is_valid());
}

TEST(KeyTests, CreatesKeyFromMovedString)
{
  Key key{std::string{"settings/theme"}};

  EXPECT_EQ(key.str(), "settings/theme");
  EXPECT_TRUE(key.valid());
}

TEST(KeyTests, CreatesKeyFromStringView)
{
  const std::string_view view{"session/token"};

  const Key key{view};

  EXPECT_EQ(key.str(), "session/token");
  EXPECT_TRUE(key.is_valid());
}

TEST(KeyTests, CreatesEmptyKeyFromNullCString)
{
  const char *text = nullptr;

  const Key key{text};

  EXPECT_TRUE(key.empty());
  EXPECT_FALSE(key.is_valid());
}

TEST(KeyTests, CreatesKeyFromCString)
{
  const Key key{"node/id"};

  EXPECT_EQ(key.str(), "node/id");
  EXPECT_TRUE(key.is_valid());
}

TEST(KeyTests, FromFactoryCreatesKey)
{
  const Key key = Key::from("store/key");

  EXPECT_EQ(key.str(), "store/key");
  EXPECT_TRUE(key.is_valid());
}

TEST(KeyTests, FromViewFactoryCreatesKey)
{
  const Key key = Key::from_view("store/view");

  EXPECT_EQ(key.str(), "store/view");
  EXPECT_TRUE(key.is_valid());
}

TEST(KeyTests, ClearMakesKeyInvalid)
{
  Key key{"temporary"};

  ASSERT_TRUE(key.is_valid());

  key.clear();

  EXPECT_TRUE(key.empty());
  EXPECT_FALSE(key.is_valid());
}

TEST(KeyTests, EqualityComparesValue)
{
  const Key left{"same"};
  const Key right{"same"};
  const Key other{"other"};

  EXPECT_EQ(left, right);
  EXPECT_NE(left, other);
}

TEST(KeyTests, LessThanOrdersLexicographically)
{
  const Key a{"a"};
  const Key b{"b"};

  EXPECT_LT(a, b);
}
