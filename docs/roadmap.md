# Project Roadmap

The roadmap describes the current intended development sequence.

It is not a permanent specification. Milestones may change when implementation experience or experimental evidence gives us a reason to change them.

Ideas that are not currently scheduled belong in `exploration-backlog.md`.

## M-1 — Repository & Engineering Bootstrap

Establish the project environment before implementing product functionality.

Goals:

* initialize the Git repository
* establish a C++20 CMake project
* configure compiler warnings
* configure clang-format
* configure clang-tidy
* add a toolchain smoke test
* add Linux CI using GCC and Clang
* establish project documentation
* establish architectural decision records
* establish experiment documentation
* preserve future technical ideas in an exploration backlog

Exit criteria:

* fresh configure succeeds
* project builds successfully
* tests pass
* formatting checks pass
* static analysis runs successfully
* Linux CI is green
* project direction and engineering practices are documented

## M0 — Project Specification

Extract the functional requirements from the original assignment and define the independent implementation.

Goals:

* document original assignment behavior
* define supported workflows
* define input and output formats
* define functional requirements
* define error behavior
* identify constraints and assumptions
* distinguish baseline requirements from extensions
* define initial correctness criteria

No detailed class architecture should be assumed before the requirements are understood.

## M1 — Correct Single-Threaded KNN Core

Build a correct single-threaded nearest-neighbor classification engine.

Initial areas:

* dataset representation
* labeled and unlabeled samples
* distance metrics
* neighbor selection
* voting and classification
* configuration
* validation
* unit tests

Supported metrics from the original assignment should initially include:

* Euclidean
* Manhattan
* Chebyshev
* Canberra
* Minkowski

A basic benchmark baseline should be introduced here so later changes can be measured against known behavior.

## M2 — Concurrency Foundations

Introduce concurrency independently of networking.

Goals:

* execute independent queries concurrently
* share immutable dataset state safely
* introduce a worker pool
* introduce work queues
* understand synchronization requirements
* investigate cancellation and shutdown
* test concurrency behavior
* use race-detection tooling where practical

The single-threaded implementation remains the correctness and performance baseline.

## M3 — Search Algorithms

Explore alternative strategies for finding nearest neighbors.

Candidates include:

* complete sorting
* bounded heap
* `std::nth_element`
* spatial indexing structures

Algorithms should be compared for:

* correctness
* computational complexity
* latency
* memory behavior
* scalability with dataset size
* scalability with vector dimensionality
* interaction with concurrency

## M4 — Network Service

Expose the engine through a networked C++ service.

Initial direction:

* TCP client/server
* request framing
* message types
* request identifiers
* validation
* partial read/write handling
* structured error responses
* connection lifecycle management

The first implementation should favor clarity over asynchronous complexity.

## M5 — Linux Async I/O

Evolve the network layer toward Linux systems programming.

Possible areas:

* non-blocking sockets
* file descriptors
* `epoll`
* event-driven connection handling
* connection state machines
* integration between I/O and worker execution
* graceful shutdown
* signals and resource handling

Architectural comparisons should be measured rather than assumed.

## M6 — Performance Engineering

Perform deeper profiling and optimization after the system has meaningful workloads.

Areas may include:

* CPU profiling
* allocation analysis
* memory layout
* cache behavior
* contention
* batching
* data locality
* vectorization
* SIMD
* memory mapping

Optimizations should document both the expected mechanism and measured result.

## M7 — Computer Vision & Machine Learning

Use the nearest-neighbor engine with image-based data.

Possible representations:

* raw pixels
* normalized pixel vectors
* handcrafted OpenCV features
* HOG or similar descriptors

Evaluate both classification quality and computational behavior.

## M8 — Deep Embeddings & Vector Search

Use pretrained neural networks to produce vector representations.

Possible pipeline:

Image
→ pretrained model
→ embedding
→ nearest-neighbor engine
→ classification or similarity result

Areas for exploration:

* embedding dimensionality
* classification quality
* similarity search
* exact versus approximate search
* index construction
* recall versus latency tradeoffs

## M9 — Robustness & Security

Strengthen the service against malformed or hostile inputs.

Areas may include:

* strict input validation
* protocol fuzzing
* malformed request handling
* request-size limits
* memory limits
* connection limits
* timeouts
* resource exhaustion
* sanitizers
* denial-of-service resistance

Authentication or TLS may be considered if they solve a meaningful requirement.

## M10 — Latency-Sensitive Engineering

Study predictable behavior under load.

Areas may include:

* P50, P95 and P99 latency
* bounded work queues
* backpressure
* deadlines
* cancellation
* overload handling
* load shedding
* request prioritization
* contention analysis
* scheduling behavior

This milestone concerns latency-sensitive engineering, not hard real-time guarantees.

## Roadmap Policy

Each milestone should be refined shortly before implementation.

We should avoid creating detailed issues for distant milestones when implementation experience may invalidate those plans.

The roadmap answers:

"What are we currently planning to build?"

The exploration backlog answers:

"What might be worth investigating later?"
