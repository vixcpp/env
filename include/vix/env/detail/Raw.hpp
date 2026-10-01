/**
 * @file Raw.hpp
 * @brief Internal host-environment access shared by vix::env implementations.
 */
#ifndef VIX_ENV_DETAIL_RAW_HPP
#define VIX_ENV_DETAIL_RAW_HPP

namespace vix::env::detail
{

  /**
   * Returns the raw host-environment value for a null-terminated key, or
   * nullptr when the key is absent. On Windows the returned value is retained
   * in thread-local storage because _dupenv_s transfers an allocation.
   */
  [[nodiscard]] const char *raw_getenv(const char *key) noexcept;

} // namespace vix::env::detail

#endif // VIX_ENV_DETAIL_RAW_HPP
