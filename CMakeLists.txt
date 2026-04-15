#  @file CMakeLists.txt
#  @author Gaspard Kirira
#
#  Copyright 2025, Gaspard Kirira. All rights reserved.
#  https://github.com/vixcpp/vix
#  Use of this source code is governed by a MIT license
#  that can be found in the License file.
#
# ====================================================================
# Vix.cpp - Env Module
# ====================================================================
# Purpose:
#   Environment variable utilities for Vix.
#   Provides helpers to read, parse, resolve, load, set, and unset
#   environment variables and .env files in a simple cross-platform way.
#
#   Builds as STATIC when sources exist, otherwise as a header-only
#   INTERFACE target.
#
# Public Targets:
#   - vix_env  : The actual library target (STATIC or INTERFACE)
#   - vix::env : Namespaced alias for consumers
#
# Public Dependencies:
#   - vix::error
#   - vix::path
#
# Installation/Export:
#   Installs into the umbrella export-set `VixTargets`.
# ====================================================================

cmake_minimum_required(VERSION 3.20)
project(vix_env VERSION 0.1.0 LANGUAGES CXX)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

# Global settings
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# --------------------------------------------------------------------
# Umbrella build policy:
# When building inside the Vix umbrella, dependencies are managed by the
# umbrella (add_subdirectory order + options). Env must not FetchContent
# nor add sibling subdirectories in umbrella mode.
# --------------------------------------------------------------------
if (DEFINED VIX_UMBRELLA_BUILD AND VIX_UMBRELLA_BUILD)
  set(VIX_ENV_FETCH_ERROR OFF CACHE BOOL "Auto-fetch vix::error if missing" FORCE)
  set(VIX_ENV_FETCH_PATH  OFF CACHE BOOL "Auto-fetch vix::path if missing" FORCE)
endif()

