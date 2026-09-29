cmake_minimum_required(VERSION 3.16)
project(ApiServer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

include(FetchContent)

# ---------------------------------------------------------------------------
# Drogon (HTTP framework). Trim it down to just what a DB-less JSON API
# needs: no ORM/DB drivers, no drogon_ctl code generator, no optional
# compression/yaml/dns extras. Keeps the build fast and dependency-light.
# ---------------------------------------------------------------------------
set(BUILD_CTL OFF CACHE BOOL "" FORCE)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_ORM OFF CACHE BOOL "" FORCE)
set(BUILD_YAML_CONFIG OFF CACHE BOOL "" FORCE)
set(BUILD_BROTLI OFF CACHE BOOL "" FORCE)
set(BUILD_C-ARES OFF CACHE BOOL "" FORCE)
set(USE_POSTGRESQL OFF CACHE BOOL "" FORCE)
set(USE_MYSQL OFF CACHE BOOL "" FORCE)
set(USE_SQLITE3 OFF CACHE BOOL "" FORCE)
set(BUILD_TESTING OFF CACHE BOOL "" FORCE) # don't build Drogon's own test suite

FetchContent_Declare(
  drogon
  GIT_REPOSITORY https://github.com/drogonframework/drogon.git
  GIT_TAG v1.9.9
  GIT_SUBMODULES_RECURSE TRUE
)
FetchContent_MakeAvailable(drogon)

# ---------------------------------------------------------------------------
# GoogleTest
# ---------------------------------------------------------------------------
enable_testing()
set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.15.2
)
FetchContent_MakeAvailable(googletest)

add_subdirectory(src)
add_subdirectory(tests)
