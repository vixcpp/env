/**
 * @file Raw.cpp
 * @brief Host-environment access for vix::env.
 */

#include <cstdlib>
#include <string>

#include <vix/env/detail/Raw.hpp>

namespace vix::env::detail
{

  const char *raw_getenv(const char *key) noexcept
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

} // namespace vix::env::detail
