#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

// Moodle 13 в) cat < f - чтение из файла вместо стандартного ввода
int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s file\n", argv[0]);
        exit(1);
    }

    pid_t pid = fork();
    if (pid == 0) {
        // Дочерний процесс
        int fd = open(argv[1], O_RDONLY);
        if (fd == -1) {
            perror("open");
            exit(1);
        }
        
        // Перенаправляем stdin из файла
        dup2(fd, STDIN_FILENO);
        close(fd);
        
        // Выполняем cat
        execlp("cat", "cat", NULL);
        perror("execlp");
        exit(1);
    } else if (pid > 0) {
        wait(NULL);
    } else {
        perror("fork");
        exit(1);
    }
    
    return 0;
}
