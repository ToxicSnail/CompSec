#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>          //
#include <string.h>         //Объявление библиотек
#include <stdlib.h>         //
#include <stdbool.h>        //
#include <Math.h>           //

void printVector(size_t len, unsigned char* Vec); //конвертация вектора в строку с последующим выводом

unsigned char* StringToBool(char str[1000], size_t bits); //конвертация строки в булев вектор

unsigned char* logMul(unsigned char* vecA, size_t strlen1, unsigned char* vecB, size_t strlen2); //функция логического умножения

unsigned char* logSum(unsigned char* vecA, size_t strlen1, unsigned char* vecB, size_t strlen2); //функция логического сложения

unsigned char* sumMod2(unsigned char* vecA, size_t strlen1, unsigned char* vecB, size_t strlen2); //функция суммы по модулю 2

void inversion(unsigned char* Vec, size_t bits); // функция инверсии

void set1(unsigned char* Vec, size_t bits, size_t bit);	//функция установки/сброса i-го бита вектора
void set0(unsigned char* Vec, size_t bits, size_t bit);	//функция установки/сброса i-го бита вектора


void shiftRight(unsigned char* Vec, size_t bits, size_t k);
void shiftLeft(unsigned char* Vec, size_t bits, size_t k);

int main()
{
	char str_A[1000] = "1010";
	char str_B[1000] = "1111";
	unsigned char* vecA = NULL;
	unsigned char* vecB = NULL;
	size_t strlen1 = strlen(str_A);
	size_t strlen2 = strlen(str_B);
	if (strlen(str_A) > 0 && strlen(str_B) > 0)
	{
		printf("-----------------------------------------\n");
		printf("Strings to bool vectors\n");
		printf("1) ");
		vecA = StringToBool(str_A, strlen1);
		printf("2) ");
		vecB = StringToBool(str_B, strlen2);
	}
	if (vecA && vecB)
	{
		printf("-----------------------------------------\n");
		printf("Boolean multiplication of two arrays:\n");
		printf("1) ");
		printVector(strlen1, vecA);
		printf("2) ");
		printVector(strlen2, vecB);
		printf("&) ");
		logMul(vecA, strlen1, vecB, strlen2);

		printf("-----------------------------------------\n");
		printf("Boolean addition of two arrays:\n");
		printf("1) ");
		printVector(strlen1, vecA);
		printf("2) ");
		printVector(strlen2, vecB);
		printf("|) ");
		logSum(vecA, strlen1, vecB, strlen2);

		printf("-----------------------------------------\n");
		printf("Sum modulo 2:\n");
		printf("1) ");
		printVector(strlen1, vecA);
		printf("2) ");
		printVector(strlen2, vecB);
		printf("^) ");
		sumMod2(vecA, strlen1, vecB, strlen2);

		printf("-----------------------------------------\n");
		printf("Inversion the first array:\n");
		printf("1)  ");
		printVector(strlen1, vecA);
		printf("--) ");
		inversion(vecA, strlen1);
		printf("Inversion the second array:\n");
		printf("2)  ");
		printVector(strlen2, vecB);
		printf("--) ");
		inversion(vecB, strlen2);

		printf("-----------------------------------------\n");
		size_t k = 1;
		if (k > 0 && k < strlen1)
		{
			printf("%s%d%s", "Set in K = (", k, ") bit in the first array:\n");
			printf("1) ");
			printVector(strlen1, vecA);
			printf("+) ");
			set1(vecA, strlen1, k);
			printf("%s%d%s", "Reset in K = (", k, ") bit in the first array:\n");
			printf("1) ");
			printVector(strlen1, vecA);
			printf("-) ");
			set0(vecA, strlen1, k);
		}
		else printf("Error in set/reset in K bit in the first array\n\n");

		printf("-----------------------------------------\n");
		k = 1;
		if (k > 0 && k < strlen2)
		{
			printf("%s%d%s", "Set in K = (", k, ") bit in the second array:\n");
			printf("2) ");
			printVector(strlen2, vecB);
			printf("+) ");
			set1(vecB, strlen2, k);
			printf("%s%d%s", "Reset in K = (", k, ") bit in the first array:\n");
			printf("2) ");
			printVector(strlen2, vecB);
			printf("-) ");
			set0(vecB, strlen2, k);
		}
		else printf("Error in set/reset in K bit in the second array\n\n");

		printf("-----------------------------------------\n");
		k = 1;
		if (k > 0 && k < strlen1)
		{
			printf("%s%d%s", "Shift right by K = (", k, ") bits in the first array:\n");
			printf("1) ");
			printVector(strlen1, vecA);
			printf(">) ");
			shiftRight(vecA, strlen1, k);
			printf("%s%d%s", "Shift right by K = (", k, ") bits in the first array:\n");
			printf("1) ");
			printVector(strlen1, vecA);
			printf("<) ");
			shiftLeft(vecA, strlen1, k);
		}
		else printf("Error in shift right/left by K bits in the first array\n\n");

		printf("-----------------------------------------\n");
		k = 1;
		if (k > 0 && k < strlen2)
		{
			printf("%s%d%s", "Shift right by K = (", k, ") bits in the second array:\n");
			printf("2) ");
			printVector(strlen2, vecB);
			printf(">) ");
			shiftRight(vecB, strlen2, k);
			printf("%s%d%s", "Shift left by K = (", k, ") bits in the second array:\n");
			printf("1) ");
			printVector(strlen2, vecB);
			printf("<) ");
			shiftLeft(vecB, strlen2, k);
		}
		else printf("Error in shift right/left by K bits in the second array\n\n");
		printf("-----------------------------------------\n");
	}
	else
	{
		printf("Error in create array");
	}
}

