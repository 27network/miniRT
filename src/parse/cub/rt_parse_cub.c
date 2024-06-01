/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt_parse_cub.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/26 20:26:10 by rgramati          #+#    #+#             */
/*   Updated: 2024/05/31 20:13:19 by rgramati         ###   ########.fr       */
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

t_clist	*rt_clist_new(char *line)
{
	t_clist	*new;

	new = ft_calloc(1, sizeof(t_clist));
	if (new)
	{
		new->line = line;
		new->next = NULL;
	}
	return (new);
}

void	rt_clist_add_back(t_clist **list, t_clist *new)
{
	t_clist	*tmp;
	
	if (!list || !new)
		return ;
	if (!*list)
	{
		*list = new;
		return ;
	}
	tmp = *list;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
}

void	rt_clist_del(t_clist *list)
{
	if (!list)
		return ;
	rt_clist_del(list->next);
	free(list->line);
	free(list);
}

static t_rt_error	rt_cub_filename_check(const char *filename)
{
	t_rt_error	err;
	char		*tmp;
	size_t		len;

	err = rt_ok();
	len = ft_strlen(filename);
	if (len < 5)
		err = rt_errd(RT_ERROR_FILE_PATH, "Wrong filename");
	if (err.type == RT_OK)
	{
		tmp = (char *)(filename + len - 4);
		if (ft_strncmp(tmp, ".cub", 5))
			err = rt_errd(RT_ERROR_FILE_PATH, "Wrong extension");
	}
	return (err);
}

static void	rt_cub_end_file(int fd, char *line)
{
	while (line)
	{
		free(line);
		line = get_next_line(fd);
	}
	free(line);
	if (fd > 2)
		close(fd);
}

static t_rt_error	rt_cub_parse_line(t_cub_map *map, char **line, const char *file, int *lc)
{
	t_rt_error	err;
	static bool	mapped = false;

	err = rt_ok();
	if ((mapped && !**line) || !ft_strchr("NSEW1 ", **line))
		err = rt_parse_error(file, (t_toc_vec2i){0, *lc}, *line, NOCHAR);
	if (err.type == RT_OK && ft_strchr("FCNSEW", **line))
	{
		if (mapped)
			err = rt_parse_error(file, (t_toc_vec2i){0, *lc}, *line, NOCHAR);
		else if (**line)
			err = rt_cub_parse_attribute(map, line, file, lc);
	}
	else if (err.type == RT_OK && **line && ft_strchr("1 ", **line))
	{
		err = rt_cub_parse_map_line(map, line, file, lc);
		mapped = true;
	}
	return (err);
}

t_rt_error	rt_cub_parse_file(t_cub_map *map, const char *file)
{
	t_rt_error	err = {0};
	char		*line;
	static int	line_count = 0;

	err = rt_cub_filename_check(file);
	map->fd = open(file, O_RDONLY);
	if (map->fd == -1)
		err = rt_errd(RT_ERROR_FILE_ACCESS, "File error");
	line = get_next_line(map->fd);
	if (err.type == RT_OK && !line)
		err = rt_parse_error(file, (t_toc_vec2i){0, 1}, "", "Empty file");
	while (err.type == RT_OK && line && ++line_count)
	{
		if (line && line[ft_strlen(line) - !! ft_strlen(line)] == '\n')
			line[ft_strlen(line) - !! ft_strlen(line)] = 0;
		err = rt_cub_parse_line(map, &line, file, &line_count);
		free(line);
		line = get_next_line(map->fd);
	}
	rt_cub_end_file(map->fd, line);
	return (err);
}

#include <stdio.h>

// int main(void)
// {

// 	// t_cub_parser	parser;

// 	// rt_cub_parser_init(&parser);
// 	// parser.pos = (t_toc_vec2i){.x = 5, .y = 5};
	
// 	// // parser.mask |= CUB_HOR;
// 	// parser.mask |= CUB_NEG;

// 	// ft_printf("parser pos = [%d : %d]\n", parser.pos.x, parser.pos.y);
// 	// rt_cub_update_pos(&parser);
// 	// ft_printf("parser pos = [%d : %d]\n", parser.pos.x, parser.pos.y);

// 	t_cub_map	map = {0};
// 	t_rt_error	err = {0};
// 	const char	name[] = "test.cub";

// 	err = rt_cub_parse_file(&map, name);
// 	if (err.type == RT_OK)
// 		err = rt_cub_check_map(&map);
// 	if (err.type == RT_OK)
// 		err = rt_cub_convert_map(&map, objects);


// 	char	**tmp = map.map;

// 	while (*tmp)
// 		free(*(tmp++));
// 	free(map.map);

// 	free(map.textures[0]);
// 	free(map.textures[1]);
// 	free(map.textures[2]);
// 	free(map.textures[3]);
// 	if (map.map_list)
// 		rt_clist_del(map.map_list);

// 	rt_error_print(err);
// }


