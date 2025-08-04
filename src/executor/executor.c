#include "../../includes/executor.h"
#include <unistd.h>
#include <sys/wait.h>

int is_builtin(const char *cmd)
{
    if (isstrequal(cmd, "echo") 
    || isstrequal(cmd, "cd") 
    || isstrequal(cmd, "pwd") 
    || isstrequal(cmd, "export") 
    || isstrequal(cmd, "unset")
    || isstrequal(cmd, "env")
    || isstrequal(cmd, "exit"))
        return (1);
    else
        return (0);
}

int execute_builtin(t_ast *node, t_env **env_list)
{
    (void)env_list;
    char **args;
    int status;

    args = node->args;
    status = 0;
    if (isstrequal(args[0], "echo"))
        status = builtin_echo(args);
    else if (isstrequal(args[0], "pwd"))
        status = builtin_pwd();
    else if (isstrequal(args[0], "cd"))
        status = builtin_cd(args, *env_list);
    else if (isstrequal(args[0], "env"))
        status = builtin_env(*env_list, args);
    else if (isstrequal(args[0], "export"))
        status = builtin_export(args, env_list);
    /* else if (isstrequal(args[0], "unset"))
        status = builtin_unset(args);
    else if (isstrequal(args[0], "exit"))
        status = builtin_exit(args);  */
    return (status);
}

char *ft_strncpyandjoin(char *path, int i, int j, char *cmd)
{
  //  (void)cmd;
    int k = 0;
    int len = j - i;
    char *str1;
    str1 = malloc(len + 1);
    while(i < j)
    {
        str1[k] = path[i];
        k++;
        i++;
    }
    str1[k] = '\0';
    char *strslash = ft_strjoin(str1, "/");
    char *res = ft_strjoin(strslash, cmd);

    return (res);
}
/////////

#include <stdlib.h>
#include <string.h>

char *ft_strjoin3(const char *s1, const char *s2, const char *s3)
{
    if (!s1 || !s2 || !s3)
        return NULL;

    size_t len1 = strlen(s1);
    size_t len2 = strlen(s2);
    size_t len3 = strlen(s3);

    char *result = malloc(len1 + len2 + len3 + 1);
    if (!result)
        return NULL;

    strcpy(result, s1);
    strcpy(result + len1, s2);
    strcpy(result + len1 + len2, s3);
    result[len1 + len2 + len3] = '\0';

    return result;
}

/////////
static char **env_list_to_array(t_env *env)
{
    int count = 0;
    t_env *tmp = env;
    while (tmp) {
        count++;
        tmp = tmp->next;
    }

    char **envp = malloc(sizeof(char *) * (count + 1));
    if (!envp) return NULL;

    int i = 0;
    while (env) {
        envp[i] = ft_strjoin3(env->key, "=", env->value); // Make sure to implement or use `asprintf`-like function
        i++;
        env = env->next;
    }
    envp[i] = NULL;
    return envp;
}

////////
static void execute_commands(t_ast *ast, t_env **env_list)
{
    pid_t pid = fork();

    if (pid == -1) {
        perror("fork failed");
        return;
    }

    if (pid == 0) {
        // Child process
        char *path = get_env_value(*env_list, "PATH");
        if (!path) {
            fprintf(stderr, "PATH not found\n");
            exit(127);
        }

        int i = 0, j = 0;
        char *full_path;
        while (path[j]) {
            if (path[j] == ':' || path[j + 1] == '\0') {
                if (path[j + 1] == '\0') j++; // Include last segment
                full_path = ft_strncpyandjoin(path, i, j, ast->args[0]);

                if (access(full_path, X_OK) == 0)
                    execve(full_path, ast->args, env_list_to_array(*env_list));

                free(full_path);
                i = j + 1;
            }
            j++;
        }

        // If we get here, command wasn't found
        fprintf(stderr, "minishell: %s: command not found\n", ast->args[0]);
        exit(127);
    }
    else {
        // Parent process waits for child
        int status;
        waitpid(pid, &status, 0);
    }
}

void execute_ast(t_ast *ast, t_env **env_list)
{
    if (!ast)
        return ;
    if (ast->type == NODE_COMMAND)
    {
        if (!ast->args || !ast->args[0])
            return ;
        if (is_builtin(ast->args[0]))
        {
            execute_builtin(ast, env_list);
        }
        else
        {
            execute_commands(ast, env_list);
            return ;
        }
    }
}