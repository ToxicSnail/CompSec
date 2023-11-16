#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

int* ArrayFilling(int size, int rangeA, int rangeB);
void Bubble(int* array, int size); //пузырек
void DirectInsert(int* array, int size); //Прямая вставка
void DirectSelection(int* array, int size); //Прямой выбор
bool CheckSortedArray(int* array, int size);
int* CopyArray(int* arr, int size);


int main()
{
	int size;
	printf("Array size is ");
	scanf("%d", &size);
	if (size == 0 || size <= 0)
	{
		printf("Array size can be int and more then 0.");
		return 0;
	}
	printf("\n");
	int* FirstArray = ArrayFilling(size, -1, 10);
	int* SecondArray = CopyArray(FirstArray, size);
	int* ThirdArray = CopyArray(FirstArray, size);
	//int* SecondArray = ArrayFilling(size, -1, 10);
	//int* ThirdArray = ArrayFilling(size, -1, 10);

	clock_t start = clock();
	DirectInsert(FirstArray, size);
	clock_t end = clock();
	double seconds1 = (double)(end - start) / CLOCKS_PER_SEC;
	if (CheckSortedArray(FirstArray, size))
	{
		printf("%s\n", "Array was been sorted");
		printf("Insert sort - %f second\n\n", seconds1); //выводим результат времени работы алгоритма в секундах
	}
	else
		printf("%s", "Array wasn't been sorted in Insert sort");
	free(FirstArray);


	start = clock();
	Bubble(SecondArray, size);
	end = clock();
	double seconds = (double)(end - start) / CLOCKS_PER_SEC;

	if (CheckSortedArray(SecondArray, size))
	{
		printf("%s\n", "Array was been sorted!");
		printf("Bubble sort - %f second\n\n", seconds); //выводим результат времени работы алгоритма в секундах

	}
	else
		printf("%s", "Array wasn't been sorted in Bubble sort");
	free(SecondArray);

	start = clock();
	DirectSelection(ThirdArray, size);
	end = clock();
	double seconds2 = (double)(end - start) / CLOCKS_PER_SEC;

	if (CheckSortedArray(ThirdArray, size))
	{
		printf("%s\n", "Array was been sorted!");
		printf("Selection sort - %f second\n", seconds2); //выводим результат времени работы алгоритма в секундах
	}
	else
		printf("%s", "Array wasn't been sorted in Selection sort");
	free(ThirdArray);

	return 0;
}

int* ArrayFilling(int size, int rangeA, int rangeB) //заполнение случайными числами динамического массива, rangeA_B - верхняя и нижняя граница соответственно
{
	int* array = (int*)malloc(size * sizeof(int)); //выделяем динамическую память
	if (array == NULL)
		return NULL;

	for (int i = 1; i < size; i++) //заполняем массив псевдослучайно "введёнными" данными
	{
		array[i] = rangeA + rand() % rangeB;
	}

	return array;
}

int* CopyArray(int* array, size_t size)
{
	int* arr_copy1 = calloc(size, sizeof(int));
	if (arr_copy1)
	{
		for (int i = 0; i < size; i++)
		{
			arr_copy1[i] = array[i];
		}
		return arr_copy1;
	}
	else return NULL;
}

void Bubble(int* array, int size_t) //сортировка Пузырьком          
{
	for (int i = 0; i < size_t - 1; i++)
	{
		for (int j = (size_t - 1); j > i; j--) //для всех элементов после i-ого
		{
			if (array[j - 1] > array[j]) //если текущий элемент меньше предыдущего
			{
				int temp = array[j - 1]; //меняем их местами
				array[j - 1] = array[j];
				array[j] = temp;
			}
		}
	}
}

void DirectInsert(int* array, int size_t)		//сортировка Прямой вставкой
{
	for (int i = 1; i < size_t; i++)
	{
		int value = array[i];					//запоминаем значение элемента
		int index = i;						//и его индекс
		while ((index > 0) && (array[index - 1] > value))
		{									//смещаем другие элементы к концу массива пока они меньше index
			array[index] = array[index - 1];
			index--;						//смещаем просмотр к началу массива
		}
		array[index] = value;					//рассматриваемый элемент помещаем на освободившееся место
	}
}

void DirectSelection(int* array, int size_t)	//сортировка прямым выбором
{
	int min, temp;							//для поиска минимального элемента и для обмена
	for (int i = 0; i < size_t - 1; i++)
	{
		min = i;							//запоминаем индекс текущего элемента
		//ищем минимальный элемент чтобы поместить на место i-ого
		for (int j = i + 1; j < size_t; j++)//для остальных элементов после i-ого
		{
			if (array[j] < array[min])				//если элемент меньше минимального,
				min = j;					//запоминаем его индекс в min
		}
		temp = array[i];						//меняем местами i-ый и минимальный элементы
		array[i] = array[min];
		array[min] = temp;
	}
}

bool CheckSortedArray(int* array, int size_t) //проверка на отсортированность массива
{
	if (size_t == 1)
		return true;
	for (int i = 0; i < size_t - 1; i++)
	{
		if ((array[i] > array[i + 1]))
		{
			return false;
		}
	}
	return true;
}
