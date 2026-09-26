# Baseline System Requirements

## 1. Purpose

The baseline system provides exact nearest-neighbor search and K-nearest-neighbor classification over multidimensional numeric vector datasets.

The baseline establishes a small, correct, deterministic reference implementation that later algorithms, concurrency models, networking layers, and optimizations can be compared against.

This document defines required behavior. It intentionally avoids prescribing detailed class structure or implementation architecture.

## 2. Domain Model

### Feature Vector

A feature vector is an ordered sequence of finite numeric values.

Feature values are represented as `double`.

### Training Sample

A training sample contains:

- a feature vector
- a class label

Class labels are strings.

Within a dataset, each training sample is identified by its zero-based position in the dataset.

This index provides a stable identity for deterministic neighbor ordering and result reporting.

### Query Sample

A query sample contains a feature vector whose class is not known to the classifier.

### Dataset

A dataset contains samples that share a common feature dimensionality.

### K

`K` is the number of nearest training samples considered during a KNN query or classification operation.

### Distance Metric

A distance metric computes a numeric distance between two feature vectors of equal dimensionality.

### Neighbor Result

A neighbor result identifies a training sample together with its computed distance from a query.

### Classification Result

A classification result contains the predicted class associated with a query.

## 3. Baseline Workflow

The baseline workflow is:

```text
Training dataset
        +
Query dataset
        +
K
        +
Distance configuration
        ↓
Exact nearest-neighbor search
        ↓
Top-K neighbors
        ↓
KNN classification
        ↓
Results
```

The baseline operates on one or more query samples.

## 4. Dataset Requirements

### FR-DATA-001

The system shall accept a labeled training dataset.

### FR-DATA-002

Each training sample shall contain:

- a non-empty feature vector
- a non-empty class label

For the baseline system, class labels shall not contain:

- the CSV delimiter `,`
- carriage return
- newline

### FR-DATA-003

The system shall accept one or more query feature vectors.

### FR-DATA-004

All training and query vectors participating in the same operation shall have identical dimensionality.

### FR-DATA-005

Feature values shall be finite numeric values.

Inputs containing invalid numeric values shall be rejected.

### FR-DATA-006

An empty training dataset shall not be considered a valid input for nearest-neighbor search or classification.

### FR-DATA-007

Malformed input shall produce an explicit failure rather than partially defined classifier behavior.

## 5. Dataset Lifecycle

### FR-LIFECYCLE-001

A successfully constructed dataset shall be immutable.

### FR-LIFECYCLE-002

A dataset shall become available to callers only after its complete contents have been parsed and validated successfully.

### FR-LIFECYCLE-003

If dataset parsing or validation fails, no partially constructed dataset shall be exposed as a valid dataset.

### FR-LIFECYCLE-004

The baseline shall not support incremental mutation, append, deletion, or in-place modification of an existing dataset.

Such capabilities may be introduced in later milestones.

## 6. CSV Input

### FR-INPUT-001

The project shall support loading training and query datasets from CSV files.

CSV is an external input representation and shall not define the internal representation of the nearest-neighbor engine.

### FR-INPUT-002

Each training CSV row shall have the form:

feature_1,feature_2,...,feature_n,label

where:

- each feature is a valid finite numeric value
- all rows use the same number of features
- `label` is a non-empty string

### FR-INPUT-003

Each query CSV row shall have the form:

feature_1,feature_2,...,feature_n

Query rows shall use the same feature dimensionality as the training dataset used for the same classification operation.

### FR-INPUT-004

The baseline CSV format shall not contain a header row.

Header support may be introduced later if a concrete use case requires it.

### FR-INPUT-005

Leading and trailing whitespace surrounding individual fields shall be ignored.

### FR-INPUT-006

Numeric fields shall support standard decimal floating-point notation, including an optional sign, decimal fraction, and scientific notation.

Examples of valid representations include:

- `3`
- `-1.25`
- `6.02e23`
- `1E-6`

The parsed value shall be finite.

### FR-INPUT-007

Empty fields shall be rejected.

