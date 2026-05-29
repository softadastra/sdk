/**
 *
 *  @file Result.hpp
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

#ifndef SOFTADASTRA_SDK_RESULT_HPP
#define SOFTADASTRA_SDK_RESULT_HPP

#include <softadastra/core/types/Result.hpp>

namespace softadastra::sdk
{
  /**
   * @brief Public SDK result type.
   *
   * Result<T, E> represents either a successful value of type T or an error of
   * type E.
   *
   * The SDK reuses the Softadastra core Result implementation instead of
   * duplicating the same error-handling primitive.
   *
   * @tparam T Success value type.
   * @tparam E Error value type.
   */
  template <typename T, typename E>
  using Result = softadastra::core::types::Result<T, E>;

} // namespace softadastra::sdk

#endif // SOFTADASTRA_SDK_RESULT_HPP
