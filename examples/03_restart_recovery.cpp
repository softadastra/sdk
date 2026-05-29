/**
 *
 *  @file 03_restart_recovery.cpp
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
#include <string>

int main()
{
  using namespace softadastra::sdk;

  const std::string wal_path =
      "data/examples/restart-recovery.wal";

  {
    Client writer{
        ClientOptions::persistent(
            "example-recovery-writer",
            wal_path)};

    const auto opened = writer.open();

    if (opened.is_err())
    {
      std::cerr << "writer open failed: "
                << opened.error().code_string()
                << ": "
                << opened.error().message()
                << "\n";

      return 1;
    }

    const auto stored =
        writer.put("session/status", "stored-before-restart");

    if (stored.is_err())
    {
      std::cerr << "writer put failed: "
                << stored.error().code_string()
                << ": "
                << stored.error().message()
                << "\n";

      return 1;
    }

    std::cout << "writer stored session/status\n";

    writer.close();
  }

  {
    Client reader{
        ClientOptions::persistent(
            "example-recovery-reader",
            wal_path)};

    const auto opened = reader.open();

    if (opened.is_err())
    {
      std::cerr << "reader open failed: "
                << opened.error().code_string()
                << ": "
                << opened.error().message()
                << "\n";

      return 1;
    }

    const auto value =
        reader.get("session/status");

    if (value.is_err())
    {
      std::cerr << "reader get failed: "
                << value.error().code_string()
                << ": "
                << value.error().message()
                << "\n";

      return 1;
    }

    std::cout << "recovered session/status : "
              << value.value().to_string()
              << "\n";

    reader.close();
  }

  return 0;
}
