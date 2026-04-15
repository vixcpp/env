/**
 *
 *  @file set_test.cpp
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

  void test_set_creates_variable()
  {
    const auto unset_error = vix::env::unset("VIX_ENV_SET_TEST");
    assert_true(!unset_error, "unset should succeed");

    const auto set_error = vix::env::set("VIX_ENV_SET_TEST", "hello");
    assert_true(!set_error, "set should succeed");

    assert_true(vix::env::has("VIX_ENV_SET_TEST"), "variable should exist after set");

    auto result = vix::env::get("VIX_ENV_SET_TEST");
    assert_true(result.ok(), "get should succeed after set");
    assert_true(result.value() == "hello", "stored value should match");

    const auto cleanup_error = vix::env::unset("VIX_ENV_SET_TEST");
    assert_true(!cleanup_error, "cleanup unset should succeed");
  }

  void test_set_overwrites_when_allowed()
  {
    auto set_error = vix::env::set("VIX_ENV_SET_TEST", "first");
    assert_true(!set_error, "initial set should succeed");

    set_error = vix::env::set("VIX_ENV_SET_TEST", "second", true);
    assert_true(!set_error, "set with overwrite=true should succeed");

    auto result = vix::env::get("VIX_ENV_SET_TEST");
    assert_true(result.ok(), "get should succeed");
    assert_true(result.value() == "second", "value should be overwritten");

    const auto unset_error = vix::env::unset("VIX_ENV_SET_TEST");
    assert_true(!unset_error, "unset should succeed");
  }

  void test_set_does_not_overwrite_when_disabled()
  {
    auto set_error = vix::env::set("VIX_ENV_SET_TEST", "first");
    assert_true(!set_error, "initial set should succeed");

    set_error = vix::env::set("VIX_ENV_SET_TEST", "second", false);
    assert_true(!set_error, "set with overwrite=false should not fail");

    auto result = vix::env::get("VIX_ENV_SET_TEST");
    assert_true(result.ok(), "get should succeed");
    assert_true(result.value() == "first", "value should remain unchanged");

    const auto unset_error = vix::env::unset("VIX_ENV_SET_TEST");
    assert_true(!unset_error, "unset should succeed");
  }

  void test_set_fails_for_empty_key()
  {
    const auto set_error = vix::env::set("", "value");
    assert_true(set_error.has_error(), "set should fail for empty key");
  }

  void test_set_fails_for_invalid_key()
  {
    const auto set_error = vix::env::set("INVALID-KEY", "value");
    assert_true(set_error.has_error(), "set should fail for invalid key");
  }
} // namespace

int main()
{
  test_set_creates_variable();
  test_set_overwrites_when_allowed();
  test_set_does_not_overwrite_when_disabled();
  test_set_fails_for_empty_key();
  test_set_fails_for_invalid_key();

  std::cout << "vix_env_set_test passed\n";
  return 0;
}
