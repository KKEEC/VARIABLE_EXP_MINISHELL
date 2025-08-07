#include "../../includes/executor.h"

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

static void free_envp(char **envp)
{
    int i = 0;
    while (envp[i])
        free(envp[i++]);
    free(envp);
}

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
        envp[i] = ft_strjoin3(env->key, "=", env->value); 
        i++;
        env = env->next;
    }
    envp[i] = NULL;
    return envp;
}
static void status_exit(int code)
{
    exit(code);
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

static  void execute_child(t_ast *ast, t_env *env_list)
{
    char *path;
    char *full_path;
    char **envp;

    path = get_env_value(env_list, "PATH");
    if (!path)
    {
        ft_printstderr("PATH not found\n");
        status_exit(127);
    }
    full_path = get_command_full_path(path, ast->args[0]);
    if (!full_path)
    {
        ft_printstderr("minishell: ");
        ft_printstderr(ast->args[0]);
        ft_printstderr(": command not found\n");
        status_exit(127);
    }
    envp = env_list_to_array(env_list);
    execve(full_path, ast->args, envp);
    free_envp(envp);
    free(full_path);
    status_exit(127);
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
