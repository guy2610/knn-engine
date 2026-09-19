# Experiments

This directory contains documented technical experiments performed during development.

Experiments are used when a question should be answered through measurement rather than assumption.

Examples include:

* comparing nearest-neighbor algorithms
* measuring concurrency scaling
* evaluating different queue designs
* investigating memory layout
* comparing networking architectures
* measuring exact versus approximate search
* comparing image representations
* analyzing tail latency under load

## Experiment Template

Each experiment should document the following.

### Question

What are we trying to learn?

### Hypothesis

What result do we expect, and why?

A hypothesis is useful even if it turns out to be wrong.

### Setup

Document enough information to reproduce the experiment:

* implementation or commit
* compiler and version
* build configuration
* operating system
* relevant hardware
* dataset
* dataset size
* vector dimensionality
* concurrency level
* configuration values

Only include variables relevant to the experiment.

### Method

Describe exactly how measurements are collected.

Include:

* warm-up behavior
* number of runs
* duration or iteration count
* metrics collected
* how aggregation is performed

### Results

Record raw or summarized measurements.

Possible metrics include:

* latency
* P50 / P95 / P99
* throughput
* CPU usage
* memory usage
* allocations
* index construction time
* accuracy
* recall

### Interpretation

Explain what the results suggest.

Separate measured facts from explanations or speculation.

### Limitations

Document factors that may affect the validity or generality of the results.

### Decision or Follow-up

If the experiment affects implementation, state what changes as a result.

If the result is inconclusive, record what should be tested next.

## Naming

Use descriptive names, for example:

```text
001-sort-vs-nth-element.md
002-thread-pool-scaling.md
003-kd-tree-dimensionality.md
```

Experiment results should not be silently rewritten when later measurements disagree. Prefer adding a new experiment or explicitly documenting why the previous result is no longer representative.
