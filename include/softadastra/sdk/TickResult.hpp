/**
 *
 *  @file TickResult.hpp
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

#ifndef SOFTADASTRA_SDK_TICK_RESULT_HPP
#define SOFTADASTRA_SDK_TICK_RESULT_HPP

#include <cstddef>

namespace softadastra::sdk
{
  /**
   * @brief Public SDK result of one synchronization tick.
   *
   * TickResult describes what happened when the SDK manually advanced the
   * synchronization pipeline once.
   *
   * It intentionally exposes only stable counters:
   * - retry work
   * - pruning work
   * - outbound batch production
   *
   * Internal sync envelopes, queues, and scheduler records remain hidden inside
   * the SDK implementation.
   */
  class TickResult
  {
  public:
    /**
     * @brief Creates an empty tick result.
     */
    TickResult() = default;

    /**
     * @brief Creates a tick result from explicit counters.
     *
     * @param retried_count Number of expired operations requeued for retry.
     * @param pruned_count Number of completed or failed entries pruned.
     * @param batch_size Number of outbound sync items produced.
     */
    TickResult(
        std::size_t retried_count,
        std::size_t pruned_count,
        std::size_t batch_size);

    /**
     * @brief Returns the number of expired operations requeued for retry.
     *
     * @return Retried operation count.
     */
    [[nodiscard]] std::size_t retried_count() const noexcept;

    /**
     * @brief Returns the number of completed or failed entries pruned.
     *
     * @return Pruned entry count.
     */
    [[nodiscard]] std::size_t pruned_count() const noexcept;

    /**
     * @brief Returns the number of outbound sync items produced.
     *
     * @return Produced batch size.
     */
    [[nodiscard]] std::size_t batch_size() const noexcept;

    /**
     * @brief Sets the number of expired operations requeued for retry.
     *
     * @param value Retried operation count.
     */
    void set_retried_count(std::size_t value) noexcept;

    /**
     * @brief Sets the number of completed or failed entries pruned.
     *
     * @param value Pruned entry count.
     */
    void set_pruned_count(std::size_t value) noexcept;

    /**
     * @brief Sets the number of outbound sync items produced.
     *
     * @param value Produced batch size.
     */
    void set_batch_size(std::size_t value) noexcept;

    /**
     * @brief Returns true if the tick produced any work.
     *
     * Work means at least one retry, prune, or outbound batch item.
     *
     * @return true if the tick produced work.
     */
    [[nodiscard]] bool has_work() const noexcept;

    /**
     * @brief Returns true if retry work happened.
     *
     * @return true if retried_count is greater than zero.
     */
    [[nodiscard]] bool retried() const noexcept;

    /**
     * @brief Returns true if pruning work happened.
     *
     * @return true if pruned_count is greater than zero.
     */
    [[nodiscard]] bool pruned() const noexcept;

    /**
     * @brief Returns true if outbound sync work was produced.
     *
     * @return true if batch_size is greater than zero.
     */
    [[nodiscard]] bool produced_batch() const noexcept;

    /**
     * @brief Returns true if all counters are zero.
     *
     * @return true if the tick result is empty.
     */
    [[nodiscard]] bool empty() const noexcept;

    /**
     * @brief Clears all counters.
     */
    void clear() noexcept;

    /**
     * @brief Compares two tick results for equality.
     *
     * @param left Left tick result.
     * @param right Right tick result.
     * @return true if both results contain the same counters.
     */
    [[nodiscard]] friend bool operator==(
        const TickResult &left,
        const TickResult &right) noexcept
    {
      return left.retried_count_ == right.retried_count_ &&
             left.pruned_count_ == right.pruned_count_ &&
             left.batch_size_ == right.batch_size_;
    }

    /**
     * @brief Compares two tick results for inequality.
     *
     * @param left Left tick result.
     * @param right Right tick result.
     * @return true if the results are different.
     */
    [[nodiscard]] friend bool operator!=(
        const TickResult &left,
        const TickResult &right) noexcept
    {
      return !(left == right);
    }

  private:
    std::size_t retried_count_{0};
    std::size_t pruned_count_{0};
    std::size_t batch_size_{0};
  };

} // namespace softadastra::sdk

#endif // SOFTADASTRA_SDK_TICK_RESULT_HPP
