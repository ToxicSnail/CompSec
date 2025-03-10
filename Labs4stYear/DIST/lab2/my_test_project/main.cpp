#include <iostream>
#include <cstring>
#include <chrono>
#include "Allocator.h"

class MyObject 
{
private:
    int id;
    float value;

public:
    MyObject(int _id, float _value) : id(_id), value(_value) {}

    ~MyObject() {}

    int getId() const { return id; }
    float getValue() const { return value; }
};

void printArray(MyObject* objArray[], size_t size) 
{
    std::cout << "[";
    for (size_t i = 0; i < size; ++i) 
    {
        std::cout << "[" << objArray[i]->getId() << ", " << objArray[i]->getValue() << "]";
        if (i < size - 1) std::cout << ", ";
    }
    std::cout << "]" << std::endl;
}

void testHeapBlocksMode() 
{
    const size_t numObjects = 5;
    Allocator allocatorHeapBlocks(sizeof(MyObject));

    MyObject* objArray[numObjects];
    auto start = std::chrono::high_resolution_clock::now();

    for (size_t i = 0; i < numObjects; ++i) 
    {
        void* memory = allocatorHeapBlocks.Allocate(sizeof(MyObject));
        objArray[i] = new (memory) MyObject(i + 1, i * 1.5f);
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "Allocation time: " << std::chrono::duration<double, std::milli>(end - start).count() << " ms" << std::endl;
    printArray(objArray, numObjects);

    start = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < numObjects; ++i) 
    {
        objArray[i]->~MyObject();
        allocatorHeapBlocks.Deallocate(objArray[i]);
    }
    end = std::chrono::high_resolution_clock::now();
    std::cout << "Deallocate time: " << std::chrono::duration<double, std::milli>(end - start).count() << " ms" << std::endl;
}

void testHeapPoolMode() {
    const size_t numObjects = 5;
    Allocator allocatorHeapPool(sizeof(MyObject), numObjects);

    MyObject* objArray[numObjects];
    auto start = std::chrono::high_resolution_clock::now();

    for (size_t i = 0; i < numObjects; ++i) 
    {
        void* memory = allocatorHeapPool.Allocate(sizeof(MyObject));
        objArray[i] = new (memory) MyObject(i + 10, i * 2.0f);
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "Allocation time: " << std::chrono::duration<double, std::milli>(end - start).count() << " ms" << std::endl;
    printArray(objArray, numObjects);

    start = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < numObjects; ++i) 
    {
        objArray[i]->~MyObject();
        allocatorHeapPool.Deallocate(objArray[i]);
    }
    end = std::chrono::high_resolution_clock::now();
    std::cout << "Deallocate time: " << std::chrono::duration<double, std::milli>(end - start).count() << " ms" << std::endl;
}

void testStaticPoolMode() 
{
    const size_t numObjects = 5;
    char staticMemoryPool[sizeof(MyObject) * numObjects];
    Allocator allocatorStaticPool(sizeof(MyObject), numObjects, staticMemoryPool);

    MyObject* objArray[numObjects];
    auto start = std::chrono::high_resolution_clock::now();

    for (size_t i = 0; i < numObjects; ++i) 
    {
        void* memory = allocatorStaticPool.Allocate(sizeof(MyObject));
        objArray[i] = new (memory) MyObject(i + 20, i * 3.0f);
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "Allocation time: " << std::chrono::duration<double, std::milli>(end - start).count() << " ms" << std::endl;
    printArray(objArray, numObjects);

    start = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < numObjects; ++i) 
    {
        objArray[i]->~MyObject();
        allocatorStaticPool.Deallocate(objArray[i]);
    }
    end = std::chrono::high_resolution_clock::now();
    std::cout << "Deallocate time: " << std::chrono::duration<double, std::milli>(end - start).count() << " ms" << std::endl;
}
int main() 
{
    std::cout << "=== Array allocator demonstration ===" << std::endl;

    std::cout << "\n=== Heap Pool Mode ===" << std::endl;
    testHeapBlocksMode();
    
    std::cout << "\n=== Heap Blocks Mode ===" << std::endl;
    testHeapPoolMode();

    std::cout << "\n=== Static Pool Mode ===" << std::endl;
    testStaticPoolMode();

    std::cout << "\n=== Shutdown ===" << std::endl;
    return 0;
}
