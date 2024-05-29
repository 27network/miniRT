/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 07:37:43 by kiroussa          #+#    #+#             */
/*   Updated: 2024/05/20 18:54:55 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include <ft/math/vector.h>
# include <rt/error.h>
# include <rt/scene.h>
# include <stdbool.h>

typedef enum s_rt_primitive
{
	RT_PRIM_POSITION,		// vec3d: [-inf - inf, -inf - inf, -inf - inf]
	RT_PRIM_NORMALIZED3D,	// vec3d: [-1.0 - 1.0, -1.0 - 1.0, -1.0 - 1.0]
	RT_PRIM_COLOR,			// vec3i: [0 - 255, 0 - 255, 0 - 255]
	RT_PRIM_RATIO,			// double: 0.0 - 1.0
	RT_PRIM_FOV,			// double: 0.0 - 180.0
	RT_PRIM_UFLOAT,			// double: 0.0 - inf
}	t_rt_primitive;

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
typedef struct s_rt_obj_parser
{
	const char			*id;
	t_rt_primitive		*primitives;
	size_t				primitives_count;
}	t_rt_obj_parser;

t_rt_error	rt_parse_scene(t_rt_scene *scene, const char *path);
t_rt_error	rt_parse_line(t_rt_scene *scene, const char *file,
				const char *line, int line_number);

t_rt_error	rt_parse_error(const char *file, t_toc_vec2i pos,
				const char *line, const char *message);

//void		rt_parser_register_type(const char *id, t_rt_parser_type type);
//void		rt_parser_register_obj(const char *id, t_rt_obj_parser parser);

#endif // PARSER_H