### FR-INPUT-008

Empty rows shall be rejected.

### FR-INPUT-009

Quoted CSV fields shall not be supported by the baseline format.

Fields containing the delimiter or line breaks shall be rejected.

### FR-INPUT-010

A CSV parsing or validation failure shall cause the entire dataset load operation to fail.

No partially parsed dataset shall be exposed as a valid dataset.

## 7. Distance Metrics

The baseline shall support the following metrics.

### Euclidean Distance

`FR-METRIC-001`

For vectors `x` and `y`:

```text
sqrt(sum((x_i - y_i)^2))
```

### Manhattan Distance

`FR-METRIC-002`

```text
sum(abs(x_i - y_i))
```

### Chebyshev Distance

`FR-METRIC-003`

```text
max(abs(x_i - y_i))
```

### Canberra Distance

`FR-METRIC-004`

```text
sum(abs(x_i - y_i) / (abs(x_i) + abs(y_i)))
```

When both coordinates are zero, that coordinate shall contribute `0` to the total distance.

### Minkowski Distance

`FR-METRIC-005`

For parameter `p`:

```text
(sum(abs(x_i - y_i)^p))^(1/p)
```

`p` shall satisfy:

```text
p >= 1
```

Minkowski distance shall be treated as a configurable metric rather than as an alias for Euclidean distance.

### FR-METRIC-006

Distance computation shall reject vectors with different dimensionality.

### FR-METRIC-007

For identical valid input vectors, each supported distance metric shall produce zero distance.

## 8. K Configuration

### FR-K-001

`K` shall be an integer.

### FR-K-002

`K` shall satisfy:

```text
1 <= K <= training_dataset_size
```

### FR-K-003

Invalid values of `K` shall be rejected before search or classification begins.

## 9. Exact Neighbor Search

### FR-SEARCH-001

The baseline implementation shall perform exact nearest-neighbor search.

### FR-SEARCH-002

For a query vector, the system shall compute neighbor candidates using the selected distance metric.

### FR-SEARCH-003

The system shall identify exactly `K` nearest training samples.

Candidates shall be ranked using the following total ordering:

1. smaller distance first
2. for equal distances, smaller training-sample index first

This ordering shall also determine membership when multiple candidates have equal distance at the Top-K selection boundary.

### FR-SEARCH-004

Neighbor results shall be returned using the same deterministic ordering defined for candidate selection:

1. ascending distance
2. ascending training-sample index for equal distances

### FR-SEARCH-005

The baseline behavior shall be deterministic for identical:

- training data
- query data
- K
- distance configuration
- implementation version

### FR-SEARCH-006

Approximate nearest-neighbor behavior is explicitly outside the baseline.

## 10. Classification

### FR-CLASS-001

KNN classification shall derive a predicted class from the labels of the selected K nearest neighbors.

### FR-CLASS-002

The baseline classification strategy shall use majority voting.

### FR-CLASS-003

Classification ties shall be resolved deterministically.

If multiple labels receive the same highest vote count, the predicted label shall be the tied label whose nearest supporting neighbor has the smallest distance to the query.

If multiple tied labels also have equal nearest supporting distances, the lexicographically smallest label shall be selected.

### FR-CLASS-004

Classification results for multiple query samples shall preserve query ordering.

### FR-CLASS-005

Running classification repeatedly with identical input and configuration shall produce identical results.

## 11. Result Requirements

### FR-RESULT-001

Nearest-neighbor search shall make the selected Top-K neighbors available to the caller.

Each neighbor result shall include at least:

- the zero-based index of the corresponding training sample
- distance from the query

### FR-RESULT-002

Classification shall produce exactly one predicted label for each successfully processed query.

### FR-RESULT-003

Batch classification results shall preserve query ordering.

### FR-RESULT-004

Each classification result shall be associated with its zero-based query index.

For a successful batch containing `N` queries, result indices shall be:

0, 1, ..., N - 1

### FR-RESULT-005

The project shall support human-readable display of classification results.

The presentation format is not part of the nearest-neighbor engine contract.

