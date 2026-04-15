/**
 *
 *  @file parse_content_test.cpp
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

#include <cstdlib>
#include <iostream>
#include <string>

#include <vix/env/ParseContent.hpp>

namespace
{
  void assert_true(bool condition, const std::string &message)
  {
    if (!condition)
    {
      std::cerr << "Assertion failed: " << message << '\n';
      std::exit(1);
    }
  }

  void test_parse_content_parses_multiple_entries()
  {
    const std::string content =
        "APP_ENV=production\n"
        "PORT=8080\n"
        "DEBUG=false\n";

    auto result = vix::env::parse_content(content, ".env");
    assert_true(result.ok(), "parse_content should succeed");

    const auto &file = result.value();
    assert_true(file.path == ".env", "path should match");
    assert_true(file.entries.size() == 3, "entries size should match");
    assert_true(file.values.size() == 3, "map size should match");

    assert_true(file.entries[0].key == "APP_ENV", "first key should match");
    assert_true(file.entries[0].value == "production", "first value should match");

    assert_true(file.entries[1].key == "PORT", "second key should match");
    assert_true(file.entries[1].value == "8080", "second value should match");

    assert_true(file.entries[2].key == "DEBUG", "third key should match");
    assert_true(file.entries[2].value == "false", "third value should match");
  }

  void test_parse_content_ignores_blank_and_comment_lines()
  {
    const std::string content =
        "\n"
        "# comment\n"
        "APP_NAME=vix\n"
        "   \n"
        "# another comment\n"
        "PORT=3000\n";

    auto result = vix::env::parse_content(content, ".env");
    assert_true(result.ok(), "parse_content should succeed");

    const auto &file = result.value();
    assert_true(file.entries.size() == 2, "only real entries should be kept");
    assert_true(file.values.size() == 2, "only real entries should be in map");

    assert_true(file.values.at("APP_NAME") == "vix", "APP_NAME should match");
    assert_true(file.values.at("PORT") == "3000", "PORT should match");
  }

  void test_parse_content_last_duplicate_value_wins_in_map()
  {
    const std::string content =
        "APP_ENV=dev\n"
        "APP_ENV=prod\n";

    auto result = vix::env::parse_content(content, ".env");
    assert_true(result.ok(), "parse_content should succeed");

    const auto &file = result.value();
    assert_true(file.entries.size() == 2, "entries should keep both occurrences");
    assert_true(file.values.size() == 1, "map should contain one key");
    assert_true(file.values.at("APP_ENV") == "prod", "last value should win in map");
  }

  void test_parse_content_supports_windows_line_endings()
  {
    const std::string content =
        "APP_ENV=production\r\n"
        "PORT=8080\r\n";

    auto result = vix::env::parse_content(content, ".env");
    assert_true(result.ok(), "parse_content should succeed for CRLF");

    const auto &file = result.value();
    assert_true(file.entries.size() == 2, "entries size should match");
    assert_true(file.values.at("APP_ENV") == "production", "APP_ENV should match");
    assert_true(file.values.at("PORT") == "8080", "PORT should match");
  }

  void test_parse_content_fails_with_line_number()
  {
    const std::string content =
        "APP_ENV=production\n"
        "INVALID LINE\n"
        "PORT=8080\n";

    auto result = vix::env::parse_content(content, ".env");
    assert_true(!result.ok(), "parse_content should fail");

    const std::string message(result.error().message());
    assert_true(message.find("line 2") != std::string::npos,
                "error message should mention line number");
  }

  void test_parse_content_handles_empty_content()
  {
    auto result = vix::env::parse_content("", ".env");
    assert_true(result.ok(), "empty content should parse successfully");

    const auto &file = result.value();
    assert_true(file.path == ".env", "path should match");
    assert_true(file.entries.empty(), "entries should be empty");
    assert_true(file.values.empty(), "map should be empty");
  }
} // namespace

int main()
{
  test_parse_content_parses_multiple_entries();
  test_parse_content_ignores_blank_and_comment_lines();
  test_parse_content_last_duplicate_value_wins_in_map();
  test_parse_content_supports_windows_line_endings();
  test_parse_content_fails_with_line_number();
  test_parse_content_handles_empty_content();

  std::cout << "vix_env_parse_content_test passed\n";
  return 0;
}
