/*
parray.h - Dynamic, sortable, multi-purpose, pointer array with optional type-checking

To the extent possible under law, the author(s) have dedicated all copyright and related and neighboring
rights to this software to the public domain worldwide. This software is distributed without any warranty.
You should have received a copy of the CC0 Public Domain Dedication along with this software.
If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.
*/

/*
parray supports the following three configurations:
#define PARRAY_EXTERN
    Default, should be used when using parray in multiple compilation units within the same project.
#define PARRAY_IMPLEMENTATION
    Must be defined in exactly one source file within a project for parray to be found by the linker.
#define PARRAY_STATIC
    Defines all parray functions as static, useful if parray is only used in a single compilation unit.

parray usage:
    A parray is a dynamic array of void pointers with a multi-purpose interface, thus allowing it to act as just a dynamic array,
    a stack, a queue, a binary-searchable list, or even all of the above at once. What data structure it acts as depends on the
    functions used on it, e.g. a parray that is only ever modified through parrayPush and parrayPop would effectively be a stack.
    If pointers of only one type are stored in a given parray and type-checking is desired, PARRAY_TYPED(TYPE, NAME, FUNC) can be
    used to define a typed parray containing TYPE pointers (e.g. pointers to struct foo), named struct NAME (e.g. struct foo_parray)
    providing all the usual parray functions but prefixed with FUNC instead of parray (e.g. fooDequeue instead of parrayDequeue).

parray performance:
    Where relevant, parray functions perform bounds checking, which may incur a small (but likely negligible) performance overhead.
    The different usage patterns supported by parray (stack, queue, regular array, etc...) are balanced in such a way that each of
    them performs as close to optimally as possible without slowing down the others, minimizing the performance cost of versatility.

parray returns:
    Getter functions which return NULL on OOB or if empty will become ambiguous if NULL values are stored in a parray, which is not
    recommended. If you wish to store NULL values in your parray you will have to do your own bounds checking to avoid this problem.
*/

//include only once
#ifndef PARRAY_H
#define PARRAY_H

//process configuration
#ifdef PARRAY_STATIC
    #define PARRAY_IMPLEMENTATION
    #define PADEF static
#else //PARRAY_EXTERN
    #define PADEF extern
#endif

