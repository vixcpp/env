/**
 * @file Legacy.cpp
 * @brief Vix2 environment-helper compatibility implementation.
 */

#include <string>

#include <vix/env/Legacy.hpp>
#include <vix/env/detail/Parse.hpp>
#include <vix/env/detail/Raw.hpp>

namespace vix::env::legacy
{

  const char *getenv(const char *name) noexcept
  {
    const char *value = detail::raw_getenv(name);
#if defined(_WIN32)
    return value != nullptr && value[0] != '\0' ? value : nullptr;
#else
    return value;
#endif
  }

  std::string env_or(std::string_view key, std::string_view fallback)
  {
    const std::string stable_key(key);
    if (const char *value = getenv(stable_key.c_str()))
    {
      return std::string(value);
    }

    return std::string(fallback);
  }

  bool env_bool(std::string_view key, bool fallback)
  {
    const std::string value = env_or(key, fallback ? "1" : "0");
    return detail::parse_bool(value).value_or(false);
  }

  int env_int(std::string_view key, int fallback)
  {
    return detail::parse_int(env_or(key)).value_or(fallback);
  }

  unsigned env_uint(std::string_view key, unsigned fallback)
  {
    return detail::parse_uint(env_or(key)).value_or(fallback);
  }

  double env_double(std::string_view key, double fallback)
  {
    return detail::parse_double(env_or(key)).value_or(fallback);
  }

} // namespace vix::env::legacy
