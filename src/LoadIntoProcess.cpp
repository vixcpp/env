/**
 *
 *  @file LoadIntoProcess.cpp
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

#include <vix/env/LoadFile.hpp>
#include <vix/env/LoadIntoProcess.hpp>
#include <vix/env/LoadLayered.hpp>
#include <vix/env/Set.hpp>

namespace vix::env
{

  namespace
  {
    [[nodiscard]] vix::error::Error inject_file_into_process(
        const EnvFile &file,
        const EnvFileOptions &options) noexcept
    {
      for (const auto &entry : file.entries)
      {
        auto err = set(
            entry.key,
            entry.value,
            options.env.overwrite_existing);

        if (err)
        {
          return err;
        }
      }

      return {};
    }
  } // namespace

  vix::error::Error load_into_process(std::string_view path,
                                      const EnvFileOptions &options)
  {
    auto loaded = load_file(path, options);
    if (!loaded)
    {
      return loaded.error();
    }

    return inject_file_into_process(loaded.value(), options);
  }

  vix::error::Error load_layered_into_process(const EnvFileOptions &options)
  {
    auto files = load_layered(options);
    if (!files)
    {
      return files.error();
    }

    for (const auto &file : files.value())
    {
      auto err = inject_file_into_process(file, options);
      if (err)
      {
        return err;
      }
    }

    return {};
  }

} // namespace vix::env
