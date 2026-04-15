/**
 *
 *  @file EnvFile.hpp
 *  @author Gaspard Kirira
 *
 *  Copyright 2025, Gaspard Kirira.
 *  All rights reserved.
 *  https://github.com/vixcpp/vix
 *
 *  Use of this source code is governed by a MIT license
 *  that can be found in the License file.
 *
 *  Vix.cpp
 */
#ifndef VIX_ENV_ENVFILE_HPP
#define VIX_ENV_ENVFILE_HPP

#include <string>

#include <vix/env/EnvFileOptions.hpp>
#include <vix/env/EnvResult.hpp>

namespace vix::env
{

  /**
   * @struct EnvFile
   * @brief In-memory representation of a parsed .env file.
   *
   * This structure keeps:
   * - the original file path
   * - the parsed ordered entries
   * - a convenient key-value map
   */
  struct EnvFile
  {
    /**
     * @brief Original file path used to load this file.
     */
    std::string path;

    /**
     * @brief Ordered parsed entries.
     */
    EnvEntryList entries;

    /**
     * @brief Parsed key-value map.
     */
    EnvMap values;
  };

  /**
   * @brief Standard result type for env file operations.
   */
  using EnvFileResult = vix::error::Result<EnvFile>;

} // namespace vix::env

#endif // VIX_ENV_ENVFILE_HPP
