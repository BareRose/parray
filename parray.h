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
    Big O performance is about what one would expect for an array-based data structure, with length/get/set/pop/clear/ditch being O(1),
    push and dequeue being amortized O(1), insert/remove being O(n), binary search being O(logn), and sort being O(n*logn) or O(n*n).
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
    static int FUNC##Length (struct NAME* parr) { \
        return parrayLength((struct parray*)parr); \
    } \
    static int FUNC##Set (struct NAME* parr, int ind, TYPE* ele) { \
        return parraySet((struct parray*)parr, ind, (void*)ele); \
    } \
    static int FUNC##IndexOf (struct NAME* parr, TYPE* ele) { \
        return parrayIndexOf((struct parray*)parr, (void*)ele); \
    } \
    static TYPE* FUNC##Get (struct NAME* parr, int ind) { \
        return (TYPE*)parrayGet((struct parray*)parr, ind); \
    } \
    static TYPE* FUNC##GetFirst (struct NAME* parr) { \
        return (TYPE*)parrayGetFirst((struct parray*)parr); \
    } \
    static TYPE* FUNC##GetLast (struct NAME* parr) { \
        return (TYPE*)parrayGetLast((struct parray*)parr); \
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
    static int FUNC##FindIndex (struct NAME* parr, int(*comp)(const void*, const TYPE**), const void* key) { \
        return parrayFindIndex((struct parray*)parr, (int(*)(const void*, const void**))comp, key); \
    } \
    static TYPE* FUNC##FindElement (struct NAME* parr, int(*comp)(const void*, const TYPE**), const void* key) { \
        return (TYPE*)parrayFindElement((struct parray*)parr, (int(*)(const void*, const void**))comp, key); \
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

//general functions
PADEF struct parray* parrayNew();
    //creates a new parray instance and returns a pointer to it
PADEF int parrayLength(struct parray*);
    //returns the current number of elements in the given parray
PADEF int parraySet(struct parray*, int, void*);
    //overwrites the element at given index in given parray with given value
    //returns the index the value was placed at, or -1 on failure
PADEF int parrayIndexOf(struct parray*, void*);
    //returns the index of given element in given parray, -1 if not found
PADEF void* parrayGet(struct parray*, int);
    //returns the element at the given index in given parray, or NULL on failure
PADEF void* parrayGetFirst(struct parray*);
    //returns the first element in the given parray, or NULL if empty
PADEF void* parrayGetLast(struct parray*);
    //returns the last element in the given parray, or NULL if empty
PADEF void parrayClear(struct parray*);
    //clears the given parray of all its elements
PADEF void parrayFree(struct parray*);
    //frees the given parray and all its internal data

//stack-like functions
PADEF int parrayPush(struct parray*, void*);
    //appends the given element to the end of the given parray and returns its index
PADEF void* parrayPop(struct parray*);
    //removes the last element in the given parray and returns it (NULL if empty)

//queue-like functions
PADEF void* parrayDequeue(struct parray*);
    //removes the first element in the given parray and returns it (NULL if empty)

//sort/search functions
PADEF int parrayFindIndex(struct parray*, int(*)(const void*, const void**), const void*);
    //returns the index of an element that evaluates as equal to given key according to given function
    //parray must be sorted for this function to work correctly, otherwise the result is undefined
PADEF void* parrayFindElement(struct parray*, int(*)(const void*, const void**), const void*);
    //returns an element that evaluates as equal to given key according to given comparison function
    //parray must be sorted for this function to work correctly, otherwise the result is undefined
PADEF void parraySortInsert(struct parray*, int(*)(const void**, const void**));
    //insertion sorts the elements in the given parray according to given comparison function
    //comparison function should return 1 if first argument is greater than second argument
    //0 if it is equal, and -1 if it is smaller, parray will be sorted smallest to greatest
PADEF void parraySortStandard(struct parray*, int(*)(const void**, const void**));
    //same as parraySortInsert but uses the C standard qsort function for sorting instead

