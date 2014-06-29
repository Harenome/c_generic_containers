#include <stdlib.h>
#include <stdio.h>

#include <cgc/vector.h>

int main (int argc, char ** argv)
{
    (void) argc; (void) argv;

    cgc_vector * vector = cgc_vector_new (sizeof (int), NULL, NULL, 6);

    for (int i = 0; i < 10; ++i)
        cgc_vector_push_back (vector, & i);

    for (unsigned int i = 0; i < 10; ++i)
    {
        int * at_i = cgc_vector_at (vector, i);
        printf ("%d ", * at_i);
    }
    printf ("\n");

    int * content = cgc_vector_at (vector, 0);
    for (unsigned int i = 0; i < cgc_vector_size (vector); ++i)
        printf ("%d ", content[i]);
    printf ("\n");

    int element = 42;
    cgc_vector_insert (vector, 10, & element);
    printf ("size: %lu; max_size: %lu\n", cgc_vector_size (vector), cgc_vector_max_size (vector));
    content = cgc_vector_at (vector, 0);
    for (unsigned int i = 0; i < cgc_vector_size (vector); ++i)
        printf ("%d ", content[i]);
    printf ("\n");

    cgc_vector_erase (vector, 2, 7);
    content = cgc_vector_at (vector, 0);
    for (unsigned int i = 0; i < cgc_vector_size (vector); ++i)
        printf ("%d ", content[i]);
    cgc_vector_free (vector);


    printf ("\n");
    vector = cgc_vector_new (sizeof (int), NULL, NULL, 6);
    for (int i = 0; i < 10; ++i)
        cgc_vector_push_back (vector, & i);
    for (int i = 0; i < 10; ++i)
        {
            int * j = cgc_vector_pop_front (vector);
            printf ("%d ", * j);
            free (j);
        }
    cgc_vector_free (vector);

    return 0;
}
