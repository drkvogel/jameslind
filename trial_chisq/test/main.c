//#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    pid_t pid;
    pid_t parent_pid;
    int status;
    
    parent_pid = getpid();

    printf("begin\n");
    
    if (0 > (pid = fork())) {
        perror("Couldn't fork.\n");
        exit(0);
    }
    
    if (0 == pid) {
        waitpid(parent_pid, &status, 0);
        
        printf("child: started.. sleeping 2 secs\n");
        fflush(stdout);
        
        sleep(2);
        
        printf("child: finished\n");
        
        exit(0);
    } else {
        printf("parent: started\n");
        
        waitpid (pid, &status, WNOHANG);
        
        printf("parent: sending child pid=%d SIGKILL=%d\n", pid, SIGKILL);
        kill(pid, SIGKILL);
        
        printf("parent: finished\n");
    }
    
    return(0);
}
