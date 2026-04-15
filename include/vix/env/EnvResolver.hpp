/**
 *
 *  @file EnvResolver.hpp
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
#ifndef VIX_ENV_ENVRESOLVER_HPP
#define VIX_ENV_ENVRESOLVER_HPP

#include <vector>

#include <vix/env/EnvFileOptions.hpp>
#include <vix/error/Result.hpp>

namespace vix::env
{

  /**
   * @brief Standard result type for resolved .env file paths.
   */
  using EnvPathListResult = vix::error::Result<std::vector<std::string>>;

  /**
   * @brief Resolve the primary .env file path according to options.
   *
   * In single-file mode, this resolves the main env file path using:
   * - base_dir
   * - filename
   * - path normalization rules
   *
   * Example:
   * - base_dir = "."
   * - filename = ".env"
   *   -> "./.env" normalized
   */
  [[nodiscard]] vix::error::Result<std::string> resolve_env_file(
      const EnvFileOptions &options = {});

  /**
   * @brief Resolve all candidate .env file paths according to layered options.
   *
   * Depending on the flags, this may include:
   * - .env
   * - .env.local
   * - .env.production
   * - .env.production.local
   *
   * Returned paths are normalized and ordered in loading order.
   */
  [[nodiscard]] EnvPathListResult resolve_env_files(
      const EnvFileOptions &options = {});

} // namespace vix::env

#endif // VIX_ENV_ENVRESOLVER_HPP
