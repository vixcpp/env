/**
 *
 *  @file LoadLayered.cpp
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

#include <string>
#include <vector>

#include <vix/env/EnvError.hpp>
#include <vix/env/EnvResolver.hpp>
#include <vix/env/LoadFile.hpp>
#include <vix/env/LoadLayered.hpp>

namespace vix::env
{

  EnvFileListResult load_layered(const EnvFileOptions &options)
  {
    auto resolved = resolve_env_files(options);
    if (!resolved)
    {
      return resolved.error();
    }

    std::vector<EnvFile> files;
    files.reserve(resolved.value().size());

    for (const auto &path : resolved.value())
    {
      auto loaded = load_file(path, options);
      if (!loaded)
      {
        const bool missing_file =
            loaded.error().category() == env_error_category() &&
            loaded.error().code() == vix::error::ErrorCode::NotFound;

        if (options.ignore_missing_files && missing_file)
        {
          continue;
        }

        return loaded.error();
      }

      files.push_back(std::move(loaded.value()));
    }

    return files;
  }

} // namespace vix::env
