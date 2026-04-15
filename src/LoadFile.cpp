/**
 *
 *  @file LoadFile.cpp
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

#include <fstream>
#include <sstream>
#include <string>
#include <string_view>

#include <vix/env/EnvError.hpp>
#include <vix/env/LoadFile.hpp>
#include <vix/env/ParseContent.hpp>

namespace vix::env
{

  EnvFileResult load_file(std::string_view path, const EnvFileOptions &options)
  {
    if (path.empty())
    {
      return make_env_error(
          EnvErrorCode::EmptyPath,
          "file path cannot be empty");
    }

    const std::string stable_path(path);
    std::ifstream input(stable_path, std::ios::in | std::ios::binary);

    if (!input.is_open())
    {
      return make_env_error(
          EnvErrorCode::FileNotFound,
          "failed to open .env file: " + stable_path);
    }

    std::ostringstream buffer;
    buffer << input.rdbuf();

    if (input.bad())
    {
      return make_env_error(
          EnvErrorCode::FileReadFailed,
          "failed to read .env file: " + stable_path);
    }

    return parse_content(buffer.str(), stable_path, options);
  }

} // namespace vix::env
