/**
 *
 *  @file Key.hpp
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

#ifndef SOFTADASTRA_SDK_KEY_HPP
#define SOFTADASTRA_SDK_KEY_HPP

#include <string>
#include <string_view>

namespace softadastra::sdk
{
  /**
   * @brief Public SDK key type.
   *
   * Key identifies a value stored through the Softadastra SDK client.
   *
   * A key is intentionally represented as a string because SDK users should not
   * depend on internal store key types.
   *
   * Rules:
   * - a valid key must not be empty
   * - key comparison is value-based
   * - ordering is lexicographical
   */
  class Key
  {
  public:
    /**
     * @brief Creates an empty invalid key.
     */
    Key() = default;

    /**
     * @brief Creates a key from a string.
     *
     * @param value Key value.
     */
    explicit Key(std::string value);

    /**
     * @brief Creates a key from a string view.
     *
     * @param value Key value.
     */
    explicit Key(std::string_view value);

    /**
     * @brief Creates a key from a C string.
     *
     * A null pointer is converted to an empty key.
     *
     * @param value Key value.
     */
    explicit Key(const char *value);

    /**
     * @brief Creates a key from a string.
     *
     * @param value Key value.
     * @return SDK key.
     */
    [[nodiscard]] static Key from(std::string value);

    /**
     * @brief Creates a key from a string view.
     *
     * @param value Key value.
     * @return SDK key.
     */
    [[nodiscard]] static Key from_view(std::string_view value);

    /**
     * @brief Returns the key string.
     *
     * @return Key value.
     */
    [[nodiscard]] const std::string &str() const noexcept;

    /**
     * @brief Returns the key string.
     *
     * Alias for str().
     *
     * @return Key value.
     */
    [[nodiscard]] const std::string &value() const noexcept;

    /**
     * @brief Returns true if the key has no value.
     *
     * @return true if the key is empty.
     */
    [[nodiscard]] bool empty() const noexcept;

    /**
     * @brief Returns true if the key can be used by the SDK.
     *
     * @return true if the key is not empty.
     */
    [[nodiscard]] bool is_valid() const noexcept;

    /**
     * @brief Backward-compatible alias for is_valid().
     *
     * @return true if the key is valid.
     */
    [[nodiscard]] bool valid() const noexcept;

    /**
     * @brief Clears the key.
     */
    void clear() noexcept;

    /**
     * @brief Compares two keys for equality.
     *
     * @param left Left key.
     * @param right Right key.
     * @return true if both keys contain the same value.
     */
    [[nodiscard]] friend bool operator==(
        const Key &left,
        const Key &right) noexcept
    {
      return left.value_ == right.value_;
    }

    /**
     * @brief Compares two keys for inequality.
     *
     * @param left Left key.
     * @param right Right key.
     * @return true if keys are different.
     */
    [[nodiscard]] friend bool operator!=(
        const Key &left,
        const Key &right) noexcept
    {
      return !(left == right);
    }

    /**
     * @brief Orders keys lexicographically.
     *
     * @param left Left key.
     * @param right Right key.
     * @return true if left is lexicographically smaller than right.
     */
    [[nodiscard]] friend bool operator<(
        const Key &left,
        const Key &right) noexcept
    {
      return left.value_ < right.value_;
    }

  private:
    std::string value_{};
  };

} // namespace softadastra::sdk

#endif // SOFTADASTRA_SDK_KEY_HPP
