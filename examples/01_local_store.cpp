/**
 *
 *  @file 01_local_store.cpp
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

#include <softadastra/sdk.hpp>
#include <iostream>

int main()
{
  using namespace softadastra::sdk;

  Client client{
      ClientOptions::memory_only("example-local-store")};

  const auto opened = client.open();

  if (opened.is_err())
  {
    std::cerr << "open failed: "
              << opened.error().code_string()
              << ": "
              << opened.error().message()
              << "\n";

    return 1;
  }

  const auto stored =
      client.put("hello", "world");

  if (stored.is_err())
  {
    std::cerr << "put failed: "
              << stored.error().code_string()
              << ": "
              << stored.error().message()
              << "\n";

    return 1;
  }

  const auto value =
      client.get("hello");

  if (value.is_err())
  {
    std::cerr << "get failed: "
              << value.error().code_string()
              << ": "
              << value.error().message()
              << "\n";

    return 1;
  }

  std::cout << "key   : hello\n";
  std::cout << "value : "
            << value.value().to_string()
            << "\n";

  client.close();

  return 0;
}
