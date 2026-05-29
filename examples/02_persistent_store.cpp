/**
 *
 *  @file 02_persistent_store.cpp
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
      ClientOptions::persistent(
          "example-persistent-store",
          "data/examples/persistent-store.wal")};

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

  const auto stored_name =
      client.put("profile/name", "Ada");

  if (stored_name.is_err())
  {
    std::cerr << "put failed: "
              << stored_name.error().code_string()
              << ": "
              << stored_name.error().message()
              << "\n";

    return 1;
  }

  const auto stored_language =
      client.put("profile/language", "C++");

  if (stored_language.is_err())
  {
    std::cerr << "put failed: "
              << stored_language.error().code_string()
              << ": "
              << stored_language.error().message()
              << "\n";

    return 1;
  }

  const auto name =
      client.get("profile/name");

  if (name.is_err())
  {
    std::cerr << "get failed: "
              << name.error().code_string()
              << ": "
              << name.error().message()
              << "\n";

    return 1;
  }

  const auto language =
      client.get("profile/language");

  if (language.is_err())
  {
    std::cerr << "get failed: "
              << language.error().code_string()
              << ": "
              << language.error().message()
              << "\n";

    return 1;
  }

  std::cout << "profile/name     : "
            << name.value().to_string()
            << "\n";

  std::cout << "profile/language : "
            << language.value().to_string()
            << "\n";

  std::cout << "entries          : "
            << client.size()
            << "\n";

  client.close();

  return 0;
}
