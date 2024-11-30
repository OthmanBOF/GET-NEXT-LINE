/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obouftou <obouftou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 21:43:54 by obouftou          #+#    #+#             */
/*   Updated: 2024/11/30 22:59:49 by obouftou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

char	*join_buff(char *bufs, char *new_buffer)
{
	char	*catcher;

	catcher = ft_strjoin(bufs, new_buffer);
	free(bufs);
	bufs = NULL;
	return (catcher);
}

char	*catch_line(char *bufs)
{
	char	*line;
	int		i;

	i = 0;
	if (!bufs[i])
		return (NULL);
	while (bufs[i] && bufs[i] != '\n')
		i++;
	if (bufs[i] == '\n')
		line = malloc(i + 2);
	else
		line = malloc(i + 1);
	if (!line)
		return (NULL);
	i = 0;
	while (bufs[i] && bufs[i] != '\n')
	{
		line[i] = bufs[i];
		i++;
	}
	if (bufs[i] == '\n')
		line[i++] = '\n';
	line[i] = '\0';
	return (line);
}

char	*get_afterline(char *bufs)
{
	int		i;
	int		j;
	char	*line;

	i = 0;
	j = 0;
	while (bufs[i] && bufs[i] != '\n')
		i++;
	if (!bufs[i])
	{
		free(bufs);
		return (NULL);
	}
	line = malloc(ft_strlen(bufs) - i + 1);
	if (!line)
		return (free(bufs), NULL);
	i++;
	while (bufs[i])
		line[j++] = bufs[i++];
	line[j] = '\0';
	free(bufs);
	return (line);
}

char	*get_buffer(int fd, char *bufs)
{
	char	*new_buffer;
	int		bytes;

	new_buffer = malloc (BUFFER_SIZE + 1);
	if (!new_buffer)
		return (NULL);
	bytes = 1;
	while (bytes > 0)
	{
		bytes = read(fd, new_buffer, BUFFER_SIZE);
		new_buffer[bytes] = '\0';
		bufs = join_buff(bufs, new_buffer);
		if (!bufs)
			return (free(bufs), NULL);
		if (ft_strchr(bufs, '\n'))
			break ;
	}
	if (bytes == -1)
		return (free (new_buffer), NULL);
	free(new_buffer);
	return (bufs);
}

char	*get_next_line(int fd)
{
	static char	*bufs[FD_SETSIZE];
	char		*ster;

	if (fd < 0 || fd >= FD_SETSIZE || BUFFER_SIZE <= 0 || read(fd, 0, 0) < 0)
		return (free(bufs[fd]), bufs[fd] = NULL, NULL);
	bufs[fd] = get_buffer(fd, bufs[fd]);
	if (!bufs[fd])
		return (NULL);
	ster = catch_line(bufs[fd]);
	if (!ster)
		return (free(bufs[fd]), bufs[fd] = NULL, NULL);
	bufs[fd] = get_afterline(bufs[fd]);
	if (!bufs[fd])
		bufs[fd] = NULL;
	return (ster);
}
#include <fcntl.h>
#include <libc.h>

int main ()
{
	int fd1 = open ("string1.txt", O_RDWR);
	int fd2 = open ("string2.txt", O_RDWR);
	int fd3 = open ("string3.txt", O_RDWR);

	char *line1 = NULL, *line2 = NULL, *line3 = NULL;

	if (fd1 < 0 || fd2 < 0 || fd3 < 0)
		return (0);

	while(1)
	{
		line1 = get_next_line(fd1);
		if (line1)
		{
			printf("%s", line1);
			free(line1);
		}
		line2 = get_next_line(fd2);
		if (line2)
		{
			printf("%s", line2);
			free(line2);
		}
		line3 = get_next_line(fd3);
		if (line3)
		{
			printf("%s", line3);
			free(line3);
		}
		if (!line1 && !line2 && !line3)
			break ;
	}
	close(fd1);
	close(fd2);
	close(fd3);
	return (0);
}
