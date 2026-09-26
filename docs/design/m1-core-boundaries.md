# M1 Core Boundaries

## Purpose

This document defines the initial responsibility and ownership boundaries for the M1 single-threaded KNN implementation.

It is an implementation design document rather than part of the functional specification.

The goal is to establish a small, composable correctness-oriented core without committing prematurely to architecture needed by later concurrency, networking, or indexing milestones.

## Design Principles

The M1 design should prefer:

- value semantics where practical
- explicit ownership
- immutable datasets after construction
- small components with focused responsibilities
- algorithms separated from input/output concerns
- deterministic behavior
- interfaces that can later support concurrency without redesigning the domain model
- standard-library types and idiomatic C++20

The baseline should avoid unnecessary inheritance hierarchies, shared mutable state, and framework-style abstractions.

## High-Level Flow

```text
External input
      ↓
Parsing and validation
      ↓
Immutable Dataset
      ↓
Distance computation
      ↓
Exact Top-K search
      ↓
Neighbor results
      ↓
Majority voting
      ↓
Classification results
      ↓
Evaluation / presentation / export
```

Batch classification orchestrates the same core operations across multiple validated queries.

## 1. Domain Data

### Feature vectors

Feature vectors represent ordered numeric coordinates.

The underlying numeric representation is `double`, as required by the baseline specification.

The exact C++ wrapper or type alias will be chosen separately from this responsibility-level design.

### Training samples

A training sample contains:

- feature values
- a class label

Its stable identity within a dataset is its zero-based dataset index.

The sample itself does not store query-specific state such as distance.

### Dataset

The dataset owns its training samples.

Once construction succeeds, the dataset is immutable through its public interface.

The dataset is responsible for maintaining structural invariants such as:

- non-empty samples
- non-empty labels
- common dimensionality
- valid finite feature values

The dataset does not perform KNN search or classification.

## 2. Query Representation

Queries are independent from the training dataset.

A query contains feature values but no classifier-known label.

Search operations should consume read-only query data.

Queries do not become part of the training dataset and should not require ownership by the search implementation.

This allows future callers such as:

- batch processors
- network requests
- Python bindings
- image-embedding pipelines

to provide query vectors without mutating shared training state.

## 3. Distance Computation

Distance computation is a focused algorithmic responsibility.

It receives:

- two compatible feature vectors
- distance configuration

and produces a numeric distance.

The baseline supports:

- Euclidean
- Manhattan
- Chebyshev
- Canberra
- Minkowski

Distance computation must not mutate either input vector.

No query-specific distance shall be stored inside a training sample.

This prevents shared mutable state and makes later concurrent search significantly easier.

## 4. Exact Neighbor Search

Exact search receives:

- an immutable dataset
- a query
- `K`
- distance configuration

and produces exactly `K` neighbor results.

A neighbor result contains:

- training-sample index
- computed distance

The search implementation owns no dataset state.

Neighbor ordering follows the baseline specification:

1. smaller distance
2. smaller training-sample index when distances are equal

The first implementation should favor clarity and correctness over search efficiency.

It may compute all candidate distances and use a simple selection strategy if that provides a trustworthy correctness reference.

Optimized selection algorithms belong to later work.

## 5. Classification

Classification is logically separate from neighbor search.

It consumes the selected neighbors and the corresponding training labels and produces a predicted label.

The baseline strategy is majority voting.

Tie-breaking follows the functional specification:

1. highest vote count
2. nearest supporting neighbor among tied labels
3. lexicographically smallest label if the previous values are equal

Separating voting from neighbor search allows both behaviors to be tested independently.

## 6. Batch Classification

Batch classification is orchestration rather than a new search algorithm.

It coordinates:

- validation of all queries
- repeated exact search
- repeated classification
- query-index assignment
- ordered result collection

The batch operation must preserve the all-or-nothing semantics defined by the specification.

The single-query algorithms should not contain batch-specific state.

This separation will allow later parallel batch execution without changing the fundamental search or classification semantics.

## 7. Evaluation

Evaluation is independent from search and classification.

It consumes:

- predictions
- ground-truth labels

and produces evaluation information such as:

- accuracy
- confusion matrix

The nearest-neighbor search implementation does not require ground truth and must not depend on evaluation functionality.

## 8. Input Adapters

CSV parsing belongs outside the core search algorithms.

The CSV input layer is responsible for:

- tokenizing rows
- numeric conversion
- CSV-specific validation
- constructing domain input data

The core algorithms should also be usable without CSV.

For example, tests or future integrations should be able to construct vectors and datasets programmatically.

## 9. Output Adapters

Human-readable display and CSV export belong outside the classification algorithm.

The core produces structured result values.

Presentation code converts those values to external representations.

This prevents serialization concerns from becoming part of classification semantics.

## Ownership Model

The intended ownership relationships are:

