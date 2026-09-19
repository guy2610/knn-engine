# Exploration Backlog

This document preserves technical directions, experiments, and extension ideas that may be worth exploring during the lifetime of the project.

Items in this document are not commitments and are not ordered by priority.

Moving an item into the roadmap should require a concrete reason, such as a learning objective, an engineering need, an observed bottleneck, or an experiment worth running.

## C++ and Software Design

* Explore value semantics versus polymorphic interfaces for search strategies.
* Use concepts where they express meaningful compile-time requirements.
* Evaluate `std::span` for non-owning vector access.
* Explore ranges and standard algorithms where they improve clarity.
* Study move semantics in dataset and index construction.
* Investigate allocator behavior for large vector collections.
* Consider custom allocators or `std::pmr` if allocation behavior becomes relevant.
* Explore compile-time versus runtime selection of distance metrics.
* Investigate type-safe configuration rather than string-driven configuration.
* Explore error-handling models such as exceptions, result types, and `std::expected` when available or appropriate.

## Nearest-Neighbor Algorithms

### Exact Search

* Full distance computation followed by sorting.
* Bounded max-heap of size K.
* `std::nth_element`.
* Quickselect-style approaches.
* KD-tree.
* Ball tree.
* VP-tree.
* Other metric-tree structures.

### Approximate Search

* HNSW.
* Locality-sensitive hashing.
* Approximate tree-based methods.
* Graph-based ANN search.
* Compare custom implementations with established ANN libraries.

### Algorithmic Questions

* At what dimensionality does KD-tree performance degrade?
* How does K affect search cost?
* How does dataset size affect each strategy?
* What is the memory cost of indexing?
* What are index construction costs?
* When does approximate search become worthwhile?
* How should duplicate or equidistant neighbors be handled?
* How do different distance metrics affect index applicability?

## Distance Computation

* Euclidean distance.
* Squared Euclidean distance where square root is unnecessary.
* Manhattan distance.
* Chebyshev distance.
* Canberra distance.
* Minkowski distance.
* Cosine distance or similarity.
* Hamming distance for appropriate representations.
* Custom metric support.

Possible optimizations:

* eliminate unnecessary temporary objects
* reduce repeated computation
* vectorize distance calculations
* improve memory locality
* batch queries
* SIMD using compiler auto-vectorization
* explicit SIMD
* AVX/AVX2/AVX-512 where appropriate
* ARM NEON exploration
* compare array-of-structures and structure-of-arrays layouts

## Concurrency

* Concurrent independent queries.
* Shared immutable dataset.
* Fixed-size thread pool.
* Dynamically sized worker pool.
* Producer-consumer queues.
* Bounded queues.
* Lock-based queues.
* Lock-free structures where justified.
* `std::jthread`.
* `std::stop_token`.
* Cooperative cancellation.
* Graceful shutdown.
* Reader/writer synchronization.
* Atomics.
* False sharing.
* Cache-line alignment.
* Contention measurement.
* Work stealing.
* Task batching.
* Per-thread scratch buffers.
* Thread-local storage.
* CPU affinity.
* NUMA considerations if hardware permits.

Potential experiments:

* single-threaded baseline versus thread pool
* throughput versus worker count
* latency versus worker count
* oversubscription behavior
* small versus large query workloads
* impact of queue design
* scaling relative to physical and logical CPU cores

## Linux Systems Programming

* POSIX sockets.
* File descriptors.
* Blocking versus non-blocking I/O.
* `poll`.
* `select`.
* `epoll`.
* Signals.
* Graceful process shutdown.
* Resource limits.
* `mmap`.
* Memory-mapped datasets.
* File-backed indexes.
* Large-file handling.
* Zero-copy techniques where applicable.
* Page faults and memory residency.
* CPU affinity.
* Process and thread inspection through Linux tools.
* `/proc`-based diagnostics where useful.

## Networking

