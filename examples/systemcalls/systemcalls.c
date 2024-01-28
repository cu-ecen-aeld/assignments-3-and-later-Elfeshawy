#include "systemcalls.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>

bool do_system(const char *cmd)
{
    /*
     * TODO: Call the system() function with the command set in the cmd
     * and   return a boolean true if the system() call completed with success
     * or false if it returned a f ailure
     */

    int result = system(cmd);

    if (WIFEXITED(result))
    {
        int exitStatus = WEXITSTATUS(result);
        return exitStatus == 0; // Return true if exit status is 0 (success)
    }

    return false; // Return false for any failure
}

bool do_exec(int count, ...)
{
    va_list args;
    va_start(args, count);
    char *command[count + 1];
    int i;
    for (i = 0; i < count; i++)
    {
        command[i] = va_arg(args, char *);
    }
    command[count] = NULL;

    /*
     * TODO: Execute a system command by calling fork, execv(),
     * and wait instead of system.
     * Use the command[0] as the full path to the command to execute
     * (first argument to execv), and use the remaining arguments
     * as the second argument to the execv() command.
     */

    pid_t pid = fork();

    if (pid == -1)
    {
        perror("Fork failed");
        va_end(args);
        return false;
    }
    else if (pid == 0)
    {
        // Child process
        execv(command[0], command);
        // If execv returns, it's an error
        perror("Execv failed");
        exit(EXIT_FAILURE);
    }
    else
    {
        // Parent process
        int status;
        waitpid(pid, &status, 0);
        va_end(args);

        return WIFEXITED(status) && (WEXITSTATUS(status) == 0);
    }
}

bool do_exec_redirect(const char *outputfile, int count, ...)
{
    va_list args;
    va_start(args, count);
    char *command[count + 1];
    int i;
    for (i = 0; i < count; i++)
    {
        command[i] = va_arg(args, char *);
    }
    command[count] = NULL;

    /*
     * TODO: Call execv, redirect standard out to a file specified by outputfile.
     * The rest of the behavior is the same as do_exec.
     */

    pid_t pid = fork();

    if (pid == -1)
    {
        perror("Fork failed");
        va_end(args);
        return false;
    }
    else if (pid == 0)
    {
        // Child process
        int fd = open(outputfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);

        if (fd == -1)
        {
            perror("Open failed");
            exit(EXIT_FAILURE);
        }

        // Redirect standard output to the file
        if (dup2(fd, STDOUT_FILENO) == -1)
        {
            perror("Dup2 failed");
            close(fd);
            exit(EXIT_FAILURE);
        }

        close(fd);

        execv(command[0], command);
        // If execv returns, it's an error
        perror("Execv failed");
        exit(EXIT_FAILURE);
    }
    else
    {
        // Parent process
        int status;
        waitpid(pid, &status, 0);
        va_end(args);

        return WIFEXITED(status) && (WEXITSTATUS(status) == 0);
    }
}

