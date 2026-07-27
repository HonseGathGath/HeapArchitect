# HeapArchitect

A minimal C17 memory allocator for studying heap internals, chunk metadata
manipulation, and memory-corruption primitives.  The library combines a
linear bump allocator (fast path) with a doubly-linked free list
(first-fit search, splitting, coalescing, double-free prevention).

## Architecture

- **Heap arena**:  `mmap`-backed contiguous reservation.
- **Chunk header** (16 bytes):
  ```
  | size (8) | prev_size (8) | next (8) | prev (8) |  payload ... |
                                ^-- pointer returned to the caller
  ```
- **Minimum allocation**:  32 bytes (header + minimal payload).
- **Free flag**:  Bit 0 of the `size` field — `1` = free, `0` = allocated.
- **Free list**:  Doubly-linked, LIFO insertion (new frees land at the tail).
- **Coalescing**:  Walks the free list and merges any block whose physical
  neighbour (at `(char*)curr + curr->size`) is also free.

## API

| Function | Purpose |
|----------|---------|
| `bumpy *new_bumpy(size_t size)` | Create a new arena via `mmap`. |
| `void *myalloc(size_t size, bumpy *bp, free_list_t *fl)` | Allocate.  Tries free-list first-fit, falls back to bump. |
| `void myfree(void *ptr, free_list_t *fl)` | Free.  Appends to the free list; rejects double-free. |
| `void unlink(chunky *ch, free_list_t *fl)` | Remove a free chunk from the list (used internally). |
| `void coalesce_free_chunks(free_list_t *fl)` | Merge physically adjacent free blocks. |

## Build

```sh
cmake -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
```

Binaries are placed in `build/bin/`:
- `app`        — integration test suite
- `test_basic` — basic allocate/free unit test
- `test_stress` — 64-allocation stress test with partial free
- `test_fragmentation` — interleaved alloc/free + coalescing

Run a test directly:

```sh
./build/bin/app
```

Run all unit tests via CTest (from the allocator subdirectory):

```sh
cmake --build build/allocator && ctest --test-dir build/allocator --output-on-failure
```

## Example output

```
=========================================
     RUNNING CUSTOM ALLOCATOR TESTS
=========================================

[TEST 1] Initial Bump Allocations
  Allocated ptr1 (64 bytes): 0x7f118c0c1028
  Allocated ptr2 (128 bytes): 0x7f118c0c1088
  Allocated ptr3 (64 bytes): 0x7f118c0c1128
  Payload Check: ptr1="Hello Allocator!", ptr2="Testing Memory Integrity..."

[TEST 2] Freeing Memory Blocks
--- FREE LIST STATE ---
  Node 0: Addr=0x7f118c0c1018 | Size=96 | FreeBit=1
  Node 1: Addr=0x7f118c0c1078 | Size=160 | FreeBit=1
-----------------------

[TEST 3] Double Free Safeguard Test
  Attempting double free on ptr1...

[TEST 4] Re-allocating Memory (First-Fit + Splitting)
  Allocated ptr4 (32 bytes): 0x7f118c0c1088
--- FREE LIST STATE ---
  Node 0: Addr=0x7f118c0c1018 | Size=96 | FreeBit=1
  Node 1: Addr=0x7f118c0c10b8 | Size=96 | FreeBit=1
-----------------------

[TEST 5] Freeing All Chunks & Running Coalesce
Before Coalescing:
--- FREE LIST STATE ---
  Node 0: Addr=0x7f118c0c1018 | Size=96 | FreeBit=1
  Node 1: Addr=0x7f118c0c10b8 | Size=96 | FreeBit=1
  Node 2: Addr=0x7f118c0c1118 | Size=96 | FreeBit=1
  Node 3: Addr=0x7f118c0c1078 | Size=64 | FreeBit=1
-----------------------

After Coalescing:
--- FREE LIST STATE ---
  Node 0: Addr=0x7f118c0c1018 | Size=352 | FreeBit=1
-----------------------

=========================================
     ALL TESTS PASSED SUCCESSFULLY!
=========================================
```

## License

MIT
