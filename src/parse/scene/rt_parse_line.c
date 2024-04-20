/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt_parse_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 21:15:42 by kiroussa          #+#    #+#             */
/*   Updated: 2024/04/18 19:38:54 by kiroussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft/math/vector.h>
#include <ft/string.h>
#include <rt/parse/parser.h>
#include <stdlib.h>

//DEBUG
#include <stdio.h>

static t_rt_error	rt_handle_identifier(
	t_rt_scene *scene __attribute__((unused)),
	t_rt_parse_ctx *ctx __attribute__((unused)),
	char *id __attribute__((unused))
) {
	return (rt_errd(RT_ERROR_UNIMPLEMENTED, "Identifier handling"));
}

t_rt_error	rt_parse_line(
	t_rt_scene *scene,
	t_rt_parse_ctx *ctx
) {
	const char	*line = ctx->line;
	int			col;
	char		*id;
	t_rt_error	err;

	if (!*line)
		return (rt_ok());
	col = 0;
	while (line[col] == ' ')
		col++;
	if (!line[col])
		return (rt_parse_error(ctx->file, ft_vec2i(ctx->line_number, 0), line,
				"Empty line with spaces"));
	id = ft_strndup(line + col, ft_strchr(line + col, ' ') - (line + col));
	if (!id || !*id)
	{
		if (id)
			free(id);
		return (rt_parse_error(ctx->file, ft_vec2i(ctx->line_number, col), line,
				"Expected an identifier"));
	}
	err = rt_handle_identifier(scene, ctx, id);
	free(id);
	return (err);
}
