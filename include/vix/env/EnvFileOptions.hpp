/**
 *
 *  @file EnvFileOptions.hpp
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
#ifndef VIX_ENV_ENVFILEOPTIONS_HPP
#define VIX_ENV_ENVFILEOPTIONS_HPP

#include <string>

#include <vix/env/EnvOptions.hpp>
#include <vix/path/PathOptions.hpp>

namespace vix::env
{

  /**
   * @enum EnvFileMode
   * @brief Strategy used when resolving and loading .env files.
   */
  enum class EnvFileMode
  {
    Single,
    Layered
  };

  /**
   * @struct EnvFileOptions
   * @brief Options controlling .env file resolution, parsing, and loading.
   *
   * This structure extends the basic env options with file-oriented behavior.
   */
  struct EnvFileOptions
  {
    /**
     * @brief Generic environment access and parsing options.
     */
    EnvOptions env{};

    /**
     * @brief Path transformation options used when resolving .env file paths.
     */
    vix::path::PathOptions path{};

    /**
     * @brief Base directory from which .env files are resolved.
     *
     * Example:
     * - "."
     * - "./config"
     * - "/app"
     */
    std::string base_dir{"."};

    /**
     * @brief Base env filename.
     *
     * Default is ".env".
     */
    std::string filename{".env"};

    /**
     * @brief Optional environment name used for layered loading.
     *
     * Example:
     * - "development"
     * - "test"
     * - "production"
     *
     * This may be used to build names such as:
     * - .env.production
     * - .env.test
     */
    std::string environment_name{};

    /**
     * @brief Resolution/loading strategy.
     */
    EnvFileMode mode{EnvFileMode::Single};

    /**
     * @brief Whether the base file (for example `.env`) should be loaded.
     */
    bool load_base_file{true};

    /**
     * @brief Whether the local override file (for example `.env.local`) should be loaded.
     */
    bool load_local_file{false};

    /**
     * @brief Whether the environment-specific file
     * (for example `.env.production`) should be loaded.
     */
    bool load_environment_file{false};

    /**
     * @brief Whether the environment-local override file
     * (for example `.env.production.local`) should be loaded.
     */
    bool load_environment_local_file{false};

    /**
     * @brief Whether missing files should be ignored.
     *
     * Useful when loading layered configurations where some files are optional.
     */
    bool ignore_missing_files{true};

    /**
     * @brief Whether parsing should ignore blank lines.
     */
    bool ignore_blank_lines{true};

    /**
     * @brief Whether parsing should ignore comment lines.
     */
    bool ignore_comments{true};

    /**
     * @brief Whether quotes around values should be removed during parsing.
     */
    bool strip_quotes{true};
  };

} // namespace vix::env

#endif // VIX_ENV_ENVFILEOPTIONS_HPP
