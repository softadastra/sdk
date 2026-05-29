/**
 *
 *  @file 04_sync_state.cpp
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
  void print_sync_state(const softadastra::sdk::SyncState &state)
  {
    std::cout << "outbox_size                 : "
              << state.outbox_size()
              << "\n";

    std::cout << "queued_count                : "
              << state.queued_count()
              << "\n";

    std::cout << "in_flight_count             : "
              << state.in_flight_count()
              << "\n";

    std::cout << "acknowledged_count          : "
              << state.acknowledged_count()
              << "\n";

    std::cout << "failed_count                : "
              << state.failed_count()
              << "\n";

    std::cout << "last_submitted_version      : "
              << state.last_submitted_version()
              << "\n";

    std::cout << "last_applied_remote_version : "
              << state.last_applied_remote_version()
              << "\n";

    std::cout << "total_retries               : "
              << state.total_retries()
              << "\n";
  }
}

int main()
{
  using namespace softadastra::sdk;

  Client client{
      ClientOptions::memory_only("example-sync-state")};

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

  const auto initial_state =
      client.sync_state();

  if (initial_state.is_err())
  {
    std::cerr << "sync_state failed: "
              << initial_state.error().code_string()
              << ": "
              << initial_state.error().message()
              << "\n";

    return 1;
  }

  std::cout << "Initial sync state\n";
  print_sync_state(initial_state.value());

  const auto stored =
      client.put("message", "hello sync");

  if (stored.is_err())
  {
    std::cerr << "put failed: "
              << stored.error().code_string()
              << ": "
              << stored.error().message()
              << "\n";

    return 1;
  }

  const auto updated_state =
      client.sync_state();

  if (updated_state.is_err())
  {
    std::cerr << "sync_state failed: "
              << updated_state.error().code_string()
              << ": "
              << updated_state.error().message()
              << "\n";

    return 1;
  }

  std::cout << "\nAfter local write\n";
  print_sync_state(updated_state.value());

  client.close();

  return 0;
}
