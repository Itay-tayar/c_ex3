//
// Created by amit.david on 02/12/2019.
//

#include "RBTree.h"
#include "Structs.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define SIZE 12

Vector *makeVector(double *arr, int size)
{
	Vector *tempVector = malloc(sizeof(Vector));
	double *vArr = malloc(sizeof(double) * size);
	for (int i = 0; i < size; i++)
	{
		vArr[i] = arr[i];
	}

	tempVector->vector = vArr;
	tempVector->len = size;

	return tempVector;
}

int printVector(const void *a, void *null)
{
	if(a == NULL)
	{
		printf("\t\t[NULL]");
	}

	if (null != NULL)
	{
		return 0;
	}

	Vector *v = (Vector*)a;

	printf("\t\t[");

	for (int i = 0; i < v->len; i++)
	{
		if(i != 0)
		{
			printf(", ");
		}
		printf("%f", (v->vector)[i]);
	}

	printf("] (size: %d)\n", v->len);
	return 1;
}

void testFreeVector(void *pVector)
{
	Vector *v = (Vector*)pVector;
	free(v->vector);
	free(v);
}

int main()
{
	Vector *arr[SIZE];
	arr[0] = makeVector((double[]){1.0, 2.0, 3.0}, 3);
	arr[1] = makeVector((double[]){1.0, 2.0, 3.0}, 3);
	arr[2] = makeVector((double[]){10.0, 10.0}, 2);
	arr[3] = makeVector((double[]){1.0, 1.0, 1.0, 1.0}, 4);
	arr[4] = makeVector((double[]){3.0, 2.0, 1.0}, 3);

	//	arr[5] = makeVector((double[]){}, 0);  // School solution doesnt support it :)
	//  arr[5] = NULL;                         // This either :D

	arr[5] = makeVector((double[]){-4.88, 3.66, -2.44}, 3);
	arr[6] = makeVector((double[]){3.3, -2.2, 1.1}, 3);
	arr[7] = makeVector((double[]){1.0, 2.0, 4.0}, 3);
	arr[8] = makeVector((double[]){0.0}, 1);
	arr[9] = makeVector((double[]){1.0, 2.0, 3.0, -4.0}, 4);
	arr[10] = makeVector((double[]){3.0, 2.0, 1.0}, 3);
	arr[11] = makeVector((double[]){1.0, -2.0, 3.0, 4.0}, 4);

	RBTree *tree = newRBTree(&vectorCompare1By1, &freeVector);

	for (int i = 0; i < SIZE; i++)
	{
		printf("Try to add vector %d:\n", i);
		printVector(arr[i], NULL);
		printf("\taddToRBTree return code: %d\n", addToRBTree(tree, arr[i]));
		printf("\tPrint all vectors in tree\n");
		printf("\tforEachRBTree return code: %d\n", forEachRBTree(tree, &printVector, NULL));
		printf("\n");
	}

	char *text;
	for (int i = 0; i < SIZE; i++)
	{
		text = containsRBTree(tree, arr[i]) ? "vector %d is in the tree\n" : "vector %d not in the tree\n";
		printf(text, i);
	}

	printf("\nChanging the return vector from findMaxNormVectorInTree shouldn't change to tree\n");
	Vector *v = findMaxNormVectorInTree(tree);
	printVector(v, NULL);
	v->len = 5;
	v->vector = (double[]){5.0, 5.0, 5.0, 5.0, 5.0};

	printf("\nChanged max vector to [5.0, 5.0, 5.0, 5.0, 5.0] (size 5)\nPrint the tree after the change:\n");
	printf("\tforEachRBTree return code: %d\n", forEachRBTree(tree, &printVector, NULL));

	printf("Free tree\n");
	freeRBTree(tree);

	testFreeVector(arr[1]);

	printf("\nTest done\n");
}