//macros
#define PARRAY_TYPED(TYPE, NAME, FUNC) \
    TYPE; struct NAME; \
    static struct NAME* FUNC##New () { \
        return (struct NAME*)parrayNew(); \
    } \
    static int FUNC##Length (const struct NAME* parr) { \
        return parrayLength((const struct parray*)parr); \
    } \
    static int FUNC##Set (struct NAME* parr, int ind, TYPE* ele) { \
        return parraySet((struct parray*)parr, ind, (void*)ele); \
    } \
    static int FUNC##IndexOf (const struct NAME* parr, TYPE* ele) { \
        return parrayIndexOf((const struct parray*)parr, (void*)ele); \
    } \
    static TYPE* FUNC##Get (const struct NAME* parr, int ind) { \
        return (TYPE*)parrayGet((const struct parray*)parr, ind); \
    } \
    static TYPE* FUNC##GetFirst (const struct NAME* parr) { \
        return (TYPE*)parrayGetFirst((const struct parray*)parr); \
    } \
    static TYPE* FUNC##GetLast (const struct NAME* parr) { \
        return (TYPE*)parrayGetLast((const struct parray*)parr); \
    } \
    static void FUNC##Clear (struct NAME* parr) { \
        parrayClear((struct parray*)parr); \
    } \
    static void FUNC##Free (struct NAME* parr) { \
        parrayFree((struct parray*)parr); \
    } \
    static int FUNC##Push (struct NAME* parr, TYPE* ele) { \
        return parrayPush((struct parray*)parr, (void*)ele); \
    } \
    static TYPE* FUNC##Pop (struct NAME* parr) { \
        return (TYPE*)parrayPop((struct parray*)parr); \
    } \
    static TYPE* FUNC##Dequeue (struct NAME* parr) { \
        return (TYPE*)parrayDequeue((struct parray*)parr); \
    } \
    static int FUNC##FindIndex (const struct NAME* parr, int(*comp)(const void*, const TYPE**), const void* key) { \
        return parrayFindIndex((const struct parray*)parr, (int(*)(const void*, const void**))comp, key); \
    } \
    static TYPE* FUNC##FindElement (const struct NAME* parr, int(*comp)(const void*, const TYPE**), const void* key) { \
        return (TYPE*)parrayFindElement((const struct parray*)parr, (int(*)(const void*, const void**))comp, key); \
    } \
    static void FUNC##SortInsert (struct NAME* parr, int(*comp)(const TYPE**, const TYPE**)) { \
        parraySortInsert((struct parray*)parr, (int(*)(const void**, const void**))comp); \
    } \
    static void FUNC##SortStandard (struct NAME* parr, int(*comp)(const TYPE**, const TYPE**)) { \
        parraySortStandard((struct parray*)parr, (int(*)(const void**, const void**))comp); \
    } \
    static int FUNC##Insert (struct NAME* parr, int ind, TYPE* ele) { \
        return parrayInsert((struct parray*)parr, ind, (void*)ele); \
    } \
    static TYPE* FUNC##Remove (struct NAME* parr, int ind) { \
        return (TYPE*)parrayRemove((struct parray*)parr, ind); \
    } \
    static TYPE* FUNC##Ditch (struct NAME* parr, int ind) { \
        return (TYPE*)parrayDitch((struct parray*)parr, ind); \
    } \
    static int FUNC##Capacity (struct NAME* parr, int cap) { \
        return parrayCapacity((struct parray*)parr, cap); \
    }

//structs
struct parray; //forward declaration

//function declarations
PADEF struct parray* parrayNew();
    //creates a new parray instance and returns a pointer to it
PADEF int parrayLength(const struct parray*);
    //returns the current number of elements in the given parray, O(1)
PADEF int parraySet(struct parray*, int, void*);
    //overwrites the element at given index in given parray with given value, O(1)
    //returns the index the value was placed at, or -1 on failure
PADEF int parrayIndexOf(const struct parray*, void*);
    //returns the index of given element in given parray, -1 if not found, O(n)
PADEF void* parrayGet(const struct parray*, int);
    //returns the element at the given index in given parray, or NULL if OOB, O(1)
PADEF void* parrayGetFirst(const struct parray*);
    //returns the first element in the given parray, or NULL if empty, O(1)
PADEF void* parrayGetLast(const struct parray*);
    //returns the last element in the given parray, or NULL if empty, O(1)
PADEF void parrayClear(struct parray*);
    //clears the given parray of all its elements (doesn't free any memory), O(1)
PADEF void parrayFree(struct parray*);
    //frees the given parray (not NULL) and all its internal data
PADEF int parrayPush(struct parray*, void*);
    //appends the given element to the end of given parray (growing if needed), amortized O(1)
    //returns the index the element was placed at, or -1 on failure
PADEF void* parrayPop(struct parray*);
    //removes the last element in given parray and returns it (NULL if empty), O(1)
PADEF void* parrayDequeue(struct parray*);
    //removes the first element in given parray and returns it (NULL if empty), amortized O(1)
PADEF int parrayFindIndex(const struct parray*, int(*)(const void*, const void**), const void*);
    //returns the index of an element that evaluates as equal to given value according to given function, O(logn)
    //parray must be sorted for this function to work correctly, otherwise the result is undefined
PADEF void* parrayFindElement(const struct parray*, int(*)(const void*, const void**), const void*);
    //returns an element that evaluates as equal to given value according to given comparison function, O(logn)
    //parray must be sorted for this function to work correctly, otherwise the result is undefined
