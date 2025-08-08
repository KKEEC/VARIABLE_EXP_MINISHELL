#ifndef UTILS_H
# define UTILS_H

#include <stdlib.h>

size_t  ft_strlen(const char *str);
int ft_isalpha(const char c);
int ft_isspace(const char c);
int ft_isschar(const char c);
int ft_isalpha(const char c);
int ft_isalnum(const char c);
int isstrequal(const char *s1, const char *s2);
int ft_strcmp(const char *s1, const char *s2);

int	ft_atoi(const char *str);

char	*ft_strcpy(char *dst, const char *src);
char	*ft_strdup(const char *str);
char    *ft_strndup(const char *str, size_t n);
char	*ft_strchr(const char *str, int c);
char	*ft_substr(char const *s, unsigned int start, size_t len);
char    *ft_strjoin(char const *s1, char const *s2);
char	*ft_strnstr(const char *big, const char *little, size_t len);

void ft_printstderr(const char *str);

void sort_string_array(char **arr, int size);



# endif