/**
 *
 *  @file SyncState.hpp
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

#ifndef SOFTADASTRA_SDK_SYNC_STATE_HPP
#define SOFTADASTRA_SDK_SYNC_STATE_HPP

#include <cstddef>
#include <cstdint>

namespace softadastra::sdk
{
  /**
   * @brief Public SDK synchronization state.
   *
   * SyncState exposes a stable developer-facing snapshot of the SDK
   * synchronization pipeline.
   *
   * It intentionally contains only counters and version markers that are useful
   * to SDK users. Internal sync engine objects, queues, envelopes, and retry
   * records remain hidden inside the SDK implementation.
   */
  class SyncState
  {
  public:
    /**
     * @brief Creates an empty synchronization state.
     */
    SyncState() = default;

    /**
     * @brief Creates a synchronization state from explicit counters.
     *
     * @param outbox_size Number of operations stored in the outbox.
     * @param queued_count Number of operations waiting to be sent.
     * @param in_flight_count Number of operations currently in flight.
     * @param acknowledged_count Number of acknowledged operations.
     * @param failed_count Number of failed operations.
     */
    SyncState(
        std::size_t outbox_size,
        std::size_t queued_count,
        std::size_t in_flight_count,
        std::size_t acknowledged_count,
        std::size_t failed_count);

    /**
     * @brief Returns the number of operations stored in the outbox.
     *
     * @return Outbox size.
     */
    [[nodiscard]] std::size_t outbox_size() const noexcept;

    /**
     * @brief Returns the number of operations waiting to be sent.
     *
     * @return Queued operation count.
     */
    [[nodiscard]] std::size_t queued_count() const noexcept;

    /**
     * @brief Returns the number of operations currently in flight.
     *
     * @return In-flight operation count.
     */
    [[nodiscard]] std::size_t in_flight_count() const noexcept;

    /**
     * @brief Returns the number of acknowledged operations.
     *
     * @return Acknowledged operation count.
     */
    [[nodiscard]] std::size_t acknowledged_count() const noexcept;

    /**
     * @brief Returns the number of failed operations.
     *
     * @return Failed operation count.
     */
    [[nodiscard]] std::size_t failed_count() const noexcept;

    /**
     * @brief Returns the last local version submitted to the sync pipeline.
     *
     * @return Last submitted local version.
     */
    [[nodiscard]] std::uint64_t last_submitted_version() const noexcept;

    /**
     * @brief Returns the last remote version applied locally.
     *
     * @return Last applied remote version.
     */
    [[nodiscard]] std::uint64_t last_applied_remote_version() const noexcept;

    /**
     * @brief Returns the total number of retry attempts performed by sync.
     *
     * @return Total retry count.
     */
    [[nodiscard]] std::uint64_t total_retries() const noexcept;

    /**
     * @brief Sets the number of operations stored in the outbox.
     *
     * @param value Outbox size.
     */
    void set_outbox_size(std::size_t value) noexcept;

    /**
     * @brief Sets the number of operations waiting to be sent.
     *
     * @param value Queued operation count.
     */
    void set_queued_count(std::size_t value) noexcept;

    /**
     * @brief Sets the number of operations currently in flight.
     *
     * @param value In-flight operation count.
     */
    void set_in_flight_count(std::size_t value) noexcept;

    /**
     * @brief Sets the number of acknowledged operations.
     *
     * @param value Acknowledged operation count.
     */
    void set_acknowledged_count(std::size_t value) noexcept;

    /**
     * @brief Sets the number of failed operations.
     *
     * @param value Failed operation count.
     */
    void set_failed_count(std::size_t value) noexcept;

    /**
     * @brief Sets the last local version submitted to the sync pipeline.
     *
     * @param value Last submitted local version.
     */
    void set_last_submitted_version(std::uint64_t value) noexcept;

    /**
     * @brief Sets the last remote version applied locally.
     *
     * @param value Last applied remote version.
     */
    void set_last_applied_remote_version(std::uint64_t value) noexcept;

    /**
     * @brief Sets the total number of retry attempts performed by sync.
     *
     * @param value Total retry count.
     */
    void set_total_retries(std::uint64_t value) noexcept;

    /**
     * @brief Returns true if there are queued operations.
     *
     * @return true if queued_count is greater than zero.
     */
    [[nodiscard]] bool has_queued() const noexcept;

    /**
     * @brief Returns true if there are in-flight operations.
     *
     * @return true if in_flight_count is greater than zero.
     */
    [[nodiscard]] bool has_in_flight() const noexcept;

    /**
     * @brief Returns true if there are failed operations.
     *
     * @return true if failed_count is greater than zero.
     */
    [[nodiscard]] bool has_failed() const noexcept;

    /**
     * @brief Returns true if the synchronization pipeline has pending work.
     *
     * Pending work means queued, in-flight, or failed operations are present.
     *
     * @return true if sync work exists.
     */
    [[nodiscard]] bool has_work() const noexcept;

    /**
     * @brief Returns true if the synchronization state is empty.
     *
     * @return true if all counters and version markers are zero.
     */
    [[nodiscard]] bool empty() const noexcept;

    /**
     * @brief Returns the number of tracked operation states.
     *
     * This is the sum of queued, in-flight, acknowledged, and failed
     * operations.
     *
     * @return Total tracked operation count.
     */
    [[nodiscard]] std::size_t total_tracked() const noexcept;

    /**
     * @brief Clears all counters and version markers.
     */
    void clear() noexcept;

    /**
     * @brief Compares two sync states for equality.
     *
     * @param left Left sync state.
     * @param right Right sync state.
     * @return true if both states contain the same counters and versions.
     */
    [[nodiscard]] friend bool operator==(
        const SyncState &left,
        const SyncState &right) noexcept
    {
      return left.outbox_size_ == right.outbox_size_ &&
             left.queued_count_ == right.queued_count_ &&
             left.in_flight_count_ == right.in_flight_count_ &&
             left.acknowledged_count_ == right.acknowledged_count_ &&
             left.failed_count_ == right.failed_count_ &&
             left.last_submitted_version_ == right.last_submitted_version_ &&
             left.last_applied_remote_version_ ==
                 right.last_applied_remote_version_ &&
             left.total_retries_ == right.total_retries_;
    }

    /**
     * @brief Compares two sync states for inequality.
     *
     * @param left Left sync state.
     * @param right Right sync state.
     * @return true if the states are different.
     */
    [[nodiscard]] friend bool operator!=(
        const SyncState &left,
        const SyncState &right) noexcept
    {
      return !(left == right);
    }

  private:
    std::size_t outbox_size_{0};
    std::size_t queued_count_{0};
    std::size_t in_flight_count_{0};
    std::size_t acknowledged_count_{0};
    std::size_t failed_count_{0};
    std::uint64_t last_submitted_version_{0};
    std::uint64_t last_applied_remote_version_{0};
    std::uint64_t total_retries_{0};
  };

} // namespace softadastra::sdk

#endif // SOFTADASTRA_SDK_SYNC_STATE_HPP
