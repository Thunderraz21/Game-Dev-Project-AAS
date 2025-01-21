#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) 
{
if (argc != 2) {
printf(stderr, "Usage: %s <integer>\n", argv[0]);
return 1;
}

int number = atoi(argv[1]);
int pipe1[2], pipe2[2];

if (pipe(pipe1) == -1 || pipe(pipe2) == -1) {
error("pipe");
return 1;
}

pid_t pid1 = fork();
if (pid1 == 0) {
close(pipe1[1]); 
int num;
read(pipe1[0], &num, sizeof(num));
close(pipe1[0]);

int square = num * num;
write(pipe1[1], &square, sizeof(square));
close(pipe1[1]);
exit(0);}
pid_t pid2 = fork();
if (pid2 == 0) {
close(pipe2[1]); 
int num;
read(pipe2[0], &num, sizeof(num));
close(pipe2[0]);

int cube = num * num * num;
write(pipe2[1], &cube, sizeof(cube));
close(pipe2[1]);
exit(0);
}
close(pipe1[0]); 
close(pipe2[0]); 

write(pipe1[1], &number, sizeof(number));
write(pipe2[1], &number, sizeof(number));

close(pipe1[1]);
close(pipe2[1]);

int square, cube;
read(pipe1[0], &square, sizeof(square));
read(pipe2[0], &cube, sizeof(cube));

printf("Square = %d\n", square);
printf("Cube = %d\n", cube);

wait(NULL);
wait(NULL);
return 0;
}