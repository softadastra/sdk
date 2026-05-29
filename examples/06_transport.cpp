/**
 *
 *  @file 06_transport.cpp
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

  ClientOptions options =
      ClientOptions::memory_only("example-transport")
          .with_local_transport(9100);

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
      client.start_transport();

  if (started.is_err())
  {
    std::cerr << "start_transport failed: "
              << started.error().code_string()
              << ": "
              << started.error().message()
              << "\n";

    return 1;
  }

  std::cout << "transport_running : "
            << (client.transport_running() ? "yes" : "no")
            << "\n";

  const Peer peer =
      Peer::local("example-peer", 9101);

  const auto connected =
      client.connect(peer);

  if (connected.is_err())
  {
    std::cerr << "connect failed: "
              << connected.error().code_string()
              << ": "
              << connected.error().message();

    if (connected.error().has_context())
    {
      std::cerr << " ("
                << connected.error().context()
                << ")";
    }

    std::cerr << "\n";
  }
  else
  {
    std::cout << "connected peer    : "
              << peer.node_id()
              << " "
              << peer.host()
              << ":"
              << peer.port()
              << "\n";
  }

  client.stop_transport();

  std::cout << "transport_running : "
            << (client.transport_running() ? "yes" : "no")
            << "\n";

  client.close();

  return 0;
}
