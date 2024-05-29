/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt_parse_cub_file.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 12:51:57 by rgramati          #+#    #+#             */
/*   Updated: 2024/05/29 14:10:16 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <fcntl.h>
#include <ft/string/parse.h>
#include <rt/parse/parser.h>
#include <rt/parse/cub_parser.h>
#include <ft/print.h>
#include <ft/string.h>
#include <ft/io.h>

static t_rt_error	rt_cub_get_color(char **line, t_toc_color *color)
{
	int	hold;

	while (*line && **line && color->a != 255)
	{
		hold = ft_atoi(*line);
		if (hold > 255 || hold < 0)
			return (rt_err(RT_OK));
		color->value = (color->value << 8) + hold;
		while (**line && ft_isdigit(**line))
			(*line)++;
		if (!**line || (**line && **line != ','))
			break ;
		(*line)++;
		if (!ft_isdigit(**line))
			break ;
	}
	return (rt_err(RT_OK));
}

static t_rt_error	rt_cub_parse_color(t_cub_map *map, char *l, const char *file, int *lc)
{
	t_rt_error	err;
	static int	id = 0;
	char		*lstart;
	t_toc_vec2i	pos;

	lstart = l++;
	if (!ft_isspace(*l))
		l = NULL;
	err = rt_err(RT_OK);
	map->colors[id] = toc_color_rgba(0, 0, 255, 0);
	while (l && *l && ft_isspace(*l))
		l++;
	if (!l)
		l = lstart + 1;
	else
		err = rt_cub_get_color(&l, &map->colors[id]);
	pos = (t_toc_vec2i){l - lstart, *lc};
	if (err.type == RT_OK && (!l || *l || map->colors[id].a != 255))
		err = rt_parse_error(file, pos, lstart, NOCHAR);
	else if (err.type != RT_OK)
		err = rt_parse_error(file, pos, lstart, "Invalid value range");
	id++;
	return (err);
}

static t_rt_error	rt_cub_parse_texture(t_cub_map *map, char *l, const char *file, int *lc)
{
	t_rt_error	err;
	char		*holds[2];
	static int	id = 0;

	err = rt_err(RT_OK);
	holds[0] = l;
	l += 2;
	while (l && *l && ft_isspace(*l))
		l++;
	holds[1] = l;
	while (l && *l && !ft_isspace(*l))
		l++;
	if (l && *l)
		err = rt_parse_error(file, (t_toc_vec2i){l - holds[0], *lc}, holds[0], NOCHAR);
	if (err.type == RT_OK && l)
	{
		if (access(holds[1], F_OK) || access(holds[1], R_OK))
			err = rt_parse_error(file, (t_toc_vec2i){holds[1] - holds[0], *lc}, holds[0], "File error");
		if (err.type == RT_OK)
			map->textures[id++] = ft_strndup(holds[1], l - holds[1]);
	}
	return (err);
}

t_rt_error	rt_cub_parse_attribute(t_cub_map *map, char **line, const char *filename, int *lc)
{
	t_rt_error	err;
	static char	*identifiers[7] = {"NO", "SO", "EA", "WE", "F", "C", NULL};
	char		**tmp;

	tmp = identifiers - 1;
	while (*(++tmp) && ft_strncmp(*line, *tmp, ft_strlen(*tmp)))
		;
	if (*tmp && ft_strlen(*tmp) == 2)
		err = rt_cub_parse_texture(map, *line, filename, lc);
	else if (*tmp)
		err = rt_cub_parse_color(map, *line, filename, lc);
	if (tmp - identifiers == 6)
		return (rt_parse_error(filename, (t_toc_vec2i){0, *lc}, *line, NOATTR));
	return (err);
}

t_rt_error	rt_cub_parse_map_line(t_cub_map *map, char **line, const char *file, int *lc)
{
	char		*ltmp;
	int			len;
	t_toc_vec2i	pos;

	len = ft_strlen(*line);
	if (len > map->width)
		map->width = len;
	ltmp = *line;
	rt_clist_add_back(&map->map_list, rt_clist_new(ft_strdup(ltmp)));
	while (*ltmp && ft_strchr("01WSEN ", *ltmp))
		ltmp++;
	pos = (t_toc_vec2i){ltmp - *line, *lc};
	if (*ltmp)
		return (rt_parse_error(file, pos, *line, NOCHAR));
	map->height++;
	return (rt_err(RT_OK));
}
