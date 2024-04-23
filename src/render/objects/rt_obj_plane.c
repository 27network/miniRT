/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt_obj_plane.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 19:18:01 by rgramati          #+#    #+#             */
/*   Updated: 2024/04/23 20:09:14 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft/math.h>
#include <ft/print.h>
#include <rt/object/plane.h>

bool	rt_obj_plane_intersect(
	t_rt_ray ray,
	t_rt_object *obj,
	t_rt_hit *hit)
{
	double	d;
	double	t;
	t_vec3d	norm;

	norm = ((t_rt_obj_plane *)obj->options)->norm;
	d = ft_vec3d_dot(norm, ray.direction);
	if (ft_fabs(d) > EPSILON)
	{
		t = ft_vec3d_dot(ft_vec3d_sub(obj->position, ray.origin), norm) / d;
		if (ft_fabs(t) > EPSILON && t >= 0)
			return (rt_hit_update(t, ray, obj, hit));
	}
	return (false);
}

t_vec3d	rt_obj_plane_norm(
	t_rt_ray ray,
	t_rt_hit hit)
{
	(void) ray;
	return (((t_rt_obj_plane *)hit.hit_object->options)->norm);
}