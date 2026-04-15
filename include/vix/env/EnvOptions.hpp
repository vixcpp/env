/**
 *
 *  @file EnvOptions.hpp
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
#ifndef VIX_ENV_ENVOPTIONS_HPP
#define VIX_ENV_ENVOPTIONS_HPP

namespace vix::env
{

  /**
   * @struct EnvOptions
   * @brief Common options controlling process environment access and parsing.
   *
   * These options are intentionally simple so they can be reused by most
   * env operations in the module.
   */
  struct EnvOptions
  {
    /**
     * @brief Whether leading and trailing whitespace should be trimmed.
     */
    bool trim_whitespace{true};

    /**
     * @brief Whether boolean parsing should be case-insensitive.
     */
    bool case_insensitive_bools{true};

    /**
     * @brief Whether an empty environment value is allowed.
     */
    bool allow_empty_values{true};

    /**
     * @brief Whether an existing process environment variable may be overwritten.
     *
     * This is mainly relevant for set/unset and load-into-process operations.
     */
    bool overwrite_existing{true};

    /**
     * @brief Whether missing environment variables should be treated as errors.
     *
     * This is mainly useful for result-based APIs.
     */
    bool require_existing{false};
  };

} // namespace vix::env

#endif // VIX_ENV_ENVOPTIONS_HPP
