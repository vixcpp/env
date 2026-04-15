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
        fs::temp_directory_path() / "vix_env_load_layered_test";

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

  void cleanup(const fs::path &dir)
  {
    std::error_code ec;
    fs::remove_all(dir, ec);
  }

  void test_load_layered_full_stack()
  {
    const fs::path dir = make_temp_dir();

    write_file(dir / ".env", "A=1\n");
    write_file(dir / ".env.local", "B=2\n");
    write_file(dir / ".env.production", "C=3\n");
    write_file(dir / ".env.production.local", "D=4\n");

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
    assert_true(result.ok(), "should load all files");

    const auto &files = result.value();
    assert_true(files.size() == 4, "should load 4 files");

    assert_true(files[0].values.at("A") == "1", "file 1 ok");
    assert_true(files[1].values.at("B") == "2", "file 2 ok");
    assert_true(files[2].values.at("C") == "3", "file 3 ok");
    assert_true(files[3].values.at("D") == "4", "file 4 ok");

    cleanup(dir);
  }

  void test_load_layered_ignore_missing()
  {
    const fs::path dir = make_temp_dir();

    write_file(dir / ".env", "A=1\n");

    vix::env::EnvFileOptions options;
    options.mode = vix::env::EnvFileMode::Layered;
    options.base_dir = dir.string();
    options.filename = ".env";
    options.environment_name = "production";
    options.ignore_missing_files = true;

    auto result = vix::env::load_layered(options);
    assert_true(result.ok(), "should succeed when ignoring missing");

    const auto &files = result.value();
    assert_true(files.size() == 1, "only one file should be loaded");

    cleanup(dir);
  }

  void test_load_layered_fail_on_missing()
  {
    const fs::path dir = make_temp_dir();

    write_file(dir / ".env", "A=1\n");

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
                "should fail when missing files");

    cleanup(dir);
  }

  void test_load_layered_parse_error()
  {
    const fs::path dir = make_temp_dir();

    write_file(dir / ".env", "A=1\n");
    write_file(dir / ".env.local", "INVALID LINE\n");

    vix::env::EnvFileOptions options;
    options.mode = vix::env::EnvFileMode::Layered;
    options.base_dir = dir.string();
    options.filename = ".env";

    options.load_base_file = true;
    options.load_local_file = true;
    options.load_environment_file = false;
    options.load_environment_local_file = false;

    options.ignore_missing_files = false;

    auto result = vix::env::load_layered(options);
    assert_true(!result.ok(), "should fail on parse error");

    cleanup(dir);
  }

  void test_load_layered_single_mode()
  {
    const fs::path dir = make_temp_dir();

    write_file(dir / ".env", "A=1\n");
    write_file(dir / ".env.local", "B=2\n");

    vix::env::EnvFileOptions options;
    options.mode = vix::env::EnvFileMode::Single;
    options.base_dir = dir.string();
    options.filename = ".env";

    auto result = vix::env::load_layered(options);
    assert_true(result.ok(), "single mode should succeed");

    const auto &files = result.value();
    assert_true(files.size() == 1, "only one file should be loaded");
    assert_true(files[0].values.at("A") == "1", "should load only .env");

    cleanup(dir);
  }

} // namespace

int main()
{
  test_load_layered_full_stack();
  test_load_layered_ignore_missing();
  test_load_layered_fail_on_missing();
  test_load_layered_parse_error();
  test_load_layered_single_mode();

  std::cout << "vix_env_load_layered_test passed\n";
  return 0;
}
