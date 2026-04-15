/**
 *
 *  @file get_uint_test.cpp
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

#include <vix/env/GetUint.hpp>
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

  void test_get_uint_parses_valid_unsigned_integer()
  {
    const auto set_error = vix::env::set("VIX_ENV_UINT_TEST", "42");
    assert_true(!set_error, "set should succeed");

    auto result = vix::env::get_uint("VIX_ENV_UINT_TEST");
    assert_true(result.ok(), "get_uint should succeed for valid unsigned integer");
    assert_true(result.value() == 42u, "parsed unsigned int should match");

    const auto unset_error = vix::env::unset("VIX_ENV_UINT_TEST");
    assert_true(!unset_error, "unset should succeed");
  }

  void test_get_uint_ignores_surrounding_spaces()
  {
    const auto set_error = vix::env::set("VIX_ENV_UINT_TEST", "  900  ");
    assert_true(!set_error, "set should succeed");

    auto result = vix::env::get_uint("VIX_ENV_UINT_TEST");
    assert_true(result.ok(), "get_uint should succeed with surrounding spaces");
    assert_true(result.value() == 900u, "parsed unsigned int should match");

    const auto unset_error = vix::env::unset("VIX_ENV_UINT_TEST");
    assert_true(!unset_error, "unset should succeed");
  }

  void test_get_uint_fails_for_negative_value()
  {
    const auto set_error = vix::env::set("VIX_ENV_UINT_TEST", "-1");
    assert_true(!set_error, "set should succeed");

    auto result = vix::env::get_uint("VIX_ENV_UINT_TEST");
    assert_true(!result.ok(), "get_uint should fail for negative value");

    const auto unset_error = vix::env::unset("VIX_ENV_UINT_TEST");
    assert_true(!unset_error, "unset should succeed");
  }

  void test_get_uint_fails_for_invalid_value()
  {
    const auto set_error = vix::env::set("VIX_ENV_UINT_TEST", "10abc");
    assert_true(!set_error, "set should succeed");

    auto result = vix::env::get_uint("VIX_ENV_UINT_TEST");
    assert_true(!result.ok(), "get_uint should fail for invalid value");

    const auto unset_error = vix::env::unset("VIX_ENV_UINT_TEST");
    assert_true(!unset_error, "unset should succeed");
  }
} // namespace

int main()
{
  test_get_uint_parses_valid_unsigned_integer();
  test_get_uint_ignores_surrounding_spaces();
  test_get_uint_fails_for_negative_value();
  test_get_uint_fails_for_invalid_value();

  std::cout << "vix_env_get_uint_test passed\n";
  return 0;
}
