#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 42
# endif

# include <unistd.h>
# include <stdlib.h>

char	*get_next_line(int fd);
size_t	gnl_ft_strlen(const char *str);
char	*gnl_ft_strchr(const char *str, int c);
char	*gnl_ft_strdup(const char *str);
char	*gnl_ft_strjoin(const char *s1, const char *s2);
size_t	gnl_ft_strlcpy(char *dst, const char *src, size_t dstsize);

#endif