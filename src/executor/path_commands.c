#include "../../includes/executor.h"

char *ft_strncpyandjoin(char *path, int i, int j, char *cmd)
{
    int k;
    int len;
    char *str1;
    char *strslash;
    char *res;

    k = 0;
    len = j - i;
    str1 = malloc(len + 1);
    while(i < j)
    {
        str1[k] = path[i];
        k++;
        i++;
    }
    str1[k] = '\0';
    strslash = ft_strjoin(str1, "/");
    if (!strslash)
    {
        free(str1);
        return NULL;
    }
    res = ft_strjoin(strslash, cmd);
    if (!res)
    {
        free(strslash);
        free(str1);
        return NULL;
    }
    free(str1);
    free(strslash);
    return (res);
}

static void free_envp(char **envp)
{
    int i = 0;
    while (envp[i])
        free(envp[i++]);
    free(envp);
}

char *ft_strjoin3(const char *s1, const char *s2, const char *s3)
{
    char *result;

    if (!s1 || !s2 || !s3)
        return NULL;
    size_t len1 = ft_strlen(s1);
    size_t len2 = ft_strlen(s2);
    size_t len3 = ft_strlen(s3);
    result = malloc(len1 + len2 + len3 + 1);
    if (!result)
        return NULL;
    ft_strcpy(result, s1);
    ft_strcpy(result + len1, s2);
    ft_strcpy(result + len1 + len2, s3);
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
        envp[i] = ft_strjoin3(env->key, "=", env->value); 
        i++;
        env = env->next;
    }
    envp[i] = NULL;
    return envp;
}

static char *get_command_full_path(char *path, char *cmd)
{
    int start_idx;
    int end_idx;
    char *full_path;

    start_idx = 0;
    end_idx = 0;
    while (path[end_idx])
    {
        if (path[end_idx] == ':' || path[end_idx + 1] == '\0')
        {
            if (path[end_idx + 1] == '\0')
                end_idx++;
            full_path = ft_strncpyandjoin(path, start_idx, end_idx, cmd);
            if (access(full_path, X_OK) == 0)
                return (full_path);
            free(full_path);
            start_idx = end_idx + 1;
        }
        end_idx++;
    }
    return (NULL);
}

static void handle_exec_error(const char *cmd, char **envp, int error_code, const char *error_msg)
{
    ft_printstderr("minishell: ");
    ft_printstderr(cmd);
    ft_printstderr(": ");
    ft_printstderr(error_msg);
    ft_printstderr("\n");
    if (envp)
        free_envp(envp);
    exit(error_code);
}

static int is_path_command(const char *cmd)
{
    if (!cmd)
        return 0;
    if (cmd[0] == '/')
        return 1;
    if (cmd[0] == '.' && cmd[1] == '/')
        return 1;
    if (cmd[0] == '.' && cmd[1] == '.' && cmd[2] == '/')
        return 1;
    return 0;
}

static  void execute_child(t_ast *ast, t_env *env_list)
{
    char *path;
    char *full_path;
    char **envp;

    envp = env_list_to_array(env_list);
    if (is_path_command(ast->args[0]))
    {
        // Check if file exists first
        if (access(ast->args[0], F_OK) == 0)
        {
            // File exists, check if it's executable
            if (access(ast->args[0], X_OK) != 0)
                handle_exec_error(ast->args[0], envp, 126, "Permission denied");
        }
        execve(ast->args[0], ast->args, envp);
        // If execve fails, check what kind of error
        if (access(ast->args[0], F_OK) == 0)
        {
            // File exists but execve failed - could be directory or permission issue
            if (access(ast->args[0], X_OK) != 0)
                handle_exec_error(ast->args[0], envp, 126, "Permission denied");
            else
                handle_exec_error(ast->args[0], envp, 126, "Is a directory");
        }
        else
            handle_exec_error(ast->args[0], envp, 127, "No such file or directory");
    }
    path = get_env_value(env_list, "PATH");
    if (!path)
    {
        ft_printstderr("PATH not found\n");
        exit(127);
    }
    full_path = get_command_full_path(path, ast->args[0]);
    if (!full_path)
        handle_exec_error(ast->args[0], envp, 127, "command not found");
    execve(full_path, ast->args, envp);
    free(full_path);
    handle_exec_error(ast->args[0], envp, 127, "command not found");
}

int  execute_commands(t_ast *ast, t_env **env_list)
{
    pid_t   pid;
    int status;

    pid = fork();
    if (pid == -1)
    {
        perror("fork");
        return (1);
    }
    if (pid == 0)
        execute_child(ast, *env_list);
    waitpid(pid, &status, 0);
    if (WIFEXITED(status))
        return WEXITSTATUS(status);
    return 1;
}
