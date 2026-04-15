/**
 *
 *  @file LoadFile.hpp
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
#ifndef VIX_ENV_LOADFILE_HPP
#define VIX_ENV_LOADFILE_HPP

#include <string_view>

#include <vix/env/EnvFile.hpp>
#include <vix/env/EnvFileOptions.hpp>

namespace vix::env
{

  /**
   * @brief Load and parse a single .env file from an explicit path.
   *
   * The file content is read, then parsed with parse_content().
   */
  [[nodiscard]] EnvFileResult load_file(std::string_view path,
                                        const EnvFileOptions &options = {});

} // namespace vix::env

#endif // VIX_ENV_LOADFILE_HPP
