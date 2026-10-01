/**
 *
 *  @file GetInt.cpp
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

#include <string_view>

#include <vix/env/EnvError.hpp>
#include <vix/env/Get.hpp>
#include <vix/env/GetInt.hpp>
#include <vix/env/detail/Parse.hpp>

namespace vix::env
{

  EnvIntResult get_int(std::string_view key)
  {
    auto result = get(key);
    if (!result)
    {
      return result.error();
    }

    if (const auto parsed = detail::parse_int(result.value()))
    {
      return *parsed;
    }

    return make_env_error(
        EnvErrorCode::InvalidValue,
        "environment value cannot be parsed as int");
  }

} // namespace vix::env
