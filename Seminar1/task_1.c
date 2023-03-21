#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <malloc/malloc.h>
#include <string.h>
#include "TEXT.h"
#include <unistd.h>

#define NAME_LEN 10

int UserExists (TEXT* txt, size_t uid)
{
    int base_id = 0;
    for (int i = 1; i < txt->num_of_str; i+=2)
    {
        base_id = atoi (txt->str_array[i].string);

        if (base_id == uid) return i;
    }
    return 0;
}

void AddInBase (const char* name, size_t user_id)
{
    FILE* file = fopen ("BASE.txt", "a");

    fprintf (file, "%s\n", name);
    fprintf (file, "%lu\n", user_id);

    fclose (file);
}


int main()
{
    TEXT txt = {};

    char name[NAME_LEN] = {};
    unsigned int user_id = getuid ();

    TxtCreate (&txt, "BASE.txt", "r");

    int base_pos = UserExists (&txt, user_id);

    if (base_pos) printf ("Hello %s\n", txt.str_array[base_pos - 1].string);

    else 
    { 
        printf ("Hello, what's your name stranger?\n");

        scanf ("%s", name);
        printf ("Hello %s\n", name);

        AddInBase (name, user_id);
    }

    TxtDestroy (&txt);

    return 0;
}