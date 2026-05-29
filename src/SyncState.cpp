/**
 *
 *  @file SyncState.cpp
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

#include <softadastra/sdk/SyncState.hpp>

namespace softadastra::sdk
{
  SyncState::SyncState(
      std::size_t outbox_size,
      std::size_t queued_count,
      std::size_t in_flight_count,
      std::size_t acknowledged_count,
      std::size_t failed_count)
      : outbox_size_(outbox_size),
        queued_count_(queued_count),
        in_flight_count_(in_flight_count),
        acknowledged_count_(acknowledged_count),
        failed_count_(failed_count)
  {
  }

  std::size_t SyncState::outbox_size() const noexcept
  {
    return outbox_size_;
  }

  std::size_t SyncState::queued_count() const noexcept
  {
    return queued_count_;
  }

  std::size_t SyncState::in_flight_count() const noexcept
  {
    return in_flight_count_;
  }

  std::size_t SyncState::acknowledged_count() const noexcept
  {
    return acknowledged_count_;
  }

  std::size_t SyncState::failed_count() const noexcept
  {
    return failed_count_;
  }

  std::uint64_t SyncState::last_submitted_version() const noexcept
  {
    return last_submitted_version_;
  }

  std::uint64_t SyncState::last_applied_remote_version() const noexcept
  {
    return last_applied_remote_version_;
  }

  std::uint64_t SyncState::total_retries() const noexcept
  {
    return total_retries_;
  }

  void SyncState::set_outbox_size(std::size_t value) noexcept
  {
    outbox_size_ = value;
  }

  void SyncState::set_queued_count(std::size_t value) noexcept
  {
    queued_count_ = value;
  }

  void SyncState::set_in_flight_count(std::size_t value) noexcept
  {
    in_flight_count_ = value;
  }

  void SyncState::set_acknowledged_count(std::size_t value) noexcept
  {
    acknowledged_count_ = value;
  }

  void SyncState::set_failed_count(std::size_t value) noexcept
  {
    failed_count_ = value;
  }

  void SyncState::set_last_submitted_version(std::uint64_t value) noexcept
  {
    last_submitted_version_ = value;
  }

  void SyncState::set_last_applied_remote_version(
      std::uint64_t value) noexcept
  {
    last_applied_remote_version_ = value;
  }

  void SyncState::set_total_retries(std::uint64_t value) noexcept
  {
    total_retries_ = value;
  }

  bool SyncState::has_queued() const noexcept
  {
    return queued_count_ > 0;
  }

  bool SyncState::has_in_flight() const noexcept
  {
    return in_flight_count_ > 0;
  }

  bool SyncState::has_failed() const noexcept
  {
    return failed_count_ > 0;
  }

  bool SyncState::has_work() const noexcept
  {
    return queued_count_ > 0 ||
           in_flight_count_ > 0 ||
           failed_count_ > 0;
  }

  bool SyncState::empty() const noexcept
  {
    return outbox_size_ == 0 &&
           queued_count_ == 0 &&
           in_flight_count_ == 0 &&
           acknowledged_count_ == 0 &&
           failed_count_ == 0 &&
           last_submitted_version_ == 0 &&
           last_applied_remote_version_ == 0 &&
           total_retries_ == 0;
  }

  std::size_t SyncState::total_tracked() const noexcept
  {
    return queued_count_ +
           in_flight_count_ +
           acknowledged_count_ +
           failed_count_;
  }

  void SyncState::clear() noexcept
  {
    outbox_size_ = 0;
    queued_count_ = 0;
    in_flight_count_ = 0;
    acknowledged_count_ = 0;
    failed_count_ = 0;
    last_submitted_version_ = 0;
    last_applied_remote_version_ = 0;
    total_retries_ = 0;
  }

} // namespace softadastra::sdk
