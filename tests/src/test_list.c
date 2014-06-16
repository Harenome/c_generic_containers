#include <stdlib.h>
#include <stdio.h>

#include <cgc/list.h>

static void * int_alloc (void)
{
    return malloc (sizeof (int));
}

static int int_copy (const void * source, void * destination)
{
    const int * s = source;
    int * d = destination;
    * d = * s;
    return 0;
}

static void int_free (void * i)
{
    free (i);
}

static int int_print (void * i)
{
    int * integer = i;
    printf ("%d ", * integer);

    return 0;
}

static int int_sum (const void * a, const void * b, void * result)
{
    const int * left = a;
    const int * right = b;
    int * r = result;
    * r = * left + * right;
    return 0;
}

static inline void print_int_list (const char * list_name, cgc_list * list)
{
    printf ("%s: [ ", list_name);
    cgc_list_map (list, int_print);
    printf ("]\n");
}

int main (int argc, char ** argv)
{
    (void) argc; (void) argv;

    cgc_list * lists[2];

    lists[0] = cgc_list_new (int_alloc, int_free, int_copy);
    for (int i = 0; i < 10; ++i)
        cgc_list_push_back (lists[0], & i);
    lists[1] = cgc_list_copy (lists[0]);
    int * popped_int = cgc_list_pop_back (lists[1]);

    print_int_list ("list_0", lists[0]);
    print_int_list ("list_1", lists[1]);

    int base = 0;
    int result = base;
    cgc_list_fold_left (lists[0], int_sum, & base, & result);
    printf ("fold_left + 0 list_0: %d\n", result);

    base = 0;
    result = base;
    cgc_list_fold_left (lists[1], int_sum, & base, & result);
    printf ("fold_left + 0 list_1: %d\n", result);

    free (popped_int);
    cgc_list_free (lists[0]);
    cgc_list_free (lists[1]);

    return 0;
}
