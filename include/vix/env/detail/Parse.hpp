/**
 * @file Parse.hpp
 * @brief Internal parsing policy shared by vix::env implementations.
 */
#ifndef VIX_ENV_DETAIL_PARSE_HPP
#define VIX_ENV_DETAIL_PARSE_HPP

#include <charconv>
#include <cctype>
#include <cstdlib>
#include <optional>
#include <string>
#include <string_view>

namespace vix::env::detail
{

  [[nodiscard]] inline char to_lower_ascii(unsigned char value) noexcept
  {
    return static_cast<char>(std::tolower(value));
  }

  [[nodiscard]] inline bool iequals(std::string_view left,
                                    std::string_view right) noexcept
  {
    if (left.size() != right.size())
    {
      return false;
    }

    for (std::size_t index = 0; index < left.size(); ++index)
    {
      if (to_lower_ascii(static_cast<unsigned char>(left[index])) !=
          to_lower_ascii(static_cast<unsigned char>(right[index])))
      {
        return false;
      }
    }

    return true;
  }

  [[nodiscard]] inline std::string_view trim(std::string_view value) noexcept
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

  [[nodiscard]] inline std::optional<bool> parse_bool(std::string_view value) noexcept
  {
    value = trim(value);

    if (value == "1" || iequals(value, "true") ||
        iequals(value, "yes") || iequals(value, "on"))
    {
      return true;
    }

    if (value == "0" || iequals(value, "false") ||
        iequals(value, "no") || iequals(value, "off"))
    {
      return false;
    }

    return std::nullopt;
  }

  [[nodiscard]] inline std::optional<int> parse_int(std::string_view value) noexcept
  {
    value = trim(value);
    if (value.empty())
    {
      return std::nullopt;
    }

    int parsed = 0;
    const auto [end, error] = std::from_chars(
        value.data(), value.data() + value.size(), parsed, 10);
    if (error != std::errc{} || end != value.data() + value.size())
    {
      return std::nullopt;
    }

    return parsed;
  }

  [[nodiscard]] inline std::optional<unsigned> parse_uint(std::string_view value) noexcept
  {
    value = trim(value);
    if (value.empty())
    {
      return std::nullopt;
    }

    unsigned parsed = 0;
    const auto [end, error] = std::from_chars(
        value.data(), value.data() + value.size(), parsed, 10);
    if (error != std::errc{} || end != value.data() + value.size())
    {
      return std::nullopt;
    }

    return parsed;
  }

  [[nodiscard]] inline std::optional<double> parse_double(std::string_view value) noexcept
  {
    value = trim(value);
    if (value.empty())
    {
      return std::nullopt;
    }

    const std::string stable_value(value);
    char *end = nullptr;
    const double parsed = std::strtod(stable_value.c_str(), &end);
    if (end == nullptr || end != stable_value.c_str() + stable_value.size())
    {
      return std::nullopt;
    }

    return parsed;
  }

} // namespace vix::env::detail

#endif // VIX_ENV_DETAIL_PARSE_HPP
