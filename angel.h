#ifndef ANGEL_H
#define ANGEL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "dict.h"
#include "lll.h"

// Public Declerations
// -------------------
// Init....
int agl_init(const char *file_name);
// Destroy
void agl_destory();
// get a value by name in dest
// if the varibale does not exists than it returns false
// otherwise true
bool agl_get_int(char *key, int *dest);
bool agl_get_float(char *key, float *dest);
bool agl_get_bool(char *key, bool *dest);
bool agl_get_string(char *key, char *dest);

// Local Declerations
// ------------------
#define ASSERT(con, msg) if (con) { printf("ERROR: %s\n", msg); return 1; }
char *buf;
Dict dict;
void *mk_token(char *val);
int _get_file_size(FILE *f);
int _read_whole_file(FILE *f, int size);
int _dict_it();
lll_t *tokenize();
bool *booldup(bool b);
int *intdup(int b);
float *floatdup(float b);

// Implementetion
// --------------
int agl_init(const char *file_name)
{
	FILE *f;
	ASSERT(!(f = fopen(file_name, "r")), "could not read file");
	int size = _get_file_size(f);
	_read_whole_file(f, size);
	fclose(f);
	dict_init(&dict, size / 20);
	ASSERT(_dict_it(), "dict it failed");
	return 0;
}

void agl_destory()
{
	free(buf);
	dict_destroy(&dict);
}

int _dict_it()
{
	lll_t *lst = tokenize();
	lll_t *curr_node = lst->next;

	while (curr_node->next->info != NULL) {
		// skip comments
		if (*((char*)curr_node->info) == '#') {
			while (*(char*)curr_node->info != '\n')
				curr_node = curr_node->next;
			continue;
		}
		// skip new lines
		if (*((char*)curr_node->info) == '\n') {
			while (*(char*)curr_node->info == '\n')
				curr_node = curr_node->next;
			continue;
		}

		dict_insert(&dict, curr_node->info, mk_token(curr_node->next->info));
		curr_node = curr_node->next->next;
	}
}

lll_t *tokenize()
{
	int i;
	bool is_val = false;
	char tmp[100];
	char *curr = buf;
	lll_t *lst, *curr_node;
	lll_init(&lst);
	lll_push(&lst, NULL);
	curr_node = lst;
	while (*curr) {
		// skip all spaces and tabs
		while (strchr(" \t\n", *curr)) curr++;
		if (!(*curr))
			break;
		// tokenize
		i = 0;
		while (!strchr(" \t\n", *curr))
			tmp[i++] = *(curr++);
		tmp[i] = '\0';

		// insert to list
		lll_insert_after(curr_node, strdup(tmp));
		curr_node = curr_node->next;

		if (*curr == '\n') {
			lll_insert_after(curr_node, strdup("\n"));
			curr_node = curr_node->next;
		}
	}
	lll_insert_after(curr_node, NULL);
	return lst;
}

void *mk_token(char *val)
{
	void *t;
	if (*val == '"') {
		char *t = strdup(++val);
		*strchr(t, '"') = '\0';
		return t;
	} else if (*val == 'y' || *val == 'n') {
		return booldup(*val - 'n');
	} else if (strchr(val, '.')) {
		return floatdup(atof(val));
	} else {
		return intdup(atoi(val));
	}
	return t;
}

bool agl_get_int(char *key, int *dest)
{
	int *tmp = dict_find(&dict, key);
	if (tmp)
		*dest = *tmp;
	return tmp;
}

bool agl_get_float(char *key, float *dest)
{
	float *tmp = dict_find(&dict, key);
	if (tmp)
		*dest = *tmp;
	return tmp;
}

bool agl_get_bool(char *key, bool *dest)
{
	bool *tmp = dict_find(&dict, key);
	if (tmp)
		*dest = *tmp;
	return tmp;
}

bool agl_get_string(char *key, char *dest)
{
	char *tmp = dict_find(&dict, key);
	if (tmp)
		strcpy(dest, tmp);
	return tmp;
}

int _read_whole_file(FILE *f, int size)
{
	buf = malloc(sizeof(char) * (size + 1));
	ASSERT(!fread(buf, sizeof(char), size, f), "Problem reading file");
	buf[size-1] = '\n';
	buf[size] = '\0';
	return 0;
}

int _get_file_size(FILE *f)
{
	fseek(f, 0, SEEK_END);
	int size = ftell(f);
	rewind(f);
	return size;
}

//dup values
bool *booldup(bool b)
{
	bool *n = malloc(sizeof(bool));
	*n = b;
	return n;
}

float *floatdup(float b)
{
	float *n = malloc(sizeof(float));
	*n = b;
	return n;
}

int *intdup(int b)
{
	int *n = malloc(sizeof(int));
	*n = b;
	return n;
}

#endif
