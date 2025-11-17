# Unit tests for the ResourcesModule gtlab module 

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

if (NOT TARGET gtest)
    include(AddGoogleTest)
endif()

add_executable(GtResourcesModuleUnitTests
    main.cpp
    test.cpp
)

target_link_libraries(GtResourcesModuleUnitTests PRIVATE GtResourcesModule gtest)

include(GoogleTest)
gtest_discover_tests(GtResourcesModuleUnitTests
                     TEST_PREFIX "GtResourcesModule."
                     DISCOVERY_MODE PRE_TEST)
