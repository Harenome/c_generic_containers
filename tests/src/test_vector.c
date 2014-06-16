#include <stdlib.h>
#include <stdio.h>

#include <cgc/vector.h>

int main (int argc, char ** argv)
{
    (void) argc; (void) argv;

    cgc_vector * vector = cgc_vector_new (sizeof (int), 5);

    for (int i = 1; i < 11; ++i)
        cgc_vector_push_back (vector, & i);

    for (unsigned int i = 0; i < 10; ++i)
    {
        int * at_i = cgc_vector_at (vector, i);
        printf("%d ", * at_i);
    }
    printf("\n");

    int * content = cgc_vector_at (vector, 0);
    for (unsigned int i = 0; i < cgc_vector_size (vector); ++i)
        printf ("%d ", content[i]);

    cgc_vector_free (vector);

    return 0;
}
