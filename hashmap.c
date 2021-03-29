
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <bsd/string.h>
#include <string.h>

#include "hashmap.h"

// Creates a likely unique hash for the given key.
int
hash(char* key)
{
    int hash = 7;

    for (int i = 0; i < strlen(key); i++) {
        hash = hash * 11 + key[i];
    }

    return hash;
}

// Allocates memory for the hashmap and data, and sets default values.
hashmap*
make_hashmap_presize(int nn)
{
    hashmap* hh = calloc(1, sizeof(hashmap));
    hh->data = calloc(nn, sizeof(hashmap_pair));
    hh->capacity = nn;
    hh->size = 0;
    return hh;
}

// Creates the initial hashmap
hashmap*
make_hashmap()
{
    return make_hashmap_presize(2);
}

// Frees all allocated memory.
void
free_hashmap(hashmap* hh)
{
    free(hh->data);
    free(hh);
}

// Determines if the given key is in the hashmap.
int
hashmap_has(hashmap* hh, char* kk)
{
    return hashmap_get(hh, kk) != -1;
}

// Gets the value at the given key.
int
hashmap_get(hashmap* hh, char* kk)
{
    int index = hash(kk) % hh->capacity;

    int ret;

    while (true) {
        if (strcmp(hh->data[index].key, kk) == 0 && hh->data[index].tomb == false) {
            ret = hh->data[index].val;
            break;
        }
        else if (hh->data[index].used == false) {
            ret = -1;
            break;
        }
        else {
            index = (index + 1) % hh->capacity;
        }
    }
    

    return ret;
}

// Puts the given value at the given key, and resizes the hashmap once
// capacity is half filled
void
hashmap_put(hashmap* hh, char* kk, int vv)
{
    if (hh->size + 1 >= (hh->capacity / 2)) {
        int old_cap = hh->capacity;
		hh->capacity *= 2;

		hashmap_pair* data = hh->data;

        hh->data = calloc(hh->capacity, sizeof(hashmap_pair));
		hh->size = 0;

        for (int i = 0; i < old_cap; i++) {
			hashmap_put(hh, data[i].key, data[i].val);
        }

        free(data);
    }


    int index = hash(kk) % hh->capacity;

    while(true) {
        if (hh->data[index].used == false || hh->data[index].tomb == true) {
            strcpy(hh->data[index].key, kk);
            hh->data[index].used = true;
            hh->data[index].val =  vv;
			hh->size++;
            break;
        }
		else if (strcmp(hh->data[index].key, kk) == 0) {
			hh->data[index].val =  vv;
			break;
		}
        else {
            index = (index + 1) % hh->capacity;
        }

    }

   
}

// Deletes the given key from the hashmap.
void
hashmap_del(hashmap* hh, char* kk)
{
    int index = hash(kk) % hh->capacity;
    hh->data[index].tomb = true;
}

// Returns the iith pair in the hashmap.
hashmap_pair
hashmap_get_pair(hashmap* hh, int ii)
{
    return hh->data[ii];
}

// Prints debug information for the hashmap.
void
hashmap_dump(hashmap* hh)
{
    printf("== hashmap dump ==\n");

    int cap = hh->capacity;
    for (int i = 0; i < cap; i++) {
        printf("Key: %s, Value: %d, Used: %d, Tomb: %d\n", 
                hh->data[i].key, hh->data[i].val, hh->data[i].used, hh->data[i].tomb);
    }
}
