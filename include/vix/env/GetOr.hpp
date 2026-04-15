/**
 *
 *  @file GetOr.hpp
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
#ifndef VIX_ENV_GETOR_HPP
#define VIX_ENV_GETOR_HPP

#include <string>
#include <string_view>

namespace vix::env
{

  /**
   * @brief Read an environment variable or return a default value.
   *
   * Invalid or missing keys return the provided default.
   */
  [[nodiscard]] std::string get_or(std::string_view key,
                                   std::string_view default_value = "");

} // namespace vix::env

#endif // VIX_ENV_GETOR_HPP
