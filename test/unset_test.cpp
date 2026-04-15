/**
 *
 *  @file unset_test.cpp
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

  void test_unset_removes_existing_variable()
  {
    auto set_error = vix::env::set("VIX_ENV_UNSET_TEST", "value");
    assert_true(!set_error, "set should succeed");

    assert_true(vix::env::has("VIX_ENV_UNSET_TEST"), "variable should exist before unset");

    auto unset_error = vix::env::unset("VIX_ENV_UNSET_TEST");
    assert_true(!unset_error, "unset should succeed");

    assert_true(!vix::env::has("VIX_ENV_UNSET_TEST"), "variable should not exist after unset");

    auto result = vix::env::get("VIX_ENV_UNSET_TEST");
    assert_true(!result.ok(), "get should fail after unset");
  }

  void test_unset_succeeds_for_missing_variable()
  {
    auto unset_error = vix::env::unset("VIX_ENV_UNSET_TEST_MISSING");
    assert_true(!unset_error, "unset should succeed for missing variable");

    assert_true(!vix::env::has("VIX_ENV_UNSET_TEST_MISSING"),
                "missing variable should still be absent");
  }

  void test_unset_fails_for_empty_key()
  {
    auto unset_error = vix::env::unset("");
    assert_true(unset_error.has_error(), "unset should fail for empty key");
  }

  void test_unset_fails_for_invalid_key()
  {
    auto unset_error = vix::env::unset("INVALID-KEY");
    assert_true(unset_error.has_error(), "unset should fail for invalid key");
  }
} // namespace

int main()
{
  test_unset_removes_existing_variable();
  test_unset_succeeds_for_missing_variable();
  test_unset_fails_for_empty_key();
  test_unset_fails_for_invalid_key();

  std::cout << "vix_env_unset_test passed\n";
  return 0;
}