PADEF void parraySortInsert(struct parray*, int(*)(const void**, const void**));
    //insertion sorts the elements in given parray according to given comparison function, O(n*n)
    //comparison function should return 1 if first argument is greater than second argument
    //0 if it is equal, and -1 if it is smaller, parray will be sorted smallest to greatest
PADEF void parraySortStandard(struct parray*, int(*)(const void**, const void**));
    //same as parraySortInsert but uses the C standard qsort function for sorting instead, O(n*logn)
PADEF int parrayInsert(struct parray*, int, void*);
    //inserts the given element at the given index in given parray, shifting other elements forward, O(n)
    //returns the index the element was placed at, or -1 on failure
PADEF void* parrayRemove(struct parray*, int);
    //removes and returns the element at given index while maintaining order of remaining elements, O(n)
    //returns NULL if given index is outside the bounds of the parray
PADEF void* parrayDitch(struct parray*, int);
    //faster alternative to parrayRemove that doesn't maintain order of remaining elements, O(1)
PADEF int parrayCapacity(struct parray*, int);
    //adjusts the internal capacity of the given parray to most closely match the given number of elements
    //returns the capacity after resizing, which may not match what was requested, or -1 on failure

//implementation section
#ifdef PARRAY_IMPLEMENTATION

//includes
#include <stdlib.h> //memory allocation
#include <string.h> //memmove/memcpy

//structs
struct parray {
    int offset;
    int length;
    int capacity;
    void** data;
};

//general functions
PADEF struct parray* parrayNew () {
    return calloc(1, sizeof(struct parray));
}
PADEF int parrayLength (const struct parray* parr) {
    return parr->length;
}
PADEF int parraySet (struct parray* parr, int ind, void* ele) {
    if ((ind < 0)||(ind >= parr->length)) return -1;
    parr->data[parr->offset+ind] = ele;
    return ind;
}
PADEF int parrayIndexOf (const struct parray* parr, void* ele) {
    for (int i = 0; i < parr->length; i++)
        if (parr->data[parr->offset+i] == ele) return i;
    return -1;
}
PADEF void* parrayGet (const struct parray* parr, int ind) {
    if ((ind < 0)||(ind >= parr->length)) return NULL;
    return parr->data[parr->offset+ind];
}
PADEF void* parrayGetFirst (const struct parray* parr) {
    if (!parr->length) return NULL;
    return parr->data[parr->offset];
}
PADEF void* parrayGetLast (const struct parray* parr) {
    if (!parr->length) return NULL;
    return parr->data[parr->offset+parr->length-1];
}
PADEF void parrayClear (struct parray* parr) {
    parr->offset = parr->length = 0;
}
PADEF void parrayFree (struct parray* parr) {
    free(parr->data); free(parr);
}

//stack-like functions
PADEF int parrayPush (struct parray* parr, void* ele) {
    if (!parr->capacity) {
        //allocate initial capacity of 1 element
        parr->data = malloc(sizeof(parr->data[0]));
        //check for malloc failure
        if (!parr->data) return -1;
        //update allocated capacity
        parr->capacity = 1;
    }
    if (parr->offset+parr->length == parr->capacity)
        if (parr->offset >= parr->length) {
            //double the available capacity by offset reset
            memcpy(&parr->data[0], &parr->data[parr->offset], sizeof(parr->data[0])*parr->length);
            parr->offset = 0;
        } else {
            //double the available capacity by reallocation
            void** ndat = realloc(parr->data, sizeof(parr->data[0])*(parr->capacity*2));
            //check for realloc failure
            if (!ndat) return -1;
            //update allocated capacity
            parr->capacity *= 2;
            parr->data = ndat;
        }
    parr->data[parr->offset+parr->length] = ele;
    return parr->length++;
}
PADEF void* parrayPop (struct parray* parr) {
    if (!parr->length) return NULL;
    parr->length--; //reduce length
    return parr->data[parr->offset+parr->length];
}

