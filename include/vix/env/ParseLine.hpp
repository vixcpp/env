/**
 *
 *  @file ParseLine.hpp
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
#ifndef VIX_ENV_PARSELINE_HPP
#define VIX_ENV_PARSELINE_HPP

#include <string_view>

#include <vix/env/EnvFileOptions.hpp>
#include <vix/env/EnvResult.hpp>

namespace vix::env
{

  /**
   * @brief Parse a single .env line into a key-value entry.
   *
   * Supported forms:
   * - KEY=VALUE
   * - KEY = VALUE
   * - export KEY=VALUE
   *
   * Ignored lines:
   * - blank lines
   * - comment lines beginning with '#'
   *
   * Inline comments are supported only for unquoted values:
   * - KEY=value # comment
   *
   * Quoted values are preserved as content and may be unquoted if
   * EnvFileOptions::strip_quotes is enabled.
   *
   * @return
   * - success with EnvEntry when the line contains a real entry
   * - success with empty key/value when the line should be ignored
   * - error if the line is syntactically invalid
   */
  [[nodiscard]] EnvEntryResult parse_line(std::string_view line,
                                          const EnvFileOptions &options = {});

} // namespace vix::env

#endif // VIX_ENV_PARSELINE_HPP
