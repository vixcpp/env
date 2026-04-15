/**
 *
 *  @file Set.hpp
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
#ifndef VIX_ENV_SET_HPP
#define VIX_ENV_SET_HPP

#include <string_view>

#include <vix/error/Error.hpp>

namespace vix::env
{

  /**
   * @brief Set an environment variable in the current process.
   *
   * @param key Environment variable name.
   * @param value Environment variable value.
   * @param overwrite Whether an existing value may be overwritten.
   * @return Structured error. Success is represented by a default-constructed error.
   */
  [[nodiscard]] vix::error::Error set(std::string_view key,
                                      std::string_view value,
                                      bool overwrite = true) noexcept;

} // namespace vix::env

#endif // VIX_ENV_SET_HPP