# Sources discovery
file(GLOB_RECURSE ENV_SOURCES CONFIGURE_DEPENDS
  "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

# Dependencies
option(VIX_ENV_FETCH_ERROR "Auto-fetch vix::error if missing" ON)
option(VIX_ENV_FETCH_PATH  "Auto-fetch vix::path if missing" ON)

if (DEFINED VIX_UMBRELLA_BUILD AND VIX_UMBRELLA_BUILD)
  if (NOT TARGET vix::error)
    message(FATAL_ERROR "[env] Umbrella build: vix::error must be provided by umbrella before env.")
  endif()

  if (NOT TARGET vix::path)
    message(FATAL_ERROR "[env] Umbrella build: vix::path must be provided by umbrella before env.")
  endif()
else()
  if (NOT TARGET vix::error)
    if (EXISTS "${CMAKE_CURRENT_LIST_DIR}/../error/CMakeLists.txt")
      message(STATUS "[env] Adding error from umbrella: ../error")
      add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/../error" "error")
    else()
      find_package(vix_error CONFIG QUIET)
    endif()

    if (NOT TARGET vix::error)
      if (VIX_ENV_FETCH_ERROR)
        include(FetchContent)
        message(STATUS "[env] Fetching vix::error via FetchContent")
        FetchContent_Declare(vix_error
          GIT_REPOSITORY https://github.com/vixcpp/error.git
          GIT_TAG        v0.1.0
        )
        FetchContent_MakeAvailable(vix_error)
      else()
        message(FATAL_ERROR
          "vix::error not found. Enable VIX_ENV_FETCH_ERROR=ON, install vix_error, or provide the target before env."
        )
      endif()
    endif()
  endif()

  if (NOT TARGET vix::path)
    if (EXISTS "${CMAKE_CURRENT_LIST_DIR}/../path/CMakeLists.txt")
      message(STATUS "[env] Adding path from umbrella: ../path")
      add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/../path" "path")
    else()
      find_package(vix_path CONFIG QUIET)
    endif()

    if (NOT TARGET vix::path)
      if (VIX_ENV_FETCH_PATH)
        include(FetchContent)
        message(STATUS "[env] Fetching vix::path via FetchContent")
        FetchContent_Declare(vix_path
          GIT_REPOSITORY https://github.com/vixcpp/path.git
          GIT_TAG        v0.1.0
        )
        FetchContent_MakeAvailable(vix_path)
      else()
        message(FATAL_ERROR
          "vix::path not found. Enable VIX_ENV_FETCH_PATH=ON, install vix_path, or provide the target before env."
        )
      endif()
    endif()
  endif()
endif()

set(VIX_ERROR_TARGET vix::error)
set(VIX_PATH_TARGET  vix::path)

# STATIC
if (ENV_SOURCES)
  message(STATUS "[env] Building STATIC library with detected sources.")

  add_library(vix_env STATIC ${ENV_SOURCES})
  add_library(vix::env ALIAS vix_env)

  target_compile_features(vix_env PUBLIC cxx_std_20)

  target_include_directories(vix_env
    PUBLIC
      $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
      $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
  )

  target_link_libraries(vix_env
    PUBLIC
      ${VIX_ERROR_TARGET}
      ${VIX_PATH_TARGET}
  )

  if (TARGET vix_warnings)
    target_link_libraries(vix_env PUBLIC vix_warnings)
  endif()

  if (VIX_ENABLE_SANITIZERS AND TARGET vix_sanitizers)
    target_link_libraries(vix_env PUBLIC vix_sanitizers)
  endif()

  set_target_properties(vix_env PROPERTIES
    OUTPUT_NAME vix_env
    VERSION ${PROJECT_VERSION}
    SOVERSION 0
    EXPORT_NAME env
  )

  install(TARGETS vix_env
    EXPORT VixTargets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
  )

# HEADER-ONLY
else()
  message(STATUS "[env] Building HEADER-ONLY library (no sources).")

  add_library(vix_env INTERFACE)
  add_library(vix::env ALIAS vix_env)

  target_compile_features(vix_env INTERFACE cxx_std_20)

  target_include_directories(vix_env INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
  )

  target_link_libraries(vix_env INTERFACE
    ${VIX_ERROR_TARGET}
    ${VIX_PATH_TARGET}
  )

  if (VIX_ENABLE_SANITIZERS AND TARGET vix_sanitizers)
    target_link_libraries(vix_env INTERFACE vix_sanitizers)
  endif()

  install(TARGETS vix_env
    EXPORT VixTargets
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
  )
endif()

# Install headers
if (NOT (DEFINED VIX_UMBRELLA_BUILD AND VIX_UMBRELLA_BUILD))
  install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
          FILES_MATCHING PATTERN "*.hpp" PATTERN "*.h")
endif()

# Package config
configure_package_config_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/cmake/vix_envConfig.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/vix_envConfig.cmake"
  INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/vix_env"
)

write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/vix_envConfigVersion.cmake"
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY SameMajorVersion
)

install(FILES
  "${CMAKE_CURRENT_BINARY_DIR}/vix_envConfig.cmake"
  "${CMAKE_CURRENT_BINARY_DIR}/vix_envConfigVersion.cmake"
  DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/vix_env"
)

install(EXPORT VixTargets
  FILE VixTargets.cmake
  NAMESPACE vix::
  DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/vix_env"
)

# Tests
option(VIX_ENV_BUILD_TESTS "Build env module tests" OFF)

if (VIX_ENV_BUILD_TESTS)
  include(CTest)
  enable_testing()
  add_subdirectory(test)
endif()

# Summary
message(STATUS "------------------------------------------------------")
message(STATUS "vix::env configured (${PROJECT_VERSION})")
if (ENV_SOURCES)
  message(STATUS "Mode: STATIC / sources found")
else()
  message(STATUS "Mode: HEADER-ONLY / no sources")
endif()
message(STATUS "Error target: ${VIX_ERROR_TARGET}")
message(STATUS "Path target:  ${VIX_PATH_TARGET}")
message(STATUS "Include dir: ${CMAKE_CURRENT_SOURCE_DIR}/include (for <vix/env/...>)")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "------------------------------------------------------")
