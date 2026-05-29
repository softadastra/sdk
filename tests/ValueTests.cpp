/**
 *
 *  @file ValueTests.cpp
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

#include <softadastra/sdk/Value.hpp>

#include <gtest/gtest.h>

#include <cstdint>
#include <span>
#include <string>
#include <vector>

namespace
{
  using softadastra::sdk::Value;
}

TEST(ValueTests, DefaultValueIsEmpty)
{
  const Value value;

  EXPECT_TRUE(value.empty());
  EXPECT_EQ(value.size(), 0U);
  EXPECT_TRUE(value.bytes().empty());
  EXPECT_TRUE(value.data().empty());
  EXPECT_EQ(value.to_string(), "");
}

TEST(ValueTests, CreatesValueFromByteContainer)
{
  Value::Container bytes{
      static_cast<std::uint8_t>('a'),
      static_cast<std::uint8_t>('b'),
      static_cast<std::uint8_t>('c')};

  const Value value{bytes};

  EXPECT_FALSE(value.empty());
  EXPECT_EQ(value.size(), 3U);
  EXPECT_EQ(value.to_string(), "abc");
}

TEST(ValueTests, CreatesValueFromMovedByteContainer)
{
  Value value{
      Value::Container{
          static_cast<std::uint8_t>('x'),
          static_cast<std::uint8_t>('y'),
          static_cast<std::uint8_t>('z')}};

  EXPECT_EQ(value.size(), 3U);
  EXPECT_EQ(value.to_string(), "xyz");
}

TEST(ValueTests, CreatesValueFromSpan)
{
  const std::vector<std::uint8_t> bytes{
      static_cast<std::uint8_t>('h'),
      static_cast<std::uint8_t>('e'),
      static_cast<std::uint8_t>('y')};

  const std::span<const std::uint8_t> view{
      bytes.data(),
      bytes.size()};

  const Value value{view};

  EXPECT_EQ(value.size(), 3U);
  EXPECT_EQ(value.to_string(), "hey");
}

TEST(ValueTests, CreatesValueFromStringView)
{
  const Value value{std::string_view{"Softadastra"}};

  EXPECT_EQ(value.size(), 11U);
  EXPECT_EQ(value.to_string(), "Softadastra");
}

TEST(ValueTests, CreatesValueFromCString)
{
  const Value value{"hello"};

  EXPECT_EQ(value.size(), 5U);
  EXPECT_EQ(value.to_string(), "hello");
}

TEST(ValueTests, CreatesEmptyValueFromNullCString)
{
  const char *text = nullptr;

  const Value value{text};

  EXPECT_TRUE(value.empty());
  EXPECT_EQ(value.size(), 0U);
  EXPECT_EQ(value.to_string(), "");
}

TEST(ValueTests, FromBytesFactoryCreatesValue)
{
  Value::Container bytes{
      static_cast<std::uint8_t>('1'),
      static_cast<std::uint8_t>('2')};

  const Value value = Value::from_bytes(bytes);

  EXPECT_EQ(value.size(), 2U);
  EXPECT_EQ(value.to_string(), "12");
}

TEST(ValueTests, FromSpanFactoryCreatesValue)
{
  const std::vector<std::uint8_t> bytes{
      static_cast<std::uint8_t>('o'),
      static_cast<std::uint8_t>('k')};

  const Value value =
      Value::from_span(
          std::span<const std::uint8_t>{
              bytes.data(),
              bytes.size()});

  EXPECT_EQ(value.size(), 2U);
  EXPECT_EQ(value.to_string(), "ok");
}

TEST(ValueTests, FromStringFactoryCreatesValue)
{
  const Value value = Value::from_string("local-first");

  EXPECT_EQ(value.size(), 11U);
  EXPECT_EQ(value.to_string(), "local-first");
}

TEST(ValueTests, SpanReturnsStoredBytes)
{
  const Value value{"abc"};

  const auto view = value.span();

  ASSERT_EQ(view.size(), 3U);
  EXPECT_EQ(view[0], static_cast<std::uint8_t>('a'));
  EXPECT_EQ(view[1], static_cast<std::uint8_t>('b'));
  EXPECT_EQ(view[2], static_cast<std::uint8_t>('c'));
}

TEST(ValueTests, MutableDataCanBeModified)
{
  Value value{"abc"};

  value.data()[0] = static_cast<std::uint8_t>('z');

  EXPECT_EQ(value.to_string(), "zbc");
}

TEST(ValueTests, ClearMakesValueEmpty)
{
  Value value{"temporary"};

  ASSERT_FALSE(value.empty());

  value.clear();

  EXPECT_TRUE(value.empty());
  EXPECT_EQ(value.size(), 0U);
}

TEST(ValueTests, EqualityComparesBytes)
{
  const Value left{"same"};
  const Value right{"same"};
  const Value other{"other"};

  EXPECT_EQ(left, right);
  EXPECT_NE(left, other);
}

TEST(ValueTests, SupportsBinaryDataWithZeroByte)
{
  const Value value{
      Value::Container{
          static_cast<std::uint8_t>('a'),
          static_cast<std::uint8_t>(0),
          static_cast<std::uint8_t>('b')}};

  EXPECT_EQ(value.size(), 3U);
  EXPECT_EQ(value.bytes()[1], 0U);

  const std::string text = value.to_string();

  ASSERT_EQ(text.size(), 3U);
  EXPECT_EQ(text[0], 'a');
  EXPECT_EQ(text[1], '\0');
  EXPECT_EQ(text[2], 'b');
}
