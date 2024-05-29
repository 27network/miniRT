/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quad.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/23 03:19:27 by kiroussa          #+#    #+#             */
/*   Updated: 2024/05/27 20:34:51 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef QUAD_H
# define QUAD_H

# include <ft/math/vector.h>
# include <rt/renderer.h>

typedef struct s_rt_obj_quad
{
	t_vec3d	norm;
	t_vec3d	wvect;
	t_vec3d	hvect;
	int		face;
}	t_rt_obj_quad;

t_rt_obj_quad	*rt_obj_quad_init(
					t_vec3d norm,
					t_vec3d wvect,
					t_vec3d hvect
					);

bool			rt_obj_quad_intersect(
					t_rt_ray ray,
					t_rt_object *obj,
					t_rt_hit *hit
					);

t_vec3d			rt_obj_quad_norm(
					t_rt_ray ray,
					t_rt_hit hit
					);

/* ************************************************************************** */

t_rt_obj_quad	*rt_obj_cube_init(
					t_vec3d xvect,
					t_vec3d yvect,
					t_vec3d zvect
					);

bool			rt_obj_cube_intersect(
					t_rt_ray ray,
					t_rt_object *obj,
					t_rt_hit *hit
					);

#endif // QUAD_H
