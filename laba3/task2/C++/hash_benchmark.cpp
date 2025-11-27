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

string generateRandomString(int length) {
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
    string result;
    result.reserve(length);

    for (int i = 0; i < length; ++i) {
        result += alphanum[rand() % (sizeof(alphanum) - 1)];
    }
    return result;
}

static void BM_AddElement(benchmark::State& state) {
    Hash table;
    int numElements = state.range(0);

    vector<string> keys;
    for (int i = 0; i < numElements; ++i) {
        keys.push_back("key" + to_string(i));
    }

    for (auto _ : state) {
        for (int i = 0; i < numElements; ++i) {
            table.addElement(keys[i], i);
        }
        state.PauseTiming();
        for (int i = 0; i < numElements; ++i) {
            table.deleteElement(keys[i]);
        }
        state.ResumeTiming();
    }
}
BENCHMARK(BM_AddElement)->Arg(10)->Arg(50)->Arg(100)->Arg(500);

static void BM_AddElementWithCollisions(benchmark::State& state) {
    Hash table;
    int numElements = state.range(0);

    vector<string> keys;
    for (int i = 0; i < numElements; ++i) {
        keys.push_back("collision_key_" + to_string(i % 5));
    }

    for (auto _ : state) {
        for (int i = 0; i < numElements; ++i) {
            table.addElement(keys[i], i);
        }
        state.PauseTiming();
        for (int i = 0; i < numElements; ++i) {
            table.deleteElement(keys[i]);
        }
        state.ResumeTiming();
    }
}
BENCHMARK(BM_AddElementWithCollisions)->Arg(10)->Arg(50)->Arg(100);

static void BM_UpdateElement(benchmark::State& state) {
    Hash table;
    int numElements = state.range(0);

    vector<string> keys;
    for (int i = 0; i < numElements; ++i) {
        keys.push_back("key" + to_string(i));
        table.addElement(keys[i], i);
    }

    for (auto _ : state) {
        for (int i = 0; i < numElements; ++i) {
            table.addElement(keys[i], i + 1000);
        }
    }
}
BENCHMARK(BM_UpdateElement)->Arg(10)->Arg(50)->Arg(100);

static void BM_FindElement(benchmark::State& state) {
    Hash table;
    int numElements = state.range(0);

    vector<string> keys;
    for (int i = 0; i < numElements; ++i) {
        keys.push_back("key" + to_string(i));
        table.addElement(keys[i], i);
    }

    for (auto _ : state) {
        for (int i = 0; i < numElements; ++i) {
            int result = table.findElement(keys[i]);
            benchmark::DoNotOptimize(result);
        }
    }
}
BENCHMARK(BM_FindElement)->Arg(10)->Arg(50)->Arg(100)->Arg(500);

static void BM_FindElementChainHead(benchmark::State& state) {
    Hash table;
    int numElements = state.range(0);

    vector<string> keys;
    for (int i = 0; i < numElements; ++i) {
        keys.push_back("chain_key_" + to_string(i));
        table.addElement(keys[i], i);
    }

    for (auto _ : state) {
        int result = table.findElement(keys[0]);
        benchmark::DoNotOptimize(result);
    }
}
BENCHMARK(BM_FindElementChainHead)->Arg(5)->Arg(10)->Arg(20);

static void BM_FindElementChainTail(benchmark::State& state) {
    Hash table;
    int numElements = state.range(0);

    vector<string> keys;
    for (int i = 0; i < numElements; ++i) {
        keys.push_back("chain_key_" + to_string(i));
        table.addElement(keys[i], i);
    }

    for (auto _ : state) {
        int result = table.findElement(keys[numElements - 1]);
        benchmark::DoNotOptimize(result);
    }
}
BENCHMARK(BM_FindElementChainTail)->Arg(5)->Arg(10)->Arg(20);

static void BM_FindElementNotFound(benchmark::State& state) {
    Hash table;
    int numElements = state.range(0);

    for (int i = 0; i < numElements; ++i) {
        table.addElement("key" + to_string(i), i);
    }

    for (auto _ : state) {
        int result = table.findElement("nonexistent_key");
        benchmark::DoNotOptimize(result);
    }
}
BENCHMARK(BM_FindElementNotFound)->Arg(10)->Arg(50)->Arg(100);

