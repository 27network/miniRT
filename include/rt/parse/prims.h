/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prims.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 19:45:05 by kiroussa          #+#    #+#             */
/*   Updated: 2024/04/18 20:54:09 by kiroussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PRIMS_H
# define PRIMS_H
# undef PRIMS_H
# ifndef __RT_PARSE_PRIMS_H__
#  define __RT_PARSE_PRIMS_H__

#  include <rt/parse/parser.h> 

typedef enum s_rt_primitive
{
	RT_PRIM_POSITION = 0,	// vec3d: [-inf - inf, -inf - inf, -inf - inf]
	RT_PRIM_NORMALIZED3D,	// vec3d: [-1.0 - 1.0, -1.0 - 1.0, -1.0 - 1.0]
	RT_PRIM_COLOR,			// vec3i: [0 - 255, 0 - 255, 0 - 255]
	RT_PRIM_RATIO,			// double: 0.0 - 1.0
	RT_PRIM_FOV,			// double: 0.0 - 180.0
	RT_PRIM_UFLOAT,			// double: 0.0 - inf
	RT_PRIM_END,			// End of primitives
}	t_rt_primitive;

extern static const char	*g_primitives[RT_PRIM_END];

typedef t_rt_error	(*t_rt_prim_parser)(const char *slice, void *data);

typedef struct s_rt_primitive_parser
{
	t_rt_primitive	type;
	void			*data;
}	t_rt_primitive_parser;

# endif // __RT_PARSE_PRIMS_H__
#endif // PRIMS_H
