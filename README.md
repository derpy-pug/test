# test

A small C++ JSON API built with [Drogon](https://github.com/drogonframework/drogon),
built with CMake and tested with CTest + GoogleTest. No database yet.

## API

| Method | Path                   | Body                     | Response                                  |
|--------|------------------------|--------------------------|--------------------------------------------|
| GET    | `/api/greeting`        | —                        | `200 {"message": "Hello, World!"}`         |
| GET    | `/api/greeting?name=X` | —                        | `200 {"message": "Hello, X!"}`             |
| POST   | `/api/greeting`        | `{"name": "X"}`          | `201 {"message": "Hello, X!"}`             |

Both endpoints return `400 {"error": "..."}` for an empty/whitespace-only
name, a name over 100 characters, a missing/non-string `name` field, or a
malformed JSON body.

## Building

```sh
cmake -S . -B build
cmake --build build -j
```

The first configure will fetch and build Drogon (and its `trantor` submodule)
and GoogleTest from source via `FetchContent` — this takes a few minutes the
first time, and is cached in `build/_deps` afterward.

### System dependencies

Drogon needs: OpenSSL, zlib, libuuid, and jsoncpp (headers + library) on the
build machine. On Debian/Ubuntu:

```sh
apt-get install -y libssl-dev zlib1g-dev uuid-dev libjsoncpp-dev
```

## Running

```sh
./build/src/api_server
# listens on 0.0.0.0:8848
```

## Testing

```sh
ctest --test-dir build --output-on-failure
```

Two test binaries:
- `unit_tests` — pure logic (`GreetingService`), no server involved.
- `integration_tests` — boots the real app on a loopback test port and
  exercises it over real HTTP with Drogon's `HttpClient`.

## Workflow

This repo uses feature branches merged into `main` via pull request — no
direct commits to `main`. GitHub Actions CI is coming in a later step.