static void BM_DeleteElement(benchmark::State& state) {
    int numElements = state.range(0);

    for (auto _ : state) {
        state.PauseTiming();
        Hash table;
        vector<string> keys;
        for (int i = 0; i < numElements; ++i) {
            keys.push_back("key" + to_string(i));
            table.addElement(keys[i], i);
        }
        state.ResumeTiming();

        for (int i = 0; i < numElements; ++i) {
            table.deleteElement(keys[i]);
        }
    }
}
BENCHMARK(BM_DeleteElement)->Arg(10)->Arg(50)->Arg(100);

static void BM_DeleteElementChainHead(benchmark::State& state) {
    int numElements = state.range(0);

    for (auto _ : state) {
        state.PauseTiming();
        Hash table;
        vector<string> keys;
        for (int i = 0; i < numElements; ++i) {
            keys.push_back("chain_key_" + to_string(i));
            table.addElement(keys[i], i);
        }
        state.ResumeTiming();

        table.deleteElement(keys[0]);
    }
}
BENCHMARK(BM_DeleteElementChainHead)->Arg(5)->Arg(10)->Arg(20);

static void BM_DeleteElementChainTail(benchmark::State& state) {
    int numElements = state.range(0);

    for (auto _ : state) {
        state.PauseTiming();
        Hash table;
        vector<string> keys;
        for (int i = 0; i < numElements; ++i) {
            keys.push_back("chain_key_" + to_string(i));
            table.addElement(keys[i], i);
        }
        state.ResumeTiming();

        table.deleteElement(keys[numElements - 1]);
    }
}
BENCHMARK(BM_DeleteElementChainTail)->Arg(5)->Arg(10)->Arg(20);

static void BM_PrintHashTable(benchmark::State& state) {
    Hash table;
    int numElements = state.range(0);

    for (int i = 0; i < numElements; ++i) {
        table.addElement("key" + to_string(i), i);
    }

    for (auto _ : state) {
        OutputDisabler disabler;
        table.printHashTable();
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_PrintHashTable)->Arg(5)->Arg(10)->Arg(20);

static void BM_HashFunction(benchmark::State& state) {
    Hash hf;
    int numKeys = state.range(0);

    vector<string> keys;
    for (int i = 0; i < numKeys; ++i) {
        keys.push_back("test_key_" + to_string(i) + "_long_string_for_hashing");
    }

    for (auto _ : state) {
        for (int i = 0; i < numKeys; ++i) {
            int hash = hf.hashFunc(keys[i], 100);
            benchmark::DoNotOptimize(hash);
        }
    }
}
BENCHMARK(BM_HashFunction)->Arg(10)->Arg(50)->Arg(100);

static void BM_MixedOperations(benchmark::State& state) {
    Hash table;
    int numOperations = state.range(0);

    for (auto _ : state) {
        for (int i = 0; i < numOperations; ++i) {
            string key = "key" + to_string(i % 100);

            if (i % 3 == 0) {
                table.addElement(key, i);
            } else if (i % 3 == 1) {
                int result = table.findElement(key);
                benchmark::DoNotOptimize(result);
            } else {
                table.deleteElement(key);
            }
        }
    }
}
BENCHMARK(BM_MixedOperations)->Arg(100)->Arg(500)->Arg(1000);

static void BM_TableCreationDestruction(benchmark::State& state) {
    int numTables = state.range(0);

    for (auto _ : state) {
        for (int i = 0; i < numTables; ++i) {
            Hash table;
            table.addElement("test", 1);
            int result = table.findElement("test");
            benchmark::DoNotOptimize(result);
        }
    }
}
BENCHMARK(BM_TableCreationDestruction)->Arg(10)->Arg(50)->Arg(100);

static void BM_EmptyTableOperations(benchmark::State& state) {
    for (auto _ : state) {
        Hash table;

        int result = table.findElement("nonexistent");
        table.deleteElement("nonexistent");
        benchmark::DoNotOptimize(result);

        OutputDisabler disabler;
        table.printHashTable();
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_EmptyTableOperations);

BENCHMARK_MAIN();