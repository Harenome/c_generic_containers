#include <stdlib.h>
#include <stdio.h>

#include <cgc/string_vector.h>

static inline void print_vector (cgc_string_vector * v)
{
    printf ("vector:\n------\n");
    for (size_t i = 0; i < cgc_string_vector_size (v); ++i)
        printf ("%s\n", cgc_string_vector_at (v, i));
}

int main (int argc, char ** argv)
{
    (void) argc; (void) argv;
    cgc_string_vector * vector = cgc_string_vector_new (1);

    cgc_string_vector_push_back (vector, "abcde");
    cgc_string_vector_push_back (vector, "fghij");
    cgc_string_vector_push_back (vector, "klmno");
    cgc_string_vector_push_back (vector, "pqrst");

    print_vector (vector);

    printf("\npop_back:\n---------\n");
    char * last = cgc_string_vector_pop_back (vector);
    printf ("%s\n\n", last);

    print_vector (vector);

    free (last);
    cgc_string_vector_free (vector);

    return 0;
}
