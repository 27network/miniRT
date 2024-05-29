/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt_cub_converter.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 14:12:34 by rgramati          #+#    #+#             */
/*   Updated: 2024/05/29 14:38:52 by rgramati         ###   ########.fr       */
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

t_rt_error	rt_cub_check_map(t_cub_map *map, char *file)
{
	t_rt_error	err;
	t_clist		*current;
	size_t		len;
	size_t		count;

	current = map->map_list;
	count = 0;
	while (current->next)
	{
		
	}
	return (err);
}
