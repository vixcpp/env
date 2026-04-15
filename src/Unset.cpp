/**
 *
 *  @file Unset.cpp
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
#include <cstdlib>
#include <string>
#include <string_view>

#include <vix/env/EnvError.hpp>
#include <vix/env/Unset.hpp>

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

  vix::error::Error unset(std::string_view key) noexcept
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

#if defined(_WIN32)
    if (_putenv_s(stable_key.c_str(), "") != 0)
    {
      return make_env_error(
          EnvErrorCode::UnsupportedOperation,
          "failed to unset environment variable");
    }
#else
    if (::unsetenv(stable_key.c_str()) != 0)
    {
      return make_env_error(
          EnvErrorCode::UnsupportedOperation,
          "failed to unset environment variable");
    }
#endif

    return {};
  }

} // namespace vix::env
