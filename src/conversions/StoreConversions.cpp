/**
 *
 *  @file StoreConversions.cpp
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

#include "StoreConversions.hpp"

namespace softadastra::sdk::internal
{
  softadastra::store::types::Key to_store_key(
      const Key &key)
  {
    return softadastra::store::types::Key{key.str()};
  }

  Key from_store_key(
      const softadastra::store::types::Key &key)
  {
    return Key{key.str()};
  }

  softadastra::store::types::Value to_store_value(
      const Value &value)
  {
    return softadastra::store::types::Value::from_bytes(
        value.bytes());
  }

  Value from_store_value(
      const softadastra::store::types::Value &value)
  {
    return Value::from_bytes(value.bytes());
  }

} // namespace softadastra::sdk::internal
