/**
 *
 *  @file GetOr.cpp
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

#include <vix/env/Get.hpp>
#include <vix/env/GetOr.hpp>

namespace vix::env
{

  std::string get_or(std::string_view key, std::string_view default_value)
  {
    auto result = get(key);
    if (!result)
    {
      return std::string(default_value);
    }

    return result.value();
  }

} // namespace vix::env
