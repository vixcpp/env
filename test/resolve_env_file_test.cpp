/**
 *
 *  @file resolve_env_file_test.cpp
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
#include <vector>

#include <vix/env/EnvResolver.hpp>

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

  bool contains_suffix(const std::string &value, const std::string &suffix)
  {
    return value.size() >= suffix.size() &&
           value.substr(value.size() - suffix.size()) == suffix;
  }

  void test_resolve_env_file_resolves_primary_file()
  {
    vix::env::EnvFileOptions options;
    options.base_dir = ".";
    options.filename = ".env";

    auto result = vix::env::resolve_env_file(options);
    assert_true(result.ok(), "resolve_env_file should succeed");
    assert_true(contains_suffix(result.value(), ".env"),
                "resolved path should end with .env");
  }

  void test_resolve_env_file_fails_for_empty_base_dir()
  {
    vix::env::EnvFileOptions options;
    options.base_dir = "";
    options.filename = ".env";

    auto result = vix::env::resolve_env_file(options);
    assert_true(!result.ok(), "resolve_env_file should fail for empty base_dir");
  }

  void test_resolve_env_file_fails_for_empty_filename()
  {
    vix::env::EnvFileOptions options;
    options.base_dir = ".";
    options.filename = "";

    auto result = vix::env::resolve_env_file(options);
    assert_true(!result.ok(), "resolve_env_file should fail for empty filename");
  }

  void test_resolve_env_files_returns_single_file_in_single_mode()
  {
    vix::env::EnvFileOptions options;
    options.mode = vix::env::EnvFileMode::Single;
    options.base_dir = ".";
    options.filename = ".env";

    auto result = vix::env::resolve_env_files(options);
    assert_true(result.ok(), "resolve_env_files should succeed in single mode");

    const auto &paths = result.value();
    assert_true(paths.size() == 1, "single mode should return one path");
    assert_true(contains_suffix(paths[0], ".env"),
                "resolved path should end with .env");
  }

  void test_resolve_env_files_returns_layered_paths_in_order()
  {
    vix::env::EnvFileOptions options;
    options.mode = vix::env::EnvFileMode::Layered;
    options.base_dir = ".";
    options.filename = ".env";
    options.environment_name = "production";
    options.load_base_file = true;
    options.load_local_file = true;
    options.load_environment_file = true;
    options.load_environment_local_file = true;

    auto result = vix::env::resolve_env_files(options);
    assert_true(result.ok(), "resolve_env_files should succeed in layered mode");

    const auto &paths = result.value();
    assert_true(paths.size() == 4, "layered mode should return four paths");

    assert_true(contains_suffix(paths[0], ".env"),
                "first path should be .env");
    assert_true(contains_suffix(paths[1], ".env.local"),
                "second path should be .env.local");
    assert_true(contains_suffix(paths[2], ".env.production"),
                "third path should be .env.production");
    assert_true(contains_suffix(paths[3], ".env.production.local"),
                "fourth path should be .env.production.local");
  }

  void test_resolve_env_files_fails_when_environment_name_is_missing()
  {
    vix::env::EnvFileOptions options;
    options.mode = vix::env::EnvFileMode::Layered;
    options.base_dir = ".";
    options.filename = ".env";
    options.environment_name = "";
    options.load_base_file = true;
    options.load_local_file = false;
    options.load_environment_file = true;
    options.load_environment_local_file = false;

    auto result = vix::env::resolve_env_files(options);
    assert_true(!result.ok(),
                "resolve_env_files should fail when environment_name is required but empty");
  }

  void test_resolve_env_files_can_resolve_only_selected_variants()
  {
    vix::env::EnvFileOptions options;
    options.mode = vix::env::EnvFileMode::Layered;
    options.base_dir = ".";
    options.filename = ".env";
    options.environment_name = "test";
    options.load_base_file = false;
    options.load_local_file = true;
    options.load_environment_file = true;
    options.load_environment_local_file = false;

    auto result = vix::env::resolve_env_files(options);
    assert_true(result.ok(), "resolve_env_files should succeed");

    const auto &paths = result.value();
    assert_true(paths.size() == 2, "only selected variants should be returned");
    assert_true(contains_suffix(paths[0], ".env.local"),
                "first path should be .env.local");
    assert_true(contains_suffix(paths[1], ".env.test"),
                "second path should be .env.test");
  }

  void test_resolve_env_files_can_return_empty_list_when_nothing_selected()
  {
    vix::env::EnvFileOptions options;
    options.mode = vix::env::EnvFileMode::Layered;
    options.base_dir = ".";
    options.filename = ".env";
    options.load_base_file = false;
    options.load_local_file = false;
    options.load_environment_file = false;
    options.load_environment_local_file = false;

    auto result = vix::env::resolve_env_files(options);
    assert_true(result.ok(), "resolve_env_files should succeed");
    assert_true(result.value().empty(),
                "no selected variants should produce an empty list");
  }
} // namespace

int main()
{
  test_resolve_env_file_resolves_primary_file();
  test_resolve_env_file_fails_for_empty_base_dir();
  test_resolve_env_file_fails_for_empty_filename();
  test_resolve_env_files_returns_single_file_in_single_mode();
  test_resolve_env_files_returns_layered_paths_in_order();
  test_resolve_env_files_fails_when_environment_name_is_missing();
  test_resolve_env_files_can_resolve_only_selected_variants();
  test_resolve_env_files_can_return_empty_list_when_nothing_selected();

  std::cout << "vix_env_resolve_env_file_test passed\n";
  return 0;
}
