#include "../../../includes/tokenizer.h"

char *handle_unquoted_esc_chars(const char *src, size_t len)
{
    char *result;
    size_t  i;
    size_t  j;

    i = 0;
    j = 0;
    result = malloc(sizeof(char) * (len + 1));
    if (!result)
        return (NULL);
    while (i < len)
    {
        if (src[i] == '\\' && (i + 1) < len)
        {
            result[j++] = src[i + 1];
            i += 2;
            continue ;
        }
        result[j++] = src[i++];
    }
    result[j] = '\0';
    return result;
}

char *handle_quoted_esc_chars(const char *src, size_t len)
{
    char *result;
    size_t i;
    size_t j;

    result = malloc(sizeof(char) * (len + 1));
    if (!result)
        return (NULL);
    i = 0;
    j = 0;
    while (i < len)
    {
        if (src[i] == '\\' && (i + 1) < len)
        {
            if (src[i + 1] == '"' || src[i + 1] == '\\' || src[i + 1] == '`')
            {
                result[j++] = src[i + 1];
                i += 2;
                continue ;
            }
        }
        result[j++] = src[i++];
    }
    result[j] = '\0';
    return (result);
}
