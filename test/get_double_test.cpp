/**
 *
 *  @file get_double_test.cpp
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

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <string>

#include <vix/env/GetDouble.hpp>
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

  void assert_near(double actual, double expected, double epsilon, const std::string &message)
  {
    if (std::fabs(actual - expected) > epsilon)
    {
      std::cerr << "Assertion failed: " << message
                << " (expected " << expected
                << ", got " << actual << ")\n";
      std::exit(1);
    }
  }

  void test_get_double_parses_decimal_value()
  {
    const auto set_error = vix::env::set("VIX_ENV_DOUBLE_TEST", "3.14159");
    assert_true(!set_error, "set should succeed");

    auto result = vix::env::get_double("VIX_ENV_DOUBLE_TEST");
    assert_true(result.ok(), "get_double should succeed for decimal value");
    assert_near(result.value(), 3.14159, 1e-12, "parsed double should match");

    const auto unset_error = vix::env::unset("VIX_ENV_DOUBLE_TEST");
    assert_true(!unset_error, "unset should succeed");
  }

  void test_get_double_parses_negative_value()
  {
    const auto set_error = vix::env::set("VIX_ENV_DOUBLE_TEST", "-42.5");
    assert_true(!set_error, "set should succeed");

    auto result = vix::env::get_double("VIX_ENV_DOUBLE_TEST");
    assert_true(result.ok(), "get_double should succeed for negative value");
    assert_near(result.value(), -42.5, 1e-12, "parsed double should match");

    const auto unset_error = vix::env::unset("VIX_ENV_DOUBLE_TEST");
    assert_true(!unset_error, "unset should succeed");
  }

  void test_get_double_ignores_surrounding_spaces()
  {
    const auto set_error = vix::env::set("VIX_ENV_DOUBLE_TEST", "  2.5  ");
    assert_true(!set_error, "set should succeed");

    auto result = vix::env::get_double("VIX_ENV_DOUBLE_TEST");
    assert_true(result.ok(), "get_double should succeed with surrounding spaces");
    assert_near(result.value(), 2.5, 1e-12, "parsed double should match");

    const auto unset_error = vix::env::unset("VIX_ENV_DOUBLE_TEST");
    assert_true(!unset_error, "unset should succeed");
  }

  void test_get_double_fails_for_invalid_value()
  {
    const auto set_error = vix::env::set("VIX_ENV_DOUBLE_TEST", "3.14abc");
    assert_true(!set_error, "set should succeed");

    auto result = vix::env::get_double("VIX_ENV_DOUBLE_TEST");
    assert_true(!result.ok(), "get_double should fail for invalid value");

    const auto unset_error = vix::env::unset("VIX_ENV_DOUBLE_TEST");
    assert_true(!unset_error, "unset should succeed");
  }
} // namespace

int main()
{
  test_get_double_parses_decimal_value();
  test_get_double_parses_negative_value();
  test_get_double_ignores_surrounding_spaces();
  test_get_double_fails_for_invalid_value();

  std::cout << "vix_env_get_double_test passed\n";
  return 0;
}
