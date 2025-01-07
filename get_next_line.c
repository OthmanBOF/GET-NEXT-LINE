/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obouftou <obouftou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 17:27:22 by obouftou          #+#    #+#             */
/*   Updated: 2024/11/30 19:34:24 by obouftou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*join_buff(char *buffer, char *new_buffer)
{
	char	*catcher;

	catcher = ft_strjoin(buffer, new_buffer);
	free(buffer);
	buffer = NULL;
	return (catcher);
}

char	*get_afterline(char *buffer)
{
	int		i;
	int		j;
	char	*line;

	i = 0;
	j = 0;
	while (buffer[i] && buffer[i] != '\n')
		i++;
	if (!buffer[i])
	{
		free(buffer);
		return (NULL);
	}
	line = malloc(ft_strlen(buffer) - i + 1);
	if (!line)
		return (free(buffer), NULL);
	i++;
	while (buffer[i])
		line[j++] = buffer[i++];
	line[j] = '\0';
	free(buffer);
	return (line);
}

char	*catch_line(char *buffer)
{
	char	*line;
	int		i;

	i = 0;
	if (!buffer[i])
		return (NULL);
	while (buffer[i] && buffer[i] != '\n')
		i++;
	if (buffer[i] == '\n')
		line = malloc(i + 2);
	else
		line = malloc(i + 1);
	if (!line)
		return (NULL);
	i = 0;
	while (buffer[i] && buffer[i] != '\n')
	{
		line[i] = buffer[i];
		i++;
	}
	if (buffer[i] == '\n')
		line[i++] = '\n';
	line[i] = '\0';
	return (line);
}

char	*get_buffer(int fd, char *buffer)
{
	char	*new_buffer;
	int		bytes;

	new_buffer = malloc ((size_t)BUFFER_SIZE + 1);
	if (!new_buffer)
		return (NULL);
	bytes = 1;
	while (bytes > 0)
	{
		bytes = read(fd, new_buffer, BUFFER_SIZE);
		new_buffer[bytes] = '\0';
		buffer = join_buff(buffer, new_buffer);
		if (!buffer)
			return (free (buffer), free (new_buffer), NULL);
		if (ft_strchr(new_buffer, '\n'))
			break ;
	}
	if (bytes == -1)
		return (free (new_buffer), NULL);
	free (new_buffer);
	return (buffer);
}

char	*get_next_line(int fd)
{
	static char	*buffer;
	char		*ster;

	if (fd < 0 || BUFFER_SIZE <= 0 || read(fd, 0, 0) < 0)
		return (free (buffer), buffer = NULL, NULL);
	buffer = get_buffer(fd, buffer);
	if (!buffer)
		return (NULL);
	ster = catch_line(buffer);
	if (!ster)
		return (free(buffer), buffer = NULL, NULL);
	buffer = get_afterline (buffer);
	if (!buffer)
		buffer = NULL;
	return (ster);
}
