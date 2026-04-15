/**
 *
 *  @file has_test.cpp
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

#include <vix/env/Has.hpp>
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

  void test_has_returns_true_for_existing_variable()
  {
    const auto set_error = vix::env::set("VIX_ENV_HAS_TEST", "present");
    assert_true(!set_error, "set should succeed");

    const bool exists = vix::env::has("VIX_ENV_HAS_TEST");
    assert_true(exists, "has should return true for existing variable");

    const auto unset_error = vix::env::unset("VIX_ENV_HAS_TEST");
    assert_true(!unset_error, "unset should succeed");
  }

  void test_has_returns_false_for_missing_variable()
  {
    const auto unset_error = vix::env::unset("VIX_ENV_HAS_TEST_MISSING");
    assert_true(!unset_error, "unset should succeed");

    const bool exists = vix::env::has("VIX_ENV_HAS_TEST_MISSING");
    assert_true(!exists, "has should return false for missing variable");
  }

  void test_has_returns_false_for_empty_key()
  {
    const bool exists = vix::env::has("");
    assert_true(!exists, "has should return false for empty key");
  }

  void test_has_returns_false_for_invalid_key()
  {
    const bool exists = vix::env::has("INVALID-KEY");
    assert_true(!exists, "has should return false for invalid key");
  }
} // namespace

int main()
{
  test_has_returns_true_for_existing_variable();
  test_has_returns_false_for_missing_variable();
  test_has_returns_false_for_empty_key();
  test_has_returns_false_for_invalid_key();

  std::cout << "vix_env_has_test passed\n";
  return 0;
}
