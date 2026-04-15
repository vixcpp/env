/**
 *
 *  @file get_bool_test.cpp
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

#include <vix/env/GetBool.hpp>
#include <vix/env/Set.hpp>
#include <vix/env/Unset.hpp>

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

  void test_get_bool_accepts_truthy_values()
  {
    const char *truthy_values[] = {"1", "true", "TRUE", "yes", "on", " On "};

    for (const char *value : truthy_values)
    {
      const auto set_error = vix::env::set("VIX_ENV_BOOL_TEST", value);
      assert_true(!set_error, "set should succeed");

      auto result = vix::env::get_bool("VIX_ENV_BOOL_TEST");
      assert_true(result.ok(), "get_bool should succeed for truthy value");
      assert_true(result.value() == true, "truthy value should parse to true");
    }

    const auto unset_error = vix::env::unset("VIX_ENV_BOOL_TEST");
    assert_true(!unset_error, "unset should succeed");
  }

  void test_get_bool_accepts_falsy_values()
  {
    const char *falsy_values[] = {"0", "false", "FALSE", "no", "off", " Off "};

    for (const char *value : falsy_values)
    {
      const auto set_error = vix::env::set("VIX_ENV_BOOL_TEST", value);
      assert_true(!set_error, "set should succeed");

      auto result = vix::env::get_bool("VIX_ENV_BOOL_TEST");
      assert_true(result.ok(), "get_bool should succeed for falsy value");
      assert_true(result.value() == false, "falsy value should parse to false");
    }

    const auto unset_error = vix::env::unset("VIX_ENV_BOOL_TEST");
    assert_true(!unset_error, "unset should succeed");
  }

  void test_get_bool_fails_for_invalid_value()
  {
    const auto set_error = vix::env::set("VIX_ENV_BOOL_TEST", "maybe");
    assert_true(!set_error, "set should succeed");

    auto result = vix::env::get_bool("VIX_ENV_BOOL_TEST");
    assert_true(!result.ok(), "get_bool should fail for invalid bool value");

    const auto unset_error = vix::env::unset("VIX_ENV_BOOL_TEST");
    assert_true(!unset_error, "unset should succeed");
  }

  void test_get_bool_fails_for_missing_variable()
  {
    const auto unset_error = vix::env::unset("VIX_ENV_BOOL_MISSING");
    assert_true(!unset_error, "unset should succeed");

    auto result = vix::env::get_bool("VIX_ENV_BOOL_MISSING");
    assert_true(!result.ok(), "get_bool should fail for missing variable");
  }
} // namespace

int main()
{
  test_get_bool_accepts_truthy_values();
  test_get_bool_accepts_falsy_values();
  test_get_bool_fails_for_invalid_value();
  test_get_bool_fails_for_missing_variable();

  std::cout << "vix_env_get_bool_test passed\n";
  return 0;
}
