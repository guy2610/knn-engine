# Project Specification

This directory contains the functional specification and verification strategy for the baseline nearest-neighbor system.

The specification describes required behavior before implementation architecture is selected.

## Documents

- `baseline-requirements.md` defines the baseline functional behavior, validation rules, deterministic semantics, and explicitly deferred capabilities.
- `verification-strategy.md` defines how requirement categories will be verified.
- `open-questions.md` tracks unresolved behavioral questions that materially affect the specification.

Implementation and architectural decisions should be documented separately through design work and ADRs when the relevant milestone requires them.

## Current Status

The baseline specification is complete for M0.

No detailed class hierarchy, ownership model, search data structure, concurrency architecture, or networking design is prescribed by this specification.

The next milestone is implementation of the correct single-threaded KNN core.
