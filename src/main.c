#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int felish_help();
int felish_meow();
int felish_exit();

char *builtin_str[] = {
    "help",
    "meow",
    "exit"
};

int (*builtin_func[]) (void) = {
    &felish_help,
    &felish_meow,
    &felish_exit
};


int felish_num_builtins()
{
    return sizeof(builtin_str) / sizeof(char *);
}

/*
 * Builtin functions
*/

/*
 * The help function displaying the commands
*/
int felish_help()
{
    printf("Felish, a 'minimalist' shell\n");

    printf("Commands:\n");

    for (int i = 0; i < felish_num_builtins(); i++)
    {
        printf(" %s\n", builtin_str[i]);
    }

    return 1;
}

/*
 * Meows. The shell is felish you know.
*/
int felish_meow()
{
    printf("Meow meow meow meow!\n");
    return 1;
}

/*
 * Exit the shell.
*/
int felish_exit()
{
    return 0;
}


// Execute the command
int felish_execute(char *arg)
{
    if (arg == NULL)
    {
        return 1;
    }

    for (int i = 0; i < felish_num_builtins(); i++)
    {
        if (strcmp(arg, builtin_str[i]) == 0)
        {
            return (*builtin_func[i])();
        }
    }

    // No builtin function found
    printf("felish: no command found\n");
    return 1;
}


// Read from stdin
char *felish_read_line(void)
{
    int bufsize = 64;
    int position = 0;
    char *buffer = malloc(sizeof(char) * bufsize);
    int c;

    if (!buffer) {
        fprintf(stderr, "felish: allocation error\n");
        exit(EXIT_FAILURE);
    }

    while (1)
    {
        c = getchar();

        // If the user presses enter, the command is finished
        // or in fact, if the user presses space
        if (c == EOF || c == '\n' )
        {
            buffer[position] = '\0';
            return buffer;
        }
        else
        {
            buffer[position] = c;
            position++;
        }

        // Reallocate if buffer has too many chars now
        if (position >= bufsize)
        {
            bufsize += 64;
            buffer = realloc(buffer, bufsize);
            if (!buffer) {
                fprintf(stderr, "felish: allocation error\n");
                exit(EXIT_FAILURE);
            }
        }
    }
}


void felish_loop(void)
{
    // This is a "minimalist" shell, only one command and no args
    char *line;
    int status;

    do
    {
        printf(":3 ");
        line = felish_read_line();
        status = felish_execute(line);

        free(line);
    }
    while (status);
}


int main(int argc, char **argv)
{
    felish_loop();

    return EXIT_SUCCESS;
}