### FR-RESULT-006

The baseline shall support exporting classification results as CSV.

The exported format shall contain:

query_index,predicted_label

and one row per classified query.

Example:

query_index,predicted_label
0,iris-setosa
1,iris-versicolor

### FR-RESULT-007

Exported rows shall appear in the same logical order as the corresponding queries.

### FR-RESULT-008

Top-K neighbor details and distances shall not be included in the baseline classification export format.

Such diagnostic or analytical exports may be introduced separately without changing classification semantics.

## 12. Batch Semantics

### FR-BATCH-001

A batch classification request shall be validated before classification results are committed.

### FR-BATCH-002

If any query in a batch is invalid, the entire batch operation shall fail.

### FR-BATCH-003

The baseline shall not return partial classification results for an invalid batch.

### FR-BATCH-004

A successful batch operation shall produce exactly one result for each query in the original query order.

### FR-BATCH-005

Failure of a batch operation shall not leave partially updated classification state.

## 13. Evaluation

### FR-EVAL-001

The project shall support evaluating classification predictions against known ground-truth labels.

### FR-EVAL-002

Evaluation shall be implemented independently from nearest-neighbor search and classification.

The search engine shall not require ground-truth labels in order to operate.

### FR-EVAL-003

The baseline evaluation functionality shall compute classification accuracy.

### FR-EVAL-004

The baseline evaluation functionality shall produce a confusion matrix.

Rows shall represent ground-truth labels.

Columns shall represent predicted labels.

The matrix shall use the union of labels appearing in the ground truth and predictions, ordered lexicographically, so that its representation is deterministic.

### FR-EVAL-005

The number of ground-truth labels shall match the number of classification results being evaluated.

### FR-EVAL-006

Ground-truth labels and predictions shall be associated according to query ordering.

### FR-EVAL-007

Invalid evaluation input shall produce an explicit failure rather than a partial metric result.

## 14. Validation Requirements

### VAL-001

Dimension mismatches shall be detected.

### VAL-002

Malformed numeric values shall be rejected.

### VAL-003

Non-finite feature values shall be rejected.

### VAL-004

Empty feature vectors shall be rejected.

### VAL-005

Empty training datasets shall be rejected for search operations.

### VAL-006

Invalid `K` values shall be rejected.

### VAL-007

Unsupported distance metrics shall be rejected.

### VAL-008

Invalid Minkowski parameters shall be rejected.

### VAL-009

Malformed dataset input shall not expose a partially constructed dataset as valid.

## 15. Correctness Requirements

### COR-001

Every distance metric shall be independently unit tested against manually verifiable values.

### COR-002

Neighbor selection shall be independently verifiable from classification voting.

### COR-003

Classification shall be tested using datasets small enough for results to be verified manually.

### COR-004

The exact baseline implementation shall serve as the correctness reference for later optimized search implementations.

### COR-005

Later concurrent implementations shall produce equivalent results to the single-threaded baseline for equivalent inputs.

### COR-006

Later indexed or approximate implementations shall document where their semantics differ from the exact baseline.

## 16. Baseline Engineering Constraints

The baseline shall prioritize:

1. correctness
2. deterministic behavior
3. testability
4. clear data semantics
5. maintainability
6. performance

Performance is important, but optimization shall not precede the establishment of a trustworthy correctness baseline.

The initial exact implementation may intentionally use a simple search strategy when doing so provides a clearer reference for later performance comparisons.

## 17. Explicitly Deferred Capabilities

The following are outside baseline correctness scope:

- parallel query execution
- thread pools
- network services
- asynchronous I/O
- Linux-specific networking
- `epoll`
- `io_uring`
- advanced spatial indexes
- approximate nearest-neighbor search
- persistent indexes
- online insertion and deletion
- Python bindings
- computer-vision feature extraction
- deep-learning inference
- learned embeddings
- GPU acceleration
- distributed search
- authentication
- TLS
- production observability
- latency SLOs

These capabilities remain part of the broader project roadmap and exploration backlog.
