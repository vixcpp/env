/**
 *
 *  @file load_layered_test.cpp
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

#include <vix/env/LoadLayered.hpp>

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
        fs::temp_directory_path() / "vix_env_load_layered_test_dir";

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

  void write_file(const fs::path &path, const std::string &content)
  {
    std::ofstream out(path, std::ios::out | std::ios::binary);
    if (!out)
    {
      std::cerr << "Failed to write file: " << path << '\n';
      std::exit(1);
    }

    out << content;
  }

  void cleanup_dir(const fs::path &dir)
  {
    std::error_code ec;
    fs::remove_all(dir, ec);
  }

  void test_load_layered_loads_existing_files_in_order()
  {
    const fs::path dir = make_temp_dir();

    write_file(dir / ".env", "APP_NAME=vix\nPORT=3000\n");
    write_file(dir / ".env.local", "PORT=4000\nDEBUG=true\n");
    write_file(dir / ".env.production", "APP_ENV=production\n");
    write_file(dir / ".env.production.local", "PORT=5000\n");

    vix::env::EnvFileOptions options;
    options.mode = vix::env::EnvFileMode::Layered;
    options.base_dir = dir.string();
    options.filename = ".env";
    options.environment_name = "production";
    options.load_base_file = true;
    options.load_local_file = true;
    options.load_environment_file = true;
    options.load_environment_local_file = true;
    options.ignore_missing_files = false;

    auto result = vix::env::load_layered(options);
    assert_true(result.ok(), "load_layered should succeed");

    const auto &files = result.value();
    assert_true(files.size() == 4, "four files should be loaded");

    assert_true(files[0].path.find(".env") != std::string::npos,
                "first file should be .env");
    assert_true(files[0].values.at("APP_NAME") == "vix",
                "base file should contain APP_NAME");

    assert_true(files[1].path.find(".env.local") != std::string::npos,
                "second file should be .env.local");
    assert_true(files[1].values.at("PORT") == "4000",
                "local file should contain PORT=4000");

    assert_true(files[2].path.find(".env.production") != std::string::npos,
                "third file should be .env.production");
    assert_true(files[2].values.at("APP_ENV") == "production",
                "env file should contain APP_ENV");

    assert_true(files[3].path.find(".env.production.local") != std::string::npos,
                "fourth file should be .env.production.local");
    assert_true(files[3].values.at("PORT") == "5000",
                "env local file should contain PORT=5000");

    cleanup_dir(dir);
  }

  void test_load_layered_ignores_missing_files_when_enabled()
  {
    const fs::path dir = make_temp_dir();

    write_file(dir / ".env", "APP_NAME=vix\n");

    vix::env::EnvFileOptions options;
    options.mode = vix::env::EnvFileMode::Layered;
    options.base_dir = dir.string();
    options.filename = ".env";
    options.environment_name = "production";
    options.load_base_file = true;
    options.load_local_file = true;
    options.load_environment_file = true;
    options.load_environment_local_file = true;
    options.ignore_missing_files = true;

    auto result = vix::env::load_layered(options);
    assert_true(result.ok(), "load_layered should succeed when missing files are ignored");

    const auto &files = result.value();
    assert_true(files.size() == 1, "only the existing .env file should be loaded");
    assert_true(files[0].values.at("APP_NAME") == "vix",
                "loaded file should contain APP_NAME");

    cleanup_dir(dir);
  }

  void test_load_layered_fails_when_missing_files_are_not_ignored()
  {
    const fs::path dir = make_temp_dir();

    write_file(dir / ".env", "APP_NAME=vix\n");

    vix::env::EnvFileOptions options;
    options.mode = vix::env::EnvFileMode::Layered;
    options.base_dir = dir.string();
    options.filename = ".env";
    options.environment_name = "production";
    options.load_base_file = true;
    options.load_local_file = true;
    options.load_environment_file = false;
    options.load_environment_local_file = false;
    options.ignore_missing_files = false;

    auto result = vix::env::load_layered(options);
    assert_true(!result.ok(),
                "load_layered should fail when a selected file is missing and ignore_missing_files=false");

    cleanup_dir(dir);
  }

  void test_load_layered_fails_when_parsing_one_file_fails()
  {
    const fs::path dir = make_temp_dir();

    write_file(dir / ".env", "APP_NAME=vix\n");
    write_file(dir / ".env.local", "INVALID LINE\n");

    vix::env::EnvFileOptions options;
    options.mode = vix::env::EnvFileMode::Layered;
    options.base_dir = dir.string();
    options.filename = ".env";
    options.load_base_file = true;
    options.load_local_file = true;
    options.ignore_missing_files = false;

    auto result = vix::env::load_layered(options);
    assert_true(!result.ok(), "load_layered should fail if one file is invalid");

    const std::string message(result.error().message());
    assert_true(message.find("line 1") != std::string::npos,
                "parse error should mention the line number");

    cleanup_dir(dir);
  }

  void test_load_layered_single_mode_loads_only_primary_file()
  {
    const fs::path dir = make_temp_dir();

    write_file(dir / ".env", "APP_NAME=vix\n");
    write_file(dir / ".env.local", "APP_NAME=local\n");

    vix::env::EnvFileOptions options;
    options.mode = vix::env::EnvFileMode::Single;
    options.base_dir = dir.string();
    options.filename = ".env";
    options.load_base_file = true;
    options.load_local_file = true;
    options.ignore_missing_files = false;

    auto result = vix::env::load_layered(options);
    assert_true(result.ok(), "single mode should succeed");

    const auto &files = result.value();
    assert_true(files.size() == 1, "single mode should load only one file");
    assert_true(files[0].path.find(".env") != std::string::npos,
                "single mode should load the primary env file");
    assert_true(files[0].values.at("APP_NAME") == "vix",
                "single mode should load only the primary file content");

    cleanup_dir(dir);
  }
} // namespace

int main()
{
  test_load_layered_loads_existing_files_in_order();
  test_load_layered_ignores_missing_files_when_enabled();
  test_load_layered_fails_when_missing_files_are_not_ignored();
  test_load_layered_fails_when_parsing_one_file_fails();
  test_load_layered_single_mode_loads_only_primary_file();

  std::cout << "vix_env_load_layered_test passed\n";
  return 0;
}
