#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
  printf("[debug]:parent,getpid()=%d\r\n", getpid());
  pid_t pid = fork();

  if (pid == -1) {
    printf("[error]:fock(),failed\r\n");
    printf("[error]:errno=%d,errstr%s\r\n", errno, strerror(errno));
    exit(0);
  }

  if (pid == 0) {
    while (1) {
      printf("[debug]:child,getpid=%d\r\n", getpid());
      sleep(2);
    }
  }

  int status;
  pid_t exit_pid = waitpid(-1, &status, 0);
  printf("[debug]:parent,waitpid(),exit_pid=%d,status=%d\r\n", exit_pid, status);
  // WIFSIGNALED(status)，宏函数，返回子进程是不是被信号终止，如果是，它会返回一个非零值
  if (WIFSIGNALED(status)) {
    // WTERMSIG(status)，宏函数，当WIFSIGNALED(status)返回非零值时，可以用这个宏来提取信号的编号
    printf("[error]:WIFSIGNALED()!=0,WTERMSIG()=%d\n", WTERMSIG(status));
  }

  while (1) {
    printf("[debug]:parent,getpid()=%d\r\n", getpid());
    sleep(2);
  }

  return 0;
}