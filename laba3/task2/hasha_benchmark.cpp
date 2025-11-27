#include <benchmark/benchmark.h>
#include "structures.h"
#include <sstream>
#include <iostream>
#include <vector>
#include <string>
#include <random>

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

static void BM_AddElementA(benchmark::State& state) {
    OutputDisabler disabler;
    int numElements = state.range(0);

    for (auto _ : state) {
        state.PauseTiming();
        HashA table;
        vector<string> keys;
        for (int i = 0; i < numElements; ++i) {
            keys.push_back("key" + to_string(i));
        }
        state.ResumeTiming();

        for (int i = 0; i < numElements; ++i) {
            table.addElementA(keys[i], i);
        }
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_AddElementA)->Arg(10)->Arg(50)->Arg(100)->Arg(200);

static void BM_AddElementAWithCollisions(benchmark::State& state) {
    OutputDisabler disabler;
    int numElements = state.range(0);

    for (auto _ : state) {
        state.PauseTiming();
        HashA table;
        vector<string> keys;
        for (int i = 0; i < numElements; ++i) {
            keys.push_back("collision_key_" + to_string(i % 10));
        }
        state.ResumeTiming();

        for (int i = 0; i < numElements; ++i) {
            table.addElementA(keys[i], i);
        }
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_AddElementAWithCollisions)->Arg(10)->Arg(50)->Arg(100);

static void BM_UpdateElementA(benchmark::State& state) {
    OutputDisabler disabler;
    int numElements = state.range(0);

    for (auto _ : state) {
        state.PauseTiming();
        HashA table;
        vector<string> keys;
        for (int i = 0; i < numElements; ++i) {
            keys.push_back("key" + to_string(i));
            table.addElementA(keys[i], i);
        }
        state.ResumeTiming();

        for (int i = 0; i < numElements; ++i) {
            table.addElementA(keys[i], i + 1000);
        }
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_UpdateElementA)->Arg(10)->Arg(50)->Arg(100);

// с рехешированием
static void BM_AddElementAWithRehash(benchmark::State& state) {
    OutputDisabler disabler;
    int numElements = state.range(0);

    for (auto _ : state) {
        HashA table;
        OutputDisabler disabler;

        for (int i = 0; i < numElements; ++i) {
            table.addElementA("key" + to_string(i), i);
        }
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_AddElementAWithRehash)->Arg(5)->Arg(10)->Arg(20)->Arg(50);

static void BM_FindElementA(benchmark::State& state) {
    OutputDisabler disabler;
    HashA table;
    int numElements = state.range(0);

    vector<string> keys;
    for (int i = 0; i < numElements; ++i) {
        keys.push_back("key" + to_string(i));
        table.addElementA(keys[i], i);
    }

    for (auto _ : state) {
        for (int i = 0; i < numElements; ++i) {
            int result = table.findElementA(keys[i]);
            benchmark::DoNotOptimize(result);
        }
    }
}
BENCHMARK(BM_FindElementA)->Arg(10)->Arg(50)->Arg(100)->Arg(200);

// для поиска с длинными цепочками
static void BM_FindElementAWithProbing(benchmark::State& state) {
    OutputDisabler disabler;
    HashA table;
    int numElements = state.range(0);

    vector<string> keys;
    for (int i = 0; i < numElements; ++i) {
        keys.push_back("collision_key_" + to_string(i));
        table.addElementA(keys[i], i);
    }

    for (auto _ : state) {
        for (int i = 0; i < numElements; ++i) {
            int result = table.findElementA(keys[i]);
            benchmark::DoNotOptimize(result);
        }
    }
}
BENCHMARK(BM_FindElementAWithProbing)->Arg(10)->Arg(20)->Arg(30);

// для поиска несуществующих элементов
static void BM_FindElementANotFound(benchmark::State& state) {
    OutputDisabler disabler;
    HashA table;
    int numElements = state.range(0);

    for (int i = 0; i < numElements; ++i) {
        table.addElementA("key" + to_string(i), i);
    }

    for (auto _ : state) {
        for (int i = 0; i < 10; ++i) {
            int result = table.findElementA("nonexistent_key_" + to_string(i));
            benchmark::DoNotOptimize(result);
        }
    }
}
BENCHMARK(BM_FindElementANotFound)->Arg(10)->Arg(50)->Arg(100);


static void BM_DeleteElementA(benchmark::State& state) {
    OutputDisabler disabler;
    int numElements = state.range(0);

    for (auto _ : state) {
        state.PauseTiming();
        HashA table;
        vector<string> keys;
        for (int i = 0; i < numElements; ++i) {
            keys.push_back("key" + to_string(i));
            table.addElementA(keys[i], i);
        }
        state.ResumeTiming();

        for (int i = 0; i < numElements; ++i) {
            table.deleteElementA(keys[i]);
        }
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_DeleteElementA)->Arg(10)->Arg(50)->Arg(100);

static void BM_DeleteElementAWithProbing(benchmark::State& state) {
    OutputDisabler disabler;
    int numElements = state.range(0);

    for (auto _ : state) {
        state.PauseTiming();
        HashA table;
        vector<string> keys;
        for (int i = 0; i < numElements; ++i) {
            keys.push_back("collision_key_" + to_string(i));
            table.addElementA(keys[i], i);
        }
        state.ResumeTiming();

        for (int i = 0; i < numElements; ++i) {
            table.deleteElementA(keys[i]);
        }
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_DeleteElementAWithProbing)->Arg(10)->Arg(20)->Arg(30);

static void BM_DeleteElementANotFound(benchmark::State& state) {
    OutputDisabler disabler;
    HashA table;
    int numElements = state.range(0);

    for (int i = 0; i < numElements; ++i) {
        table.addElementA("key" + to_string(i), i);
    }

    for (auto _ : state) {
        for (int i = 0; i < 10; ++i) {
            table.deleteElementA("nonexistent_key_" + to_string(i));
        }
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_DeleteElementANotFound)->Arg(10)->Arg(50)->Arg(100);

static void BM_HashFunctionA(benchmark::State& state) {
    OutputDisabler disabler;
    HashA hsa;
    int numKeys = state.range(0);

    vector<string> keys;
    for (int i = 0; i < numKeys; ++i) {
        keys.push_back("test_key_" + to_string(i) + "_long_string_for_hashing");
    }

    for (auto _ : state) {
        for (int i = 0; i < numKeys; ++i) {
            int hash = hsa.hashFunc(keys[i], 100);
            benchmark::DoNotOptimize(hash);
        }
    }
}
BENCHMARK(BM_HashFunctionA)->Arg(10)->Arg(50)->Arg(100);

static void BM_RehashOperation(benchmark::State& state) {
    OutputDisabler disabler;
    int numElements = state.range(0);

    for (auto _ : state) {
        state.PauseTiming();
        HashA table;
        for (int i = 0; i < numElements; ++i) {
            table.addElementA("key" + to_string(i), i);
        }
        state.ResumeTiming();

        table.addElementA("trigger_rehash", 999);
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_RehashOperation)->Arg(5)->Arg(6)->Arg(7);

static void BM_PrintHashTableA(benchmark::State& state) {
    OutputDisabler disabler;
    HashA table;
    int numElements = state.range(0);

    for (int i = 0; i < numElements; ++i) {
        table.addElementA("key" + to_string(i), i);
    }

    for (auto _ : state) {
        table.printHashTableA();
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_PrintHashTableA)->Arg(5)->Arg(10)->Arg(20);

static void BM_OperationsWithDeleted(benchmark::State& state) {
    OutputDisabler disabler;
    int numElements = state.range(0);

    for (auto _ : state) {
        state.PauseTiming();
        HashA table;
        vector<string> keys;
        for (int i = 0; i < numElements; ++i) {
            keys.push_back("key" + to_string(i));
            table.addElementA(keys[i], i);
        }
        for (int i = 0; i < numElements / 2; ++i) {
            table.deleteElementA(keys[i]);
        }
        state.ResumeTiming();

        for (int i = 0; i < numElements; ++i) {
            int result = table.findElementA(keys[i]);
            benchmark::DoNotOptimize(result);
        }
    }
}
BENCHMARK(BM_OperationsWithDeleted)->Arg(10)->Arg(20);

static void BM_TableCreationDestructionA(benchmark::State& state) {
    int numTables = state.range(0);

    for (auto _ : state) {
        for (int i = 0; i < numTables; ++i) {
            HashA table;
            table.addElementA("test", 1);
            int result = table.findElementA("test");
            benchmark::DoNotOptimize(result);
        }
    }
}
BENCHMARK(BM_TableCreationDestructionA)->Arg(10)->Arg(50)->Arg(100);

static void BM_EmptyTableOperationsA(benchmark::State& state) {
    for (auto _ : state) {
        HashA table;
        OutputDisabler disabler;

        int result = table.findElementA("nonexistent");
        table.deleteElementA("nonexistent");
        benchmark::DoNotOptimize(result);

        table.printHashTableA();
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_EmptyTableOperationsA);

BENCHMARK_MAIN();