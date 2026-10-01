/**
 *
 *  @file Has.cpp
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

#include <string>

#include <vix/env/Has.hpp>
#include <vix/env/detail/Raw.hpp>

namespace vix::env
{

  bool has(std::string_view key) noexcept
  {
    if (key.empty())
    {
      return false;
    }

    const std::string stable_key(key);
    return detail::raw_getenv(stable_key.c_str()) != nullptr;
  }

} // namespace vix::env