//insert/remove functions
PADEF int parrayInsert(struct parray*, int, void*);
    //inserts the given element at the given index in given parray, shifting other elements forward
    //returns the index the element was placed at, or -1 on failure
PADEF void* parrayRemove(struct parray*, int);
    //removes and returns the element at given index while maintaining order of remaining elements
    //returns NULL if given index is outside the bounds of the parray
PADEF void* parrayDitch(struct parray*, int);
    //faster alternative to parrayRemove that doesn't maintain order of remaining elements

//memory-related functions
PADEF int parrayCapacity(struct parray*, int);
    //adjusts the internal capacity of the given parray to most closely match the given number of elements
    //returns the resulting capacity after resizing, which may not match what was requested

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
    //creates a new parray instance and returns a pointer to it
    struct parray* parr = malloc(sizeof(struct parray));
    parr->offset = parr->length = 0; parr->capacity = 8; //initial capacity of 8
    parr->data = malloc(sizeof(void*)*parr->capacity);
    //return
    return parr;
}
PADEF int parrayLength (struct parray* parr) {
    //returns the current number of elements in the given parray
    return parr->length;
}
PADEF int parraySet (struct parray* parr, int ind, void* ele) {
    //overwrites the element at given index in given parray with given value
    //returns the index the value was placed at, or -1 on failure
    if ((ind < 0)||(ind >= parr->length)) return -1;
    parr->data[parr->offset+ind] = ele;
    return ind;
}
PADEF int parrayIndexOf (struct parray* parr, void* ele) {
    //returns the index of given element in given parray, -1 if not found
    for (int i = 0; i < parr->length; i++)
        if (parr->data[parr->offset+i] == ele) return i;
    return -1;
}
PADEF void* parrayGet (struct parray* parr, int ind) {
    //returns the element at the given index in given parray, or NULL on failure
    if ((ind < 0)||(ind >= parr->length)) return NULL;
    return parr->data[parr->offset+ind];
}
PADEF void* parrayGetFirst (struct parray* parr) {
    //returns the first element in the given parray, or NULL if empty
    if (!parr->length) return NULL;
    return parr->data[parr->offset];
}
PADEF void* parrayGetLast (struct parray* parr) {
    //returns the last element in the given parray, or NULL if empty
    if (!parr->length) return NULL;
    return parr->data[parr->offset+parr->length-1];
}
PADEF void parrayClear (struct parray* parr) {
    //clears the given parray of all its elements
    parr->offset = parr->length = 0;
}
PADEF void parrayFree (struct parray* parr) {
    //frees the given parray and all its internal data
    free(parr->data); free(parr);
}

//stack-like functions
PADEF int parrayPush (struct parray* parr, void* ele) {
    //appends the given element to the end of the given parray and returns its index
    if (parr->offset+parr->length == parr->capacity)
        if (parr->offset >= parr->length) {
            //double the available capacity by offset reset
            memcpy(&parr->data[0], &parr->data[parr->offset], sizeof(void*)*parr->length);
            parr->offset = 0;
        } else {
            //double the available capacity by reallocation
            parr->data = realloc(parr->data, sizeof(void*)*(parr->capacity *= 2));
        }
    parr->data[parr->offset+parr->length] = ele;
    return parr->length++;
}
PADEF void* parrayPop (struct parray* parr) {
    //removes the last element in the given parray and returns it (NULL if empty)
    if (!parr->length) return NULL;
    parr->length--; //reduce length
    return parr->data[parr->offset+parr->length];
}

//queue-like functions
PADEF void* parrayDequeue (struct parray* parr) {
    //removes the first element in the given parray and returns it (NULL if empty)
    if (!parr->length) return NULL;
    parr->length--; //reduce length
    return parr->data[parr->offset++];
}

