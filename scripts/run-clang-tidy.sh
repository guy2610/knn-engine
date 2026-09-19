#!/usr/bin/env bash

set -euo pipefail

BUILD_DIR="${1:-build/debug}"

if ! command -v clang-tidy >/dev/null 2>&1; then
    echo "error: clang-tidy was not found in PATH" >&2
    exit 1
fi

if [[ ! -f "${BUILD_DIR}/compile_commands.json" ]]; then
    echo "error: ${BUILD_DIR}/compile_commands.json was not found" >&2
    echo "run CMake configure first" >&2
    exit 1
fi

EXTRA_ARGS=()

if [[ "$(uname -s)" == "Darwin" ]]; then
    if ! command -v xcrun >/dev/null 2>&1; then
        echo "error: xcrun was not found" >&2
        exit 1
    fi

    SDK_PATH="$(xcrun --show-sdk-path)"
    EXTRA_ARGS+=(
        "--extra-arg=-isysroot"
        "--extra-arg=${SDK_PATH}"
    )
fi

clang-tidy \
    tests/toolchain_smoke.cpp \
    -p "${BUILD_DIR}" \
    "${EXTRA_ARGS[@]}"