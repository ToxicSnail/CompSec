# Allocator Integration

## Описание проекта
Проект демонстрирует использование механизма аллокации памяти с фиксированными блоками в C++ путем интеграции кастомного аллокатора. Включает три режима работы аллокатора:

- **Heap Blocks Mode** – выделение памяти блоками в куче.
- **Heap Pool Mode** – использование пула памяти в куче.
- **Static Pool Mode** – использование статического пула памяти.

Проект состоит из трех частей:
- **allocator/** – исходный код аллокатора.
- **my_test_project/** – пользовательский проект, демонстрирующий работу аллокатора на трех режимах работы.
- **SAT-DPLL/** - проект, в котором реализовано решение задачи выполнимости булевой формулы (SAT).

## Структура проекта
```
project-root/ 
├── allocator/  
├── my_test_project/
| ├── main.cpp
| └── main_without_arrays.cpp
├── SAT_DPLL/ 
│ ├── main.cpp
│ ├── BBV.h/cpp,
│ ├── BoolEquation.h/cpp
│ ├── BoolInterval.h/cpp
│ ├── NodeBoolTreeh/cpp 
│ ├── IBranchingStrategy.h 
│ ├── MinOccurenceBranchingStrategy 
│ └── SatExamples/ 
├── .gitignore 
├── CMakeLists.txt 
└── README.md 
```

## Схема работы проекта SAT_DPLL
![sat_dpll](https://github.com/ToxicSnail/DIST_lab2/blob/dev/sat_dpll.png)

## Использование кастомных аллокаторов

В проекте применяется **фиксированный аллокатор памяти**. Для каждого типа создаётся отдельный аллокатор:

- `allocCNF` — для массива `BoolInterval*`
- `allocBoolInterval` — для интервалов CNF
- `allocBoolEquation` — для формул уравнений
- `allocNodeBoolTree` — для узлов дерева
- `allocStrategy` — для стратегии ветвления

## Пример аллокации
Фрагмент кода демонстрирует выделение и освобождение памяти с использованием кастомного аллокатора:

```cpp
// выделение памяти
void* mem = allocBoolInterval.Allocate(sizeof(BoolInterval));
BoolInterval* obj = new (mem) BoolInterval(...);

// освобождение
obj->~BoolInterval();
allocBoolInterval.Deallocate(obj);
```
## Стратегия ветвления
Выделена через интерфейс IBranchingStrategy, реализована конкретная стратегия:
```cpp 
class IBranchingStrategy 
{
public:
    virtual int chooseColumn(const BoolEquation& equation) = 0;
    virtual ~IBranchingStrategy() = default;
};
```
И пример реализации — `MinOccurenceBranchingStrategy`.

Таким образом, алгоритм не зависит от конкретной стратегии — её можно подменить в main().

## Сборка и запуск
### Требования
- Компилятор C++ (GCC, Clang, MSVC)
- CMake

### Сборка
```sh
mkdir build
cd build
cmake ..
make
```

### Запуск
```bash
./SAT_DPLL/sat_dpll
```

## Замер времени
В `main.cpp` добавлен код замера времени выполнения алгоритма:
```cpp
auto start = std::chrono::high_resolution_clock::now();

// реализация и запуск алгоритма

auto end = std::chrono::high_resolution_clock::now();
std::chrono::duration<double> diff = end - start;
std::cout << "Time spent: " << diff.count() << " s\n";
```

## Очистка и освобождение памяти
В конце `main()` вызывается:
- обход всех узлов дерева и ручной вызов деструкторов;
- возврат блоков памяти в аллокаторы;
- освобождение `CNF` и стратегии.

## Преимущества выбранной схемы аллокации
- Меньшее потребление памяти и времени при грамотной расчитаной задействованной памяти для проекта
- Возможность разных аллокаторов под разные типы
- Контроль времени жизни объектов

Это обеспечивает **отсутствие утечек памяти**.

## Авторы
- ToxicSnail

## Лицензия
Проект распространяется под лицензией MIT.

