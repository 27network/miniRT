/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt_obj_quad.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/27 19:45:57 by rgramati          #+#    #+#             */
/*   Updated: 2024/05/13 17:52:24 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>
#include <ft/math.h>
#include <ft/print.h>
#include <rt/object/quad.h>

static inline bool	rt_quad_inside(
	t_rt_object *obj,
	t_vec3d hitpoint
)	{
	const t_rt_obj_quad	*quad = (t_rt_obj_quad *)(obj)->options;
	const t_vec3d		v = ft_vec3d_sub(hitpoint, obj->position);
	const t_vec3d		size = ft_vec3d(ft_vec3d_len(quad->wvect), ft_vec3d_len(quad->hvect), 0.);
	const t_vec3d		proj = ft_vec3d(ft_vec3d_dot(v, quad->wvect) / size.x, ft_vec3d_dot(v, quad->hvect) / size.y, 0.);
	
	return ((proj.x < size.x && proj.x > 0.) && (proj.y < size.y && proj.y > 0.));
}

bool	rt_obj_quad_intersect(
	t_rt_ray ray,
	t_rt_object *obj,
	t_rt_hit *hit
)	{
	double	d;
	double	t;
	t_vec3d	norm;
    t_vec3d hitpoint;

	norm = ((t_rt_obj_quad *)obj->options)->norm;
	d = ft_vec3d_dot(norm, ray.direction);
	if (ft_fabs(d) > EPSILON)
	{
		t = ft_vec3d_dot(ft_vec3d_sub(obj->position, ray.origin), norm) / d;
		if (ft_fabs(t) > EPSILON && t >= 0)
		{
			hit->dist = t;
            hitpoint = ft_vec3d_add(ray.origin, ft_vec3d_mult(ray.direction, t));
			return (rt_quad_inside(obj, hitpoint));
		}
	}
	return (false);
}

t_vec3d	rt_obj_quad_norm(
	t_rt_ray ray,
	t_rt_hit hit
)	{
	(void) ray;
	t_vec3d			norm;
	t_rt_obj_quad	*quad;

	quad = ((t_rt_obj_quad *)hit.hit_object->options);
	if (ft_vec3d_dot(ray.direction, quad->norm) > 0.0)
		norm = ft_vec3d(0.0f, 0.0f, 0.0f);
	else
		norm = quad->norm;
	return (norm);
}