#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define bufSize 2048
#define timeoutInS 1

char* readStdinLine();
void timeout(int sig);

int main(int argc, char *argv[])
{
    // Set signal handler
    signal(SIGALRM, timeout);

    FILE* fp;
    char buf[bufSize];
    if (argc != 2) // out.txt in argv[1];
    {
        fprintf(stderr,
                "Usage: %s <anwser-file>\n", argv[0]);
        return 1;
    }
    if ((fp = fopen(argv[1], "r")) == NULL)
    {
        perror("fopen source-file");
        return 1;
    }

    int good = 1;
    while (fgets(buf, sizeof(buf), fp) != NULL)
    {
        buf[strlen(buf) - 1] = '\0';
        alarm(timeoutInS); // Timeout handler
        if(strcmp(readStdinLine(), &buf[0]) != 0)
        {
            // One error : wrong
            good = 0;
        }
    }
    fclose(fp);
    alarm(0); // Remove timeout handler

    if(good)
    {
        printf("GOOD answer!\n");
    }
    else
    {
        printf("BAD answer...\n");
    }

    return 0;
}

void timeout(int sig)
{
    if(sig == SIGALRM)
    {
        fprintf(stderr,
                "Timeout: your program did not provide answer in due time.\n");
        exit(1);
    }
}

char* readStdinLine()
{
    int c;
    size_t p4kB = 4096, i = 0;
    void *newPtr = NULL;
    char *ptrString = (char *) malloc(p4kB * sizeof (char));

    while (ptrString != NULL && (c = getchar()) != '\n' && c != EOF)
    {
        if (i == p4kB * sizeof (char))
        {
            p4kB += 4096;
            if ((newPtr = realloc(ptrString, p4kB * sizeof (char))) != NULL)
                ptrString = (char*) newPtr;
            else
            {
                free(ptrString);
                return NULL;
            }
        }
        ptrString[i++] = c;
    }

    if (ptrString != NULL)
    {
        ptrString[i] = '\0';
        ptrString = (char *) realloc(ptrString, strlen(ptrString) + 1);
    }
    else return NULL;

    return ptrString;
}
