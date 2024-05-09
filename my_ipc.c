#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define BAD_ARGNUM 255
#define INVALID_SWITCH 254
#define MISSING_CMD 253

void option_array(char *arr[], char *s) {
  int i = 0;
  int j = 0;
  int k = 0;
  char tmp[100] = "";

  while (s[j] != '\0') {
    if (s[j] == ' ') {
      tmp[k] = '\0';
      arr[i] = strdup(tmp);
      i++;
      strcpy(tmp, "");
      k = 0;
    } else {
      tmp[k++] = s[j];
    }
    j++;
  }
  tmp[k] = '\0';
  arr[i] = strdup(tmp);

  arr[i + 1] = NULL;
}

int main(int argc, char *argv[]) {
  int pipefd[2];
  pid_t cpid, cpid2;
  char buf;
  int status1, status2;
  char generator[100] = "";
  char consumer[100] = "";

  if (argc != 5) {
            (argv[0]);
    return BAD_ARGNUM;
  }

  for (int i = 1; i < argc; i = i + 2) {
    if (strcmp(argv[i], "-g") == 0) {
      strcpy(generator, argv[i + 1]);
    } else if (strcmp(argv[i], "-c") == 0) {
      strcpy(consumer, argv[i + 1]);
    } else {
      fprintf(stderr, "Invalid switch %s\n", argv[i]);
      return INVALID_SWITCH;
    }
  }

  if (strlen(generator) == 0 || strlen(consumer) == 0) {
    fprintf(stderr, "Missing generator or consumer command\n");
    return MISSING_CMD;
  }

  char *gen_opt[20];
  char *con_opt[20];

  option_array(gen_opt, generator);
  option_array(con_opt, consumer);

  if (pipe(pipefd) == -1) {
    return errno;
  }

  cpid = fork();
  if (cpid == -1) {
    return errno;
  }

  if (cpid == 0) {
    close(pipefd[0]);
    dup2(pipefd[1], 1);
    close(pipefd[1]);
    execvp(gen_opt[0], gen_opt);
    return errno;
  } else {
    if ((cpid2 = fork()) == 0) {
      close(pipefd[1]);
      dup2(pipefd[0], 0);
      close(pipefd[0]);
      execvp(con_opt[0], con_opt);
      return errno;
    } else if (cpid2 == -1) {
      return errno;
    }
  }

  close(pipefd[0]);
  close(pipefd[1]);

  if (waitpid(cpid, &status1, 0) == -1) {
    return errno;
  }

  if (waitpid(cpid2, &status2, 0) == -1) {
    return errno;
  }

  return 0;
}
