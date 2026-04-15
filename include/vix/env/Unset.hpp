/**
 *
 *  @file Unset.hpp
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
#ifndef VIX_ENV_UNSET_HPP
#define VIX_ENV_UNSET_HPP

#include <string_view>

#include <vix/error/Error.hpp>

namespace vix::env
{

  /**
   * @brief Remove an environment variable from the current process.
   *
   * @param key Environment variable name.
   * @return Structured error. Success is represented by a default-constructed error.
   */
  [[nodiscard]] vix::error::Error unset(std::string_view key) noexcept;

} // namespace vix::env

#endif // VIX_ENV_UNSET_HPP
