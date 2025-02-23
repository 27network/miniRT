/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   plane.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/23 03:19:27 by kiroussa          #+#    #+#             */
/*   Updated: 2024/04/23 15:06:09 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PLANE_H
# define PLANE_H

# include <ft/math/vector.h>
# include <rt/renderer.h>

typedef struct s_rt_obj_plane
{
	t_vec3d	norm;
}	t_rt_obj_plane;

t_rt_obj_plane	*rt_obj_plane_init(t_vec3d norm);

bool	rt_obj_plane_intersect(t_rt_ray ray, t_rt_object *obj, t_rt_hit *hit);

t_vec3d	rt_obj_plane_norm(t_rt_ray ray, t_rt_hit hit);

#endif // PLANE_H
