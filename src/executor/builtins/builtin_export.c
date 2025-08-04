#include "../../../includes/executor.h"

char    *trimquotes(char *value)
{
    int i;

    i = 0;
    char *temp;
    int new_len;
    int val_len = (int)ft_strlen(value);
    if ((value[0] == '"' && value[val_len - 1] == '"') || (value[0] == '\'' && value[val_len - 1] == '\''))
    {
        new_len = val_len - 2;
        i = 1;
    }
    else
        return value;
    temp = malloc(new_len + 1);

    int j = 0;
    while (value[i] && i < val_len - 1)
    {
        printf("value of index to copy: start %d", i);
        temp[j] = value[i];
        i++;
        j++;
    }
    temp[j] = '\0';
    return temp;
    
}

static int is_valid_key(const char *key)
{
    int i;

    i = 0;
    if (!key || (!ft_isalpha(key[0]) && key[0] != '_'))
        return (0);
    while (key[i])
    {
        if (key[i] == '=' || key[i] == '+')
            break ;
        if (!ft_isalnum(key[i]) && key[i] != '_')
            return (0);
        i++;
    }
    return (1);
}

static int case_withargs_export(char **args, t_env **env_list)
{
    printf("Hello from casewithargs export;\n");
    int i;
    char *key;
    char *value;
    char *equal_sign;
    char *existing;
    char *plus_equal;

    i = 1;
    while (args[i])
    {
        key = NULL;
        value = NULL;
        equal_sign = ft_strchr(args[i], '=');
        plus_equal = ft_strnstr(args[i], "+=", ft_strlen(args[i]));
        if (plus_equal)
            key = ft_substr(args[i], 0, plus_equal - args[i]);
        else if (equal_sign)
            key = ft_substr(args[i], 0, equal_sign - args[i]);
        else
            key = ft_strdup(args[i]);
        
        if (!is_valid_key(args[i]))
        {
            printf("export: `%s`: not a valid identifier\n", args[i]);
            free(key);
            i++;
            continue ;
        }
        if (plus_equal)
        {
            value = ft_strdup(plus_equal + 2);
           char *newval = trimquotes(value);
            ///need to remove "" from the value
            printf("value: %s\n", newval);
            existing = get_env_value(*env_list, key);
            if (existing)
            {
                char *new_value = ft_strjoin(existing, newval);
                //char *new_value = ft_strjoin(existing, value);
                printf("value: %s\n", new_value);
                update_env(env_list, key, new_value);
                free(new_value);
            }
            else
                update_env(env_list, key, newval);//update_env(env_list, key, value);
            free(value);
        }
        else if (equal_sign)
        {
            
            value = ft_strdup(equal_sign + 1);
            char *newval = trimquotes(value);
            printf("value: %s\n", value);
            update_env(env_list, key, newval);
            free(value);
        }
        else
        {
            if (!get_env_value(*env_list, args[i]))
                update_env(env_list, args[i], NULL);
        }
        free(key);
        i++;
    }
    return (0);
}


//need to check if its valid currently the fucntion is void, but maybe need int 
static int  env_list_size(t_env *env)
{
    int count;

    count = 0;
    while (env)
    {
        count++;
        env = env->next;
    }
    return (count);
}

static char *line_join(t_env *entry)
{
    char *temp;
    char *quoted_val;
    char *final_val;
    char *with_equal;
    char *line;

    temp = ft_strjoin("declare -x ", entry->key);
    quoted_val = ft_strjoin("\"", entry->value);
    final_val = ft_strjoin(quoted_val, "\"");
    with_equal = ft_strjoin("=", final_val);
    line = ft_strjoin(temp, with_equal);
    free(temp);
    free(quoted_val);
    free(final_val);
    free(with_equal);
    return (line);

}
static char *format_export_line(t_env *entry)
{
    char *line;

    if (entry->value)
    {
        line = line_join(entry);
        printf("debug print: %s\n", line);
    }
    else
    {
        line = ft_strjoin("declare -x ", entry->key);
        printf("debug print: entry_key:%s line: %s\n", entry->key, line);
    }
    return (line);
}

static void sort_string_array(char **arr, int size)
{
    char *tmp;
    int i;
    int j;

    i = 0;
    while (i < size - 1)
    {
        j = i + 1;
        while (j < size)
        {
            if (strcmp(arr[i], arr[j]) > 0) //need top use ft_strcmp instead of strcmp
            {
                tmp = arr[i];
                arr[i] = arr[j];
                arr[j] = tmp;
            }
            j++;
        }
        i++;
    }
}
static void  case_noargs_export(t_env *env_list)
{
    printf("Hello from casewithargs export;\n");
    t_env   *current;
    int size;
    char **lines;
    int i;
    int j;
    
    size = env_list_size(env_list);
    current = env_list;
    lines = malloc(sizeof(char *) * (size + 1));
    i = 0;
    while (current)
    {
        lines[i++] = format_export_line(current);
        current = current->next;
    }
    lines[i] = NULL;

    sort_string_array(lines, size);
    j = 0;

    while (j < size)
    {

        if (lines[j])
        {
            printf("%s\n", lines[j]);
            free(lines[j]);
        }
        j++;
    }
    free(lines);
}

int builtin_export(char **args, t_env **env_list)
{
    //if no arguments are there just print all key value env_list
    if (!args[1])
        return case_noargs_export(*env_list), 0;
    //if there are aarguments
    
    return case_withargs_export(args, env_list);
    //return printf("\n\n\n YES ARGS hello\n");
}