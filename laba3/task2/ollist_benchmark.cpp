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

static void BM_AddAtHead(benchmark::State& state) {
    for (auto _ : state) {
        state.PauseTiming();
        OLList list;
        for (int i = 0; i < state.range(0); i++) {
            list.addBackOL("element" + to_string(i));
        }
        state.ResumeTiming();
        
        list.addFrontOL("newHead");
    }
}
BENCHMARK(BM_AddAtHead)->Arg(10)->Arg(20)->Arg(30);

static void BM_AddAtTail(benchmark::State& state) {
    for (auto _ : state) {
        state.PauseTiming();
        OLList list;
        for (int i = 0; i < state.range(0); i++) {
            list.addBackOL("element" + to_string(i));
        }
        state.ResumeTiming();
        
        list.addBackOL("newTail");
    }
}
BENCHMARK(BM_AddAtTail)->Arg(10)->Arg(20)->Arg(30);

static void BM_SearchNodeMiddle(benchmark::State& state) {
    OLList list;
    for (int i = 0; i < state.range(0); i++) {
        list.addBackOL("element" + to_string(i));
    }
    
    for (auto _ : state) {
        int middle = state.range(0) / 2;
        OutputDisabler disabler;
        list.findValOL("element" + to_string(middle));
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_SearchNodeMiddle)->Arg(10)->Arg(20)->Arg(30);

static void BM_SearchNodeTail(benchmark::State& state) {
    OLList list;
    for (int i = 0; i < state.range(0); i++) {
        list.addBackOL("element" + to_string(i));
    }
    
    for (auto _ : state) {
        OutputDisabler disabler;
        list.findValOL("element" + to_string(state.range(0) - 1));
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_SearchNodeTail)->Arg(10)->Arg(20)->Arg(30);

static void BM_SearchNodeNotFound(benchmark::State& state) {
    OLList list;
    for (int i = 0; i < state.range(0); i++) {
        list.addBackOL("element" + to_string(i));
    }
    
    for (auto _ : state) {
        OutputDisabler disabler;
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_SearchNodeNotFound)->Arg(10)->Arg(20)->Arg(30);

static void BM_AddBeforeIndex(benchmark::State& state) {
    for (auto _ : state) {
        state.PauseTiming();
        OLList list;
        for (int i = 0; i < state.range(0); i++) {
            list.addBackOL("element" + to_string(i));
        }
        state.ResumeTiming();
        
        int middle = state.range(0) / 2;
        OutputDisabler disabler;
        list.insertBeforeIndOL(middle,"newMiddle");
    }
}
BENCHMARK(BM_AddBeforeIndex)->Arg(10)->Arg(20)->Arg(30);

static void BM_AddAfterIndex(benchmark::State& state) {
    for (auto _ : state) {
        state.PauseTiming();
        OLList list;
        for (int i = 0; i < state.range(0); i++) {
            list.addBackOL("element" + to_string(i));
        }
        state.ResumeTiming();
        
        int middle = state.range(0) / 2;
        OutputDisabler disabler;
        list.insertAfterIndOL(middle, "newMiddle");
    }
}
BENCHMARK(BM_AddAfterIndex)->Arg(10)->Arg(20)->Arg(30);

static void BM_DeleteAtHead(benchmark::State& state) {
    for (auto _ : state) {
        state.PauseTiming();
        OLList list;
        for (int i = 0; i < state.range(0); i++) {
            list.addBackOL("element" + to_string(i));
        }
        state.ResumeTiming();
        
        OutputDisabler disabler;
        list.remFrontOL();
    }
}
BENCHMARK(BM_DeleteAtHead)->Arg(10)->Arg(20)->Arg(30);

static void BM_DeleteAtTail(benchmark::State& state) {
    for (auto _ : state) {
        state.PauseTiming();
        OLList list;
        for (int i = 0; i < state.range(0); i++) {
            list.addBackOL("element" + to_string(i));
        }
        state.ResumeTiming();
        
        OutputDisabler disabler;
        list.remBackOL();
    }
}
BENCHMARK(BM_DeleteAtTail)->Arg(10)->Arg(20)->Arg(30);

static void BM_ValueDeleteHead(benchmark::State& state) {
    for (auto _ : state) {
        state.PauseTiming();
        OLList list;
        for (int i = 0; i < state.range(0); i++) {
            list.addBackOL("element" + to_string(i));
        }
        state.ResumeTiming();
        
        OutputDisabler disabler;
        list.remValOL("element0");
    }
}
BENCHMARK(BM_ValueDeleteHead)->Arg(10)->Arg(20)->Arg(30);

static void BM_ValueDeleteMiddle(benchmark::State& state) {
    for (auto _ : state) {
        state.PauseTiming();
        OLList list;
        for (int i = 0; i < state.range(0); i++) {
            list.addBackOL("element" + to_string(i));
        }
        state.ResumeTiming();
        
        int middle = state.range(0) / 2;
        OutputDisabler disabler;
        list.remValOL("element" + to_string(middle));
    }
}
BENCHMARK(BM_ValueDeleteMiddle)->Arg(10)->Arg(20)->Arg(30);

static void BM_PrintList(benchmark::State& state) {
    OLList list;
    for (int i = 0; i < state.range(0); i++) {
        list.addBackOL("element" + to_string(i));
    }
    
    for (auto _ : state) {
        OutputDisabler disabler;
        list.printListOL();
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_PrintList)->Arg(10)->Arg(20)->Arg(30);

static void BM_MixedOperations(benchmark::State& state) {
    for (auto _ : state) {
        state.PauseTiming();
        OLList list;
        state.ResumeTiming();
        
        OutputDisabler disabler;
        for (int i = 0; i < state.range(0); i++) {
            list.addBackOL("element" + to_string(i));
        }
        
        if (state.range(0) > 2) {
            list.insertBeforeIndOL(state.range(0) / 2, "middle1");
            list.insertAfterIndOL(state.range(0) / 2, "middle2");
            list.remFrontOL();
            list.remBackOL();
        }
    }
}
BENCHMARK(BM_MixedOperations)->Arg(10)->Arg(20)->Arg(30);

static void BM_DeleteBeforeIndex(benchmark::State& state) {
    for (auto _ : state) {
        state.PauseTiming();
        OLList list;
        for (int i = 0; i < state.range(0); i++) {
            list.addBackOL("element" + to_string(i));
        }
        state.ResumeTiming();
        
        int middle = state.range(0) / 2;
        if (middle > 0) {
            OutputDisabler disabler;
            list.deleteBeforeIndOL(middle);
        }
    }
}
BENCHMARK(BM_DeleteBeforeIndex)->Arg(10)->Arg(20)->Arg(30);

static void BM_DeleteAfterIndex(benchmark::State& state) {
    for (auto _ : state) {
        state.PauseTiming();
        OLList list;
        for (int i = 0; i < state.range(0); i++) {
            list.addBackOL("element" + to_string(i));
        }
        state.ResumeTiming();
        
        int middle = state.range(0) / 2;
        if (middle < state.range(0) - 1) {
            OutputDisabler disabler;
            list.deleteAfterIndOL(middle);
        }
    }
}
BENCHMARK(BM_DeleteAfterIndex)->Arg(10)->Arg(20)->Arg(30);

BENCHMARK_MAIN();