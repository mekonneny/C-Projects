/* Yonathan Mekonnen
   11/27/17
   Assignment 4
   Description: Created Sample Unix Shell
   */


#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#define MAX_LEN 50
int cd(char **args);

void pid();
void ppid();
void exit();
char arguments[MAX_LEN];

//built in shell commands
char *commands[] = {
  "cd",
  "pid",
  "ppid",
  "exit"
};
//built in shell functions
int (*functions[]) (char **) = {
  &cd,
  &pid,
  &ppid,
  &exit
};

int amount() {
  return sizeof(commands) / sizeof(char *);
}

//Printing out the current working directory and changing it to the user's spcified input
int cd(char **args)
{
  if (args[1] == NULL) {
    char cwd[1024];
     if (getcwd(cwd, sizeof(cwd)) != NULL)
         fprintf(stdout, "Current working dir: %s\n", cwd);
  } else {
    if (chdir(args[1]) != 0) {
      perror("error: ");
    }
  }
  return 1;
}
//Implementing Pid Function
void pid()
{
 fprintf(stdout, "Present Pid = %d\n",getpid());
}
//Implementing Ppid function
void ppid()
{
 fprintf(stdout, "Parent Pid = %d\n",getppid());
}


//Implementing exit function
void exit()
{
  raise(SIGKILL);
}

//method that executes the program
int launch(char **args)
{
  pid_t pid;
  int status;

  pid = fork();
  if (pid == 0) {

    if (execvp(args[0], args) == -1) {
      perror("error: ");
    }
    exit(EXIT_FAILURE);
  } else if (pid < 0) {

    perror("error: ");
  } else {

    do {
      waitpid(pid, &status, WUNTRACED);
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
  }
//evaluates and prints out exit status
  int condition;
  for (int i = 0; i < 1; i++){
    wait(&condition);
    printf("Value returned from child process = %d\n", WEXITSTATUS(condition));
  }

  return 1;
}

//launches program
int run(char **args)
{
  int i;

  if (args[0] == NULL) { return 1; }

  for (i = 0; i < amount(); i++) {
    if (strcmp(args[0], commands[i]) == 0)
      return (*functions[i])(args);
  }

  return launch(args);
}

#define TAKECOMMANDSIZE 1024

 char *takeCommand(void)
 {
   char *line = NULL;
   size_t size = 0;
   getline(&line, &size, stdin);
   return line;
 }

#define cmdSize 64
#define cmdDelim " \t\r\n\a"

char **parse(char *line)
{
  int size = cmdSize, place = 0;
  char **cmds = malloc(size * sizeof(char*));
  char *cmd, **cmds_backup;

  cmd = strtok(line, cmdDelim);
  while (cmd != NULL) {
    cmds[place] = cmd;
    place++;

    if (place >= size) {
      size += cmdSize;
      cmds_backup = cmds;
      cmds = realloc(cmds, size * sizeof(char*));
      if (!cmds) {
		    free(cmds_backup);
        fprintf(stderr, " error\n");
        exit(EXIT_FAILURE);
      }
    }

    cmd = strtok(NULL, cmdDelim);
  }
  cmds[place] = NULL;
  return cmds;
}

//looping through input and continuing to execute it
void loop(char* prompt)
{
  char *line;
  char **args;
  int status;

  do {

    printf( "%s$ ", prompt );
    line = takeCommand();
    args = parse(line);
    status = run(args);

    free(line);
    free(args);
  } while (status);


}
//Implements Signal Handler
void handler(int num) {
  fprintf(stdout, "Signal Caught: ");
}



int main(int argc, char **argv)
{

  char* prompt = "my257sh";
  //allows user to select user defined prompt
  if( argc > 2) {
     		if(strcmp(argv[1], "-p") == 0)
     			prompt = argv[2];
      }

  signal(SIGINT, handler);
  loop(prompt);
  return EXIT_SUCCESS;
}
