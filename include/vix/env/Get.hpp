/**
 *
 *  @file Get.hpp
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
#ifndef VIX_ENV_GET_HPP
#define VIX_ENV_GET_HPP

#include <string_view>

#include <vix/env/EnvResult.hpp>

namespace vix::env
{

  /**
   * @brief Read an environment variable as a string.
   *
   * Returns an error if the key is invalid or the variable is missing.
   */
  [[nodiscard]] EnvStringResult get(std::string_view key);

} // namespace vix::env

#endif // VIX_ENV_GET_HPP
