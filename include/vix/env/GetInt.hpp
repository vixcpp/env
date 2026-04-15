/**
 *
 *  @file GetInt.hpp
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
#ifndef VIX_ENV_GETINT_HPP
#define VIX_ENV_GETINT_HPP

#include <string_view>

#include <vix/env/EnvResult.hpp>

namespace vix::env
{

  /**
   * @brief Read an environment variable as a signed integer.
   *
   * Parsing is strict:
   * - leading/trailing whitespace is ignored
   * - remaining content must be a valid base-10 integer
   */
  [[nodiscard]] EnvIntResult get_int(std::string_view key);

} // namespace vix::env

#endif // VIX_ENV_GETINT_HPP