void printVector(size_t len, unsigned char* Vec)
{
	unsigned char m = 1;
	char strprint[1000] = { 0 };
	for (int i = 0, k = 0; i < len; i++)
	{
		if (Vec[i / 8] & m)
		{
			strprint[i] = '1';
		}
		else
		{
			strprint[i] = '0';
		}
		m = m << 1;
		if (m == '\0')
		{
			m = 1;
		}
	}
	for (int i = 0, k = 0; i < len; i++, k++)
	{
		if (k == 8)
		{
			printf(" ");
			k = 0;
		}
		printf("%c", strprint[i]);
	}
	printf("\n");
}

unsigned char* StringToBool(char str[1000], size_t bits)//
{
	int bytes = ((bits - 1) / 8) + 1;
	unsigned char* vec = (unsigned char*)calloc((bytes), sizeof(unsigned char));	//Выделение памяти под массив vec размером bytes (число байтов)
	unsigned char m = 1;	//Инициализация переменной m со значением 1, которая будет использоваться в качестве маски.

	if (vec)	//проверка на успешное выделение памяти под вектор.
	{
		for (int i = 0; i < bits; i++)	//цикл для перебора символов массива str до максимальной длины maxlen.
		{
			if ((str[i] != '0') && (i < bits))	//проверка, что символ не равен '0' и индекс i меньше длины вектора len.
			{
				vec[i / 8] = m | vec[i / 8];	//преобразование символа в бит и запись его в соответствующий байт вектора vec.
			}
			m = m << 1;	//сдвиг бита m на один влево.
			if (m == '\0')	//проверка, что значение бита m равно нулю.
			{
				m = 1;	//присваивание переменной m значение единицы.
			}
		}
	}
	printVector(bits, vec);	//вызов функции для вывода вектора на экран
	return vec;	//возврат указателя на вектор vec
}

unsigned char* logMul(unsigned char* vecA, size_t strlen1, unsigned char* vecB, size_t strlen2)//
{
	size_t maxlen = fmax(strlen1, strlen2);
	size_t bytes = ((maxlen - 1) / 8) + 1;
	unsigned char* vec = (unsigned char*)calloc((bytes + 1), sizeof(unsigned char));	//выделяем память под новый массив vec размером bytes + 1
	unsigned char m = 1;	//Инициализация переменной m со значением 1, которая будет использоваться в качестве маски.
	if (vec && vecA && vecB)	//проверяем, что указатели на массивы не являются нулевыми.
	{
		for (int i = 0; i < maxlen; i++) // запускаем цикл, который пробегает по всем индексам массива длиной maxlen.
		{
			if ((m & vecA[i / 8]) && (m & vecB[i / 8]))	//проверяем, что биты на позиции i % 8 у массивов vecA и vecB установлены в 1, 
			{											//и только в этом случае устанавливаем бит на соответствующей позиции в массиве vec.
				vec[i / 8] = m | vec[i / 8];//устанавливаем бит в массиве vec на позиции i / 8, используя битовое OR с переменной m.
			}
			m = m << 1;	// сдвигаем переменную m на один бит влево.
			if (m == '\0')	//проверяем, что переменная m равна нулю.
			{
				m = 1;	//если переменная m равна нулю, то устанавливаем ее значение в 1.
			}
		}
	}
	printVector(maxlen, vec);//выводим массив vec на экран с помощью функции printVector.
	printf("\n"); //выводим символ новой строки на экран.
	return vec; //возвращаем указатель на массив vec
}

