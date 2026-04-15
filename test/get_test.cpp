/**
 *
 *  @file get_test.cpp
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

#include <vix/env/Get.hpp>
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

  void test_get_returns_value_when_variable_exists()
  {
    const auto set_error = vix::env::set("VIX_ENV_GET_TEST_VALUE", "hello");
    assert_true(!set_error, "set should succeed");

    auto result = vix::env::get("VIX_ENV_GET_TEST_VALUE");
    assert_true(result.ok(), "get should succeed for existing variable");
    assert_true(result.value() == "hello", "value should match");

    const auto unset_error = vix::env::unset("VIX_ENV_GET_TEST_VALUE");
    assert_true(!unset_error, "unset should succeed");
  }

  void test_get_fails_when_variable_does_not_exist()
  {
    const auto unset_error = vix::env::unset("VIX_ENV_GET_TEST_MISSING");
    assert_true(!unset_error, "unset should succeed even if variable is absent");

    auto result = vix::env::get("VIX_ENV_GET_TEST_MISSING");
    assert_true(!result.ok(), "get should fail for missing variable");
  }

  void test_get_fails_for_empty_key()
  {
    auto result = vix::env::get("");
    assert_true(!result.ok(), "get should fail for empty key");
  }

  void test_get_fails_for_invalid_key()
  {
    auto result = vix::env::get("INVALID-KEY");
    assert_true(!result.ok(), "get should fail for invalid key");
  }
} // namespace

int main()
{
  test_get_returns_value_when_variable_exists();
  test_get_fails_when_variable_does_not_exist();
  test_get_fails_for_empty_key();
  test_get_fails_for_invalid_key();

  std::cout << "vix_env_get_test passed\n";
  return 0;
}
