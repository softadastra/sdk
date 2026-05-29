/**
 *
 *  @file 05_tick.cpp
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
  void print_tick_result(const softadastra::sdk::TickResult &result)
  {
    std::cout << "retried_count : "
              << result.retried_count()
              << "\n";

    std::cout << "pruned_count  : "
              << result.pruned_count()
              << "\n";

    std::cout << "batch_size    : "
              << result.batch_size()
              << "\n";

    std::cout << "has_work      : "
              << (result.has_work() ? "yes" : "no")
              << "\n";
  }
}

int main()
{
  using namespace softadastra::sdk;

  Client client{
      ClientOptions::memory_only("example-tick")};

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
      client.put("message", "tick example");

  if (stored.is_err())
  {
    std::cerr << "put failed: "
              << stored.error().code_string()
              << ": "
              << stored.error().message()
              << "\n";

    return 1;
  }

  const auto tick =
      client.tick();

  if (tick.is_err())
  {
    std::cerr << "tick failed: "
              << tick.error().code_string()
              << ": "
              << tick.error().message()
              << "\n";

    return 1;
  }

  std::cout << "Tick result\n";
  print_tick_result(tick.value());

  const auto state =
      client.sync_state();

  if (state.is_ok())
  {
    std::cout << "\nSync has work : "
              << (state.value().has_work() ? "yes" : "no")
              << "\n";
  }

  client.close();

  return 0;
}
