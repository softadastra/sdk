# ========================
# sdk-cpp tests
# ========================

find_package(GTest CONFIG QUIET)

if(NOT GTest_FOUND)
    include(FetchContent)

    FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
    )

    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

    FetchContent_MakeAvailable(googletest)
endif()

# ========================
# Test helper
# ========================

function(sdk_cpp_add_test test_name source_file)
    add_executable(${test_name} ${source_file})

    target_link_libraries(${test_name}
        PRIVATE
            sdk-cpp
            GTest::gtest_main
    )

    target_compile_features(${test_name}
        PRIVATE
            cxx_std_20
    )

    add_test(
        NAME ${test_name}
        COMMAND ${test_name}
    )
endfunction()

# ========================
# Unit tests
# ========================

sdk_cpp_add_test(sdk_cpp_error_tests ErrorTests.cpp)
sdk_cpp_add_test(sdk_cpp_key_tests KeyTests.cpp)
sdk_cpp_add_test(sdk_cpp_value_tests ValueTests.cpp)
sdk_cpp_add_test(sdk_cpp_client_options_tests ClientOptionsTests.cpp)

# ========================
# Client tests
# ========================

sdk_cpp_add_test(sdk_cpp_client_lifecycle_tests ClientLifecycleTests.cpp)
sdk_cpp_add_test(sdk_cpp_client_store_tests ClientStoreTests.cpp)
sdk_cpp_add_test(sdk_cpp_client_recovery_tests ClientRecoveryTests.cpp)
sdk_cpp_add_test(sdk_cpp_client_sync_tests ClientSyncTests.cpp)
sdk_cpp_add_test(sdk_cpp_client_transport_tests ClientTransportTests.cpp)
sdk_cpp_add_test(sdk_cpp_client_discovery_tests ClientDiscoveryTests.cpp)
sdk_cpp_add_test(sdk_cpp_client_metadata_tests ClientMetadataTests.cpp)
