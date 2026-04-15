/**
 *
 *  @file ParseContent.hpp
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
#ifndef VIX_ENV_PARSECONTENT_HPP
#define VIX_ENV_PARSECONTENT_HPP

#include <string_view>

#include <vix/env/EnvFile.hpp>
#include <vix/env/EnvFileOptions.hpp>

namespace vix::env
{

  /**
   * @brief Parse the full textual content of a .env file.
   *
   * The parser processes the content line by line using parse_line().
   * Ignored lines are skipped. Repeated keys overwrite previous values
   * in the final map, while entries keep their original order.
   *
   * @param content Full textual content of the .env file.
   * @param path Original file path associated with this content.
   * @param options Parsing options.
   * @return Parsed EnvFile on success.
   */
  [[nodiscard]] EnvFileResult parse_content(std::string_view content,
                                            std::string_view path = {},
                                            const EnvFileOptions &options = {});

} // namespace vix::env

#endif // VIX_ENV_PARSECONTENT_HPP
