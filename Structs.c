#include "Structs.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "RBTree.h"

#define FAIL 0
#define START_LEN 0
#define FLAG_FAIL 1
#define  FLAG_SUCCESS 0

/**
* CompFunc for strings (assumes strings end with "\0")
* @param a - char* pointer
* @param b - char* pointer
* @return equal to 0 iff a == b. lower than 0 if a < b. Greater than 0 iff b < a. (lexicographic
        * order)
*/
int stringCompare(const void *a, const void *b)
{
    char *str1 = (char*)a;
    char *str2 = (char*)b;
    return strcmp(str1,str2);
}

/**
 * ForEach function that concatenates the given word to pConcatenated. pConcatenated is already allocated with
 * enough space.
 * @param word - char* to add to pConcatenated
 * @param pConcatenated - char*
 * @return 0 on failure, other on success
 */
int concatenate(const void *word, void *pConcatenated)
{
    char *str1 = (char*)word;
    char *str2 = (char*)pConcatenated;
    strcat(str2,str1);
    strcat(str2,"\n");
    return 1;
}

/**
 * FreeFunc for strings
 */
void freeString(void *s)
{
    char *str = (char*)s;
    if (str == NULL)
    {
        return;
    }
    free(str);
}

/**
 * CompFunc for Vectors, compares element by element, the vector that has the first larger
 * element is considered larger. If vectors are of different lengths and identify for the length
 * of the shorter vector, the shorter vector is considered smaller.
 * @param a - first vector
 * @param b - second vector
 * @return equal to 0 iff a == b. lower than 0 if a < b. Greater than 0 iff b < a.
 */
int vectorCompare1By1(const void *a, const void *b)
{
    Vector *vec1 = (Vector*)a;
    Vector *vec2 = (Vector*)b;
    int i = 0;
    int j = 0;
    while (i < vec1->len && j < vec2->len)
    {
        if (vec1->vector[i] > vec2->vector[j])
        {
            return 1;
        }
        else if(vec1->vector[i] < vec2->vector[j])
        {
            return -1;
        }
        ++i;
        ++j;
    }
    if (vec1->len == vec2->len)
    {
        return 0;
    }
    if (vec1->len > vec2->len)
    {
        return 1;
    }
    else
    {
        return -1;
    }
}

/**
 * FreeFunc for vectors
 */
void freeVector(void *pVector)
{
    if (pVector == NULL)
    {
        return;
    }
    Vector *vec = (Vector*)pVector;
    free(vec->vector);
    free(vec);
}

/**
 * Calculate the sum in the sqrt norm of the given vector
 * @param vec Vector
 * @return the sum
 */
double calculateNorm(const Vector *vec)
{
    double norm = START_LEN;
    for (int i = 0; i < vec->len; ++i)
    {
        norm += (vec->vector[i] * vec->vector[i]);
    }
    return norm;
}

/**
 * Copy the data of pVector into pmaxvector.
 * @param pVector the given copy
 * @param pMaxVector the vector to copy into
 */
void copyVector(const void *pVector, void *pMaxVector)
{
    Vector *vec = (Vector *) pVector;
    Vector *vecMax = (Vector *) pMaxVector;
    for (int i = 0; i < vec->len; ++i)
    {
        (*vecMax).vector[i] = (*vec).vector[i];
    }

}



/**
 * copy pVector to pMaxVector if : 1. The norm of pVector is greater then the norm of pMaxVector.
 * 								   2. pMaxVector == NULL.
 * @param pVector pointer to Vector
 * @param pMaxVector pointer to Vector
 * @return 1 on success, 0 on failure (if pVector == NULL: failure).
*/
int copyIfNormIsLarger(const void *pVector, void *pMaxVector)
{
    if(pVector == NULL||pMaxVector == NULL)
    {
        return FLAG_FAIL;
    }
    int flag = FLAG_SUCCESS;
    Vector *vec = (Vector*)pVector;
    Vector *vecMax = (Vector*)pMaxVector;
    if (vecMax->vector == NULL)
    {
        flag = FLAG_FAIL;
        (*vecMax).len = (*vec).len;
        (*vecMax).vector = (double *) malloc(sizeof(double) * (*vec).len);
        if ((*vecMax).vector == NULL)
        {
            return FAIL;
        }
        copyVector(vec, vecMax);
    }
    double norm1 = calculateNorm(vec);
    double norm2 = calculateNorm(vecMax);
    if (norm1 <= norm2)
    {
        return 1;
    }
    if (norm2 < norm1)
    {
        (*vecMax).len = (*vec).len;
        free((*vecMax).vector);
        (*vecMax).vector = (double *) malloc (sizeof(double) * (*vec).len);
        if ((*vecMax).vector == NULL)
        {
            if (flag == FLAG_FAIL)
            {
                freeVector(vecMax);
            }
            return FAIL;
        }
        copyVector(vec, vecMax);
    }
    return 1;
}


/**
 * @param tree a pointer to a tree of Vectors
 * @return pointer to a *copy* of the vector that has the largest norm (L2 Norm).
 */
Vector *findMaxNormVectorInTree(RBTree *tree)
{
    if (tree == NULL)
    {
        return NULL;
    }
    Vector *maxVector = (Vector*) malloc(sizeof(Vector));
    maxVector->vector = NULL;
    maxVector->len = START_LEN;
    int flag = forEachRBTree(tree, copyIfNormIsLarger, maxVector);
    if (flag == FAIL)
    {
        freeVector(maxVector);
        return NULL;
    }
    return maxVector;
}





