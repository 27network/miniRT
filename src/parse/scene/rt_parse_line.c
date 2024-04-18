/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt_parse_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 21:15:42 by kiroussa          #+#    #+#             */
/*   Updated: 2024/04/18 10:20:58 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft/math/vector.h>
#include <ft/string.h>
#include <rt/parse/parser.h>
#include <stdlib.h>

//DEBUG
#include <stdio.h>

t_rt_error	rt_parse_line(
		t_rt_scene *scene,
		const char *file,
		const char *line,
		int line_number
) {
	int			col;
	char		*id;

	if (!*line)
		return (rt_ok());
	col = 0;
	while (line[col] == ' ')
		col++;
	if (!line[col])
		return (rt_parse_error(file, ft_vec2i(line_number, 0), line,
				"Empty line with spaces"));
	id = ft_strndup(line + col, ft_strchr(line + col, ' ') - (line + col));
	if (!id || !*id)
	{
		free(id);
		return (rt_parse_error(file, ft_vec2i(line_number, col), line,
				"Expected an identifier"));
	}
	(void) scene;
	printf("id: %s\n", id);
	free(id);
	return (rt_ok());
}
