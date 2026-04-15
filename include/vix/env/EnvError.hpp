/**
 *
 *  @file EnvError.hpp
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
#ifndef VIX_ENV_ENVERROR_HPP
#define VIX_ENV_ENVERROR_HPP

#include <string>
#include <utility>

#include <vix/error/Error.hpp>
#include <vix/error/ErrorCategory.hpp>
#include <vix/error/ErrorCode.hpp>

namespace vix::env
{

  /**
   * @enum EnvErrorCode
   * @brief Environment-specific semantic error codes.
   *
   * These codes describe common failures related to:
   * - process environment access
   * - .env file resolution
   * - .env file loading
   * - parsing environment entries
   *
   * They complement the generic Vix error system.
   */
  enum class EnvErrorCode
  {
    None = 0,
    EmptyKey,
    InvalidKey,
    EmptyPath,
    InvalidPath,
    FileNotFound,
    FileReadFailed,
    ParseFailed,
    InvalidLine,
    InvalidValue,
    MissingValue,
    UnsupportedOperation,
    AlreadyExists,
    NotFound
  };

  /**
   * @brief Return the default env error category.
   */
  [[nodiscard]] inline constexpr vix::error::ErrorCategory env_error_category() noexcept
  {
    return vix::error::ErrorCategory("env");
  }

  /**
   * @brief Convert an EnvErrorCode to a generic Vix ErrorCode.
   */
  [[nodiscard]] inline constexpr vix::error::ErrorCode to_error_code(EnvErrorCode code) noexcept
  {
    using vix::error::ErrorCode;

    switch (code)
    {
    case EnvErrorCode::None:
      return ErrorCode::Ok;

    case EnvErrorCode::EmptyKey:
    case EnvErrorCode::InvalidKey:
    case EnvErrorCode::EmptyPath:
    case EnvErrorCode::InvalidPath:
    case EnvErrorCode::InvalidLine:
    case EnvErrorCode::InvalidValue:
    case EnvErrorCode::MissingValue:
      return ErrorCode::InvalidArgument;

    case EnvErrorCode::FileNotFound:
    case EnvErrorCode::NotFound:
      return ErrorCode::NotFound;

    case EnvErrorCode::FileReadFailed:
      return ErrorCode::IoError;

    case EnvErrorCode::ParseFailed:
      return ErrorCode::ParseError;

    case EnvErrorCode::UnsupportedOperation:
      return ErrorCode::NotSupported;

    case EnvErrorCode::AlreadyExists:
      return ErrorCode::AlreadyExists;
    }

    return ErrorCode::Unknown;
  }

  /**
   * @brief Convert an EnvErrorCode to a human-readable name.
   */
  [[nodiscard]] inline const char *to_string(EnvErrorCode code) noexcept
  {
    switch (code)
    {
    case EnvErrorCode::None:
      return "none";
    case EnvErrorCode::EmptyKey:
      return "empty_key";
    case EnvErrorCode::InvalidKey:
      return "invalid_key";
    case EnvErrorCode::EmptyPath:
      return "empty_path";
    case EnvErrorCode::InvalidPath:
      return "invalid_path";
    case EnvErrorCode::FileNotFound:
      return "file_not_found";
    case EnvErrorCode::FileReadFailed:
      return "file_read_failed";
    case EnvErrorCode::ParseFailed:
      return "parse_failed";
    case EnvErrorCode::InvalidLine:
      return "invalid_line";
    case EnvErrorCode::InvalidValue:
      return "invalid_value";
    case EnvErrorCode::MissingValue:
      return "missing_value";
    case EnvErrorCode::UnsupportedOperation:
      return "unsupported_operation";
    case EnvErrorCode::AlreadyExists:
      return "already_exists";
    case EnvErrorCode::NotFound:
      return "not_found";
    }

    return "unknown";
  }

  /**
   * @brief Build a structured Vix error from an EnvErrorCode.
   *
   * @param code Environment-specific error code.
   * @param message Human-readable message.
   */
  [[nodiscard]] inline vix::error::Error make_env_error(EnvErrorCode code, std::string message)
  {
    return vix::error::Error(
        to_error_code(code),
        env_error_category(),
        std::move(message));
  }

} // namespace vix::env

#endif // VIX_ENV_ENVERROR_HPP
