#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

// Moodle 13 д) cmd1 & cmd2 & ... & cmdN - параллельное выполнение
int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s cmd1 cmd2 ... cmdN\n", argv[0]);
        exit(1);
    }
pid_t pids[argc - 1];
    
    // Запускаем все команды параллельно
    for (int i = 1; i < argc; i++) {
        pid_t pid = fork();
        
        if (pid == 0) {
            // Дочерний процесс выполняет команду
	    execl("/bin/sh", "sh", "-c", argv[1], NULL);
            perror("execlp");
            exit(1);
        } else if (pid > 0) {
            pids[i - 1] = pid;
        } else {
            perror("fork");
            exit(1);
        }
    }
    
    // Ждем завершения всех дочерних процессов
    for (int i = 0; i < argc - 1; i++) {
        waitpid(pids[i], NULL, 0);
    }
    
    return 0;
}
