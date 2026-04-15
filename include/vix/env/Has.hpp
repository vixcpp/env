/**
 *
 *  @file Has.hpp
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
#ifndef VIX_ENV_HAS_HPP
#define VIX_ENV_HAS_HPP

#include <string_view>

namespace vix::env
{

  /**
   * @brief Return true if an environment variable exists.
   *
   * This checks the current process environment.
   */
  [[nodiscard]] bool has(std::string_view key) noexcept;

} // namespace vix::env

#endif // VIX_ENV_HAS_HPP
