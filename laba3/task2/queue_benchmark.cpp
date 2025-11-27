#include <benchmark/benchmark.h>
#include "structures.h"
#include <sstream>
#include <iostream>

class OutputDisabler {
    streambuf* original_cout;
    stringstream buffer;
public:
    OutputDisabler() : original_cout(cout.rdbuf()) {
        cout.rdbuf(buffer.rdbuf());
    }

    ~OutputDisabler() {
        cout.rdbuf(original_cout);
    }
};

static void BM_QueueEnqueue(benchmark::State& state) {
    Queue queue;
    OutputDisabler disabler;
    int iterations = state.range(0);

    for (auto _ : state) {
        for (int i = 0; i < iterations; ++i) {
            queue.addBackQueue("item" + to_string(i));
        }
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_QueueEnqueue)->Arg(100)->Arg(1000)->Arg(10000);

static void BM_QueueDequeue(benchmark::State& state) {
    int iterations = state.range(0);

    for (auto _ : state) {
        state.PauseTiming();
        Queue queue;
        for (int i = 0; i < iterations; ++i) {
            queue.addBackQueue("item" + to_string(i));
        }
        state.ResumeTiming();

        OutputDisabler disabler;
        for (int i = 0; i < iterations; ++i) {
            queue.remFrontQueue();
        }
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_QueueDequeue)->Arg(100)->Arg(1000)->Arg(10000);

static void BM_QueueDequeueSingle(benchmark::State& state) {
    Queue queue;
    OutputDisabler disabler;

    for (auto _ : state) {
        state.PauseTiming();
        queue.addBackQueue("item1");
        state.ResumeTiming();

        queue.remFrontQueue();
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_QueueDequeueSingle);

static void BM_QueueMixedOperations(benchmark::State& state) {
    Queue queue;
    OutputDisabler disabler;
    int iterations = state.range(0);

    for (auto _ : state) {
        for (int i = 0; i < iterations; ++i) {
            queue.addBackQueue("item" + to_string(i));
            if (i % 2 == 0) {
                queue.remFrontQueue();
            }
        }
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_QueueMixedOperations)->Arg(100)->Arg(1000)->Arg(10000);

static void BM_QueuePrint(benchmark::State& state) {
    Queue queue;
    int iterations = state.range(0);

    for (int i = 0; i < iterations; ++i) {
        queue.addBackQueue("item" + to_string(i));
    }

    for (auto _ : state) {
        OutputDisabler disabler;
        queue.printQueue();
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_QueuePrint)->Arg(10)->Arg(100)->Arg(1000);

BENCHMARK_MAIN();