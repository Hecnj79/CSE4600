/***************************************************************************//**
  @file         main.c
  @author       Hector Najera
  @ID           006868830
  @date         2/24/2023
  @purpose      Allows the user to set the shell name and terminator symbol.
                Also allows them to give new names to built in commands and
                save those names on a seperate file.
*******************************************************************************/
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
char* shellname = "myshell";
char* newNames[10] = {};
char* oldNames[10] = {};
int n = 0;
char* terminator = ">";
char* listNewNames[10] = {};
char* saveNewNames;
char* readNewNames;
/*
  Function Declarations for builtin shell commands:
 */
int lsh_cd(char **args);
int lsh_help(char **args);
int lsh_exit(char **args);
int setshellname(char **args);
int newname(char **args);
int oldname(char **args);
int setterminator(char **args);
int listnewnames(char **args);
int savenewnames(char **args);
int readnewnames (char **args);
/*
  List of builtin commands, followed by their corresponding functions.
 */
char *builtin_str[] = {
  "cd",
  "help",
  "exit",
  "setshellname",
  "newname",
  "setterminator",
  "listnewnames",
  "savenewnames",
  "readnewnames"
};
int (*builtin_func[]) (char **) = {
  &lsh_cd,
  &lsh_help,
  &lsh_exit,
  &setshellname,
  &newname,
  &setterminator,
  &listnewnames,
  &savenewnames,
  &readnewnames
};
int lsh_num_builtins() {
  return sizeof(builtin_str) / sizeof(char *);
}
/*
  Builtin function implementations.
*/
/**
   @brief Bultin command: change directory.
   @param args List of args.  args[0] is "cd".  args[1] is the directory.
   @return Always returns 1, to continue executing.
 */
int lsh_cd(char **args)
{
  if (args[1] == NULL) {
    fprintf(stderr, "lsh: expected argument to \"cd\"\n");
  } else {
    if (chdir(args[1]) != 0) {
      perror("lsh");
    }
  }
  return 1;
}
/**
   @brief Builtin command: print help.
   @param args List of args.  Not examined.
   @return Always returns 1, to continue executing.
 */
int lsh_help(char **args)
{
  int i;
  printf("Stephen Brennan's LSH\n");
  printf("Type program names and arguments, and hit enter.\n");
  printf("The following are built in:\n");
  for (i = 0; i < lsh_num_builtins(); i++) {
    printf("  %s\n", builtin_str[i]);
  }
  printf("Use the man command for information on other programs.\n");
  return 1;
}
/**
   @brief Builtin command: exit.
   @param args List of args.  Not examined.
   @return Always returns 0, to terminate execution.
 */
int lsh_exit(char **args)
{
  return 0;
}
//lists the new names the user gave to the built in commands by using setnewname
int listnewnames(char **args)
{
  for(int i = 0; i < 10; i++)
  {
    if(strcmp(newNames[i], "(null)")==0)
      i=10;
    else printf("%s %s \n", newNames[i], oldNames[i]);
  }
}
//reads the new names saved to a file and prints them on the terminal
int readnewnames (char **args)
{
  if (args[1] == NULL)
 {
    fprintf(stderr, "readnewname expects args\n");
    return 1;
  }
  else
    readNewNames = args[1];

  FILE *fptr;
  fptr = fopen(readNewNames, "r");
  if(fptr == NULL)
  {
    fprintf(stderr, "file doesnt exist\n");
    return 1;
  }
  char newn[100];
  char oldn[100];
  for(int i = 0; i < 10; i++)
  {
    fscanf(fptr, "%s ", &newn);
    fscanf(fptr, "%s", &oldn);
    printf("%s %s\n", newn, oldn);
  }
  fclose(fptr);
  return 1;
}
//saves the new names the user gave to the built in commands and saves it on a file
int savenewnames(char **args)
{
  if (args[1] == NULL)
  {
    fprintf(stderr, "savenewname expects args\n");
    return 1;
  }
  else
    saveNewNames = args[1];

  FILE *fptr;
  fptr = fopen(saveNewNames, "w");
  for(int i = 0; i < 10; i++)
    fprintf(fptr, "%s %s\n", newNames[i], oldNames[i]);
  fclose(fptr);
  return 1;
}
//changes the terminator symbol
int setterminator(char **args)
{
  if (args[1] == NULL)
    terminator = ">";
  else
    terminator = args[1];
  return 1;
}
//gives the built in commands a new name
int newname(char **args)
{
  if (args[1] == NULL)
    fprintf(stderr, "newname expects args\n");
  else
  {
    if(args[2] == NULL)
    {
      int i = 0;
      while(i < n && strcmp(args[1], newNames[i])!=0)
        i++;
      if(i<n)
      {
        newNames[i] = newNames[n-1];
        oldNames[i] = oldNames[n-1];
        n--;
      }
    }
    else
    {
      int i = 0;
      while(i < n && strcmp(args[2], oldNames[i])!=0)
        i++;
      if(i < n)
        newNames[i] = args[1];
      else
      {
        newNames[n] = args[1];
        oldNames[n] = args[2];
        n++;
      }
    }
  }
}
//gives the shell a new name
int setshellname(char **args)
{
  if (args[1] == NULL)
  shellname = "myshell";
  else
  shellname = args[1];
  return 1;
}

