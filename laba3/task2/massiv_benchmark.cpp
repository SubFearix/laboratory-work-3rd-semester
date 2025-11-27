#include <benchmark/benchmark.h>
#include "structures.h"
#include <string>

static void BM_AddAtIndexBeginning(benchmark::State& state) {
    for (auto _ : state) {
        state.PauseTiming();
        Array arr;
        for (int i = 0; i < state.range(0); i++) {
            arr.addBackArray("element" + to_string(i));
        }
        state.ResumeTiming();
        
        arr.addElemIndArray(0, "newElement");
    }
}
BENCHMARK(BM_AddAtIndexBeginning)->Arg(10)->Arg(25)->Arg(50);

static void BM_AddAtIndexMiddle(benchmark::State& state) {
    for (auto _ : state) {
        state.PauseTiming();
        Array arr;
        for (int i = 0; i < state.range(0); i++) {
            arr.addBackArray("element" + to_string(i));
        }
        state.ResumeTiming();
        
        int middle = arr.getSize() / 2;
        arr.addElemIndArray(middle, "newElement");
    }
}
BENCHMARK(BM_AddAtIndexMiddle)->Arg(10)->Arg(25)->Arg(50);

static void BM_AddAtIndexEnd(benchmark::State& state) {
    for (auto _ : state) {
        state.PauseTiming();
        Array arr;
        for (int i = 0; i < state.range(0); i++) {
            arr.addBackArray("element" + to_string(i));
        }
        state.ResumeTiming();
        
        arr.addElemIndArray(arr.getSize(), "newElement");
    }
}
BENCHMARK(BM_AddAtIndexEnd)->Arg(10)->Arg(25)->Arg(50);

static void BM_AddAtIndexWithResize(benchmark::State& state) {
    for (auto _ : state) {
        state.PauseTiming();
        Array arr(state.range(0));
        for (int i = 0; i < state.range(0); i++) {
            arr.addBackArray("element" + to_string(i));
        }
        state.ResumeTiming();
        
        arr.addElemIndArray(state.range(0) / 2, "newElement");
    }
}
BENCHMARK(BM_AddAtIndexWithResize)->Arg(10)->Arg(25)->Arg(50);

static void BM_RemoveFromIndex(benchmark::State& state) {
    for (auto _ : state) {
        state.PauseTiming();
        Array arr;
        for (int i = 0; i < state.range(0); i++) {
            arr.addBackArray("element" + to_string(i));
        }
        state.ResumeTiming();
        
        arr.remElemIndArray(state.range(0) / 2);
    }
}
BENCHMARK(BM_RemoveFromIndex)->Arg(10)->Arg(25)->Arg(50);

static void BM_GetElementIndex(benchmark::State& state) {
    Array arr;
    for (int i = 0; i < state.range(0); i++) {
        arr.addBackArray("element" + to_string(i));
    }
    
    for (auto _ : state) {
        benchmark::DoNotOptimize(arr.getElemIndArray(state.range(0) / 2));
    }
}
BENCHMARK(BM_GetElementIndex)->Arg(10)->Arg(25)->Arg(50);

static void BM_ReplacementElement(benchmark::State& state) {
    for (auto _ : state) {
        state.PauseTiming();
        Array arr;
        for (int i = 0; i < state.range(0); i++) {
            arr.addBackArray("element" + to_string(i));
        }
        state.ResumeTiming();
        
        arr.changeElemIndArray(state.range(0) / 2, "newValue");
    }
}
BENCHMARK(BM_ReplacementElement)->Arg(10)->Arg(25)->Arg(50);

static void BM_PushBackArr(benchmark::State& state) {
    for (auto _ : state) {
        state.PauseTiming();
        Array arr;
        state.ResumeTiming();
        
        for (int i = 0; i < state.range(0); i++) {
            arr.addBackArray("element" + to_string(i));
        }
    }
}
BENCHMARK(BM_PushBackArr)->Arg(10)->Arg(25)->Arg(50);

BENCHMARK_MAIN();