/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkc <kkc@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 16:03:36 by kkc               #+#    #+#             */
/*   Updated: 2024/12/02 17:56:38 by kkc              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

// Helper function to extract a line from the buffer

size_t	gnl_ft_strlen(const char *str)
{
	size_t	i;

	i = 0;
	while (str[i] != '\0')
		i++;
	return (i);
}

static char	*extract_line(char **buffer)
{
	char	*newline_pos;
	char	*line;
	char	*remaining;
	size_t	len;

	newline_pos = gnl_ft_strchr(*buffer, '\n');
	if (newline_pos)
	{
		len = newline_pos - *buffer + 1;
		line = (char *)malloc(len + 1);
		if (!line)
			return (NULL);
		gnl_ft_strlcpy(line, *buffer, len + 1);
		remaining = gnl_ft_strdup(newline_pos + 1);
		free(*buffer);
		*buffer = remaining;
	}
	else
	{
		line = gnl_ft_strdup(*buffer);
		free(*buffer);
		*buffer = NULL;
	}
	return (line);
}

// Helper function to read and append to the buffer
static int	read_and_store(int fd, char **buffer)
{
	char	*temp;
	char	*read_buffer;
	ssize_t	bytes_read;

	read_buffer = (char *)malloc(BUFFER_SIZE + 1);
	if (!read_buffer)
		return (-1);
	bytes_read = read(fd, read_buffer, BUFFER_SIZE);
	if (bytes_read <= 0)
	{
		free(read_buffer);
		return (bytes_read);
	}
	read_buffer[bytes_read] = '\0';
	temp = gnl_ft_strjoin(*buffer, read_buffer);
	free(read_buffer);
	if (!temp)
	{
		free(*buffer);
		*buffer = NULL;
		return (-1);
	}
	free(*buffer);
	*buffer = temp;
	return (bytes_read);
}

static char	*init_and_manage_buff(int fd, char **buffer)
{
	if (!*buffer)
		*buffer = gnl_ft_strdup("");
	while (!gnl_ft_strchr(*buffer, '\n'))
	{
		if (read_and_store(fd, buffer) <= 0)
			break ;
	}
	return (*buffer);
}
// Main get_next_line function

char	*get_next_line(int fd)
{
	static char	*buffer;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	buffer = init_and_manage_buff(fd, &buffer);
	if (!buffer)
		return (NULL);
	if (buffer && *buffer)
	{
		line = extract_line(&buffer);
		return (line);
	}
	free(buffer);
	buffer = NULL;
	return (NULL);
}