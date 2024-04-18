/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt_parse_scene.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 07:40:50 by kiroussa          #+#    #+#             */
/*   Updated: 2024/04/18 12:31:32 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft/io.h>
#include <ft/string.h>
#include <rt/parse/parser.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/**
 * This function checks if the given path is to a directory, which would result
 * in an error.
 * 
 * Since we don't have access to traditional stat or lstat, we'll use open in
 * a read-write mode first to check if errno is set to EISDIR. If it is, we'll
 * return an error. If it's not, we'll close the file descriptor try to reopen
 * it in read-only mode.
 *
 * @param path The path to check.
 * @return The error struct.
 */
static t_rt_error	rt_check_directory(const char *path)
{
	int	fd;

	fd = open(path, O_RDWR);
	if (fd < 0)
	{
		if (errno == EISDIR)
			return (rt_errd(RT_ERROR_FILE_ACCESS, strerror(errno)));
		fd = open(path, O_RDONLY);
		if (fd < 0)
			return (rt_errd(RT_ERROR_FILE_ACCESS, strerror(errno)));
		if (close(fd) < 0)
			return (rt_errd(RT_ERROR_FILE_ACCESS, strerror(errno)));
		return (rt_ok());
	}
	if (close(fd) < 0)
		return (rt_errd(RT_ERROR_FILE_ACCESS, strerror(errno)));
	return (rt_ok());
}

static t_rt_error	rt_check_file(const char *path)
{
	char	*extension;

	if (!path || !*path)
		return (rt_errd(RT_ERROR_FILE_PATH, "Invalid (empty) path."));
	extension = ft_strrchr(path, '.');
	if (!extension || ft_strcmp(extension, ".rt") != 0)
		return (rt_errd(RT_ERROR_FILE_PATH, "Invalid file extension "
				"(should be '.rt')."));
	return (rt_check_directory(path));
}

static t_rt_error	rt_parse_scene0(t_rt_scene *scene, const char *file, int fd)
{
	t_rt_error	err;
	char		*line;
	int			line_number;
	size_t		line_length;

	line_number = 0;
	err = rt_ok();
	while (1)
	{
		line = get_next_line(fd);
		if (!line)
			break ;
		line_length = ft_strlen(line);
		if (line_length > 0 && line[line_length - 1] == '\n')
			line[line_length - 1] = '\0';
		if (err.type == RT_OK)
			err = rt_parse_line(scene, file, line, line_number);
		free(line);
		line_number++;
	}
	return (err);
}

t_rt_error	rt_parse_scene(t_rt_scene *scene, const char *path)
{
	int			fd;
	t_rt_error	err;

	if (!scene)
		return (rt_errd(RT_ERROR_NULLPTR, "Invalid (NULL) scene pointer."));
	err = rt_check_file(path);
	if (err.type != RT_OK)
		return (err);
	fd = open(path, O_RDONLY);
	if (fd < 0)
		return (rt_errd(RT_ERROR_FILE_ACCESS, strerror(errno)));
	err = rt_parse_scene0(scene, path, fd);
	if (close(fd) < 0 && err.type == RT_OK)
		err = rt_errd(RT_ERROR_FILE_ACCESS, strerror(errno));
	return (err);
}
