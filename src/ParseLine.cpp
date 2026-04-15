/**
 *
 *  @file ParseLine.cpp
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

#include <cctype>
#include <string>
#include <string_view>

#include <vix/env/EnvError.hpp>
#include <vix/env/ParseLine.hpp>

namespace vix::env
{

  namespace
  {
    [[nodiscard]] std::string_view trim(std::string_view value) noexcept
    {
      std::size_t begin = 0;
      std::size_t end = value.size();

      while (begin < end &&
             std::isspace(static_cast<unsigned char>(value[begin])))
      {
        ++begin;
      }

      while (end > begin &&
             std::isspace(static_cast<unsigned char>(value[end - 1])))
      {
        --end;
      }

      return value.substr(begin, end - begin);
    }

    [[nodiscard]] bool starts_with(std::string_view value,
                                   std::string_view prefix) noexcept
    {
      return value.size() >= prefix.size() &&
             value.substr(0, prefix.size()) == prefix;
    }

    [[nodiscard]] bool is_valid_key(std::string_view key) noexcept
    {
      if (key.empty())
      {
        return false;
      }

      const unsigned char first = static_cast<unsigned char>(key.front());
      if (!(std::isalpha(first) || key.front() == '_'))
      {
        return false;
      }

      for (const char c : key)
      {
        const unsigned char uc = static_cast<unsigned char>(c);
        if (!(std::isalnum(uc) || c == '_'))
        {
          return false;
        }
      }

      return true;
    }

    [[nodiscard]] std::string strip_inline_comment(std::string_view value)
    {
      bool in_single_quotes = false;
      bool in_double_quotes = false;

      for (std::size_t i = 0; i < value.size(); ++i)
      {
        const char c = value[i];

        if (c == '\'' && !in_double_quotes)
        {
          in_single_quotes = !in_single_quotes;
          continue;
        }

        if (c == '"' && !in_single_quotes)
        {
          in_double_quotes = !in_double_quotes;
          continue;
        }

        if (c == '#' && !in_single_quotes && !in_double_quotes)
        {
          if (i == 0 || std::isspace(static_cast<unsigned char>(value[i - 1])))
          {
            return std::string(trim(value.substr(0, i)));
          }
        }
      }

      return std::string(trim(value));
    }

    [[nodiscard]] std::string maybe_strip_quotes(std::string_view value,
                                                 const EnvFileOptions &options)
    {
      if (!options.strip_quotes || value.size() < 2)
      {
        return std::string(value);
      }

      const char first = value.front();
      const char last = value.back();

      if ((first == '"' && last == '"') ||
          (first == '\'' && last == '\''))
      {
        return std::string(value.substr(1, value.size() - 2));
      }

      return std::string(value);
    }
  } // namespace

  EnvEntryResult parse_line(std::string_view line, const EnvFileOptions &options)
  {
    std::string_view working = options.env.trim_whitespace ? trim(line) : line;

    if (working.empty())
    {
      return EnvEntry{};
    }

    if (options.ignore_comments && !working.empty() && working.front() == '#')
    {
      return EnvEntry{};
    }

    if (starts_with(working, "export "))
    {
      working = trim(working.substr(7));
    }

    const std::size_t equal_pos = working.find('=');
    if (equal_pos == std::string_view::npos)
    {
      return make_env_error(
          EnvErrorCode::InvalidLine,
          "invalid .env line: missing '=' separator");
    }

    std::string_view raw_key = working.substr(0, equal_pos);
    std::string_view raw_value = working.substr(equal_pos + 1);

    if (options.env.trim_whitespace)
    {
      raw_key = trim(raw_key);
      raw_value = trim(raw_value);
    }

    if (raw_key.empty())
    {
      return make_env_error(
          EnvErrorCode::EmptyKey,
          "invalid .env line: key cannot be empty");
    }

    if (!is_valid_key(raw_key))
    {
      return make_env_error(
          EnvErrorCode::InvalidKey,
          "invalid .env line: key is invalid");
    }

    std::string value = strip_inline_comment(raw_value);

    if (!options.env.allow_empty_values && value.empty())
    {
      return make_env_error(
          EnvErrorCode::MissingValue,
          "invalid .env line: value cannot be empty");
    }

    value = maybe_strip_quotes(value, options);

    return EnvEntry{std::string(raw_key), std::move(value)};
  }

} // namespace vix::env
