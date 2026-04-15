/**
 *
 *  @file ParseContent.cpp
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

#include <vix/env/EnvError.hpp>
#include <vix/env/ParseContent.hpp>
#include <vix/env/ParseLine.hpp>

namespace vix::env
{

  EnvFileResult parse_content(std::string_view content,
                              std::string_view path,
                              const EnvFileOptions &options)
  {
    EnvFile file;
    file.path = std::string(path);

    std::size_t line_start = 0;
    std::size_t line_number = 1;

    while (line_start <= content.size())
    {
      const std::size_t line_end = content.find('\n', line_start);
      const bool has_more = line_end != std::string_view::npos;

      std::string_view line = has_more
                                  ? content.substr(line_start, line_end - line_start)
                                  : content.substr(line_start);

      if (!line.empty() && line.back() == '\r')
      {
        line.remove_suffix(1);
      }

      auto parsed = parse_line(line, options);
      if (!parsed)
      {
        std::string message = "failed to parse .env content at line ";
        message += std::to_string(line_number);
        message += ": ";
        message += std::string(parsed.error().message());

        return make_env_error(
            EnvErrorCode::ParseFailed,
            std::move(message));
      }

      const EnvEntry &entry = parsed.value();
      if (!entry.key.empty())
      {
        file.entries.push_back(entry);
        file.values[entry.key] = entry.value;
      }

      if (!has_more)
      {
        break;
      }

      line_start = line_end + 1;
      ++line_number;
    }

    return file;
  }

} // namespace vix::env
