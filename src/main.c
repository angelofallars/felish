#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int felish_help();
int felish_meow();
int felish_exit();
int felish_guessing_game();

char *felish_read_line(void);

char *builtin_str[] = {
    "help",
    "meow",
    "exit",
    "guessgame"
};

int (*builtin_func[]) (char **) = {
    &felish_help,
    &felish_meow,
    &felish_exit,
    &felish_guessing_game
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
int felish_help(char** args)
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
int felish_meow(char** args)
{
    printf("Meow meow meow meow!\n");
    return 1;
}

/*
 * Exit the shell.
*/
int felish_exit(char** args)
{
    return 0;
}

/*
 * Felish Guessing Game.
*/
#define MAX_GUESS_INT 10
#define MAX_GUESS_TRIES 5
int felish_guessing_game(char** args)
{
    int randnum = (rand() % MAX_GUESS_INT - 1) + 1;

    printf("Meoww!! Guess a number between 1 to %i!\n", MAX_GUESS_INT);
    printf("You only have %i tries!\n", MAX_GUESS_TRIES);

    int tries = 0;
    
    while (tries < MAX_GUESS_TRIES)
    {
        printf(">>> ");
        char *answer = felish_read_line();

        if (atoi(answer) == randnum)
        {
            printf("Wow! Correct! Meow meow!\n");
            return 1;
        }
        else if (atoi(answer) > randnum)
        {
            printf("Not quite, meow... Too high...\n");
            tries++;
        }
        else
        {
            printf("Not quite, meow... Too low...\n");
            tries++;
        }
    }

    printf("Too bad! No more tries! Better luck next time, nyaa~\n");
    return 1;
}


// Launch a program
int felish_launch(char **args)
{
    pid_t pid, wpid;
    int status;

    pid = fork();

    if (pid == 0)
    {
        // Child process
        if (execvp(args[0], (args)) == -1)
        {
            perror("felish");
        }
        exit(EXIT_FAILURE);
    }
    else if (pid < 0)
    {
        // Error forking
        perror("felish");
    }
    // Parent process
    else
    {
        do
        {
            wpid = waitpid(pid, &status, WUNTRACED);
        }
        while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    return 1;
}


// Execute the command
int felish_execute(char **args)
{
    // Empty command
    if (args[0] == NULL)
    {
        return 1;
    }

    for (int i = 0; i < felish_num_builtins(); i++)
    {
        if (strcmp(args[0], builtin_str[i]) == 0)
        {
            return (*builtin_func[i])(args);
        }
    }

    felish_launch(args);
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


// Split a line into tokens for commands
char **felish_split_line(char *line)
{
    int bufsize = 64, position = 0;
    char **tokens = malloc(bufsize * sizeof(char*));
    char *token;

    if (!tokens)
    {
        fprintf(stderr, "felish: allocation error\n");
        exit(EXIT_FAILURE);
    }

    token = strtok(line, " ");
    while (token != NULL)
    {
        tokens[position] = token;
        position++;

        token = strtok(NULL, " ");
    }
    tokens[position] = NULL;

    return tokens;
}


void felish_loop(void)
{
    // This is a "minimalist" shell, only one command and no args
    char *line;
    char **args;
    int status;

    do
    {
        printf(":3 ");
        line = felish_read_line();
        args = felish_split_line(line);
        status = felish_execute(args);

        free(line);
        free(args);
    }
    while (status);
}


int main(int argc, char **argv)
{
    felish_loop();

    return EXIT_SUCCESS;
}
