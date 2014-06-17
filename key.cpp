#include <sys/types.h>
#include <sys/wait.h>

#include <unistd.h>


#include <cstdlib>
#include <cstring>
#include <cstdio>

#include <iostream>
using namespace std;


struct KeyStrokeEvent
{
    int time;
    enum type
    {
        PRESS,
        RELEASE
    };
    char value;
};

int main(int argc, char** argv)
{
    int fd[2];
    pid_t pid;
    pipe(fd);
    int rpipe = fd[0];
    int wpipe = fd[1];

    pid = fork();
    if (pid == 0)
    {
        close(rpipe);
        dup2(wpipe, STDOUT_FILENO);
        system("xev");
        exit(0);
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
            system(event.c_str());
        }
    }
}