* TCP client/server architecture.
* Binary protocol.
* Text protocol baseline for comparison.
* Message framing.
* Length-prefix framing.
* Request IDs.
* Multiple requests per connection.
* Partial reads and writes.
* Connection state machines.
* Connection timeouts.
* Reconnection behavior.
* Protocol versioning.
* Structured error responses.
* Batch queries.
* Streaming large datasets.
* Client libraries.
* Backpressure between network and compute layers.

Potential comparison:

* thread-per-connection
* event-driven I/O plus worker pool

## Performance Engineering

### Measurement

* Microbenchmarks.
* End-to-end benchmarks.
* Throughput.
* P50 latency.
* P95 latency.
* P99 latency.
* Maximum latency.
* Memory usage.
* Allocation count.
* CPU utilization.
* Scaling efficiency.

### Tools

* Google Benchmark.
* Linux `perf`.
* Valgrind.
* Callgrind.
* heap profiling.
* compiler optimization reports.
* flame graphs.
* sanitizers.
* hardware performance counters where available.

### Areas to Investigate

* cache locality
* branch behavior
* memory bandwidth
* unnecessary copies
* dynamic allocations
* vector layout
* batching
* contention
* synchronization overhead
* compiler optimization levels
* link-time optimization
* profile-guided optimization
* SIMD
* memory mapping

Performance work should begin from a measured bottleneck rather than an assumed one.

## Machine Learning

* KNN classification baseline.
* Dataset normalization.
* Feature scaling.
* Train/test separation.
* Validation sets.
* K selection.
* Hyperparameter experiments.
* Confusion matrices.
* Accuracy.
* Precision and recall where appropriate.
* Class imbalance.
* Weighted KNN.
* Distance-weighted voting.
* Cross-validation.
* Comparison against simple reference models.

Possible reference models:

* logistic regression
* SVM
* decision trees
* small neural networks

The purpose of additional models would primarily be experimental comparison rather than turning the project into a general ML framework.

## Computer Vision

Possible image representations:

* raw pixels
* resized and normalized pixels
* color histograms
* edges
* HOG
* handcrafted OpenCV descriptors
* learned embeddings

Possible applications:

* image classification
* image similarity search
* nearest-image retrieval
* duplicate or near-duplicate detection

Experiments could compare classification and retrieval quality across different representations while using the same nearest-neighbor engine.

## Deep Learning

* Use pretrained models rather than initially training large models from scratch.
* ResNet embeddings.
* MobileNet embeddings.
* Vision Transformer embeddings.
* Compare embedding models.
* Study embedding dimensionality.
* Normalize embeddings.
* Explore cosine versus Euclidean distance.
* Batch inference.
* Separate inference from vector search.
* Investigate model-serving boundaries.

Possible pipeline:

Image
→ neural network
→ embedding vector
→ nearest-neighbor engine
→ classification or similarity results

Potential later exploration:

* fine-tuning
* metric learning
* contrastive learning
* triplet loss

## Vector Search

* Generalize beyond classification.
* Store arbitrary vectors and metadata.
* Top-K similarity queries.
* Index construction.
* Persistent indexes.
* Index loading.
* Incremental insertion.
* Deletion.
* Index rebuilding.
* Metadata filtering.
* Batch search.
* Exact versus approximate search.
* Recall-at-K measurements.

Possible long-term exploration:

* sharding
* distributed indexes
* replication
* routing queries across shards

These should only be explored if the single-node system creates a meaningful reason to do so.

## Python Integration

* Python bindings using pybind11.
* Expose the C++ search engine to Python.
* Python tooling for dataset preparation.
* Python experiment orchestration.
* Python visualization of benchmark results.
* Python ML/CV pipeline using the C++ engine underneath.

Potential experiment:

Compare Python-level KNN implementations against the C++ engine for identical workloads.

## Robustness and Security

* Strict parsing.
* Input validation.
* Integer-overflow checks.
* Bounds validation.
* Dataset dimension validation.
* Request-size limits.
* Dataset-size limits.
* Connection limits.
* Timeouts.
* Memory quotas.
* Malformed request handling.
* Unexpected disconnect handling.
* Fuzz testing.
* Protocol fuzzing.
* Property-based testing.
* AddressSanitizer.
* UndefinedBehaviorSanitizer.
* ThreadSanitizer.
* MemorySanitizer where practical.
* Resource-exhaustion testing.
* Basic denial-of-service resistance.

