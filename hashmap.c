#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "hashmap.h"


typedef struct HashMap HashMap;
int enlarge_called=0;

struct HashMap {
    Pair ** buckets;
    long size; //cantidad de datos/pairs en la tabla
    long capacity; //capacidad de la tabla
    long current; //indice del ultimo dato accedido
};

Pair * createPair( char * key,  void * value) {
    Pair * new = (Pair *)malloc(sizeof(Pair));
    new->key = key;
    new->value = value;
    return new;
}

long hash( char * key, long capacity) {
    unsigned long hash = 0;
     char * ptr;
    for (ptr = key; *ptr != '\0'; ptr++) {
        hash += hash*32 + tolower(*ptr);
    }
    return hash%capacity;
}

int is_equal(void* key1, void* key2){
    if(key1==NULL || key2==NULL) return 0;
    if(strcmp((char*)key1,(char*)key2) == 0) return 1;
    return 0;
}


void insertMap(HashMap * map, char * key, void * value) {
  int position = hash(key, map->capacity);

  if (map->buckets[position] == NULL)
  {
    Pair* newElem = createPair(key, value);
    map->buckets[position] = newElem;
    map->size++;
    return;
  }
  else
  {
    while (map->buckets[position] != NULL)
      {
        if (is_equal(map->buckets[position]->key, key))
        {
          map->buckets[position]->value = value;
          return;
        }
        position = (position + 1) % map->capacity;
      }
    Pair* newElem = createPair(key, value);
    map->buckets[position] = newElem;
    map->size++;
    return;
  }

}

void enlarge(HashMap * map) {
    enlarge_called = 1; //no borrar (testing purposes)
    Pair** old_buckets = map->buckets;
    map->capacity *= 2;
    map->buckets = (Pair**)malloc(sizeof(Pair*) * map->capacity);
    for (int i = 0; i < map->capacity; i++)
      {
        map->buckets[i] = NULL;
      }
    for (int i = 0; i < map->capacity / 2; i++)
      {
        if (old_buckets[i] != NULL)
        {
          insertMap(map, old_buckets[i]->key, old_buckets[i]->value);
        }
      }
    free(old_buckets);
    
}


HashMap * createMap(long capacity)
{
  HashMap* map = (HashMap*)malloc(sizeof(HashMap));
  map->buckets = (Pair**)calloc(capacity, sizeof(Pair*));
  map->size = 0;
  map->capacity = capacity;
  map->current = -1;
  return map;
  
  
  
  return NULL;
  
}

void eraseMap(HashMap * map,  char * key) 
{
  int position = hash(key, map->capacity);
  if (map->buckets[position] == NULL)
  {
    return;
  }
  else
  {
    while (map->buckets[position] != NULL)
      {
        if (is_equal(map->buckets[position]->key, key))
        {
          map->buckets[position]->key = NULL;
          map->size--;
          return;
        }
        position = (position + 1) % map->capacity;
      }
  }


}

Pair * searchMap(HashMap * map,  char * key) 
{
  int position = hash(key, map->capacity);
  if (map->buckets[position] == NULL)
  {
    return NULL;
  }
  else
  {
    while (map->buckets[position] != NULL)
      {
        if (is_equal(map->buckets[position]->key, key))
        {
          map->current = position;
          return map->buckets[position];
        }
        position = (position + 1) % map->capacity;
      }
    
    return NULL;
    
  }
  
  
  
  return NULL;
}

Pair * firstMap(HashMap * map) 
{
  if (map == NULL) return NULL;
  for (int i = 0; i < map->capacity; i++)
    {
      if (map->buckets[i]->key != NULL)
      {
        map->current = i;
        return map->buckets[i];
      }
    }
  return NULL;
}

Pair * nextMap(HashMap * map) 
{
  if (map == NULL) return NULL;
  int i;
  for (i = map->current + 1; i < map->capacity; i++)
    {
      if (map->buckets[i] != NULL)
      {
        map->current = i;
        return map->buckets[i];
      }
      
    }
  return NULL;
}
