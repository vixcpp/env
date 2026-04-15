/**
 *
 *  @file GetUint.hpp
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
#ifndef VIX_ENV_GETUINT_HPP
#define VIX_ENV_GETUINT_HPP

#include <string_view>

#include <vix/env/EnvResult.hpp>

namespace vix::env
{

  /**
   * @brief Read an environment variable as an unsigned integer.
   *
   * Parsing is strict:
   * - leading/trailing whitespace is ignored
   * - remaining content must be a valid base-10 unsigned integer
   */
  [[nodiscard]] EnvUintResult get_uint(std::string_view key);

} // namespace vix::env

#endif // VIX_ENV_GETUINT_HPP
