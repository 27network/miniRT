/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quad.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/23 03:19:27 by kiroussa          #+#    #+#             */
/*   Updated: 2024/04/27 19:47:08 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef QUAD_H
# define QUAD_H

# include <ft/math/vector.h>
# include <rt/renderer.h>

typedef struct s_rt_obj_quad
{
	t_vec3d	norm;
    t_vec3d size;
}	t_rt_obj_quad;

t_rt_obj_quad	*rt_obj_quad_init(t_vec3d norm, t_vec3d size);

bool	rt_obj_quad_intersect(t_rt_ray ray, t_rt_object *obj, t_rt_hit *hit);

t_vec3d	rt_obj_quad_norm(t_rt_ray ray, t_rt_hit hit);

#endif // QUAD_H