```text
Dataset
└── owns TrainingSample values
    ├── owns feature storage
    └── owns label storage

Caller / batch input
└── owns query storage

Exact search
└── borrows Dataset and query as read-only inputs
    └── returns owned Neighbor result values

Classification
└── borrows Dataset / neighbor information
    └── returns an owned classification result

Evaluation
└── borrows result and ground-truth sequences
    └── returns owned evaluation results
```

Neighbor results should identify samples by stable dataset index rather than storing raw pointers or owning copies of complete training samples.

This avoids unnecessary copying and reduces lifetime hazards.

## Deliberately Avoided Design

M1 should not begin with a large stateful `KNNEngine` object that owns:

- dataset
- queries
- current K
- current metric
- previous results
- parser state
- output state

Such a design would combine unrelated lifetimes and responsibilities.

A service-level object may later compose the core components when networking or long-lived application state becomes necessary.

M1 should also avoid introducing inheritance merely to model algorithms that can be represented more simply.

## Expected Initial Components

At the responsibility level, M1 is expected to contain concepts equivalent to:

```text
Dataset / TrainingSample
Distance configuration
Distance computation
Neighbor result
Exact search
Voting / classification
Batch classification
Evaluation
CSV input adapter
Result export adapter
```

These names are not yet mandatory C++ class names.

Exact types, function signatures, error representation, and compile-time versus runtime metric selection will be decided separately.

## Future Compatibility

These boundaries are intentionally chosen so later milestones can build on them.

### Concurrency

Immutable datasets and stateless search operations allow multiple queries to read the same dataset concurrently.

### Search algorithms

Alternative exact or indexed search implementations can produce the same neighbor-result abstraction.

### Networking

A network layer can translate requests into the same query and configuration inputs used by the local implementation.

### ML and vector search

Embeddings can become another source of numeric feature vectors without changing the search semantics.

### Approximate search

Approximate algorithms may reuse the surrounding dataset, query, result, and evaluation concepts while defining different accuracy guarantees.

## Feature Representation

M1 shall distinguish between owned feature storage and borrowed read-only access.

The initial representation is:

```cpp
using FeatureVector = std::vector<double>;
using FeatureView = std::span<const double>;
```

`FeatureVector` represents owned feature storage.

`FeatureView` represents non-owning read-only access to an existing feature sequence.

Algorithmic operations such as distance computation should prefer `FeatureView` when ownership is not required.

For example:

```cpp
double euclidean_distance(FeatureView lhs, FeatureView rhs);
```

This avoids unnecessary copying and makes ownership explicit at function boundaries.

A dedicated `FeatureVector` class is deliberately not introduced in M1.

Such a type would become justified if feature vectors later require meaningful type-specific invariants, metadata, storage strategies, or behavior that cannot be expressed cleanly by the surrounding domain types.

Validation such as dimensional compatibility and finite-value requirements remains the responsibility of the appropriate dataset, parsing, and operation boundaries rather than being hidden inside a container wrapper.

## Training Sample and Dataset Representation

### TrainingSample

A training sample is a simple domain value containing owned feature storage and a class label.

The initial representation is conceptually:

```cpp
struct TrainingSample {
    FeatureVector features;
    std::string label;
};
```

`TrainingSample` does not store its dataset index.

Its stable identity is determined by its zero-based position within the owning `Dataset`.

Avoiding a duplicated stored index prevents the sample's internal state from becoming inconsistent with its actual dataset position.

A training sample also does not contain query-specific state such as computed distance.

### Dataset

`Dataset` owns the complete collection of training samples and protects the structural invariants required by the baseline system.

Its interface should provide read-only access after successful construction.

A conceptual interface is:

```cpp
class Dataset {
public:
    [[nodiscard]] std::size_t size() const noexcept;
    [[nodiscard]] std::size_t dimension() const noexcept;

    [[nodiscard]] const TrainingSample& sample(std::size_t index) const;
    [[nodiscard]] std::span<const TrainingSample> samples() const noexcept;

private:
    std::vector<TrainingSample> samples_;
    std::size_t dimension_;
};
```

The exact construction API and error representation are intentionally deferred until the M1 error model is selected.

### Dataset Invariants

A successfully constructed baseline training dataset guarantees:

- at least one training sample exists
- every feature vector is non-empty
- every feature vector has the same dimensionality
- every feature value is finite
- every label is non-empty
- every label satisfies the baseline label restrictions

Because these invariants are established during construction, search and classification code should not repeatedly validate the internal dataset structure.

Operation-specific validation, such as checking query dimensionality or validating `K`, remains the responsibility of the relevant operation boundary.

### Immutability

After successful construction, `Dataset` shall expose no public API for:

- modifying an existing sample
- appending samples
- deleting samples
- replacing feature values
- replacing labels

Read-only access may be provided through references and spans whose lifetime is bounded by the lifetime of the dataset.

### Cached Dimensionality

The dataset stores its dimensionality explicitly.

