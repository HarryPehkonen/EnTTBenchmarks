#include <benchmark/benchmark.h>
#include <entt/entt.hpp>
#include <entt/entity/registry.hpp>
#include <random>
#include <vector>

// Component types for our benchmarks
struct Position {
    float x, y, z;
};

struct Velocity {
    float x, y, z;
};

struct Health {
    float value;
};

struct Name {
    std::string name;
};

// Helper function to setup a registry with entities and components
entt::registry setup_registry(std::size_t entity_count) {
    // Create a standard EnTT registry
    entt::registry registry;
    
    // Create entities with different component combinations
    std::vector<entt::entity> entities;
    entities.reserve(entity_count);
    
    for (std::size_t i = 0; i < entity_count; ++i) {
        entities.push_back(registry.create());
    }
    
    // Random distribution to decide component attachment probability
    std::mt19937 rng(42); // Fixed seed for reproducibility
    std::uniform_real_distribution<float> dist(0.0f, 1.0f);
    
    // Assign components with different probabilities to create a realistic scenario
    for (auto entity : entities) {
        // Position has 100% probability (the component we always want to access)
        registry.emplace<Position>(entity, Position{dist(rng), dist(rng), dist(rng)});
        
        // Velocity has 80% probability
        if (dist(rng) < 0.8f) {
            registry.emplace<Velocity>(entity, Velocity{dist(rng), dist(rng), dist(rng)});
        }
        
        // Health has 60% probability
        if (dist(rng) < 0.6f) {
            registry.emplace<Health>(entity, Health{dist(rng) * 100.0f});
        }
        
        // Name has 40% probability
        if (dist(rng) < 0.4f) {
            registry.emplace<Name>(entity, Name{"Entity-" + std::to_string(static_cast<int>(entity))});
        }
    }
    
    return registry;
}

// Benchmark: Iteration using a single component view (Position only)
static void BM_SingleComponentView(benchmark::State& state) {
    auto registry = setup_registry(state.range(0));
    
    // Only used for verification to avoid optimization
    float sum = 0.0f;
    
    for (auto _ : state) {
        auto view = registry.view<Position>();
        
        // Use the view
        for (auto [entity, position] : view.each()) {
            sum += position.x + position.y + position.z;
        }
    }
    
    // Make sure the sum is used to prevent optimization
    benchmark::DoNotOptimize(sum);
}

// Benchmark: Iteration using multi-component view (Position, Velocity, Health)
static void BM_MultiComponentView(benchmark::State& state) {
    auto registry = setup_registry(state.range(0));
    
    float sum = 0.0f;
    
    for (auto _ : state) {
        auto view = registry.view<Position, Velocity, Health>();
        
        // Use the view but only access Position
        for (auto [entity, position, velocity, health] : view.each()) {
            sum += position.x + position.y + position.z;
        }
    }
    
    benchmark::DoNotOptimize(sum);
}

// Benchmark: Iteration using get<Position>() instead of unpacking
static void BM_MultiComponentViewGet(benchmark::State& state) {
    auto registry = setup_registry(state.range(0));
    
    float sum = 0.0f;
    
    for (auto _ : state) {
        auto view = registry.view<Position, Velocity, Health>();
        
        // Access Position using get<>
        for (auto entity : view) {
            const auto& position = view.get<Position>(entity);
            sum += position.x + position.y + position.z;
        }
    }
    
    benchmark::DoNotOptimize(sum);
}

// Benchmark: Using a more complex view with all components
static void BM_ComplexView(benchmark::State& state) {
    auto registry = setup_registry(state.range(0));
    
    float sum = 0.0f;
    
    for (auto _ : state) {
        auto view = registry.view<Position, Velocity, Health, Name>();
        
        // Only access Position
        for (auto entity : view) {
            const auto& position = view.get<Position>(entity);
            sum += position.x + position.y + position.z;
        }
    }
    
    benchmark::DoNotOptimize(sum);
}

// Benchmark: Using a more complex view with all components
static void BM_LimitedComplexView(benchmark::State& state) {
    auto registry = setup_registry(state.range(0));
    
    float sum = 0.0f;
    
    for (auto _ : state) {

        // Without Name
        auto view = registry.view<Position, Velocity, Health>();
        
        // Only access Position
        for (auto entity : view) {
            const auto& position = view.get<Position>(entity);
            sum += position.x + position.y + position.z;
        }
    }
    
    benchmark::DoNotOptimize(sum);
}

// Benchmark: Using storage() direct access
static void BM_StorageDirectAccess(benchmark::State& state) {
    auto registry = setup_registry(state.range(0));
    
    float sum = 0.0f;
    
    for (auto _ : state) {
        const auto& storage = registry.storage<Position>();
        
        for(auto [entity, position] : storage.each()) {
            sum += position.x + position.y + position.z;
        }
    }
    
    benchmark::DoNotOptimize(sum);
}

// Removed BM_RegistryEach as it's not compatible with EnTT v3.14.0
// Likewise, BM_SnapshotIteration had API compatibility issues
// Keeping only the benchmarks that work with the current EnTT version

static void BM_FilteredViewIteration(benchmark::State& state) {
    auto registry = setup_registry(state.range(0));

    float sum = 0.0f;

    for (auto _ : state) {
        auto view = registry.view<Position>();
        view.each([&](auto entity, auto& position) {
            if (position.x > 0.0f) {
                sum += position.x + position.y + position.z;
            }
        });
    }

    benchmark::DoNotOptimize(sum);
}

// Removed BM_CustomStorageIteration as it had compatibility issues

// Register benchmarks with different entity counts
BENCHMARK(BM_SingleComponentView)->Range(1000, 100000)->Unit(benchmark::kMicrosecond);
BENCHMARK(BM_MultiComponentView)->Range(1000, 100000)->Unit(benchmark::kMicrosecond);
BENCHMARK(BM_MultiComponentViewGet)->Range(1000, 100000)->Unit(benchmark::kMicrosecond);
BENCHMARK(BM_ComplexView)->Range(1000, 100000)->Unit(benchmark::kMicrosecond);
BENCHMARK(BM_LimitedComplexView)->Range(1000, 100000)->Unit(benchmark::kMicrosecond);
BENCHMARK(BM_StorageDirectAccess)->Range(1000, 100000)->Unit(benchmark::kMicrosecond);
BENCHMARK(BM_FilteredViewIteration)->Range(1000, 100000)->Unit(benchmark::kMicrosecond);

BENCHMARK_MAIN();