unsigned char* logSum(unsigned char* vecA, size_t strlen1, unsigned char* vecB, size_t strlen2) //
{
	size_t maxlen = fmax(strlen1, strlen2);
	size_t bytes = ((maxlen - 1) / 8) + 1;
	unsigned char* vec = (unsigned char*)calloc((bytes + 1), sizeof(unsigned char)); //выделяем память под новый массив vec размером bytes + 1
	unsigned char m = 1; //Инициализация переменной m со значением 1, которая будет использоваться в качестве маски.
	if (vec && vecA && vecB) //проверяем, что указатели на массивы не являются нулевыми.
	{
		for (int i = 0; i < maxlen; i++) // запускаем цикл, который пробегает по всем индексам массива длиной maxlen.
		{
			if ((m & vecA[i / 8]) || (m & vecB[i / 8])) // Если хотя бы один из битов векторов равен 1, 
			{											//то бит в соответствующей позиции в результирующем векторе также становится равным 1.
				vec[i / 8] = m | vec[i / 8];			//
			}
			m = m << 1;		//Переменная-маска с помощью операции сдвига влево обновляется на каждой итерации, 
			if (m == '\0')	//а при достижении значения '\0' возвращается к начальному значению 1.
			{				//
				m = 1;		//
			}
		}
	}
	printVector(maxlen, vec);	//После выполнения цикла результирующий вектор печатается с помощью функции printVector  
	printf("\n");				//и выводится перевод строки.
	return vec;					//Затем функция возвращает указатель на результирующий вектор.
}

unsigned char* sumMod2(unsigned char* vecA, size_t strlen1, unsigned char* vecB, size_t strlen2) // 
{
	size_t maxlen = fmax(strlen1, strlen2);
	size_t bytes = ((maxlen - 1) / 8) + 1;
	unsigned char* vec = (unsigned char*)calloc((bytes + 1), sizeof(unsigned char)); //выделяем память под новый массив vec размером bytes + 1
	unsigned char m = 1; //Инициализация переменной m со значением 1, которая будет использоваться в качестве маски.
	if (vec && vecA && vecB) //проверяем, что указатели на массивы не являются нулевыми.
	{
		for (int i = 0; i < maxlen; i++) //запускаем цикл, который пробегает по всем индексам массива длиной maxlen.
		{
			if ((m & vecA[i / 8]) ^ (m & vecB[i / 8])) //проверка на несовпадение битов i/8-ого элемента массивов vecA и vecB в позиции m. 
			{										   //Если биты не совпадают, то выполняется следующее действие.
				vec[i / 8] = m | vec[i / 8]; //устанавливается бит в позиции m для i/8-ого элемента массива vec.
			}
			m = m << 1; //смещение битов влево на 1 позицию.
			if (m == '\0') //проверка на то, что переменная m равна нулю.
			{
				m = 1; //если m равно нулю, то оно присваивается значение 1.
			}
		}
	}
	printVector(maxlen, vec); //выводится массив vec на экран.
	printf("\n"); //выводится перевод строки.
	return vec; // возвращается массив vec.
}
void inversion(unsigned char* Vec, size_t bits) //
{
	int bytes = ((bits - 1) / 8) + 1;
	unsigned char* vec = (unsigned char*)calloc((bytes + 1), sizeof(unsigned char)); //выделяем память под новый массив vec размером bytes + 1
	unsigned char m = 1; //Инициализация переменной m со значением 1, которая будет использоваться в качестве маски.
	if (vec && Vec) //проверка, что указатель vec и указатель на исходный массив Vec не являются нулевыми.
	{
		for (int i = 0; i < bits; i++) //цикл по всем элементам исходного массива Vec, где i - индекс текущего элемента.
		{
			if (!(m & Vec[i / 8])) //проверка, что бит m не установлен в текущем элементе исходного массива Vec.
			{
				vec[i / 8] = m | vec[i / 8]; //установка бита m в массиве vec для элемента с индексом i.
			}
			m = m << 1; //сдвиг значения переменной m на 1 бит влево.
			if (m == '\0') //проверка, что переменная m равна 0.
			{
				m = 1; //присвоение переменной m значения 1.
			}
		}
	}
	printVector(bits, vec); //вызов функции printVector для вывода элементов массива vec на экран.
	printf("\n"); //выводится перевод строки.
	free(vec); //освобождение памяти, выделенной под массив vec.
}

