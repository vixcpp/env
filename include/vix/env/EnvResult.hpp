/**
 *
 *  @file EnvResult.hpp
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
#ifndef VIX_ENV_ENVRESULT_HPP
#define VIX_ENV_ENVRESULT_HPP

#include <string>
#include <unordered_map>
#include <vector>

#include <vix/error/Result.hpp>

namespace vix::env
{

  /**
   * @brief Single parsed environment entry.
   */
  struct EnvEntry
  {
    std::string key;
    std::string value;
  };

  /**
   * @brief Parsed key-value map for .env content.
   */
  using EnvMap = std::unordered_map<std::string, std::string>;

  /**
   * @brief Ordered list of parsed environment entries.
   */
  using EnvEntryList = std::vector<EnvEntry>;

  /**
   * @brief Standard result type for env operations returning a string.
   */
  using EnvStringResult = vix::error::Result<std::string>;

  /**
   * @brief Standard result type for env operations returning a boolean.
   */
  using EnvBoolResult = vix::error::Result<bool>;

  /**
   * @brief Standard result type for env operations returning a signed integer.
   */
  using EnvIntResult = vix::error::Result<int>;

  /**
   * @brief Standard result type for env operations returning an unsigned integer.
   */
  using EnvUintResult = vix::error::Result<unsigned>;

  /**
   * @brief Standard result type for env operations returning a double.
   */
  using EnvDoubleResult = vix::error::Result<double>;

  /**
   * @brief Standard result type for env operations returning a parsed env entry.
   */
  using EnvEntryResult = vix::error::Result<EnvEntry>;

  /**
   * @brief Standard result type for env operations returning many parsed entries.
   */
  using EnvEntryListResult = vix::error::Result<EnvEntryList>;

  /**
   * @brief Standard result type for env operations returning an env key-value map.
   */
  using EnvMapResult = vix::error::Result<EnvMap>;

} // namespace vix::env

#endif // VIX_ENV_ENVRESULT_HPP
