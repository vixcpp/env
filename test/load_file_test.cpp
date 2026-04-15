/**
 *
 *  @file load_file_test.cpp
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
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

#include <vix/env/LoadFile.hpp>

namespace
{
  namespace fs = std::filesystem;

  void assert_true(bool condition, const std::string &message)
  {
    if (!condition)
    {
      std::cerr << "Assertion failed: " << message << '\n';
      std::exit(1);
    }
  }

  fs::path make_temp_dir()
  {
    const fs::path dir =
        fs::temp_directory_path() / "vix_env_load_file_test_dir";

    std::error_code ec;
    fs::remove_all(dir, ec);
    fs::create_directories(dir, ec);

    if (ec)
    {
      std::cerr << "Failed to create temp directory: " << dir << '\n';
      std::exit(1);
    }

    return dir;
  }

  void test_load_file_reads_and_parses_env_file()
  {
    const fs::path dir = make_temp_dir();
    const fs::path file = dir / ".env";

    {
      std::ofstream out(file);
      out << "APP_ENV=production\n";
      out << "PORT=8080\n";
      out << "DEBUG=false\n";
    }

    auto result = vix::env::load_file(file.string());
    assert_true(result.ok(), "load_file should succeed");

    const auto &env_file = result.value();
    assert_true(env_file.path == file.string(), "path should match");
    assert_true(env_file.entries.size() == 3, "entries size should match");
    assert_true(env_file.values.at("APP_ENV") == "production", "APP_ENV should match");
    assert_true(env_file.values.at("PORT") == "8080", "PORT should match");
    assert_true(env_file.values.at("DEBUG") == "false", "DEBUG should match");

    std::error_code ec;
    fs::remove_all(dir, ec);
  }

  void test_load_file_fails_for_missing_file()
  {
    const fs::path dir = make_temp_dir();
    const fs::path file = dir / "missing.env";

    auto result = vix::env::load_file(file.string());
    assert_true(!result.ok(), "load_file should fail for missing file");

    std::error_code ec;
    fs::remove_all(dir, ec);
  }

  void test_load_file_fails_for_empty_path()
  {
    auto result = vix::env::load_file("");
    assert_true(!result.ok(), "load_file should fail for empty path");
  }

  void test_load_file_reports_parse_errors()
  {
    const fs::path dir = make_temp_dir();
    const fs::path file = dir / ".env";

    {
      std::ofstream out(file);
      out << "APP_ENV=production\n";
      out << "INVALID LINE\n";
    }

    auto result = vix::env::load_file(file.string());
    assert_true(!result.ok(), "load_file should fail for invalid content");

    const std::string message(result.error().message());
    assert_true(message.find("line 2") != std::string::npos,
                "parse error should mention line number");

    std::error_code ec;
    fs::remove_all(dir, ec);
  }
} // namespace

int main()
{
  test_load_file_reads_and_parses_env_file();
  test_load_file_fails_for_missing_file();
  test_load_file_fails_for_empty_path();
  test_load_file_reports_parse_errors();

  std::cout << "vix_env_load_file_test passed\n";
  return 0;
}
