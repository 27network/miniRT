/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 07:37:43 by kiroussa          #+#    #+#             */
/*   Updated: 2024/04/18 20:09:45 by kiroussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include <ft/math/vector.h>
# include <rt/error.h>
# include <rt/scene.h>
# include <stdbool.h>

typedef struct s_rt_parse_ctx
{
	const char	*file;
	char		*line;
	int			line_number;
}	t_rt_parse_ctx;

/**
 * Primitive parser, used to parse a single value from a string and store it in
 * the provided `data` structure at the `offset` position.
 */
typedef struct s_rt_parser
{
	size_t		offset;
	void		*data;
	bool		(*parse)(void);
}	t_rt_parser;

/**
 * Object parser, used to parse an entire object from a configuration line.
 * This is basically a wrapper for the list of primitive parsers, 
 * sorted in a strict order;
 */
typedef struct s_rt_object_parser
{
	const char			*id;
	t_rt_primitive		*primitives;
	size_t				primitives_count;
}	t_rt_object_parser;

t_rt_error	rt_parse_scene(t_rt_scene *scene, const char *path);
t_rt_error	rt_parse_line(t_rt_scene *scene, t_rt_parse_ctx *ctx);

t_rt_error	rt_parse_error(const char *file, t_vec2i pos,
				const char *line, const char *message);

void		rt_parser_register_type(const char *id, t_rt_parser_type type);
void		rt_parser_register_obj(const char *id, t_rt_obj_parser parser);

#endif // PARSER_H
