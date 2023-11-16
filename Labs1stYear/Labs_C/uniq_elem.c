#include <stdio.h>
#include<stdbool.h>

int main()
{
	const int szz = 5;                                  //задаем размер нашего массива
	int array[10] = {0,0,0, 1,11, 1.12};            	//задаем наш массив целочисленными значениями
	int max = array[0];                                 //возьмем за максимум первый элемент массива
	for (int i = 1; i < szz; i++)                       //проходя по массиву и сравнивая, мы находим
		if (array[i] > max) max = array[i];             //реальный максимум
	bool unique = true;                                 //unique = true
	int notExist = max + 1;                             //notExist больше максимума на единицу
	int countEl = 0;                                    //количество уникальных элементов = 0 
	for (int i = 0; i < szz; i++)                       //
	{
		int jx = i + 1;                                 //
		if (array[i] != notExist)                       //
		{
			unique = true;                              //
			while (jx < szz)                            //
			{   
				if (array[jx] != notExist)              //
				{
					if (array[i] == array[jx])          //
					{
						array[jx] = notExist;           //
						unique = false;                 //
					}
				}
				jx++;                                   //
			}
		}
		else                                            //
			unique = false;                             //
		if (unique)                                     //
			countEl++;                                  //
		else
			array[i] = notExist;                        //
	}
	printf("countEl = %d", countEl);                    //

	return 0;                                           //
}
