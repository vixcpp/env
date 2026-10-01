/**
 * @file Legacy.hpp
 * @brief Historical environment-helper semantics for Vix2 compatibility.
 *
 * New Vix code uses the result-returning vix::env APIs. This narrow surface
 * exists so deprecated Vix2 adapters can preserve their established fallback
 * behavior without owning environment access or parsing themselves.
 */
#ifndef VIX_ENV_LEGACY_HPP
#define VIX_ENV_LEGACY_HPP

#include <string>
#include <string_view>

namespace vix::env::legacy
{

  [[nodiscard]] const char *getenv(const char *name) noexcept;
  [[nodiscard]] std::string env_or(std::string_view key,
                                   std::string_view fallback = "");
  [[nodiscard]] bool env_bool(std::string_view key, bool fallback = false);
  [[nodiscard]] int env_int(std::string_view key, int fallback = 0);
  [[nodiscard]] unsigned env_uint(std::string_view key, unsigned fallback = 0u);
  [[nodiscard]] double env_double(std::string_view key, double fallback = 0.0);

} // namespace vix::env::legacy

#endif // VIX_ENV_LEGACY_HPP
