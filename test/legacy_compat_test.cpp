#include <cmath>
#include <cstdlib>
#include <iostream>
#include <string>

#include <vix/env/Legacy.hpp>
#include <vix/env/Set.hpp>
#include <vix/env/Unset.hpp>

namespace
{
  constexpr const char *key = "VIX_ENV_LEGACY_COMPAT_TEST";

  void assert_true(bool condition, const std::string &message)
  {
    if (!condition)
    {
      std::cerr << "Assertion failed: " << message << '\n';
      std::exit(1);
    }
  }

  void set_value(const char *value)
  {
    const auto error = vix::env::set(key, value);
    assert_true(!error, "set should succeed");
  }

  void clear_value()
  {
    const auto error = vix::env::unset(key);
    assert_true(!error, "unset should succeed");
  }

  void test_lookup_and_fallback_behavior()
  {
    clear_value();
    assert_true(vix::env::legacy::getenv(key) == nullptr,
                "missing variable should return null");
    assert_true(vix::env::legacy::env_or(key, "fallback") == "fallback",
                "missing variable should use string fallback");
    assert_true(vix::env::legacy::env_bool(key, true),
                "missing bool should use true fallback");
    assert_true(vix::env::legacy::env_int(key, 17) == 17,
                "missing int should use fallback");
    assert_true(vix::env::legacy::env_uint(key, 23u) == 23u,
                "missing unsigned should use fallback");
    assert_true(vix::env::legacy::env_double(key, 1.5) == 1.5,
                "missing double should use fallback");

    set_value("value");
    const char *raw = vix::env::legacy::getenv(key);
    assert_true(raw != nullptr && std::string(raw) == "value",
                "present variable should be returned");
    assert_true(vix::env::legacy::env_or(key, "fallback") == "value",
                "present variable should override fallback");

    set_value("");
#if defined(_WIN32)
    assert_true(vix::env::legacy::getenv(key) == nullptr,
                "Windows compatibility treats an empty variable as absent");
    assert_true(vix::env::legacy::env_or(key, "fallback") == "fallback",
                "Windows empty variable should use string fallback");
    assert_true(vix::env::legacy::env_bool(key, true),
                "Windows empty bool should use fallback");
#else
    const char *empty = vix::env::legacy::getenv(key);
    assert_true(empty != nullptr && std::string(empty).empty(),
                "POSIX compatibility retains a present empty variable");
    assert_true(vix::env::legacy::env_or(key, "fallback").empty(),
                "POSIX empty variable should not use string fallback");
    assert_true(!vix::env::legacy::env_bool(key, true),
                "POSIX empty bool should evaluate false");
#endif
    assert_true(vix::env::legacy::env_int(key, 17) == 17,
                "empty int should use fallback");
    assert_true(vix::env::legacy::env_uint(key, 23u) == 23u,
                "empty unsigned should use fallback");
    assert_true(vix::env::legacy::env_double(key, 1.5) == 1.5,
                "empty double should use fallback");
  }

  void test_boolean_behavior()
  {
    const char *truthy[] = {"1", "true", "TRUE", "yes", "On", " on "};
    for (const char *value : truthy)
    {
      set_value(value);
      assert_true(vix::env::legacy::env_bool(key),
                  "truthy spelling should evaluate true");
    }

    const char *falsy[] = {"0", "false", "FALSE", "no", "Off", " off "};
    for (const char *value : falsy)
    {
      set_value(value);
      assert_true(!vix::env::legacy::env_bool(key, true),
                  "falsy spelling should evaluate false");
    }

    set_value("not-a-bool");
    assert_true(!vix::env::legacy::env_bool(key, true),
                "invalid bool remains false rather than using fallback");
  }

  void test_numeric_behavior()
  {
    set_value(" -42 ");
    assert_true(vix::env::legacy::env_int(key, 9) == -42,
                "trimmed signed integer should parse");
    set_value("999999999999999999999999");
    assert_true(vix::env::legacy::env_int(key, 9) == 9,
                "signed overflow should use fallback");
    set_value("12x");
    assert_true(vix::env::legacy::env_int(key, 9) == 9,
                "malformed signed integer should use fallback");

    set_value(" 42 ");
    assert_true(vix::env::legacy::env_uint(key, 9u) == 42u,
                "trimmed unsigned integer should parse");
    set_value("-1");
    assert_true(vix::env::legacy::env_uint(key, 9u) == 9u,
                "negative unsigned integer should use fallback");
    set_value("999999999999999999999999");
    assert_true(vix::env::legacy::env_uint(key, 9u) == 9u,
                "unsigned overflow should use fallback");

    set_value(" 2.5 ");
    assert_true(vix::env::legacy::env_double(key, 9.0) == 2.5,
                "trimmed double should parse");
    set_value("2.5x");
    assert_true(vix::env::legacy::env_double(key, 9.0) == 9.0,
                "malformed double should use fallback");
    set_value("1e9999");
    assert_true(std::isinf(vix::env::legacy::env_double(key, 9.0)),
                "fully consumed floating overflow retains strtod behavior");
  }
} // namespace

int main()
{
  test_lookup_and_fallback_behavior();
  test_boolean_behavior();
  test_numeric_behavior();
  clear_value();

  std::cout << "vix_env_legacy_compat_test passed\n";
  return 0;
}