//sort/search functions
PADEF int parrayFindIndex (struct parray* parr, int(*comp)(const void*, const void**), const void* key) {
    //returns the index of an element that evaluates as equal to given value according to given function
    //parray must be sorted for this function to work correctly, otherwise the result is undefined
    void** res = bsearch(key, &parr->data[parr->offset], parr->length, sizeof(void*), (int(*)(const void*, const void*))comp);
    if (!res) return -1; //element not found
    return (res - &parr->data[parr->offset])/(sizeof(void*));
}
PADEF void* parrayFindElement (struct parray* parr, int(*comp)(const void*, const void**), const void* key) {
    //returns an element that evaluates as equal to given value according to given comparison function
    //parray must be sorted for this function to work correctly, otherwise the result is undefined
    void** res = bsearch(key, &parr->data[parr->offset], parr->length, sizeof(void*), (int(*)(const void*, const void*))comp);
    if (!res) return NULL; //element not found
    return *res;
}
PADEF void parraySortInsert (struct parray* parr, int(*comp)(const void**, const void**)) {
    //insertion sorts the elements in the given parray according to given comparison function
    //comparison function should return 1 if first argument is greater than second argument
    //0 if it is equal, and -1 if it is smaller, parray will be sorted smallest to greatest
    for (int j = 1; j < parr->length; j++)
        for (int i = parr->offset+j; (i > parr->offset)&&(comp((const void**)&parr->data[i-1], (const void**)&parr->data[i]) > 0); i--) {
            void* temp = parr->data[i];
            parr->data[i] = parr->data[i-1];
            parr->data[i-1] = temp;
        }
}
PADEF void parraySortStandard (struct parray* parr, int(*comp)(const void**, const void**)) {
    //same as parraySortInsert but uses the C standard qsort function for sorting instead
    qsort(&parr->data[parr->offset], parr->length, sizeof(void*), (int(*)(const void*, const void*))comp);
}

//insert/remove functions
PADEF int parrayInsert (struct parray* parr, int ind, void* ele) {
    //inserts the given element at the given index in given parray, shifting other elements forward
    //returns the index the element was placed at, or -1 on failure
    if ((ind < 0)||(ind >= parr->length)) return -1;
    if (parr->offset+parr->length == parr->capacity)
        if (parr->offset >= parr->length) {
            memcpy(&parr->data[0], &parr->data[parr->offset], parr->length);
            parr->offset = 0;
        } else
            parr->data = realloc(parr->data, sizeof(void*)*(parr->capacity *= 2));
    memmove(&parr->data[parr->offset+ind+1], &parr->data[parr->offset+ind], sizeof(void*)*(parr->length-ind));
    parr->data[parr->offset+ind] = ele; parr->length++;
    return ind;
}
PADEF void* parrayRemove (struct parray* parr, int ind) {
    //removes and returns the element at given index while maintaining order of remaining elements
    //returns NULL if given index is outside the bounds of the parray
    if ((ind < 0)||(ind >= parr->length)) return NULL;
    void* ele = parr->data[parr->offset+ind]; parr->length--;
    memmove(&parr->data[parr->offset+ind], &parr->data[parr->offset+ind+1], sizeof(void*)*(parr->length-ind));
    return ele;
}
PADEF void* parrayDitch (struct parray* parr, int ind) {
    //faster alternative to parrayRemove that doesn't maintain order of remaining elements
    if ((ind < 0)||(ind >= parr->length)) return NULL;
    void* ele = parr->data[parr->offset+ind]; parr->data[parr->offset+ind] = parrayPop(parr);
    return ele;
}

//memory-related functions
PADEF int parrayCapacity (struct parray* parr, int cap) {
    //adjusts the internal capacity of the given parray to most closely match the given number of elements
    //returns the resulting capacity after resizing, which may not match what was requested
    if (cap < parr->length) cap = parr->length;
    if (parr->offset) {
        memmove(&parr->data[0], &parr->data[parr->offset], parr->length);
        parr->offset = 0;
    }
    parr->data = realloc(parr->data, sizeof(void*)*(parr->capacity = cap));
    return parr->capacity;
}
    
#endif //PARRAY_IMPLEMENTATION
#endif //PARRAY_H