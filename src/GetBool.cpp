/**
 *
 *  @file GetBool.cpp
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

#include <cctype>
#include <string>
#include <string_view>

#include <vix/env/EnvError.hpp>
#include <vix/env/Get.hpp>
#include <vix/env/GetBool.hpp>

namespace vix::env
{

  namespace
  {
    [[nodiscard]] char to_lower_ascii(unsigned char c) noexcept
    {
      return static_cast<char>(std::tolower(c));
    }

    [[nodiscard]] bool iequals(std::string_view left, std::string_view right) noexcept
    {
      if (left.size() != right.size())
      {
        return false;
      }

      for (std::size_t i = 0; i < left.size(); ++i)
      {
        if (to_lower_ascii(static_cast<unsigned char>(left[i])) !=
            to_lower_ascii(static_cast<unsigned char>(right[i])))
        {
          return false;
        }
      }

      return true;
    }

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

  EnvBoolResult get_bool(std::string_view key)
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
          "environment value cannot be parsed as bool");
    }

    if (value == "1" ||
        iequals(value, "true") ||
        iequals(value, "yes") ||
        iequals(value, "on"))
    {
      return true;
    }

    if (value == "0" ||
        iequals(value, "false") ||
        iequals(value, "no") ||
        iequals(value, "off"))
    {
      return false;
    }

    return make_env_error(
        EnvErrorCode::InvalidValue,
        "environment value cannot be parsed as bool");
  }

} // namespace vix::env
