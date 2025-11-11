#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

// Moodle 13 a)   cmd > f  — вместо стандратного вывода выводим в файл  f,  команда  cmd и файл  f задаются параметрами argv[1]  и argv[2]  для моделирующей программы
int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Нужно два аргумента\n");
        exit(1);
    }
    
    pid_t pid = fork();
    if (pid == 0) {
	// Дочерний процесс
        int fd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd == -1) {
            perror("open");
            exit(1);
        }
        
        // Перенаправляем stdout в файл	
        dup2(fd, STDOUT_FILENO);
        close(fd);
        
	// Выполняем команду
        execl("/bin/sh", "sh", "-c", argv[1], NULL);
        perror("execlp");
        exit(1);
    } else if (pid > 0) {
	// Родительский процесс ждет завершения
        wait(NULL);
    } else {
        perror("fork");
        exit(1);
    }
    
    return 0;
}
