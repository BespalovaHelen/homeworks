#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

// Moodle 13 г) cmd1; cmd2; ...; cmdN - последовательное выполнение
int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s cmd1 cmd2 ... cmdN\n", argv[0]);
        exit(1);
    }

    for (int i = 1; i < argc; i++) {
        pid_t pid = fork();
        
        if (pid == 0) {
            // Дочерний процесс выполняет команду
	    execl("/bin/sh", "sh", "-c", argv[1], NULL);
            perror("execlp");
            exit(1);
        } else if (pid > 0) {
            // Родитель ждет завершения дочернего процесса
            wait(NULL);
        } else {
            perror("fork");
            exit(1);
        }
    }
    
    return 0;
}
