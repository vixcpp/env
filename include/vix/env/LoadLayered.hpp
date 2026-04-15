/**
 *
 *  @file LoadLayered.hpp
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
#ifndef VIX_ENV_LOADLAYERED_HPP
#define VIX_ENV_LOADLAYERED_HPP

#include <vector>

#include <vix/env/EnvFile.hpp>
#include <vix/env/EnvFileOptions.hpp>
#include <vix/error/Result.hpp>

namespace vix::env
{

  /**
   * @brief Result type for loading multiple .env files.
   */
  using EnvFileListResult = vix::error::Result<std::vector<EnvFile>>;

  /**
   * @brief Resolve and load layered .env files according to options.
   *
   * Depending on options, this may load:
   * - .env
   * - .env.local
   * - .env.production
   * - .env.production.local
   *
   * Missing files may be ignored if ignore_missing_files is enabled.
   */
  [[nodiscard]] EnvFileListResult load_layered(
      const EnvFileOptions &options = {});

} // namespace vix::env

#endif // VIX_ENV_LOADLAYERED_HPP
