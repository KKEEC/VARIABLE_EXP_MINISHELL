#ifndef UTILS_H
# define UTILS_H

#include <stdlib.h>

size_t  ft_strlen(const char *str);
int ft_isalpha(const char c);
int ft_isspace(const char c);
int ft_isschar(const char c);
int ft_isalpha(const char c);
int ft_isalnum(const char c);
char	*ft_strdup(const char *str);
char *ft_strndup(const char *str, size_t n);
int isstrequal(const char *s1, const char *s2);
char	*ft_strchr(const char *str, int c);
char	*ft_substr(char const *s, unsigned int start, size_t len);
char    *ft_strjoin(char const *s1, char const *s2);
char	*ft_strnstr(const char *big, const char *little, size_t len);

# endif