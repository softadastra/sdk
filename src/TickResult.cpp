/**
 *
 *  @file TickResult.cpp
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

#include <softadastra/sdk/TickResult.hpp>

namespace softadastra::sdk
{
  TickResult::TickResult(
      std::size_t retried_count,
      std::size_t pruned_count,
      std::size_t batch_size)
      : retried_count_(retried_count),
        pruned_count_(pruned_count),
        batch_size_(batch_size)
  {
  }

  std::size_t TickResult::retried_count() const noexcept
  {
    return retried_count_;
  }

  std::size_t TickResult::pruned_count() const noexcept
  {
    return pruned_count_;
  }

  std::size_t TickResult::batch_size() const noexcept
  {
    return batch_size_;
  }

  void TickResult::set_retried_count(std::size_t value) noexcept
  {
    retried_count_ = value;
  }

  void TickResult::set_pruned_count(std::size_t value) noexcept
  {
    pruned_count_ = value;
  }

  void TickResult::set_batch_size(std::size_t value) noexcept
  {
    batch_size_ = value;
  }

  bool TickResult::has_work() const noexcept
  {
    return retried_count_ > 0 ||
           pruned_count_ > 0 ||
           batch_size_ > 0;
  }

  bool TickResult::retried() const noexcept
  {
    return retried_count_ > 0;
  }

  bool TickResult::pruned() const noexcept
  {
    return pruned_count_ > 0;
  }

  bool TickResult::produced_batch() const noexcept
  {
    return batch_size_ > 0;
  }

  bool TickResult::empty() const noexcept
  {
    return retried_count_ == 0 &&
           pruned_count_ == 0 &&
           batch_size_ == 0;
  }

  void TickResult::clear() noexcept
  {
    retried_count_ = 0;
    pruned_count_ = 0;
    batch_size_ = 0;
  }

} // namespace softadastra::sdk
