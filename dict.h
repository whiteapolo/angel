#ifndef DICT_H
#define DICT_H

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "lll.h"

#define FREE_INFO(info, fun) do { if (fun != NULL) fun(info); } while (0)

typedef struct {
	char *key;
	void *info;
} Val;

typedef struct {
	lll_t **managers;
	unsigned int size;
} Dict;

unsigned long hash(const unsigned char *str);
void (*g_print_info)(void *) = NULL;

void dict_init(Dict *dict, int size);
void dict_insert(Dict *dict, char *key, void *info);
void dict_remove(Dict *dict, char *key);
void *dict_find(Dict *dict, char *key);
void dict_destroy(Dict *dict);

void free_val(void *val)
{
	free(((Val*)val)->info);
	free(((Val*)val)->key);
	free((Val*)val);
}

void print_val(void *val)
{
	printf("KEY: %s, VALUE: ", ((Val*)val)->key);
	g_print_info(((Val*)val)->info);
	putchar('\n');
}

void dict_init(Dict *dict, int size)
{
	dict->size = size * 3;
	dict->managers = calloc(sizeof(lll_t*), dict->size);
}

void dict_insert(Dict *dict, char *key, void *info)
{
	int h = hash(key) % dict->size;
	Val *val = malloc(sizeof(Val));
	val->key = strdup(key);
	val->info = info;
	lll_push(&dict->managers[h], val);
}

void dict_remove(Dict *dict, char *key)
{
	int h = hash(key) % dict->size;
	lll_t *curr = dict->managers[h];
	lll_t *pre = NULL;
	while (curr != NULL && strcmp(curr->info, key)) {
		pre = curr;
		curr = curr->next;
	}

	if (curr != NULL)
		if (pre == NULL)
			FREE_INFO(lll_pop(&dict->managers[h]), free_val);
		else
			FREE_INFO(lll_remove_after(pre), free_val);
}

void *dict_find(Dict *dict, char *key)
{
	int h = hash(key) % dict->size;
	lll_t *curr = dict->managers[h];
	lll_t *pre = NULL;
	while (curr != NULL && strcmp(((Val*)(curr->info))->key, key)) {
		pre = curr;
		curr = curr->next;
	}

	if (curr != NULL)
		return ((Val*)curr->info)->info;
	else
		return NULL;
}

void dict_print(Dict *dict, void (*print_info)(void *))
{
	g_print_info = print_info;
	for (int i = 0; i < dict->size; i++)
		if (dict->managers[i])
			lll_print(dict->managers[i], print_val);
		else
			puts("----------------");
}

void dict_destroy(Dict *dict)
{
	for (int i = 0; i < dict->size; i++)
		if (dict->managers[i])
			lll_clear(&dict->managers[i], free_val);
	free(dict->managers);
}

unsigned long hash(const unsigned char *str)
{
	int c;
	unsigned long hash = 5381;
	while (c = *str++)
		hash = ((hash << 5) + hash) + c;
	return hash;
}

#endif
