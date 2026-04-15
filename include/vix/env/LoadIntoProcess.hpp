/**
 *
 *  @file LoadIntoProcess.hpp
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
#ifndef VIX_ENV_LOADINTOPROCESS_HPP
#define VIX_ENV_LOADINTOPROCESS_HPP

#include <string_view>

#include <vix/error/Error.hpp>
#include <vix/env/EnvFileOptions.hpp>

namespace vix::env
{

  /**
   * @brief Load a single .env file into the current process environment.
   *
   * Parsed keys are injected via set().
   */
  [[nodiscard]] vix::error::Error load_into_process(
      std::string_view path,
      const EnvFileOptions &options = {});

  /**
   * @brief Resolve and load layered .env files into the current process environment.
   *
   * Files are loaded in resolver order, so later files may override earlier ones
   * depending on options.env.overwrite_existing.
   */
  [[nodiscard]] vix::error::Error load_layered_into_process(
      const EnvFileOptions &options = {});

} // namespace vix::env

#endif // VIX_ENV_LOADINTOPROCESS_HPP
