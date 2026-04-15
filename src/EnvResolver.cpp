/**
 *
 *  @file EnvResolver.cpp
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
#include <string_view>
#include <vector>

#include <vix/env/EnvError.hpp>
#include <vix/env/EnvResolver.hpp>
#include <vix/path/Join.hpp>
#include <vix/path/Normalize.hpp>

namespace vix::env
{

  namespace
  {
    [[nodiscard]] vix::error::Result<std::string> normalize_joined_path(
        std::string_view base_dir,
        std::string_view filename,
        const vix::path::PathOptions &path_options)
    {
      if (base_dir.empty())
      {
        return make_env_error(
            EnvErrorCode::EmptyPath,
            "base directory cannot be empty");
      }

      if (filename.empty())
      {
        return make_env_error(
            EnvErrorCode::EmptyPath,
            "env filename cannot be empty");
      }

      auto joined = vix::path::join(base_dir, filename, path_options);
      if (!joined)
      {
        return joined.error();
      }

      auto normalized = vix::path::normalize(joined.value(), path_options);
      if (!normalized)
      {
        return normalized.error();
      }

      return normalized.value();
    }

    [[nodiscard]] std::string append_suffix(std::string_view base_name,
                                            std::string_view suffix)
    {
      std::string out(base_name);
      out += suffix;
      return out;
    }

    [[nodiscard]] void push_candidate(std::vector<std::string> &candidates,
                                      std::string_view filename,
                                      const EnvFileOptions &options,
                                      vix::error::Error &error)
    {
      if (error.has_error())
      {
        return;
      }

      auto resolved = normalize_joined_path(
          options.base_dir,
          filename,
          options.path);

      if (!resolved)
      {
        error = resolved.error();
        return;
      }

      candidates.push_back(std::move(resolved.value()));
    }
  } // namespace

  vix::error::Result<std::string> resolve_env_file(const EnvFileOptions &options)
  {
    return normalize_joined_path(
        options.base_dir,
        options.filename,
        options.path);
  }

  EnvPathListResult resolve_env_files(const EnvFileOptions &options)
  {
    std::vector<std::string> candidates;
    vix::error::Error error;

    const std::string &base_name = options.filename;
    const std::string &environment = options.environment_name;

    if (base_name.empty())
    {
      return make_env_error(
          EnvErrorCode::EmptyPath,
          "env filename cannot be empty");
    }

    if (options.mode == EnvFileMode::Single)
    {
      auto resolved = resolve_env_file(options);
      if (!resolved)
      {
        return resolved.error();
      }

      candidates.push_back(std::move(resolved.value()));
      return candidates;
    }

    if (options.load_base_file)
    {
      push_candidate(candidates, base_name, options, error);
    }

    if (options.load_local_file)
    {
      push_candidate(
          candidates,
          append_suffix(base_name, ".local"),
          options,
          error);
    }

    if (options.load_environment_file)
    {
      if (environment.empty())
      {
        return make_env_error(
            EnvErrorCode::InvalidValue,
            "environment_name cannot be empty when load_environment_file is enabled");
      }

      push_candidate(
          candidates,
          append_suffix(base_name, "." + environment),
          options,
          error);
    }

    if (options.load_environment_local_file)
    {
      if (environment.empty())
      {
        return make_env_error(
            EnvErrorCode::InvalidValue,
            "environment_name cannot be empty when load_environment_local_file is enabled");
      }

      push_candidate(
          candidates,
          append_suffix(base_name, "." + environment + ".local"),
          options,
          error);
    }

    if (error)
    {
      return error;
    }

    return candidates;
  }

} // namespace vix::env
