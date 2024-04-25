/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt_obj_plane.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 19:18:01 by rgramati          #+#    #+#             */
/*   Updated: 2024/04/24 16:27:02 by rgramati         ###   ########.fr       */
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
		{
			hit->dist = t;
			return (true);
		}
	}
	return (false);
}

t_vec3d	rt_obj_plane_norm(
	t_rt_ray ray,
	t_rt_hit hit)
{
	(void) ray;
	t_vec3d			norm;
	t_rt_obj_plane	*plane;

	plane = ((t_rt_obj_plane *)hit.hit_object->options);
	if (ft_vec3d_dot(ray.direction, plane->norm) > 0.0)
		norm = ft_vec3d(0.0f, 0.0f, 0.0f);
	else
		norm = plane->norm;
	return (norm);
}