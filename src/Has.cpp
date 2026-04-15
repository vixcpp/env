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

#include <cstdlib>
#include <string>

#include <vix/env/Has.hpp>

namespace vix::env
{

  namespace
  {
    [[nodiscard]] const char *get_env_raw(const char *key) noexcept
    {
#if defined(_WIN32)
      static thread_local std::string value;
      value.clear();

      char *buffer = nullptr;
      std::size_t length = 0;

      if (_dupenv_s(&buffer, &length, key) != 0 || buffer == nullptr)
      {
        return nullptr;
      }

      value.assign(buffer);
      free(buffer);

      return value.c_str();
#else
      return std::getenv(key);
#endif
    }
  } // namespace

  bool has(std::string_view key) noexcept
  {
    if (key.empty())
    {
      return false;
    }

    const std::string stable_key(key);
    return get_env_raw(stable_key.c_str()) != nullptr;
  }

} // namespace vix::env
