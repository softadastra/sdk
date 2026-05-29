/**
 *
 *  @file sdk.hpp
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

#ifndef SOFTADASTRA_SDK_HPP
#define SOFTADASTRA_SDK_HPP

#include <softadastra/sdk/Version.hpp>

#include <softadastra/sdk/Error.hpp>
#include <softadastra/sdk/Result.hpp>

#include <softadastra/sdk/Key.hpp>
#include <softadastra/sdk/Value.hpp>
#include <softadastra/sdk/Peer.hpp>
#include <softadastra/sdk/NodeInfo.hpp>

#include <softadastra/sdk/SyncState.hpp>
#include <softadastra/sdk/TickResult.hpp>

#include <softadastra/sdk/ClientOptions.hpp>
#include <softadastra/sdk/Client.hpp>

/**
 * @brief Main namespace for the Softadastra C++ SDK.
 *
 * The Softadastra C++ SDK exposes a small, stable, developer-facing API over
 * the Softadastra local-first runtime foundation.
 *
 * Public API:
 * - softadastra::sdk::Client
 * - softadastra::sdk::ClientOptions
 * - softadastra::sdk::Error
 * - softadastra::sdk::Result
 * - softadastra::sdk::Key
 * - softadastra::sdk::Value
 * - softadastra::sdk::Peer
 * - softadastra::sdk::NodeInfo
 * - softadastra::sdk::SyncState
 * - softadastra::sdk::TickResult
 *
 * Internal modules such as store, WAL, sync, transport, discovery, and
 * metadata remain hidden behind the SDK facade.
 */
namespace softadastra::sdk
{
}

#endif // SOFTADASTRA_SDK_HPP
