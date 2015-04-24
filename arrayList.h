struct arrayList{
  void **array;
  int length;
  int size;
  void (*free_fn)(void*);
};

struct arrayList* arrayList_new(void(*free_fn)(void*));

void arrayList_free(struct arrayList *al);

void* arrayList_get(struct arrayList *al, int i);

int arrayList_set(struct arrayList *al, void *data, int i);

int arrayList_add(struct arrayList *al, void *data);

int arrayList_length(struct arrayList *al);