void set1(unsigned char* Vec, size_t bits, size_t k)
{
	int bytes = ((bits - 1) / 8) + 1;	//объявление переменной k и присвоение ей значения maxlen + 1.
	unsigned char* vec = (unsigned char*)calloc((bytes), sizeof(unsigned char));	//выделение памяти под массив vec длиной bytes байт
	if (vec && Vec)		//проверка того, что указатели vec и Vec не равны NULL
	{
		unsigned char m = 1;	//объявление переменной маски m и присвоение ей значения 1
		for (int i = 0; i < bits; i++)	//начало цикла, который проходит по всем битам до максимальной длины.
		{
			if (m & Vec[i / 8])		//проверка того, что бит под номером i в массиве Vec установлен в 1.
			{
				vec[i / 8] = m | vec[i / 8];	//установка бита под номером i в массиве vec в 1.
			}
			m = m << 1;	// сдвиг переменной m на 1 бит влево.
			if (m == '\0')	//проверка того, что m равно нулю.
			{
				m = 1;	//присвоение переменной m значения 1.
			}
		}
	}

	int i = k / 8;	//вычисляем индекс байта, в котором находится бит, который нужно установить или сбросить.
	k = k % 8;	//вычисляем номер бита внутри байта.
	unsigned char m = 1;	// объявляем маску, которая будет использоваться для установки или сброса бита.
	if (k % 8 == 0)		//если номер бита равен нулю, 
	{					//
		m = m;			//то оставляем m без изменения, 
	}					//
	else				//иначе 
	{					//
		m = m << k;		//переносим бит в нужную позицию с помощью операции побитового сдвига влево.
	}
	vec[i] = vec[i] | m;

	printVector(bits, vec);	//выводим получившийся вектор на экран.
	printf("\n"); //выводится перевод строки.
	free(vec);	//освобождаем выделенную под вектор память.
}

void set0(unsigned char* Vec, size_t bits, size_t k)
{
	int bytes = ((bits - 1) / 8) + 1;	//объявление переменной k и присвоение ей значения maxlen + 1.
	unsigned char* vec = (unsigned char*)calloc((bytes), sizeof(unsigned char));	//выделение памяти под массив vec длиной bytes байт
	if (vec && Vec)		//проверка того, что указатели vec и Vec не равны NULL
	{
		unsigned char m = 1;	//объявление переменной маски m и присвоение ей значения 1
		for (int i = 0; i < bits; i++)	//начало цикла, который проходит по всем битам до максимальной длины.
		{
			if (m & Vec[i / 8])		//проверка того, что бит под номером i в массиве Vec установлен в 1.
			{
				vec[i / 8] = m | vec[i / 8];	//установка бита под номером i в массиве vec в 1.
			}
			m = m << 1;	// сдвиг переменной m на 1 бит влево.
			if (m == '\0')	//проверка того, что m равно нулю.
			{
				m = 1;	//присвоение переменной m значения 1.
			}
		}
	}

	int i = k / 8;	//вычисляем индекс байта, в котором находится бит, который нужно установить или сбросить.
	k = k % 8;	//вычисляем номер бита внутри байта.
	unsigned char m = 1;	// объявляем маску, которая будет использоваться для установки или сброса бита.
	if (k % 8 == 0)		//если номер бита равен нулю, 
	{					//
		m = m;			//то оставляем m без изменения, 
	}					//
	else				//иначе 
	{					//
		m = m << k;		//переносим бит в нужную позицию с помощью операции побитового сдвига влево.
	}
	vec[i] = vec[i] | m;		//то устанавливаем бит с помощью операции побитового ИЛИ, 
	vec[i] = vec[i] ^ m;
	printVector(bits, vec);	//выводим получившийся вектор на экран.
	printf("\n"); //выводится перевод строки.
	free(vec);	//освобождаем выделенную под вектор память.
}


