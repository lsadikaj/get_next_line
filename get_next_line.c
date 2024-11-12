/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 09:38:20 by lsadikaj          #+#    #+#             */
/*   Updated: 2024/11/12 15:16:38 by lsadikaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static ssize_t	read_buffer(int fd, char *buffer)
{
	ssize_t	bytes_read;

	bytes_read = read(fd, buffer, BUFFER_SIZE);
	if (bytes_read >= 0)
		buffer[bytes_read] = '\0';
	return (bytes_read);
}

static char	*append_buffer(char *left_c, char *buffer)
{
	char	*temp;

	if (!left_c)
		left_c = ft_strdup(buffer);
	else
	{
		temp = left_c;
		left_c = ft_strjoin(temp, buffer);
		free(temp);
	}
	return (left_c);
}

static char	*fill_line_buffer(int fd, char *left_c)
{
	ssize_t	bytes_read;
	char	buffer[BUFFER_SIZE + 1];

	bytes_read = read_buffer(fd, buffer);
	while (bytes_read > 0)
	{
		left_c = append_buffer(left_c, buffer);
		if (!left_c)
			return (NULL);
		if (ft_strchr(buffer, '\n'))
			break ;
		bytes_read = read_buffer(fd, buffer);
	}
	if (bytes_read == -1)
	{
		free(left_c);
		return (NULL);
	}
	return (left_c);
}

static char	*set_line(char **left_c)
{
	char	*line;
	char	*new_left_c;
	size_t	i;

	i = 0;
	if (!*left_c || **left_c == '\0')
		return (NULL);
	while ((*left_c)[i] != '\n' && (*left_c)[i] != '\0')
		i++;
	if ((*left_c)[i] == '\n')
	{
		line = ft_substr(*left_c, 0, i + 1);
		new_left_c = ft_strdup(*left_c + i + 1);
	}
	else
	{
		line = ft_strdup(*left_c);
		new_left_c = NULL;
	}
	free(*left_c);
	*left_c = new_left_c;
	return (line);
}

char	*get_next_line(int fd)
{
	static char	*left_c = NULL;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0 || read(fd, 0, 0) < 0)
	{
		free(left_c);
		left_c = NULL;
		return (NULL);
	}
	left_c = fill_line_buffer(fd, left_c);
	if (!left_c)
		return (NULL);
	line = set_line(&left_c);
	if (!line)
	{
		free(left_c);
		left_c = NULL;
	}
	return (line);
}
