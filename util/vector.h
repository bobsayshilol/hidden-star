#ifndef TYPE_VECTOR
#define TYPE_VECTOR

// Define a vector type
typedef struct
{
	int bounds;
	int count;
	void **data;
} Vector;

int vector_init(Vector *v, int initial_size);
void vector_fill(Vector *c, void *value);
int vector_get_size(Vector *v);
void vector_add(Vector *v, void *value);
void *vector_get(Vector *v, int index);
void vector_set(Vector *v, int index, void *value);
void vector_remove(Vector *v, int index);
void vector_resize(Vector *v, int new_size);
void vector_free(Vector *c);
void vector_free_and_free(Vector *v);

#endif
