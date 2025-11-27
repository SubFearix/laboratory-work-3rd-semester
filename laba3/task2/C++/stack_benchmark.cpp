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

static void BM_StackPopEmpty(benchmark::State& state) {
    Stack stack;
    OutputDisabler disabler;

    for (auto _ : state) {
        stack.popStack();
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_StackPopEmpty);

static void BM_StackPrintEmpty(benchmark::State& state) {
    Stack stack;
    OutputDisabler disabler;

    for (auto _ : state) {
        stack.printStack();
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_StackPrintEmpty);

static void BM_StackMultiplePop(benchmark::State& state) {
    Stack stack;
    OutputDisabler disabler;
    int iterations = state.range(0);

    for (auto _ : state) {
        for (int i = 0; i < iterations; ++i) {
            stack.popStack();
        }
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_StackMultiplePop)->Arg(1000)->Arg(10000);

static void BM_StackPushPop(benchmark::State& state) {
    Stack stack;
    OutputDisabler disabler;

    for (auto _ : state) {
        stack.pushStack("test");
        stack.popStack();
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_StackPushPop);

BENCHMARK_MAIN();