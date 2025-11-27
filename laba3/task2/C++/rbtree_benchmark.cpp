#include <benchmark/benchmark.h>
#include <random>
#include <vector>
#include <unordered_set>

#include "structures.h"

vector<int> generateUniqueRandomNumbers(int count, int minVal = 1, int maxVal = 1000) {
    unordered_set<int> numbers;
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dist(minVal, maxVal);

    while (numbers.size() < count) {
        numbers.insert(dist(gen));
    }
    return vector<int>(numbers.begin(), numbers.end());
}

static void BM_InsertSingleNode(benchmark::State& state) {
    for (auto _ : state) {
        RBTree tree;
        tree.insertNode(42);
    }
}
BENCHMARK(BM_InsertSingleNode);

static void BM_InsertMultipleNodes(benchmark::State& state) {
    int numNodes = state.range(0);
    vector<int> keys = generateUniqueRandomNumbers(numNodes);

    for (auto _ : state) {
        RBTree tree;
        for (int key : keys) {
            tree.insertNode(key);
        }
    }
}
BENCHMARK(BM_InsertMultipleNodes)->Arg(5)->Arg(10)->Arg(20);

static void BM_FindExistingNode(benchmark::State& state) {
    RBTree tree;
    vector<int> keys = generateUniqueRandomNumbers(10);
    for (int key : keys) {
        tree.insertNode(key);
    }
    int targetKey = keys[5];

    for (auto _ : state) {
        auto node = tree.searchNode(targetKey);
        benchmark::DoNotOptimize(node);
    }
}
BENCHMARK(BM_FindExistingNode);

static void BM_FindMissingNode(benchmark::State& state) {
    RBTree tree;
    vector<int> keys = generateUniqueRandomNumbers(10);
    for (int key : keys) {
        tree.insertNode(key);
    }

    for (auto _ : state) {
        auto node = tree.searchNode(9999);
        benchmark::DoNotOptimize(node);
    }
}
BENCHMARK(BM_FindMissingNode);

static void BM_DeleteNode(benchmark::State& state) {
    vector<int> keys = generateUniqueRandomNumbers(10);
    int keyToDelete = keys[5];

    for (auto _ : state) {
        state.PauseTiming();
        RBTree tree;
        for (int key : keys) {
            tree.insertNode(key);
        }
        auto nodeToDelete = tree.searchNode(keyToDelete);
        state.ResumeTiming();

        tree.deleteNode(nodeToDelete->getData());
    }
}
BENCHMARK(BM_DeleteNode);

static void BM_FindMinimum(benchmark::State& state) {
    RBTree tree;
    vector<int> keys = generateUniqueRandomNumbers(10);
    for (int key : keys) {
        tree.insertNode(key);
    }

    for (auto _ : state) {
        auto minNode = tree.findMin(tree.getRoot());
        benchmark::DoNotOptimize(minNode);
    }
}
BENCHMARK(BM_FindMinimum);

static void BM_MixedOperations(benchmark::State& state) {
    vector<int> initialKeys = generateUniqueRandomNumbers(5);
    vector<int> operationKeys = generateUniqueRandomNumbers(3, 100, 200);

    for (auto _ : state) {
        RBTree tree;
        for (int key : initialKeys) {
            tree.insertNode(key);
        }

        for (int i = 0; i < 3; ++i) {
            tree.insertNode(operationKeys[i]);
            auto node = tree.searchNode(initialKeys[i % initialKeys.size()]);
            benchmark::DoNotOptimize(node);
        }
    }
}
BENCHMARK(BM_MixedOperations);

static void BM_InsertSortedData(benchmark::State& state) {
    int numNodes = state.range(0);

    for (auto _ : state) {
        RBTree tree;
        for (int i = 1; i <= numNodes; ++i) {
            tree.insertNode(i);
        }
    }
}
BENCHMARK(BM_InsertSortedData)->Arg(5)->Arg(10);

BENCHMARK_MAIN();