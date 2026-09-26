# Verification Strategy

## Purpose

This document defines how the baseline system requirements will be verified.

The goal is to maintain traceability between:

Requirement
→ Observable behavior
→ Verification method

Detailed test cases belong in the test suite rather than in this document.

## Verification Principles

### Verify behavior through public boundaries

Tests should prefer observable behavior over internal implementation details.

Implementation structure may change without requiring unrelated tests to change.

### Keep correctness references simple

The initial exact implementation should remain simple enough to serve as a trustworthy reference for later optimized implementations.

### Test invalid behavior explicitly

Validation requirements are part of the system contract and should be tested rather than treated as incidental error handling.

### Separate algorithm correctness from integration correctness

Distance calculations, neighbor selection, voting, parsing, evaluation, and batch behavior should be independently verifiable where practical.

### Preserve determinism

Requirements involving deterministic ordering or tie-breaking should use exact expected outputs rather than loose comparisons.

## Requirement Verification

### Dataset Requirements

Covers:

- `FR-DATA-*`
- `FR-LIFECYCLE-*`

Verification:

- unit tests for dataset construction
- rejection tests for empty or malformed samples
- dimensionality validation tests
- immutability verification through the public API
- failure tests confirming that invalid construction does not expose a partially valid dataset

### CSV Input

Covers:

- `FR-INPUT-*`

Verification:

- parser tests using small temporary or fixture files
- valid training CSV cases
- valid query CSV cases
- whitespace handling
- decimal and scientific notation
- malformed numeric input
- empty fields
- empty rows
- unsupported quoted fields
- delimiter-containing labels
- inconsistent dimensionality
- all-or-nothing parsing behavior

CSV parsing tests should verify the external contract without depending on the internal representation of the KNN engine.

### Distance Metrics

Covers:

- `FR-METRIC-*`

Verification:

- unit tests using manually calculated vectors
- zero-distance tests for identical vectors
- dimensionality mismatch tests
- Canberra zero-denominator cases
- Minkowski tests for multiple valid values of `p`
- invalid Minkowski parameter tests

Useful invariant checks may include:

- non-negative distance
- zero distance for identical vectors
- symmetry for supported metrics

Tests should not rely only on invariants. Known numeric examples are required.

### K Configuration

Covers:

- `FR-K-*`

Verification:

- minimum valid `K`
- maximum valid `K`
- `K = 0`
- negative values where representable by the input boundary
- `K > training_dataset_size`

### Exact Neighbor Search

Covers:

- `FR-SEARCH-*`

Verification:

Use small datasets where the correct neighbor ordering can be calculated manually.

Tests should cover:

- normal Top-K selection
- `K = 1`
- `K = training_dataset_size`
- equal-distance candidates
- equal-distance candidates around the Top-K boundary
- deterministic ordering by training-sample index
- repeated execution with identical input

Later optimized exact-search implementations shall be compared against the baseline implementation over shared test datasets.

### Classification

Covers:

- `FR-CLASS-*`

Verification:

Use manually verifiable labeled datasets.

Tests should include:

- clear majority
- multiple labels
- majority independent of input ordering where semantics require it
- vote tie resolved by nearest supporting neighbor
- equal nearest supporting distances resolved lexicographically
- repeated classification producing identical results

Neighbor selection and voting should be independently testable so that classification failures can be localized.

### Result Requirements

Covers:

- `FR-RESULT-*`

Verification:

- exact Top-K result contents
- training-sample index correctness
- distance correctness
- result ordering
- query-index mapping
- human-readable output smoke tests where appropriate
- CSV export tests
- exported row ordering
- confirmation that baseline classification export excludes neighbor diagnostics

### Batch Semantics

Covers:

- `FR-BATCH-*`

Verification:

- valid multi-query batch
- preserved query order
- exact result count
- malformed first query
- malformed middle query
- malformed final query
- confirmation that invalid batches produce no partial result set

The primary invariant is:

```text
successful_result_count == query_count
```

Otherwise the batch operation fails as a whole.

### Evaluation

Covers:

- `FR-EVAL-*`

Verification:

Use small prediction and ground-truth sequences with manually known outcomes.

Tests should include:

- perfect accuracy
- zero accuracy
- partial accuracy
- mismatched input lengths
- single-class confusion matrix
- multi-class confusion matrix
- labels present only in predictions
- labels present only in ground truth
- deterministic lexicographic label ordering

Expected confusion matrices should be compared exactly.

### Validation Requirements

Covers:

- `VAL-*`

Verification:

Each validation requirement should have at least one negative test that demonstrates rejection of the invalid condition.

Validation tests should verify externally observable failure behavior rather than implementation-specific exceptions or error types until the error model is explicitly designed.

### Correctness Requirements

Covers:

- `COR-*`

Verification:

These requirements primarily constrain the test architecture and future development process.

They shall be enforced through:

- independent metric tests
- independent neighbor-selection tests
- manually verifiable classification fixtures
- shared reference datasets
- comparison of optimized exact search against the baseline
- comparison of concurrent execution against single-threaded results

Approximate algorithms introduced later require their own accuracy and recall criteria rather than strict equality with exact search.

## Test Layers

The baseline test suite is expected to contain several layers.

### Unit Tests

Used for:

- distance metrics
- dataset validation
- voting
- neighbor ordering
- evaluation logic
- small parser behavior

### Integration Tests

Used for:

- CSV to dataset conversion
- dataset to classification pipeline
- batch processing
- export behavior

### Regression Tests

Added when a defect is discovered that could reasonably recur.

The regression test should reproduce the failure before the fix and remain in the suite afterward.

## Performance Verification

Performance is not a baseline correctness criterion.

However, benchmark infrastructure may be introduced during baseline development so that later milestones have comparable measurements.

Performance tests must not replace correctness tests.

## Future Verification Extensions

Later milestones may add:

- ThreadSanitizer
- AddressSanitizer
- UndefinedBehaviorSanitizer
- fuzz testing
- property-based testing
- concurrency stress testing
- performance regression testing
- protocol compatibility testing
- end-to-end network tests
- approximate-search recall benchmarks

These extensions should preserve the exact baseline as the primary correctness reference where applicable.