Possible later extensions:

* authentication
* authorization
* TLS

Security mechanisms should address an explicit threat model rather than being added decoratively.

## Latency-Sensitive Engineering

* Define latency objectives.
* Measure tail latency.
* Bounded queues.
* Backpressure.
* Request deadlines.
* Cancellation.
* Load shedding.
* Admission control.
* Request prioritization.
* Queue discipline.
* Overload behavior.
* Worker saturation.
* Contention under load.
* Predictability versus maximum throughput.

Potential experiments:

* throughput-oriented configuration versus latency-oriented configuration
* bounded versus unbounded queues
* behavior above saturation point
* impact of batch size on tail latency

This work should be described as latency-sensitive or soft-real-time engineering unless hard real-time guarantees can actually be demonstrated.

## GPU and Hardware Acceleration

* CUDA distance computation.
* GPU batch search.
* CPU versus GPU crossover point.
* Data-transfer overhead.
* GPU indexing approaches.
* SIMD CPU versus GPU comparison.
* Apple Metal exploration if useful for local experimentation.

GPU acceleration should only be introduced once CPU behavior is well understood.

## Storage and Persistence

* Binary dataset format.
* Persistent indexes.
* Memory-mapped vector storage.
* Metadata storage.
* Crash-safe index creation.
* Versioned storage formats.
* Efficient startup and loading.
* Dataset checksums.
* Large-dataset handling.

## Observability

* Structured logging.
* Log levels.
* Request IDs.
* Timing instrumentation.
* Metrics.
* Query counts.
* Error counts.
* Queue depth.
* Worker utilization.
* Latency histograms.
* Prometheus-compatible metrics.
* Debug diagnostics.

Instrumentation should be designed so that measurement itself does not dominate the workload being measured.

## APIs and Integration

Possible external interfaces:

* native C++ API
* TCP protocol
* Python bindings
* REST gateway
* gRPC

New interfaces should be added only when they demonstrate a useful boundary or enable a meaningful experiment.

## Deployment and Reproducibility

* Docker.
* Reproducible build environment.
* Linux development container.
* Benchmark dataset scripts.
* Automated benchmark runners.
* Release builds.
* Versioned releases.
* Artifact generation.
* Packaging.

## Testing

* Unit tests.
* Integration tests.
* End-to-end tests.
* Regression tests.
* Property-based tests.
* Fuzz tests.
* Concurrency stress tests.
* Performance regression tests.
* Protocol compatibility tests.

Possible future investigation:

* deterministic concurrency testing
* fault injection
* network failure simulation

## Engineering Process

* Architectural Decision Records.
* Experiment reports.
* Benchmark methodology documentation.
* Explicit assumptions.
* Definition of Done per milestone.
* Small reviewable changes.
* Maintain a buildable main branch.
* Record technical debt rather than silently accumulating it.
* Revisit earlier decisions when evidence changes.

## Open Research Questions

Examples of questions that may drive future work:

* Which exact nearest-neighbor strategy performs best for different combinations of dataset size and dimensionality?
* At what dimensionality do tree-based indexes stop being useful?
* How does memory layout affect distance-computation throughput?
* How well does query execution scale with CPU cores?
* Where does synchronization become the bottleneck?
* When does asynchronous networking materially outperform simpler connection models?
* How much does approximate search improve latency for a given recall target?
* How do handcrafted visual features compare with deep embeddings for nearest-neighbor classification?
* How do queueing and overload policies affect P99 latency?
* At what workload does GPU acceleration become worthwhile?
* Which optimizations improve benchmark numbers but hurt maintainability enough not to be worthwhile?

## Backlog Policy

This document is intentionally broader than the roadmap.

An item may remain here indefinitely.

Before promoting an exploration item into active work, we should be able to answer:

1. What question are we trying to answer?
2. What engineering or learning value does it provide?
3. What does it depend on?
4. How will we know whether the experiment or implementation succeeded?
