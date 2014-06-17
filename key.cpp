#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include <cstdlib>
#include <cstring>
#include <cstdio>

#include <iostream>

using namespace std;

int main(int argc, char** argv)
{
    int fd[2];
    pid_t pid;
    int r = pipe(fd);
    if(r!=0)
    {
        fprintf(stderr, "Unable to create pipe!\n");
        exit(EXIT_FAILURE);
    }

    int rpipe = fd[0];
    int wpipe = fd[1];

    pid = fork();
    if (pid == 0)
    {
        close(rpipe);
        dup2(wpipe, STDOUT_FILENO);
        int r = system("xev");
        if(r!=0)
        {
            fprintf(stderr, "xev error!\n");
            exit(EXIT_FAILURE);
        }
        exit(EXIT_SUCCESS);
    }
    else
    {
        close(wpipe);
        char buf[1025];
        while(true)
        {
            int status;
            pid_t result = waitpid(pid, &status, WNOHANG);
            if(result!=0)
                break;

            size_t nbytes = read(rpipe, buf, sizeof(buf)-1);
            string event(buf, 0, nbytes);
            event = string("perl regex.pl \"")+event+string("\"");
            int r = system(event.c_str());
            if(r!=0)
            {
                fprintf(stderr, "Perl error!\n");
                exit(EXIT_FAILURE);
            }
        }
    }
}
