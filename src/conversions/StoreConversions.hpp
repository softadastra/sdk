/**
 *
 *  @file StoreConversions.hpp
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

#ifndef SOFTADASTRA_SDK_STORE_CONVERSIONS_HPP
#define SOFTADASTRA_SDK_STORE_CONVERSIONS_HPP

#include <softadastra/sdk/Key.hpp>
#include <softadastra/sdk/Value.hpp>

#include <softadastra/store/types/Key.hpp>
#include <softadastra/store/types/Value.hpp>

namespace softadastra::sdk::internal
{
  /**
   * @brief Converts a public SDK key to an internal store key.
   *
   * @param key Public SDK key.
   * @return Internal store key.
   */
  [[nodiscard]] softadastra::store::types::Key to_store_key(
      const Key &key);

  /**
   * @brief Converts an internal store key to a public SDK key.
   *
   * @param key Internal store key.
   * @return Public SDK key.
   */
  [[nodiscard]] Key from_store_key(
      const softadastra::store::types::Key &key);

  /**
   * @brief Converts a public SDK value to an internal store value.
   *
   * @param value Public SDK value.
   * @return Internal store value.
   */
  [[nodiscard]] softadastra::store::types::Value to_store_value(
      const Value &value);

  /**
   * @brief Converts an internal store value to a public SDK value.
   *
   * @param value Internal store value.
   * @return Public SDK value.
   */
  [[nodiscard]] Value from_store_value(
      const softadastra::store::types::Value &value);

} // namespace softadastra::sdk::internal

#endif // SOFTADASTRA_SDK_STORE_CONVERSIONS_HPP
