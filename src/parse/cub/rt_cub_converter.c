/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt_cub_converter.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 14:12:34 by rgramati          #+#    #+#             */
/*   Updated: 2024/06/04 14:25:38 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft/math/vector.h"
#include "ft/mem.h"
#include "rt/color.h"
#include "rt/error.h"
#include "rt/object/quad.h"
#include "rt/scene.h"
#include "tocard/util.h"
#include <SDL2/SDL_stdinc.h>
#include <unistd.h>
#include <fcntl.h>
#include <ft/string/parse.h>
#include <rt/parse/parser.h>
#include <rt/parse/cub_parser.h>
#include <ft/print.h>
#include <ft/string.h>
#include <ft/io.h>

t_rt_error	rt_cub_map_error(t_cub_map *map, const char *msg, t_rt_error err)
{
	char	**tmp;

	tmp = map->map;
	while (tmp && *tmp)
		free(*(tmp++));
	free(map->map);
	return (rt_errd(err.type, (void *) msg));
}

static t_rt_error	rt_cub_fill_map(t_cub_map *map)
{
	t_clist	*list;
	char	**tmp;
	int		ccount;

	tmp = map->map - 1;
	list = map->map_list;
	while (++tmp && list)
	{
		ccount = 0;
		while (ccount < map->width)
		{
			if ((size_t)ccount >= ft_strlen(list->line) || list->line[ccount] == ' ')
				(*tmp)[ccount++] = 'S';
			else if (list->line[ccount] == '1')
				(*tmp)[ccount++] = 'X';
			else
				(*tmp)[ccount++] = '0';
		}
		list = list->next;
	}
	if (*tmp || list)
		return (rt_errd(RT_ERROR_ALLOC, "Map filling failed"));
	return (rt_ok());
}

t_rt_error	rt_cub_check_map(t_cub_map *map)
{
	t_rt_error	err;
	char		**tmp;

	map->map = ft_calloc(map->height + 1, sizeof(char *));
	if (!map->map)
		return (rt_cub_map_error(map, "Failed to allocate map", rt_err(RT_ERROR_ALLOC)));
	tmp = map->map;
	while (tmp - map->map < map->height)
	{
		*tmp = ft_calloc(map->width + 1, sizeof(char));
		if (!*tmp)
			return (rt_cub_map_error(map, "Failed to allocate map line", rt_err(RT_ERROR_ALLOC)));
		tmp++;
	}
	err = rt_cub_fill_map(map);

	char	**testtt = map->map;
	while (*testtt)
	{
		ft_printf("[%s]\n", *testtt);
		testtt++;
	}

	return (err);
}

int	rt_cub_update_mask(t_cub_parser *parser, t_cub_map *map)
{
	int	nb_wall;

	nb_wall = 0;
	parser->mask = CUB_ZERO;
	if (parser->pos.y > 0)
		parser->mask |= CUB_SOU * (map->map[parser->pos.y - 1][parser->pos.x] == '0');
	if (parser->pos.x < map->width - 1)
		parser->mask |= CUB_WES * (map->map[parser->pos.y][parser->pos.x + 1] == '0');
	if (parser->pos.y < map->height - 1)
		parser->mask |= CUB_NOR * (map->map[parser->pos.y + 1][parser->pos.x] == '0');
	if (parser->pos.x > 0)
		parser->mask |= CUB_EAS * (map->map[parser->pos.y][parser->pos.x - 1] == '0');
	if (parser->mask == CUB_ZERO)
		return (0);
	nb_wall = !!(parser->mask & CUB_NOR) + !!(parser->mask & CUB_WES) + !!(parser->mask & CUB_SOU) + !!(parser->mask & CUB_EAS);
	return (nb_wall);
}

t_rt_error	rt_cub_convert_map(t_cub_map *map, t_rt_object **objects)
{
	t_cub_parser	parser;

	parser.mask = CUB_ZERO;
	parser.pos = (t_toc_vec2i){0, 0};
	parser.walls = NULL;
	while (parser.pos.y < map->height)
	{
		parser.pos.x = 0;
		while (parser.pos.x < map->width)
		{
			if (map->map[parser.pos.y][parser.pos.x] == 'X')
				map->wall_count += rt_cub_update_mask(&parser, map);
			parser.pos.x++;
		}
		parser.pos.y++;
	}

	ft_printf("[%d] murs\n", map->wall_count);

	parser.walls = ft_calloc(map->wall_count, sizeof(t_rt_object));
	if (!parser.walls)
		return (rt_cub_map_error(map, "Failed to allocate walls", rt_err(RT_ERROR_ALLOC)));
	parser.pos.x = 0;
	while (parser.pos.x < map->wall_count)
	{
		rt_object_init(&parser.walls[parser.pos.x], RT_OBJ_QUAD);
		parser.pos.x++;
	}

	t_rt_object	*last_wall = parser.walls;
	parser.pos = (t_toc_vec2i){0, 0};
	while (parser.pos.y < map->height)
	{
		parser.pos.x = 0;
		while (parser.pos.x < map->width)
		{
			if (map->map[parser.pos.y][parser.pos.x] == 'X' && rt_cub_update_mask(&parser, map))
			{
				ft_printf("BUILDING WALL AT [%d][%d]\n", parser.pos.x, map->height - parser.pos.y - 1);
				if (parser.mask & CUB_NOR)
				{
					rt_obj_set_pos(last_wall, parser.pos.x, 0, map->height - parser.pos.y - 1);
					last_wall->options = rt_obj_quad_init(ft_vec3d(0, 0, -1), ft_vec3d(1, 0, 0), ft_vec3d(0, 2, 0));
					last_wall->mat.obj_color = rt_color_to_norm(rt_color(0xFFFFFFFF));
					last_wall->mat.emi_color = rt_color_to_norm(rt_color(0xFFFFFFFF));
					// last_wall->mat.spe_prob = 1;
					// last_wall->mat.smoothness = 0.95;
					last_wall->mat.emi_strength = 10.0;
					last_wall++;
				}
				if (parser.mask & CUB_WES)
				{
					rt_obj_set_pos(last_wall, parser.pos.x + 1, 0, map->height - parser.pos.y - 1);
					last_wall->options = rt_obj_quad_init(ft_vec3d(1, 0, 0), ft_vec3d(0, 0, 1), ft_vec3d(0, 2, 0));
					last_wall->mat.obj_color = rt_color_to_norm(rt_color(0xFF550527));
					last_wall->mat.spe_color = rt_color_to_norm(rt_color(0xFF550527));
					last_wall++;
				}
				if (parser.mask & CUB_SOU)
				{
					rt_obj_set_pos(last_wall, parser.pos.x + 1, 0, map->height - parser.pos.y);
					last_wall->options = rt_obj_quad_init(ft_vec3d(0, 0, 1), ft_vec3d(-1, 0, 0), ft_vec3d(0, 2, 0));
					last_wall->mat.obj_color = rt_color_to_norm(rt_color(0xFFFAA613));
					last_wall->mat.spe_color = rt_color_to_norm(rt_color(0xFFFAA613));
					// last_wall->mat.spe_prob = 1;
					// last_wall->mat.smoothness = 0.95;
					last_wall++;
				}
				if (parser.mask & CUB_EAS)
				{
					rt_obj_set_pos(last_wall, parser.pos.x, 0, map->height - parser.pos.y);
					last_wall->options = rt_obj_quad_init(ft_vec3d(-1, 0, 0), ft_vec3d(0, 0, -1), ft_vec3d(0, 2, 0));
					last_wall->mat.obj_color = rt_color_to_norm(rt_color(0xFFF44708));
					last_wall->mat.spe_color = rt_color_to_norm(rt_color(0xFFF44708));
					// last_wall->mat.spe_prob = 1;
					// last_wall->mat.smoothness = 0.95;
					last_wall++;
				}
			}
			parser.pos.x++;
		}
		parser.pos.y++;
	}
	*objects = parser.walls;
	return (rt_ok());
}