//queue-like functions
PADEF void* parrayDequeue (struct parray* parr) {
    if (!parr->length) return NULL;
    parr->length--; //reduce length
    return parr->data[parr->offset++];
}

//sort/search functions
PADEF int parrayFindIndex (const struct parray* parr, int(*comp)(const void*, const void**), const void* key) {
    void** res = bsearch(key, &parr->data[parr->offset], parr->length, sizeof(parr->data[0]), (int(*)(const void*, const void*))comp);
    if (!res) return -1; //element not found
    return res - &parr->data[parr->offset];
}
PADEF void* parrayFindElement (const struct parray* parr, int(*comp)(const void*, const void**), const void* key) {
    void** res = bsearch(key, &parr->data[parr->offset], parr->length, sizeof(parr->data[0]), (int(*)(const void*, const void*))comp);
    if (!res) return NULL; //element not found
    return *res;
}
PADEF void parraySortInsert (struct parray* parr, int(*comp)(const void**, const void**)) {
    for (int j = 1; j < parr->length; j++)
        for (int i = parr->offset+j; (i > parr->offset)&&(comp((const void**)&parr->data[i-1], (const void**)&parr->data[i]) > 0); i--) {
            void* temp = parr->data[i];
            parr->data[i] = parr->data[i-1];
            parr->data[i-1] = temp;
        }
}
PADEF void parraySortStandard (struct parray* parr, int(*comp)(const void**, const void**)) {
    qsort(&parr->data[parr->offset], parr->length, sizeof(parr->data[0]), (int(*)(const void*, const void*))comp);
}

//insert/remove functions
PADEF int parrayInsert (struct parray* parr, int ind, void* ele) {
    if ((ind < 0)||(ind >= parr->length)) return -1;
    if (parr->offset+parr->length == parr->capacity)
        if (parr->offset >= parr->length) {
            //double the available capacity by offset reset
            memcpy(&parr->data[0], &parr->data[parr->offset], sizeof(parr->data[0])*parr->length);
            parr->offset = 0;
        } else {
            //double the available capacity by reallocation
            void** ndat = realloc(parr->data, sizeof(parr->data[0])*(parr->capacity*2));
            //check for realloc failure
            if (!ndat) return -1;
            //update allocated capacity
            parr->capacity *= 2;
            parr->data = ndat;
        }
    memmove(&parr->data[parr->offset+ind+1], &parr->data[parr->offset+ind], sizeof(parr->data[0])*(parr->length-ind));
    parr->data[parr->offset+ind] = ele; parr->length++;
    return ind;
}
PADEF void* parrayRemove (struct parray* parr, int ind) {
    if ((ind < 0)||(ind >= parr->length)) return NULL;
    void* ele = parr->data[parr->offset+ind]; parr->length--;
    memmove(&parr->data[parr->offset+ind], &parr->data[parr->offset+ind+1], sizeof(parr->data[0])*(parr->length-ind));
    return ele;
}
PADEF void* parrayDitch (struct parray* parr, int ind) {
    if ((ind < 0)||(ind >= parr->length)) return NULL;
    void* ele = parr->data[parr->offset+ind]; parr->data[parr->offset+ind] = parrayPop(parr);
    return ele;
}

//memory-related functions
PADEF int parrayCapacity (struct parray* parr, int cap) {
    if (cap < parr->length) cap = parr->length;
    if (parr->offset) {
        memmove(&parr->data[0], &parr->data[parr->offset], sizeof(parr->data[0])*parr->length);
        parr->offset = 0;
    }
    if (cap != parr->capacity) {
        //adjust available capacity by reallocation
        void** ndat = realloc(parr->data, sizeof(parr->data[0])*cap);
        //check for realloc failure
        if (!ndat) return -1;
        //update allocated capacity
        parr->capacity = cap;
        parr->data = ndat;
    }
    return parr->capacity;
}
    
#endif //PARRAY_IMPLEMENTATION
#endif //PARRAY_H