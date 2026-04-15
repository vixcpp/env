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

#include <charconv>
#include <cctype>
#include <string>
#include <string_view>
#include <system_error>

#include <vix/env/EnvError.hpp>
#include <vix/env/Get.hpp>
#include <vix/env/GetInt.hpp>

namespace vix::env
{

  namespace
  {
    [[nodiscard]] std::string_view trim(std::string_view value) noexcept
    {
      std::size_t begin = 0;
      std::size_t end = value.size();

      while (begin < end &&
             std::isspace(static_cast<unsigned char>(value[begin])))
      {
        ++begin;
      }

      while (end > begin &&
             std::isspace(static_cast<unsigned char>(value[end - 1])))
      {
        --end;
      }

      return value.substr(begin, end - begin);
    }
  } // namespace

  EnvIntResult get_int(std::string_view key)
  {
    auto result = get(key);
    if (!result)
    {
      return result.error();
    }

    const std::string_view value = trim(result.value());
    if (value.empty())
    {
      return make_env_error(
          EnvErrorCode::InvalidValue,
          "environment value cannot be parsed as int");
    }

    int parsed = 0;
    const auto [ptr, ec] = std::from_chars(
        value.data(),
        value.data() + value.size(),
        parsed,
        10);

    if (ec != std::errc{} || ptr != value.data() + value.size())
    {
      return make_env_error(
          EnvErrorCode::InvalidValue,
          "environment value cannot be parsed as int");
    }

    return parsed;
  }

} // namespace vix::env
