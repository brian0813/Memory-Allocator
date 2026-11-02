# Memory Allocator in C

A custom memory allocator implemented in C11 using a fixed 1 KB memory pool, focusing on memory management, pointer arithmetic, and linked-list operations.

## Features

- **First-Fit Allocation:** Reuses available memory blocks through a singly linked list.
- **Block Splitting & Coalescing:** Splits oversized free blocks and merges adjacent free blocks to reduce fragmentation.
- **Memory Alignment:** Supports aligned allocation using `max_align_t`.
- **Defensive Memory Handling:** Rejects invalid allocation sizes, handles invalid pointers, and ignores repeated frees of tracked free blocks.
- **Testing & Benchmarking:** Includes automated functional tests, stress testing, and performance measurement.

## Memory Layout

```text
Memory Pool (1024 Bytes)

┌──────────────┬─────────────┬──────────────┬─────────────┐
│ BlockHeader  │ Payload A   │ BlockHeader  │ Payload B   │
└──────────────┴─────────────┴──────────────┴─────────────┘
```

Each `BlockHeader` contains `size`, `is_free`, and `next` to manage memory blocks using a singly linked list.

## Build & Run

**Run automated tests:**

```powershell
.\run_tests.ps1
```

**Run performance benchmark:**

```powershell
gcc -std=c11 -O2 benchmark.c allocator.c -o benchmark.exe
.\benchmark.exe
```

Requires GCC with C11 support and Windows PowerShell for the test script.

## Testing & Performance

- Functional tests covering allocation, block reuse, splitting, coalescing, and invalid pointer handling.
- **Stress Test:** 100,000 allocation/free iterations.
- **Benchmark:** 1,000,000 allocation/free pairs.

| Benchmark Metric | Result |
|---|---|
| Iterations | 1,000,000 |
| Total CPU Time | 0.004 s |
| Average Time per Pair | 4.000 ns |

*Benchmark measured using `clock()`. Results are preliminary and may vary depending on compiler optimization, timer resolution, and system environment.*

## Limitations

Educational implementation using a fixed-size 1 KB memory pool. Not thread-safe and does not support `realloc()` or `calloc()`. Stale pointers to reused addresses cannot be reliably detected.