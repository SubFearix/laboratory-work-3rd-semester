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

static void BM_addFrontTL(benchmark::State& state) {
    for (auto _ : state) {
        state.PauseTiming();
        TLList list;
        for (int i = 0; i < state.range(0); i++) {
            list.addBackTL("element" + to_string(i));
        }
        state.ResumeTiming();
        
        OutputDisabler disabler;
        list.addFrontTL("newHead");
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_addFrontTL)->Arg(10)->Arg(20)->Arg(30);

static void BM_addBackTL(benchmark::State& state) {
    for (auto _ : state) {
        state.PauseTiming();
        TLList list;
        for (int i = 0; i < state.range(0); i++) {
            list.addBackTL("element" + to_string(i));
        }
        state.ResumeTiming();
        
        OutputDisabler disabler;
        list.addBackTL("newTail");
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_addBackTL)->Arg(10)->Arg(20)->Arg(30);

static void BM_insertBeforeIndTL(benchmark::State& state) {
    for (auto _ : state) {
        state.PauseTiming();
        TLList list;
        for (int i = 0; i < state.range(0); i++) {
            list.addBackTL("element" + to_string(i));
        }
        state.ResumeTiming();
        
        int middle = state.range(0) / 2;
        OutputDisabler disabler;
        list.insertBeforeIndTL(middle, "newMiddle");
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_insertBeforeIndTL)->Arg(10)->Arg(20)->Arg(30);

static void BM_insertAfterIndTL(benchmark::State& state) {
    for (auto _ : state) {
        state.PauseTiming();
        TLList list;
        for (int i = 0; i < state.range(0); i++) {
            list.addBackTL("element" + to_string(i));
        }
        state.ResumeTiming();
        
        int middle = state.range(0) / 2;
        OutputDisabler disabler;
        list.insertAfterIndTL(middle, "newMiddle");
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_insertAfterIndTL)->Arg(10)->Arg(20)->Arg(30);

static void BM_SearchValueHead(benchmark::State& state) {
    TLList list;
    for (int i = 0; i < state.range(0); i++) {
        list.addBackTL("element" + to_string(i));
    }
    
    for (auto _ : state) {
        OutputDisabler disabler;
        list.findValTL("element0");
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_SearchValueHead)->Arg(10)->Arg(20)->Arg(30);

static void BM_SearchValueMiddle(benchmark::State& state) {
    TLList list;
    for (int i = 0; i < state.range(0); i++) {
        list.addBackTL("element" + to_string(i));
    }
    
    for (auto _ : state) {
        int middle = state.range(0) / 2;
        OutputDisabler disabler;
        list.findValTL("element" + to_string(middle));
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_SearchValueMiddle)->Arg(10)->Arg(20)->Arg(30);

static void BM_SearchValueTail(benchmark::State& state) {
    TLList list;
    for (int i = 0; i < state.range(0); i++) {
        list.addBackTL("element" + to_string(i));
    }
    
    for (auto _ : state) {
        OutputDisabler disabler;
        list.findValTL("element" + to_string(state.range(0) - 1));
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_SearchValueTail)->Arg(10)->Arg(20)->Arg(30);

static void BM_SearchValueNotFound(benchmark::State& state) {
    TLList list;
    for (int i = 0; i < state.range(0); i++) {
        list.addBackTL("element" + to_string(i));
    }
    
    for (auto _ : state) {
        OutputDisabler disabler;
        list.findValTL("nonexistent");
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_SearchValueNotFound)->Arg(10)->Arg(20)->Arg(30);

static void BM_DeleteAtHead(benchmark::State& state) {
    for (auto _ : state) {
        state.PauseTiming();
        TLList list;
        for (int i = 0; i < state.range(0); i++) {
            list.addBackTL("element" + to_string(i));
        }
        state.ResumeTiming();
        
        OutputDisabler disabler;
        list.remFrontTL();
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_DeleteAtHead)->Arg(10)->Arg(20)->Arg(30);

static void BM_DeleteAtTail(benchmark::State& state) {
    for (auto _ : state) {
        state.PauseTiming();
        TLList list;
        for (int i = 0; i < state.range(0); i++) {
            list.addBackTL("element" + to_string(i));
        }
        state.ResumeTiming();
        
        OutputDisabler disabler;
        list.remBackTL();
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_DeleteAtTail)->Arg(10)->Arg(20)->Arg(30);

static void BM_DeleteBeforeIndex(benchmark::State& state) {
    for (auto _ : state) {
        state.PauseTiming();
        TLList list;
        for (int i = 0; i < state.range(0); i++) {
            list.addBackTL("element" + to_string(i));
        }
        state.ResumeTiming();
        
        int middle = state.range(0) / 2;
        if (middle > 0) {
            OutputDisabler disabler;
            list.deleteBeforeIndTL(middle);
            benchmark::ClobberMemory();
        }
    }
}
BENCHMARK(BM_DeleteBeforeIndex)->Arg(10)->Arg(20)->Arg(30);

static void BM_deleteAfterIndTL(benchmark::State& state) {
    for (auto _ : state) {
        state.PauseTiming();
        TLList list;
        for (int i = 0; i < state.range(0); i++) {
            list.addBackTL("element" + to_string(i));
        }
        state.ResumeTiming();
        
        int middle = state.range(0) / 2;
        if (middle < state.range(0) - 1) {
            OutputDisabler disabler;
            list.deleteAfterIndTL(middle);
            benchmark::ClobberMemory();
        }
    }
}
BENCHMARK(BM_deleteAfterIndTL)->Arg(10)->Arg(20)->Arg(30);

static void BM_DeleteValueHead(benchmark::State& state) {
    for (auto _ : state) {
        state.PauseTiming();
        TLList list;
        for (int i = 0; i < state.range(0); i++) {
            list.addBackTL("element" + to_string(i));
        }
        state.ResumeTiming();
        
        OutputDisabler disabler;
        list.remValTL("element0");
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_DeleteValueHead)->Arg(10)->Arg(20)->Arg(30);

static void BM_DeleteValueMiddle(benchmark::State& state) {
    for (auto _ : state) {
        state.PauseTiming();
        TLList list;
        for (int i = 0; i < state.range(0); i++) {
            list.addBackTL("element" + to_string(i));
        }
        state.ResumeTiming();
        
        int middle = state.range(0) / 2;
        OutputDisabler disabler;
        list.remValTL("element" + to_string(middle));
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_DeleteValueMiddle)->Arg(10)->Arg(20)->Arg(30);

static void BM_DeleteValueTail(benchmark::State& state) {
    for (auto _ : state) {
        state.PauseTiming();
        TLList list;
        for (int i = 0; i < state.range(0); i++) {
            list.addBackTL("element" + to_string(i));
        }
        state.ResumeTiming();
        
        OutputDisabler disabler;
        list.remValTL("element" + to_string(state.range(0) - 1));
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_DeleteValueTail)->Arg(10)->Arg(20)->Arg(30);

static void BM_PrintTLList(benchmark::State& state) {
    TLList list;
    for (int i = 0; i < state.range(0); i++) {
        list.addBackTL("element" + to_string(i));
    }
    
    for (auto _ : state) {
        OutputDisabler disabler;
        list.printListTL();
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_PrintTLList)->Arg(10)->Arg(20)->Arg(30);

static void BM_MixedAddRemove(benchmark::State& state) {
    for (auto _ : state) {
        TLList list;
        OutputDisabler disabler;
        
        for (int i = 0; i < state.range(0); i++) {
            list.addBackTL("element" + to_string(i));
            if (i % 3 == 0) {
                list.addFrontTL("headElement" + to_string(i));
            }
            if (i % 4 == 0 && i > 0) {
                list.remBackTL();
            }
            if (i % 5 == 0 && i > 0) {
                list.remFrontTL();
            }
        }
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_MixedAddRemove)->Arg(10)->Arg(20)->Arg(30);

static void BM_MixedInsertOperations(benchmark::State& state) {
    for (auto _ : state) {
        TLList list;
        OutputDisabler disabler;
        
        for (int i = 0; i < state.range(0); i++) {
            list.addBackTL("base" + to_string(i));
        }
        
        if (state.range(0) > 2) {
            list.insertBeforeIndTL(state.range(0) / 2, "before_middle");
            list.insertAfterIndTL(state.range(0) / 2, "after_middle");
            list.addFrontTL("new_head");
            list.addBackTL("new_tail");
        }
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_MixedInsertOperations)->Arg(10)->Arg(20)->Arg(30);

static void BM_MixedDeleteOperations(benchmark::State& state) {
    for (auto _ : state) {
        state.PauseTiming();
        TLList list;
        for (int i = 0; i < state.range(0) * 2; i++) {
            list.addBackTL("element" + to_string(i));
        }
        state.ResumeTiming();
        
        OutputDisabler disabler;
        if (state.range(0) > 3) {
            list.remFrontTL();
            list.remBackTL();
            list.deleteBeforeIndTL(state.range(0) / 2);
            list.deleteAfterIndTL(state.range(0) / 4);
            list.remValTL("element" + to_string(state.range(0)));
        }
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_MixedDeleteOperations)->Arg(10)->Arg(20)->Arg(30);

static void BM_EmptyListOperations(benchmark::State& state) {
    for (auto _ : state) {
        TLList list;
        OutputDisabler disabler;
        
        list.remFrontTL();
        list.remBackTL();
        list.findValTL("anything");
        list.printListTL();
        
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_EmptyListOperations);

BENCHMARK_MAIN();