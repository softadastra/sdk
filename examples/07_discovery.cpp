/**
 *
 *  @file 07_discovery.cpp
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
  void print_peer(const softadastra::sdk::Peer &peer)
  {
    std::cout << "  - node_id : "
              << peer.node_id()
              << "\n";

    std::cout << "    host    : "
              << peer.host()
              << "\n";

    std::cout << "    port    : "
              << peer.port()
              << "\n";
  }
}

int main()
{
  using namespace softadastra::sdk;

  ClientOptions options =
      ClientOptions::memory_only("example-discovery")
          .with_local_discovery(5051);

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

  const auto started =
      client.start_discovery();

  if (started.is_err())
  {
    std::cerr << "start_discovery failed: "
              << started.error().code_string()
              << ": "
              << started.error().message()
              << "\n";

    return 1;
  }

  std::cout << "discovery_running : "
            << (client.discovery_running() ? "yes" : "no")
            << "\n";

  const auto peers =
      client.peers();

  if (peers.is_err())
  {
    std::cerr << "peers failed: "
              << peers.error().code_string()
              << ": "
              << peers.error().message()
              << "\n";

    client.stop_discovery();
    client.close();

    return 1;
  }

  std::cout << "discovered_peers  : "
            << peers.value().size()
            << "\n";

  if (peers.value().empty())
  {
    std::cout << "  no peers discovered\n";
  }
  else
  {
    for (const auto &peer : peers.value())
    {
      print_peer(peer);
    }
  }

  client.stop_discovery();

  std::cout << "discovery_running : "
            << (client.discovery_running() ? "yes" : "no")
            << "\n";

  client.close();

  return 0;
}
