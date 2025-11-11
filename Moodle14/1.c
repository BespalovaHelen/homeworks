#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Отсутствуют арументы\n");
        exit(1);
    }

    int n = argc - 1; // количество команд
    int pipes[n-1][2]; // массив каналов
    
    // Создаем все необходимые каналы
    for (int i = 0; i < n-1; i++) {
        if (pipe(pipes[i]) == -1) {
            perror("pipe");
            exit(1);
        }
    }

    // Запускаем все процессы
    for (int i = 0; i < n; i++) {
        pid_t pid = fork();
        
        if (pid == 0) {
            // Дочерний процесс
            
            // Перенаправляем ввод (кроме первого процесса)
            if (i > 0) {
                dup2(pipes[i-1][0], STDIN_FILENO);
            }
            
            // Перенаправляем вывод (кроме последнего процесса)
            if (i < n-1) {
                dup2(pipes[i][1], STDOUT_FILENO);
            }
            
            // Закрываем все файловые дескрипторы каналов
            for (int j = 0; j < n-1; j++) {
                close(pipes[j][0]);
                close(pipes[j][1]);
            }
            
            // Выполняем команду
	    execl("/bin/sh", "sh", "-c", argv[i+1], NULL);
            perror("execlp");
            exit(1);
        } else if (pid < 0) {
            perror("fork");
            exit(1);
        }
    }

    // Родительский процесс закрывает все каналы
    for (int i = 0; i < n-1; i++) {
        close(pipes[i][0]);
        close(pipes[i][1]);
    }
    
    // Ждем завершения всех дочерних процессов
    for (int i = 0; i < n; i++) {
        wait(NULL);
    }
    
    return 0;
}
