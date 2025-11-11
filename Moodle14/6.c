#include <unistd.h>

int main(int argc, char *argv[]) {
    // Просто передаем управление стандартной утилите echo
    // Сдвигаем аргументы так, чтобы argv[0] стал "echo"
    argv[0] = "echo";
    execvp("echo", argv);
    
    // Если execvp вернул ошибку (не должен), возвращаем код ошибки
    return 1;
}
