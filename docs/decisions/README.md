# Architectural Decision Records

This directory contains Architectural Decision Records (ADRs) for technical decisions that have meaningful long-term consequences for the project.

An ADR should be created when a decision:

* affects architecture or major component boundaries
* introduces an important dependency
* constrains future implementation choices
* changes concurrency, networking, storage, or performance behavior
* has meaningful alternatives or tradeoffs
* would otherwise be difficult to understand later from the code alone

Small implementation choices do not require ADRs.

## ADR Format

Each ADR should contain:

### Status

One of:

* Proposed
* Accepted
* Superseded
* Deprecated

### Context

What problem or decision are we dealing with?

Include relevant constraints and assumptions.

### Options Considered

What realistic alternatives were considered?

Avoid listing artificial alternatives that were never serious candidates.

### Decision

What was chosen?

### Rationale

Why was this option chosen?

Focus on tradeoffs rather than simply listing advantages.

### Consequences

What becomes easier, harder, or constrained because of this decision?

### Revisit Conditions

What evidence or change in requirements would justify reconsidering the decision?

## Naming

Use sequential identifiers:

```text
ADR-0001-cpp20.md
ADR-0002-example-decision.md
ADR-0003-example-decision.md
```

ADRs should describe the decision at the time it was made. If a decision changes substantially, prefer creating a new ADR that supersedes the previous one rather than rewriting history.
