/**
 *
 *  @file 08_metadata.cpp
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

namespace
{
  void print_node_info(const softadastra::sdk::NodeInfo &info)
  {
    std::cout << "node_id      : "
              << info.node_id()
              << "\n";

    std::cout << "display_name : "
              << info.display_name()
              << "\n";

    std::cout << "label        : "
              << info.label()
              << "\n";

    std::cout << "hostname     : "
              << info.hostname()
              << "\n";

    std::cout << "os_name      : "
              << info.os_name()
              << "\n";

    std::cout << "version      : "
              << info.version()
              << "\n";

    std::cout << "started_at   : "
              << info.started_at_ms()
              << " ms\n";

    std::cout << "uptime       : "
              << info.uptime_ms()
              << " ms\n";

    std::cout << "capabilities : "
              << info.capabilities().size()
              << "\n";

    for (const auto &capability : info.capabilities())
    {
      std::cout << "  - "
                << capability
                << "\n";
    }
  }
}

int main()
{
  using namespace softadastra::sdk;

  ClientOptions options =
      ClientOptions::memory_only("example-metadata")
          .with_metadata("Example Metadata Node", "0.1.0");

  Client client{options};

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

  const auto info =
      client.refresh_node_info();

  if (info.is_err())
  {
    std::cerr << "refresh_node_info failed: "
              << info.error().code_string()
              << ": "
              << info.error().message()
              << "\n";

    client.close();

    return 1;
  }

  print_node_info(info.value());

  client.close();

  return 0;
}
