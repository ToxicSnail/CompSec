#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

unsigned long FillVectorFromArr(int* mx, int sz)
{
	unsigned long vec = 0, mask = 1;
	if (mx && sz > 0 && sz < 32) {
		for (int i = 0; i < sz; i++) {
			if (mx[i] >= 0 && mx[i] < 32) {
				mask = (unsigned long)1 << mx[i];
				vec = vec | mask;
			}
		}
	}
	return vec;
}
int weightOfVector(unsigned long vec) {
	int weight = 0;
	while (vec) {
		weight++;
		vec = vec & (vec - 1);
	}
	return weight;
}
void PutValuesToSet(int* Mx, int size, unsigned long vec) {
	if (Mx && size > 0 && vec) {
		unsigned long mask = 1;
		int i = -1;
		int value = 0;
		while (size && vec) {
			if (mask & vec) {
				Mx[++i] = value;
				vec = vec ^ mask;
			}
			value++;
			mask = mask << 1;
		}
	}
}
int main(int argc, char** argv) {
	unsigned long u, v, r, m;
	int setA[32] = { 2,5,7,9,10,4,8,9 };
	int setB[32] = { 0,5,7,6,3,4,8,9 };
	int size = 8;
	int* setI = NULL;
	unsigned long valSetA = FillVectorFromArr(setA, size);
	unsigned long valSetB = FillVectorFromArr(setB, size);
	unsigned long intersection = valSetA & valSetB;
	if (intersection) {
		printf("Solution is not empty\n");
		int weight = weightOfVector(intersection);
		if (weight)
		{
			setI = (int*)malloc(sizeof(int) * weight);
			if (setI)
			{
				PutValuesToSet(setI, weight, intersection);
				for (int i = 0; i < weight; i++)
				{
					printf("%d, ", setI[i]);
				}
				free(setI);
				setI = NULL;
			}
			else
				printf("Exeption: Bad allocation\n");
		}
	}
	else
		printf("Solution is empty\n");
	return 0;
