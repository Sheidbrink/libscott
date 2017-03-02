#ifndef DOUBLELL_H
#define DOUBLELL_H
struct dll_node {
	void *data;
	struct dll_node *next;
	struct dll_node *prev;
};

struct dll_node* dll_create(void *data);
void dll_add(struct dll_node* head, struct dll_node* toAdd);
struct dll_node* dll_delete(struct dll_node* toDelete);

#endif
