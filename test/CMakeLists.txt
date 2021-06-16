cmake_minimum_required(VERSION 3.6)

project(vmm_test)

include_directories(
    "${ROOT_PATH}/include"
    "${PROJECT_SOURCE_DIR}"
)


aux_source_directory(${PROJECT_SOURCE_DIR} SRCS)


enable_testing()

set(CMAKE_CXX_STANDARD 11)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)



add_executable(vmm_test ${SRCS} )
target_link_libraries(vmm_test gtest_main vmm_shared)

include(GoogleTest)
gtest_discover_tests(vmm_test)