/**
  @brief Launch a program and wait for it to terminate.
  @param args Null terminated list of arguments (including program).
  @return Always returns 1, to continue execution.
 */
int lsh_launch(char **args)
{
  pid_t pid, wpid;
  int status;
  pid = fork();
  if (pid == 0) {
    // Child process
    if (execvp(args[0], args) == -1) {
      perror("lsh");
    }
    exit(EXIT_FAILURE);
  } else if (pid < 0) {
    // Error forking
    perror("lsh");
  } else {
    // Parent process
    do {
      wpid = waitpid(pid, &status, WUNTRACED);
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
  }
  return 1;
}
/**
   @brief Execute shell built-in or launch program.
   @param args Null terminated list of arguments.
   @return 1 if the shell should continue running, 0 if it should terminate
 */
int lsh_execute(char **args)
{
  int i;
  if (args[0] == NULL) {
    // An empty command was entered.
printf("args was 0");
    return 1;
  }

  for(int i = 0; i < n; i++)
    if(strcmp(args[0], newNames[i])==0)
      args[0] = oldNames[i];

  for (i = 0; i < lsh_num_builtins(); i++) {
    if (strcmp(args[0], builtin_str[i]) == 0) {
      return (*builtin_func[i])(args);
    }
  }
  return lsh_launch(args);
}
#define LSH_RL_BUFSIZE 1024
/**
   @brief Read a line of input from stdin.
   @return The line from stdin.
 */
char *lsh_read_line(void)
{
  int bufsize = LSH_RL_BUFSIZE;
  int position = 0;
  char *buffer = malloc(sizeof(char) * bufsize);
  int c;
  if (!buffer) {
    fprintf(stderr, "lsh: allocation error\n");
    exit(EXIT_FAILURE);
  }
  while (1) {
    // Read a character
    c = getchar();
    // If we hit EOF, replace it with a null character and return.
    if (c == EOF || c == '\n') {
      buffer[position] = '\0';
      return buffer;
    } else {
      buffer[position] = c;
    }
    position++;
    // If we have exceeded the buffer, reallocate.
    if (position >= bufsize) {
      bufsize += LSH_RL_BUFSIZE;
      buffer = realloc(buffer, bufsize);
      if (!buffer) {
        fprintf(stderr, "lsh: allocation error\n");
        exit(EXIT_FAILURE);
      }
    }
  }
}
#define LSH_TOK_BUFSIZE 64
#define LSH_TOK_DELIM " \t\r\n\a"
/**
   @brief Split a line into tokens (very naively).
   @param line The line.
   @return Null-terminated array of tokens.
 */
char **lsh_split_line(char *line)
{
  int bufsize = LSH_TOK_BUFSIZE, position = 0;
  char **tokens = malloc(bufsize * sizeof(char*));
  char *token;
  if (!tokens) {
    fprintf(stderr, "lsh: allocation error\n");
    exit(EXIT_FAILURE);
  }
  token = strtok(line, LSH_TOK_DELIM);
  while (token != NULL) {
    tokens[position] = token;
    position++;
    if (position >= bufsize) {
      bufsize += LSH_TOK_BUFSIZE;
      tokens = realloc(tokens, bufsize * sizeof(char*));
      if (!tokens) {
        fprintf(stderr, "lsh: allocation error\n");
        exit(EXIT_FAILURE);
      }
    }
    token = strtok(NULL, LSH_TOK_DELIM);
  }
  tokens[position] = NULL;
  return tokens;
}
/**
   @brief Loop getting input and executing it.
 */
void lsh_loop(void)
{
  char *line;
  char **args;
  int status;
  do {
    printf("%s %s ", shellname,terminator);
    line = lsh_read_line();
    args = lsh_split_line(line);
    status = lsh_execute(args);
    //free(line);
    //free(args);
  } while (status);
}
/**
   @brief Main entry point.
   @param argc Argument count.
   @param argv Argument vector.
   @return status code
 */
int main(int argc, char **argv)
{
  // Load config files, if any.
  // Run command loop.
  lsh_loop();
  // Perform any shutdown/cleanup.
  return EXIT_SUCCESS;
}
