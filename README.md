# EnTTBenchmarks

A comprehensive benchmark suite for performance testing of different entity iteration methods in EnTT v3.14.0.

## Overview

This project compares the performance of various approaches to iterate over entities and components in the EnTT Entity-Component-System (ECS) library. The benchmarks measure the execution time for different iteration methods across varying numbers of entities.

## Benchmark Results

The following results were obtained with:
- EnTT v3.14.0
- Google Benchmark v1.8.3
- C++17
- Build type: Release (`cmake -DCMAKE_BUILD_TYPE=Release ..`)

All times are in microseconds. Lower is better.

| Benchmark Method             | 1,000 entities | 4,096 entities | 32,768 entities | 100,000 entities |
|------------------------------|----------------|----------------|-----------------|------------------|
| SingleComponentView          | 2.65 μs        | 11.0 μs        | 87.4 μs         | 271 μs           |
| MultiComponentView           | 4.30 μs        | 21.5 μs        | 185 μs          | 571 μs           |
| MultiComponentViewGet        | 4.90 μs        | 21.6 μs        | 187 μs          | 577 μs           |
| ComplexView                  | 2.88 μs        | 15.5 μs        | 178 μs          | 631 μs           |
| LimitedComplexView           | 4.62 μs        | 21.7 μs        | 183 μs          | 588 μs           |
| StorageDirectAccess          | 2.64 μs        | 10.8 μs        | 86.7 μs         | 268 μs           |
| FilteredViewIteration        | 2.65 μs        | 11.1 μs        | 89.5 μs         | 280 μs           |

## Key Findings

1. **Direct storage access is fastest**: The `StorageDirectAccess` method, which accesses component storage directly, consistently shows the best performance across all entity counts.

2. **Single component views are efficient**: The `SingleComponentView` method is nearly as fast as direct storage access, making it a good choice when you only need one component.

3. **Multi-component views have overhead**: The `MultiComponentView` and `MultiComponentViewGet` methods show approximately 2x slower performance compared to single-component iteration.

4. **Unpacking vs. get<T>()**: There is negligible difference between unpacking components in the view's each() method versus using get<T>() for access.

5. **Filtering has minimal overhead**: The `FilteredViewIteration` method shows that simple runtime filtering has very little performance impact compared to the base view iteration.

6. **Complex views scale differently**: The `ComplexView` (with all components) performs better on small entity counts but scales worse with larger entity counts.

## Iteration Methods Explained

1. **SingleComponentView**: Iterates over entities with a single component (Position).
   ```cpp
   auto view = registry.view<Position>();
   for (auto [entity, position] : view.each()) {
       // Use position data
   }
   ```

2. **MultiComponentView**: Iterates over entities with multiple components, unpacking all components.
   ```cpp
   auto view = registry.view<Position, Velocity, Health>();
   for (auto [entity, position, velocity, health] : view.each()) {
       // Use components
   }
   ```

3. **MultiComponentViewGet**: Iterates over entities with multiple components, but explicitly gets only the needed component.
   ```cpp
   auto view = registry.view<Position, Velocity, Health>();
   for (auto entity : view) {
       const auto& position = view.get<Position>(entity);
       // Use position data
   }
   ```

4. **StorageDirectAccess**: Accesses the component storage directly, bypassing the view mechanism.
   ```cpp
   const auto& storage = registry.storage<Position>();
   for(auto [entity, position] : storage.each()) {
       // Use position data
   }
   ```

5. **FilteredViewIteration**: Uses a view with a runtime filter condition.
   ```cpp
   auto view = registry.view<Position>();
   view.each([&](auto entity, auto& position) {
       if (position.x > 0.0f) {
           // Use position data
       }
   });
   ```

## Recommendations

Based on the benchmark results:

1. When you need only a single component, use either `StorageDirectAccess` or `SingleComponentView`.
2. When you need to filter entities at runtime, use a simple view with a condition in the loop body.
3. For maximum performance with multi-component views, only include components you actually need in the view.
4. If you need all entities with a specific component, regardless of other components, prefer `storage().each()` for best performance.

## How to Build and Run

```bash
# Clone the repository
git clone https://github.com/yourusername/EnTTBenchmarks.git
cd EnTTBenchmarks

# Create build directory
mkdir -p build && cd build

# Build in Release mode
cmake -DCMAKE_BUILD_TYPE=Release ..
make

# Run all benchmarks
./EnTTBenchmarks

# Run a specific benchmark
./EnTTBenchmarks --benchmark_filter=BM_SingleComponentView
```

## Dependencies

- [EnTT](https://github.com/skypjack/entt) v3.14.0
- [Google Benchmark](https://github.com/google/benchmark) v1.8.3

The CMake file automatically downloads and configures these dependencies.

## License

This project is in the public domain. See the [LICENSE](LICENSE) file for details.
