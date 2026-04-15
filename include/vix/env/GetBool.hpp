/**
 *
 *  @file GetBool.hpp
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
#ifndef VIX_ENV_GETBOOL_HPP
#define VIX_ENV_GETBOOL_HPP

#include <string_view>

#include <vix/env/EnvResult.hpp>

namespace vix::env
{

  /**
   * @brief Read an environment variable as a boolean.
   *
   * Recognized truthy values:
   * - 1
   * - true
   * - yes
   * - on
   *
   * Recognized falsy values:
   * - 0
   * - false
   * - no
   * - off
   *
   * Comparison is case-insensitive.
   */
  [[nodiscard]] EnvBoolResult get_bool(std::string_view key);

} // namespace vix::env

#endif // VIX_ENV_GETBOOL_HPP
