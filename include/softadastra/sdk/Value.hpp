/**
 *
 *  @file Value.hpp
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

#ifndef SOFTADASTRA_SDK_VALUE_HPP
#define SOFTADASTRA_SDK_VALUE_HPP

#include <cstddef>
#include <cstdint>
#include <span>
#include <string>
#include <string_view>
#include <vector>

namespace softadastra::sdk
{
  /**
   * @brief Public SDK value type.
   *
   * Value represents binary-safe data stored through the Softadastra SDK
   * client.
   *
   * The SDK does not interpret the content of a value. Text, JSON, binary
   * payloads, metadata, and application-specific formats are all stored as raw
   * bytes.
   *
   * Empty values are valid.
   */
  class Value
  {
  public:
    /**
     * @brief Byte container used by the SDK value.
     */
    using Container = std::vector<std::uint8_t>;

    /**
     * @brief Creates an empty value.
     */
    Value() = default;

    /**
     * @brief Creates a value from a byte container.
     *
     * @param bytes Value bytes.
     */
    explicit Value(Container bytes);

    /**
     * @brief Creates a value from a byte span.
     *
     * @param bytes Value bytes.
     */
    explicit Value(std::span<const std::uint8_t> bytes);

    /**
     * @brief Creates a value from a string view.
     *
     * The string content is copied as raw bytes.
     *
     * @param text Text payload.
     */
    explicit Value(std::string_view text);

    /**
     * @brief Creates a value from a C string.
     *
     * A null pointer is converted to an empty value.
     *
     * @param text Text payload.
     */
    explicit Value(const char *text);

    /**
     * @brief Creates a value from a byte container.
     *
     * @param bytes Value bytes.
     * @return SDK value.
     */
    [[nodiscard]] static Value from_bytes(Container bytes);

    /**
     * @brief Creates a value from a byte span.
     *
     * @param bytes Value bytes.
     * @return SDK value.
     */
    [[nodiscard]] static Value from_span(
        std::span<const std::uint8_t> bytes);

    /**
     * @brief Creates a value from text.
     *
     * The text is copied as raw bytes. No UTF-8 validation is performed.
     *
     * @param text Text payload.
     * @return SDK value.
     */
    [[nodiscard]] static Value from_string(std::string_view text);

    /**
     * @brief Returns the stored bytes.
     *
     * @return Read-only byte container.
     */
    [[nodiscard]] const Container &bytes() const noexcept;

    /**
     * @brief Returns the stored bytes.
     *
     * Alias for bytes().
     *
     * @return Read-only byte container.
     */
    [[nodiscard]] const Container &data() const noexcept;

    /**
     * @brief Returns the stored bytes as a mutable container.
     *
     * @return Mutable byte container.
     */
    [[nodiscard]] Container &data() noexcept;

    /**
     * @brief Returns a read-only byte span over the stored data.
     *
     * @return Read-only byte span.
     */
    [[nodiscard]] std::span<const std::uint8_t> span() const noexcept;

    /**
     * @brief Returns true if the value has no bytes.
     *
     * @return true if the value is empty.
     */
    [[nodiscard]] bool empty() const noexcept;

    /**
     * @brief Returns the number of stored bytes.
     *
     * @return Value size in bytes.
     */
    [[nodiscard]] std::size_t size() const noexcept;

    /**
     * @brief Clears the value.
     */
    void clear() noexcept;

    /**
     * @brief Converts the stored bytes to a string.
     *
     * This function does not validate UTF-8. It simply builds a std::string
     * from the stored bytes.
     *
     * @return String created from the stored bytes.
     */
    [[nodiscard]] std::string to_string() const;

    /**
     * @brief Compares two values for equality.
     *
     * @param left Left value.
     * @param right Right value.
     * @return true if both values contain the same bytes.
     */
    [[nodiscard]] friend bool operator==(
        const Value &left,
        const Value &right)
    {
      return left.data_ == right.data_;
    }

    /**
     * @brief Compares two values for inequality.
     *
     * @param left Left value.
     * @param right Right value.
     * @return true if values are different.
     */
    [[nodiscard]] friend bool operator!=(
        const Value &left,
        const Value &right)
    {
      return !(left == right);
    }

  private:
    Container data_{};
  };

} // namespace softadastra::sdk

#endif // SOFTADASTRA_SDK_VALUE_HPP
