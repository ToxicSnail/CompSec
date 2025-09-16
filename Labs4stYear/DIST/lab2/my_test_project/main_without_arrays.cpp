#include <iostream>
#include "Allocator.h"

class MyObject 
{
private:
    int id;
    float value;

public:
    MyObject(int _id, float _value) : id(_id), value(_value) 
    {
        std::cout << "Создан объект " << id << " со значением " << value << std::endl;
    }

    ~MyObject() 
    {
        std::cout << "Удален объект " << id << std::endl;
    }

    void display() const 
    {
        std::cout << "Объект " << id << " со значением " << value << std::endl;
    }
};

int main() 
{
    std::cout << "=== Демонстрация аллокатора ===" << std::endl;

    // Режим блоков кучи: неограниченные блоки по 100 байт
    Allocator allocatorHeapBlocks(sizeof(MyObject));

    // Выделение памяти под объект
    void* memory1 = allocatorHeapBlocks.Allocate(sizeof(MyObject));
    MyObject* obj1 = new (memory1) MyObject(1, 10.5f);

    obj1->display();

    obj1->~MyObject();
    allocatorHeapBlocks.Deallocate(obj1);

    std::cout << "-------------------------------" << std::endl;

    // Режим пула кучи
    Allocator allocatorHeapPool(sizeof(MyObject), 3);

    MyObject* objects[3];

    for (int i = 0; i < 3; ++i) 
    {
        void* memory = allocatorHeapPool.Allocate(sizeof(MyObject));
        objects[i] = new (memory) MyObject(i + 2, i * 2.5f);
    }

    for (int i = 0; i < 3; ++i) 
    {
        objects[i]->display();
    }

    for (int i = 0; i < 3; ++i) 
    {
        objects[i]->~MyObject();
        allocatorHeapPool.Deallocate(objects[i]);
    }

    std::cout << "-------------------------------" << std::endl;

    // Режим статического пула
    char staticMemoryPool[sizeof(MyObject) * 2];
    Allocator allocatorStaticPool(sizeof(MyObject), 2, staticMemoryPool);

    void* memory2 = allocatorStaticPool.Allocate(sizeof(MyObject));
    MyObject* obj2 = new (memory2) MyObject(5, 42.0f);

    obj2->display();

    obj2->~MyObject();
    allocatorStaticPool.Deallocate(obj2);

    std::cout << "=== Завершение работы ===" << std::endl;

    return 0;
}
