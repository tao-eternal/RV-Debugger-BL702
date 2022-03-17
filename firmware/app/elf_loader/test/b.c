extern int add(int a, int b);

typedef int (*add_func_t)(int, int);
static void* static_add = (void*)add;

int test_extern_add(int a, int b) { return add(a, b); }

int test_static_add(int a, int b) { return ((add_func_t)static_add)(a, b); }

int test_local_add(int a, int b) {
  add_func_t local_add = (add_func_t)static_add;
  return local_add(a, b);
}