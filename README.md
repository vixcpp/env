# Vix Env

The **env module** provides a modern, consistent, and cross-platform API
for working with environment variables and `.env` files in C++.

It is designed to be:

- simple to use
- explicit (no hidden behavior)
- predictable across platforms
- aligned with modern developer workflows

---

## Philosophy

Environment handling in Vix follows strict principles:

- no global magic
- explicit loading
- explicit parsing
- structured errors (`Result<T>`)
- layered configuration support

The goal is to bring a Node.js / Deno-like DX
to environment management in C++ without breaking C++ principles.

---

## Install

Using Vix:

```bash
vix add @vix/env
```

---

## Quick Start

```cpp
#include <vix/env/Env.hpp>
#include <iostream>

int main()
{
  auto app = vix::env::get_or("APP_NAME", "Vix");
  std::cout << "APP_NAME = " << app << std::endl;
}
```

---

## Basic API

### Check existence

```cpp
vix::env::has("PORT");
```

### Get value

```cpp
auto result = vix::env::get("PORT");

if (result)
{
  std::cout << result.value();
}
```

### Get with default

```cpp
auto port = vix::env::get_or("PORT", "3000");
```

### Set variable

```cpp
vix::env::set("APP_ENV", "production");
```

Overwrite control:

```cpp
vix::env::set("APP_ENV", "production", true);
```

### Unset variable

```cpp
vix::env::unset("APP_ENV");
```

---

## Typed Access

### Boolean

```cpp
auto debug = vix::env::get_bool("APP_DEBUG");

if (debug && debug.value())
{
  // debug mode
}
```

Accepted values:

- true: 1, true, yes, on
- false: 0, false, no, off

### Integer

```cpp
auto port = vix::env::get_int("PORT");
```

### Unsigned Integer

```cpp
auto workers = vix::env::get_uint("WORKERS");
```

### Double

```cpp
auto ratio = vix::env::get_double("RATIO");
```

---

## Working with .env files

### Load a single file

```cpp
auto file = vix::env::load_file(".env");

if (file)
{
  for (const auto &entry : file.value().entries)
  {
    std::cout << entry.key << " = " << entry.value << "\n";
  }
}
```

### Load into process

```cpp
auto err = vix::env::load_into_process(".env");

if (err)
{
  std::cerr << err.message();
}
```

---

## Layered .env support

Vix supports modern layered configuration:

- `.env`
- `.env.local`
- `.env.production`
- `.env.production.local`

### Load layered files

```cpp
vix::env::EnvFileOptions options;

options.mode = vix::env::EnvFileMode::Layered;
options.environment_name = "production";
options.ignore_missing_files = true;

auto files = vix::env::load_layered(options);
```

### Load layered into process

```cpp
auto err = vix::env::load_layered_into_process(options);
```

### Loading order

- `.env`
- `.env.local`
- `.env.production`
- `.env.production.local`

Later files override earlier ones.

---

## Configuration

```cpp
vix::env::EnvFileOptions options;

options.base_dir = ".";
options.filename = ".env";
options.environment_name = "production";
options.mode = vix::env::EnvFileMode::Layered;

options.load_base_file = true;
options.load_local_file = true;
options.load_environment_file = true;
options.load_environment_local_file = true;

options.ignore_missing_files = true;
options.env.overwrite_existing = true;
```

---

## Error Handling

All operations use `Result<T>` or `Error`.

```cpp
auto result = vix::env::get("PORT");

if (!result)
{
  std::cerr << result.error().message();
}
```

---

## Parsing Rules

- supports `KEY=value`
- supports `export KEY=value`
- ignores empty lines
- ignores `#` comments

Supports inline comments:

```
APP_NAME=vix # comment
```

Supports quoted values:

```
TITLE="Hello World"
```

---

## Design Notes

- no filesystem access in parsing logic
- `.env` loading is explicit (never automatic)
- environment injection is controlled (`overwrite_existing`)
- layered resolution is deterministic
- cross-platform (POSIX + Windows)

---

## Module Dependencies

- `vix_error`
- `vix_path`

---

## Example Use Case

```cpp
vix::env::EnvFileOptions options;
options.environment_name = "production";

vix::env::load_layered_into_process(options);

auto port = vix::env::get_or("PORT", "8080");
```

---

## Summary

The env module gives you:

- simple env access
- typed values
- `.env` parsing
- layered configuration
- process injection

All with:

- explicit behavior
- strong error handling
- zero magic

