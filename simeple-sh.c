#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
    char prompt[256] = "your@computerrrr ";
    size_t n = 0;
    char delim[] = " ";
    char *ptr;
    int charsRead;
    char **arrS;
    int strNum = 0;
    int idx = 0;
    char *input;
    char *inputCopy;
    char cwd[256];
    char *path;

    while (1)
    {
        input = NULL;
        inputCopy = NULL;

        printf("%s", prompt);

        if (getcwd(cwd, sizeof(cwd)) != NULL)
        {
            printf("%s", cwd);
        }

        printf(" $ ");

        charsRead = getline(&input, &n, stdin);
        if (charsRead == -1)
        {
            return (0);
        }

        inputCopy = malloc(sizeof(char) * charsRead);
        strcpy(inputCopy, input);

        ptr = strtok(input, delim);
        while (ptr != NULL)
        {
            ptr = strtok(NULL, delim);
            strNum++;
        }
        strNum++;

        arrS = malloc(strNum * sizeof(char *));
        ptr = strtok(inputCopy, delim);
        for (idx = 0; ptr != NULL; idx++)
        {
            arrS[idx] = malloc(sizeof(char) * strlen(ptr));
            strcpy(arrS[idx], ptr);
            ptr = strtok(NULL, delim);

            for (unsigned long z = 0; z < strlen(arrS[idx]); z++)
            { // delete newline
                if (arrS[idx][z] == '\n')
                {
                    arrS[idx][z] = '\0';
                }
            }
        }
        arrS[idx] = NULL;

        pid_t pid;
        int status;
        path = "/bin/";          // set path at bin
        char file_path[50];      // full file path
        strcpy(file_path, path); // Assign path to file_path
        strcat(file_path, arrS[0]);

        for (unsigned long z = 0; z < strlen(file_path); z++)
        { // delete newline
            if (file_path[z] == '\n')
            {
                file_path[z] = '\0';
            }
        }

        if (strcmp(arrS[0], "exit") == 0)
        {
            free(input);
            free(inputCopy);
            for (unsigned long idx = 0; idx < sizeof(arrS); idx++)
            {
                free(arrS[idx]);
            }
            free(arrS);
            exit(0);
        }
        else if (strcmp(arrS[0], "cd") == 0)
        {
            if (chdir(arrS[1]) != 0)
            {
                printf("%s", arrS[1]);
                perror("chdir");
            }
        }
        else
        {
            if ((pid = fork()) < 0)
            { // fork a child process
                printf("***Forking child failed\n");
                exit(0);
                break;
            }
            else if (pid == 0)
            { // for the child process
                if (execvp(file_path, arrS) < 0)
                {
                    printf("***unkown command***\n");
                    exit(1);
                }
            }
            else
            {
                while (wait(&status) != pid);
            }
        }

        // free(input);
        // free(inputCopy);
        // for (unsigned long idx = 0; idx < sizeof(arrS); idx++)
        // {
        //     free(arrS[idx]);
        // }
        // free(arrS);
    }

    free(input);
    free(inputCopy);
    for (idx = 0; idx < sizeof(arrS); idx++)
    {
        free(arrS[idx]);
    }
    free(arrS);

    return (0);
}