void shiftRight(unsigned char* Vec, size_t bits, size_t k)	//????
{
	int bytes = ((bits - 1) / 8) + 1;
	unsigned char* vec = (unsigned char*)calloc((bytes), sizeof(unsigned char));	//выделение памяти под массив vec длиной bytes байт
	if (vec && Vec)		//проверка того, что указатели vec и Vec не равны NULL
	{
		unsigned char m = 1;	// объявление переменной маски m и присвоение ей значения 1.
		for (int i = 0; i < bits; i++)	//начало цикла, который проходит по всем битам до максимальной длины.
		{
			if (m & Vec[i / 8])		//проверка того, что бит под номером i в массиве Vec установлен в 1.
			{
				vec[i / 8] = m | vec[i / 8];	//установка бита под номером i в массиве vec в 1.
			}
			m = m << 1;	//сдвиг переменной m на 1 бит влево.
			if (m == '\0') //проверка того, что m равно нулю.
			{
				m = 1;	//присвоение переменной m значения 1.
			}
		}
	}

	/*
	unsigned char m1;	//объявление переменной маски m1
	int count = ((bits - 1) / 8);	//Вычисление количества элементов в битовом массиве vec по его длине len.

	for (; count > 0; count--)	//выполняем цикл от количества итераций count до 1
	{
		for (int i = 0; i < k; i++)					//выполняем вложенный цикл от 0 до k
		{											//
			m1 = (vec[count - 1] >> 7);				//сдвигаем значение на 7 битов вправо и сохраняем результат в переменной m1
			vec[count] = vec[count] << 1;			//сдвигаем значение вектора vec влево на 1 бит
			vec[count] = vec[count] | m1;			//применяем логическое ИЛИ между vec[count] и m1 и сохраняем результат в vec[count]
			vec[count - 1] = vec[count - 1] << 1;	//сдвигаем значение вектора vec[count - 1] влево на 1 бит
		}
	}

	*/

	unsigned char mask1;
	unsigned char save_byte = 0; // переменная для того чтобы сохранатять перенос

	mask1 = (1 << k + 1) - 1; //сначала +, потом сдвиг, потом инвертируем, в итоге получается маска, содержащая k+1 единичек начиная с нулевого бита

	bytes = ((bits - 1) / 8);

	while (bytes)
	{
		vec[bytes - 1] >>= k; //сдвигаем последний байт вектора на k
		save_byte = vec[bytes - 2] & mask1;
		vec[bytes - 1] = (save_byte << 8 - k) | vec[bytes - 1];
		bytes--;

	}

	printVector(bits, vec);	//если флаг flagprintvec установлен в значение истина, то вызываем функцию printVector, передавая ей параметры len и vec
	printf("\n"); //выводим символ переноса строки
	free(vec); //освобождаем память, выделенную под вектор vec
}

void shiftLeft(unsigned char* Vec, size_t bits, size_t k)	//????
{
	int bytes = ((bits - 1) / 8) + 1;
	unsigned char* vec = (unsigned char*)calloc((bytes), sizeof(unsigned char));	//выделение памяти под массив vec длиной bytes байт
	if (vec && Vec)		//проверка того, что указатели vec и Vec не равны NULL
	{
		unsigned char m = 1;	// объявление переменной маски m и присвоение ей значения 1.
		for (int i = 0; i < bits; i++)	//начало цикла, который проходит по всем битам до максимальной длины.
		{
			if (m & Vec[i / 8])		//проверка того, что бит под номером i в массиве Vec установлен в 1.
			{
				vec[i / 8] = m | vec[i / 8];	//установка бита под номером i в массиве vec в 1.
			}
			m = m << 1;	//сдвиг переменной m на 1 бит влево.
			if (m == '\0') //проверка того, что m равно нулю.
			{
				m = 1;	//присвоение переменной m значения 1.
			}
		}
	}
	/*
	unsigned char m1;	//объявление переменной маски m1
	int count = ((bits - 1) / 8);	//Вычисление количества элементов в битовом массиве vec по его длине len.
	for (int i = 0; i < count; i++)		//выполняем цикл от 0 до количества итераций count
	{
		for (int j = 0; j < k; j++)		//выполняем вложенный цикл от 0 до k
		{
			m1 = (vec[i + 1] << 7);	//сдвигаем значение на 7 битов влево и сохраняем результат в переменной m1
			vec[i] = vec[i] >> 1;	//сдвигаем значение вектора vec[i] вправо на 1 бит
			vec[i] = vec[i] | m1;	//применяем логическое или между vec[i] и m1 и сохраняем результат в vec[i]
			vec[i + 1] = vec[i + 1] >> 1;	//сдвигаем значение вектора vec[i + 1] вправо на 1 бит
		}
	}
	*/

	unsigned char mask1;
	unsigned char save_byte = 0;

	mask1 = (1 << k + 1) - 1;

	bytes = ((bits - 1) / 8);

	while (bytes)
	{
		vec[bytes - 1] <<= k;
		save_byte = vec[bytes - 2] & mask1;
		vec[bytes - 1] = (save_byte >> 8 - k + 1) | vec[bytes - 1];
		bytes--;

	}

	printVector(bits, vec);	//если флаг flagprintvec установлен в значение истина, то вызываем функцию printVector, передавая ей параметры len и vec
	printf("\n"); //выводим символ переноса строки
	free(vec); //освобождаем память, выделенную под вектор vec
}
