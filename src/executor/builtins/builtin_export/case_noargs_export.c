#include "../../../../includes/executor.h"

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
    }
    else
    {
        line = ft_strjoin("declare -x ", entry->key);
    }
    return (line);
}


void  case_noargs_export(t_env *env_list)
{
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