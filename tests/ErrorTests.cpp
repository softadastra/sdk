/**
 *
 *  @file ErrorTests.cpp
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

#include <softadastra/sdk/Error.hpp>
#include <gtest/gtest.h>

namespace
{
  using softadastra::sdk::Error;
}

TEST(ErrorTests, DefaultErrorIsOk)
{
  const Error error;

  EXPECT_EQ(error.code(), Error::Code::None);
  EXPECT_TRUE(error.ok());
  EXPECT_FALSE(error.has_error());
  EXPECT_FALSE(error.has_context());
  EXPECT_TRUE(error.message().empty());
  EXPECT_TRUE(error.context().empty());
  EXPECT_EQ(error.code_string(), "none");
}

TEST(ErrorTests, NoneFactoryCreatesOkError)
{
  const Error error = Error::none();

  EXPECT_EQ(error.code(), Error::Code::None);
  EXPECT_TRUE(error.ok());
  EXPECT_FALSE(error.has_error());
  EXPECT_EQ(error.code_string(), "none");
}

TEST(ErrorTests, MakeCreatesExplicitError)
{
  const Error error =
      Error::make(
          Error::Code::InvalidArgument,
          "invalid input",
          "field:name");

  EXPECT_EQ(error.code(), Error::Code::InvalidArgument);
  EXPECT_EQ(error.message(), "invalid input");
  EXPECT_EQ(error.context(), "field:name");
  EXPECT_TRUE(error.has_error());
  EXPECT_FALSE(error.ok());
  EXPECT_TRUE(error.has_context());
  EXPECT_EQ(error.code_string(), "invalid_argument");
}

TEST(ErrorTests, FactoryMethodsSetExpectedCodes)
{
  EXPECT_EQ(
      Error::unknown("x").code(),
      Error::Code::Unknown);

  EXPECT_EQ(
      Error::invalid_argument("x").code(),
      Error::Code::InvalidArgument);

  EXPECT_EQ(
      Error::invalid_state("x").code(),
      Error::Code::InvalidState);

  EXPECT_EQ(
      Error::not_found("x").code(),
      Error::Code::NotFound);

  EXPECT_EQ(
      Error::already_exists("x").code(),
      Error::Code::AlreadyExists);

  EXPECT_EQ(
      Error::io_error("x").code(),
      Error::Code::IoError);

  EXPECT_EQ(
      Error::store_error("x").code(),
      Error::Code::StoreError);

  EXPECT_EQ(
      Error::sync_error("x").code(),
      Error::Code::SyncError);

  EXPECT_EQ(
      Error::transport_error("x").code(),
      Error::Code::TransportError);

  EXPECT_EQ(
      Error::discovery_error("x").code(),
      Error::Code::DiscoveryError);

  EXPECT_EQ(
      Error::metadata_error("x").code(),
      Error::Code::MetadataError);

  EXPECT_EQ(
      Error::internal_error("x").code(),
      Error::Code::InternalError);
}

TEST(ErrorTests, ToStringReturnsStableNames)
{
  EXPECT_EQ(Error::to_string(Error::Code::None), "none");
  EXPECT_EQ(Error::to_string(Error::Code::Unknown), "unknown");
  EXPECT_EQ(
      Error::to_string(Error::Code::InvalidArgument),
      "invalid_argument");
  EXPECT_EQ(
      Error::to_string(Error::Code::InvalidState),
      "invalid_state");
  EXPECT_EQ(Error::to_string(Error::Code::NotFound), "not_found");
  EXPECT_EQ(
      Error::to_string(Error::Code::AlreadyExists),
      "already_exists");
  EXPECT_EQ(Error::to_string(Error::Code::IoError), "io_error");
  EXPECT_EQ(Error::to_string(Error::Code::StoreError), "store_error");
  EXPECT_EQ(Error::to_string(Error::Code::SyncError), "sync_error");
  EXPECT_EQ(
      Error::to_string(Error::Code::TransportError),
      "transport_error");
  EXPECT_EQ(
      Error::to_string(Error::Code::DiscoveryError),
      "discovery_error");
  EXPECT_EQ(
      Error::to_string(Error::Code::MetadataError),
      "metadata_error");
  EXPECT_EQ(
      Error::to_string(Error::Code::InternalError),
      "internal_error");
}

TEST(ErrorTests, ClearResetsError)
{
  Error error =
      Error::internal_error(
          "something failed",
          "runtime");

  ASSERT_TRUE(error.has_error());
  ASSERT_TRUE(error.has_context());

  error.clear();

  EXPECT_EQ(error.code(), Error::Code::None);
  EXPECT_TRUE(error.ok());
  EXPECT_FALSE(error.has_error());
  EXPECT_FALSE(error.has_context());
  EXPECT_TRUE(error.message().empty());
  EXPECT_TRUE(error.context().empty());
}

TEST(ErrorTests, EqualityUsesCodeMessageAndContext)
{
  const Error first =
      Error::invalid_state(
          "not open",
          "client");

  const Error second =
      Error::invalid_state(
          "not open",
          "client");

  const Error third =
      Error::invalid_state(
          "closed",
          "client");

  EXPECT_EQ(first, second);
  EXPECT_NE(first, third);
}
