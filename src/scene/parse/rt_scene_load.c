/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt_scene_load.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 07:40:50 by kiroussa          #+#    #+#             */
/*   Updated: 2024/02/22 08:28:08 by kiroussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft/io.h>
#include <ft/string.h>
#include <rt/scene/parser.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

static t_rt_error	rt_scene_parse_line(t_rt_scene *line, char *line, int ln)
{
	t_rt_error	err;

	err = rt_ok();
	return (err);
}

static t_rt_error	rt_scene_parse(t_rt_scene *scene, int fd)
{
	t_rt_error	err;
	char		*line;
	int			line_number;

	line_number = 0;
	err = rt_ok();
	while (1)
	{
		line = get_next_line(fd);
		if (!line)
			break ;
		if (err.type == RT_OK)
			err = rt_scene_parse_line(scene, line, line_number);
		free(line);
		line_number++;
	}
	return (err);
}

t_rt_error	rt_scene_load(t_rt_scene *scene, const char *path)
{
	char		*extension;
	int			fd;
	t_rt_error	err;

	if (!scene)
		return (rt_errd(RT_ERROR_NULLPTR, "Invalid (NULL) scene pointer."));
	if (!path || !*path)
		return (rt_errd(RT_ERROR_FILE_PATH, "Invalid (empty) path."));
	extension = ft_strrchr(path, '.');
	if (!extension || ft_strcmp(extension, ".rt") != 0)
		return (rt_errd(RT_ERROR_FILE_PATH, "Invalid file extension "
				"(should be '.rt')."));
	fd = open(path, O_RDONLY);
	if (fd < 0)
		return (rt_errd(RT_ERROR_FILE_ACCESS, strerror(errno)));
	err = rt_scene_parse(scene, fd);
	if (close(fd) < 0)
		return (rt_errd(RT_ERROR_FILE_ACCESS, strerror(errno)));
	return (err);
}
