/**
 *
 *  @file parse_line_test.cpp
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

#include <vix/env/EnvFileOptions.hpp>
#include <vix/env/ParseLine.hpp>

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

  void test_parse_line_parses_simple_key_value()
  {
    auto result = vix::env::parse_line("APP_ENV=production");
    assert_true(result.ok(), "parse_line should succeed");

    const auto &entry = result.value();
    assert_true(entry.key == "APP_ENV", "key should match");
    assert_true(entry.value == "production", "value should match");
  }

  void test_parse_line_parses_with_spaces()
  {
    auto result = vix::env::parse_line(" PORT = 8080 ");
    assert_true(result.ok(), "parse_line should succeed");

    const auto &entry = result.value();
    assert_true(entry.key == "PORT", "key should match");
    assert_true(entry.value == "8080", "value should match");
  }

  void test_parse_line_supports_export_prefix()
  {
    auto result = vix::env::parse_line("export DB_HOST=localhost");
    assert_true(result.ok(), "parse_line should succeed");

    const auto &entry = result.value();
    assert_true(entry.key == "DB_HOST", "key should match");
    assert_true(entry.value == "localhost", "value should match");
  }

  void test_parse_line_ignores_blank_line()
  {
    auto result = vix::env::parse_line("   ");
    assert_true(result.ok(), "blank line should be ignored successfully");

    const auto &entry = result.value();
    assert_true(entry.key.empty(), "ignored line should have empty key");
    assert_true(entry.value.empty(), "ignored line should have empty value");
  }

  void test_parse_line_ignores_comment_line()
  {
    auto result = vix::env::parse_line("# comment");
    assert_true(result.ok(), "comment line should be ignored successfully");

    const auto &entry = result.value();
    assert_true(entry.key.empty(), "ignored comment should have empty key");
    assert_true(entry.value.empty(), "ignored comment should have empty value");
  }

  void test_parse_line_strips_inline_comment_for_unquoted_value()
  {
    auto result = vix::env::parse_line("APP_NAME=vix # app name");
    assert_true(result.ok(), "parse_line should succeed");

    const auto &entry = result.value();
    assert_true(entry.key == "APP_NAME", "key should match");
    assert_true(entry.value == "vix", "inline comment should be stripped");
  }

  void test_parse_line_preserves_hash_inside_quotes()
  {
    auto result = vix::env::parse_line("APP_NAME=\"vix # runtime\"");
    assert_true(result.ok(), "parse_line should succeed");

    const auto &entry = result.value();
    assert_true(entry.key == "APP_NAME", "key should match");
    assert_true(entry.value == "vix # runtime", "quoted hash should be preserved");
  }

  void test_parse_line_strips_quotes_when_enabled()
  {
    vix::env::EnvFileOptions options;
    options.strip_quotes = true;

    auto result = vix::env::parse_line("TITLE=\"Hello Vix\"", options);
    assert_true(result.ok(), "parse_line should succeed");

    const auto &entry = result.value();
    assert_true(entry.key == "TITLE", "key should match");
    assert_true(entry.value == "Hello Vix", "quotes should be stripped");
  }

  void test_parse_line_preserves_quotes_when_disabled()
  {
    vix::env::EnvFileOptions options;
    options.strip_quotes = false;

    auto result = vix::env::parse_line("TITLE=\"Hello Vix\"", options);
    assert_true(result.ok(), "parse_line should succeed");

    const auto &entry = result.value();
    assert_true(entry.key == "TITLE", "key should match");
    assert_true(entry.value == "\"Hello Vix\"", "quotes should be preserved");
  }

  void test_parse_line_fails_without_equal_sign()
  {
    auto result = vix::env::parse_line("INVALID_LINE");
    assert_true(!result.ok(), "parse_line should fail without '='");
  }

  void test_parse_line_fails_for_empty_key()
  {
    auto result = vix::env::parse_line("=value");
    assert_true(!result.ok(), "parse_line should fail for empty key");
  }

  void test_parse_line_fails_for_invalid_key()
  {
    auto result = vix::env::parse_line("INVALID-KEY=value");
    assert_true(!result.ok(), "parse_line should fail for invalid key");
  }

  void test_parse_line_fails_for_empty_value_when_disallowed()
  {
    vix::env::EnvFileOptions options;
    options.env.allow_empty_values = false;

    auto result = vix::env::parse_line("EMPTY_VALUE=", options);
    assert_true(!result.ok(), "parse_line should fail when empty values are disallowed");
  }
} // namespace

int main()
{
  test_parse_line_parses_simple_key_value();
  test_parse_line_parses_with_spaces();
  test_parse_line_supports_export_prefix();
  test_parse_line_ignores_blank_line();
  test_parse_line_ignores_comment_line();
  test_parse_line_strips_inline_comment_for_unquoted_value();
  test_parse_line_preserves_hash_inside_quotes();
  test_parse_line_strips_quotes_when_enabled();
  test_parse_line_preserves_quotes_when_disabled();
  test_parse_line_fails_without_equal_sign();
  test_parse_line_fails_for_empty_key();
  test_parse_line_fails_for_invalid_key();
  test_parse_line_fails_for_empty_value_when_disallowed();

  std::cout << "vix_env_parse_line_test passed\n";
  return 0;
}
