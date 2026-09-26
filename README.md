# knn-engine

A modern C++20 nearest-neighbor engine built from scratch with an emphasis on correctness, performance, concurrency, and systems engineering.

The project begins with exact K-nearest-neighbor classification and is designed to evolve through measured engineering work into a broader nearest-neighbor and vector-search system.

## Goals

The project is used to explore and implement:

* idiomatic modern C++
* nearest-neighbor algorithms and data structures
* concurrency and parallel query execution
* performance engineering and profiling
* Linux systems programming
* network services and asynchronous I/O
* machine learning and computer vision workloads
* vector search and learned embeddings
* robustness, fuzzing, and defensive engineering
* latency-sensitive system design

Correctness and measurement come before optimization.

## Current Status

The engineering bootstrap and baseline system specification are complete.

Implementation of the KNN engine has not yet started.

The repository contains:

* C++20 CMake configuration
* GCC and Clang build validation
* compiler warning configuration
* clang-format
* clang-tidy
* automated tests
* Linux CI
* engineering decision records
* experiment documentation
* project roadmap and exploration backlog

KNN functionality has not yet been implemented.

## Build

Configure a debug build:

```bash
cmake --preset debug
```

Build:

```bash
cmake --build --preset debug
```

Run tests:

```bash
ctest --preset debug
```

## Code Quality

Check formatting:

```bash
clang-format --dry-run --Werror tests/toolchain_smoke.cpp
```

Run static analysis:

```bash
./scripts/run-clang-tidy.sh
```

## Documentation

Project documentation is maintained under `docs/`.

* `docs/vision.md` defines the engineering direction and principles.
* `docs/roadmap.md` defines the current milestone sequence.
* `docs/exploration-backlog.md` preserves technical directions that may be explored later.
* `docs/decisions/` contains Architectural Decision Records.
* `docs/experiments/` contains documented technical experiments.
* `docs/spec/` will contain the system specification.

## Development Approach

The project follows several principles:

* maintain a correct baseline before optimizing
* measure performance rather than assume it
* document meaningful technical decisions
* keep `main` buildable
* introduce complexity incrementally
* compare alternative designs through reproducible experiments
* use abstractions only when they solve an actual engineering problem

## Language and Tooling

* C++20
* CMake
* GCC / Clang
* clang-format
* clang-tidy
* CTest
* GitHub Actions

## Roadmap

Development is organized into milestones covering:

1. project specification
2. single-threaded KNN core
3. concurrency
4. search algorithms
5. networking
6. Linux asynchronous I/O
7. performance engineering
8. computer vision and machine learning
9. vector search and deep embeddings
10. robustness and security
11. latency-sensitive engineering

See [`docs/roadmap.md`](docs/roadmap.md) for the current plan.
