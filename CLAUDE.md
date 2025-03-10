# EnTTBenchmarks - Build & Style Guide

## Build Commands
- Build project: `mkdir -p /home/harri/Dropbox/code/EnTTBenchmarks/build && cd /home/harri/Dropbox/code/EnTTBenchmarks/build && cmake .. && make`
- Run benchmarks: `/home/harri/Dropbox/code/EnTTBenchmarks/build/EnTTBenchmarks`
- Run specific benchmark: `/home/harri/Dropbox/code/EnTTBenchmarks/build/EnTTBenchmarks --benchmark_filter=BM_SingleComponentView`
- Run with options: `/home/harri/Dropbox/code/EnTTBenchmarks/build/EnTTBenchmarks --benchmark_time_unit=ms`

## Code Style Guidelines
- Use C++17 standard
- Prefer `auto` for complex types and iterators
- Component structs: Simple POD types with public members
- Benchmarks: Prefix with `BM_`, use `benchmark::State&` parameter
- Use `benchmark::DoNotOptimize()` to prevent compiler optimization
- Include function names in benchmark registration
- Naming: CamelCase for types, snake_case for variables and functions
- EnTT style: Registry-based ECS with views and storage access
- Error handling: Use assertions in debug, direct handling in benchmarks