/**
 *
 *  @file Get.cpp
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
#include <vix/env/detail/Raw.hpp>

namespace vix::env
{

  namespace
  {
    [[nodiscard]] bool is_valid_env_key(std::string_view key) noexcept
    {
      if (key.empty())
      {
        return false;
      }

      const unsigned char first = static_cast<unsigned char>(key.front());
      if (!(std::isalpha(first) || key.front() == '_'))
      {
        return false;
      }

      for (const char c : key)
      {
        const unsigned char uc = static_cast<unsigned char>(c);
        if (!(std::isalnum(uc) || c == '_'))
        {
          return false;
        }
      }

      return true;
    }
  } // namespace

  EnvStringResult get(std::string_view key)
  {
    if (key.empty())
    {
      return make_env_error(EnvErrorCode::EmptyKey, "environment key cannot be empty");
    }

    if (!is_valid_env_key(key))
    {
      return make_env_error(EnvErrorCode::InvalidKey, "environment key is invalid");
    }

    const std::string stable_key(key);

    const char *value = detail::raw_getenv(stable_key.c_str());
    if (value == nullptr)
    {
      return make_env_error(
          EnvErrorCode::NotFound,
          "environment variable was not found");
    }

    return std::string(value);
  }

} // namespace vix::env
