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

static int int_diff_right (const void * a, void * result)
{
    const int * left = a;
    int * r = result;
    * r = * left - * r;
    return 0;
}

static int int_diff_left (void * result, const void * a)
{
    const int * left = a;
    int * r = result;
    * r = * r - * left;
    return 0;
}

static inline void print_int_list (const char * list_name, cgc_list * list)
{
    printf ("%s: [ ", list_name);
    cgc_list_map (list, int_print);
    printf ("]\n");
}

static inline void fold_left_diff (const char * list_name, const cgc_list * list)
{
    int base_result = 0;
    cgc_list_fold_left (list, int_diff_left, & base_result);
    printf ("fold_left (-) %d %s = %d\n", 0, list_name, base_result);
}

static inline void fold_right_diff (const char * list_name, const cgc_list * list)
{
    int base_result = 0;
    cgc_list_fold_right (list, int_diff_right, & base_result);
    printf ("fold_right (-) %d %s = %d\n", 0, list_name, base_result);
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

    fold_left_diff ("list_0", lists[0]);
    fold_right_diff ("list_0", lists[0]);
    fold_left_diff ("list_1", lists[1]);
    fold_right_diff ("list_1", lists[1]);

    cgc_list_erase (lists[0], 1, 4);
    print_int_list ("list_0", lists[0]);

    int i = 42;
    cgc_list_insert (lists[0], 2, & i);
    print_int_list ("list_0", lists[0]);

    free (popped_int);
    cgc_list_free (lists[0]);
    cgc_list_free (lists[1]);

    return 0;
}
