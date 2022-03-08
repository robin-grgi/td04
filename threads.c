#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/syscall.h>
#include <sys/types.h>

void *fn_thread(void *arg) {
  for (int i = 0; i < 5; i++) {
    long n = (long)arg;
    if (n >= 0) {
      sleep(n);
    }
    else {
      pthread_exit((void *)1);
    }
    printf("PID : %d, Posix Thread ID : %d, Linux Thread ID: %ld\n", getpid(), (int)pthread_self(), syscall(SYS_gettid));
  }
  pthread_exit((void *)0);
}

int main(int argc, char const *argv[]) {
  if (argc < 3) {
    printf("please input two integers in args.\n");
  }
  else {
    long n1 = atoi(argv[1]);
    long n2 = atoi(argv[2]);
    printf("Program start PID : %d\n", getpid());
    pthread_t t1, t2;
    long return1, return2;
    pthread_create(&t1, NULL, fn_thread, (void*)n1);
    pthread_create(&t2, NULL, fn_thread, (void*)n2);
    pthread_join(t1, (void*) &return1);
    pthread_join(t2, (void*) &return2);
    printf("Status for n1 : %ld\nStatus for n2 : %ld\n ", return1, return2);
  }
  return 0;
}