Because the dataset is immutable, the value is established once during construction and remains valid for the dataset lifetime.

This avoids repeatedly deriving a fundamental dataset property from individual samples and gives callers a direct way to validate queries.

### Ownership

The ownership relationship is:

```text
Dataset
└── std::vector<TrainingSample>
    ├── FeatureVector
    │   └── owns double storage
    └── std::string
        └── owns label storage
```

Search operations borrow the dataset through a read-only reference and return independent result values.

## M1 Design Decisions

The following decisions define the initial M1 implementation.

### Feature Representation

```cpp
using FeatureVector = std::vector<double>;
using FeatureView = std::span<const double>;
```

Owned data uses `FeatureVector`.

Read-only algorithm inputs should prefer `FeatureView` to avoid unnecessary copies and make borrowing explicit.

A dedicated feature-vector class is not required in M1.

### Training Samples and Dataset

`TrainingSample` is a simple value type containing:

- owned feature data
- class label

The sample does not store its dataset index or query-specific distance.

`Dataset` owns all training samples and exposes them read-only after successful construction.

The sample's zero-based position within the dataset is its stable identity.

Dataset invariants are validated once during construction.

### Error Handling

M1 uses exceptions for invalid operations and construction failures.

Standard exceptions such as `std::invalid_argument` should be used where appropriate.

CSV parsing may define a small dedicated parse-error type containing useful context such as the failing line.

A custom result type is not required in M1.

### Distance Model

Distance selection occurs at runtime.

The initial model is conceptually:

```cpp
enum class DistanceMetric {
    Euclidean,
    Manhattan,
    Chebyshev,
    Canberra,
    Minkowski
};

struct DistanceConfig {
    DistanceMetric metric{DistanceMetric::Euclidean};
    double minkowski_p{2.0};
};
```

Distance computation uses a simple runtime dispatch such as a `switch`.

Templates, inheritance, and virtual metric interfaces are intentionally avoided in M1.

### Exact Search

Exact search is stateless.

It receives:

- immutable dataset
- query
- `K`
- distance configuration

and returns owned neighbor results containing:

- training-sample index
- distance

The baseline implementation uses:

```text
full scan
→ compute all distances
→ deterministic sort
→ take first K
```

This intentionally simple `O(N log N)` implementation serves as the correctness reference for later optimized approaches.

### Classification

Neighbor search and voting remain separate responsibilities.

Classification composes:

```text
exact search
→ majority vote
→ predicted label
```

This allows search correctness and voting correctness to be tested independently.

### Batch Classification

Batch classification is orchestration over the single-query operations.

All query inputs are validated before classification begins.

Successful batches return results in original query order.

Invalid batches return no partial result set.

### Evaluation

Evaluation remains independent from the search and classification implementation.

It consumes predictions and ground truth and produces:

- accuracy
- confusion matrix

### CSV Input

CSV parsing is an adapter around the core domain model.

Parser functions operate on `std::istream` rather than opening files directly.

This keeps file-system concerns outside the parser and enables simple testing with in-memory streams.

### Output

Result presentation operates on `std::ostream`.

Human-readable output and CSV export remain separate from search and classification logic.

No exporter class hierarchy is required.

### Testing

M1 uses GoogleTest integrated with CTest.

Tests are organized around:

- dataset invariants
- distance metrics
- exact search
- deterministic ordering
- classification and tie-breaking
- batch behavior
- evaluation
- CSV parsing
- end-to-end baseline integration

Mocks are not expected to be necessary.

Google Benchmark may be introduced once the correctness baseline is operational.

### C++ Conventions

M1 uses:

- namespace `knn`
- value semantics by default
- explicit ownership
- `const` and `std::span` for borrowed read-only access
- `[[nodiscard]]` for important returned values
- RAII
- standard-library facilities where appropriate

M1 does not require:

- raw owning pointers
- `std::shared_ptr`
- virtual interfaces
- mutable global state
- a large stateful `KNNEngine` object

### Initial Source Layout

```text
include/knn/
    features.hpp
    dataset.hpp
    distance.hpp
    search.hpp
    classification.hpp
    evaluation.hpp
    csv.hpp
    output.hpp

src/
    dataset.cpp
    distance.cpp
    search.cpp
    classification.cpp
    evaluation.cpp
    csv.cpp
    output.cpp

tests/
    dataset_test.cpp
    distance_test.cpp
    search_test.cpp
    classification_test.cpp
    evaluation_test.cpp
    csv_test.cpp
    integration_test.cpp
```

A small CLI may be added near the end of M1 after the core pipeline is functional.

## M1 Design Principle

The central ownership rule is:

```text
Dataset owns training data.
Algorithms borrow data.
Results own their output.
Query-specific state does not live inside the dataset.
```

These decisions are intentionally minimal.

More advanced choices such as alternative search structures, concurrency abstractions, networking architecture, compile-time metric specialization, and service-level state belong to later milestones.