/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt_obj_quad.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/27 19:45:57 by rgramati          #+#    #+#             */
/*   Updated: 2024/06/01 18:06:00 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft/math.h>
#include <ft/print.h>
#include <rt/object/quad.h>
extern bool g_debug;

static inline bool	rt_obj_quad_inside(
	t_rt_object *obj,
	t_vec3d hitpoint
)	{
	const t_rt_obj_quad	*quad = (t_rt_obj_quad *)(obj)->options;
	const t_vec3d		v = ft_vec3d_sub(hitpoint, obj->pos);
	const t_vec3d		size = ft_vec3d(ft_vec3d_len(quad->wvect), ft_vec3d_len(quad->hvect), 0.);
	const t_vec3d		proj = ft_vec3d(ft_vec3d_dot(v, quad->wvect) / size.x, ft_vec3d_dot(v, quad->hvect) / size.y, 0.);
	
	return ((proj.x < size.x && proj.x > 0.) && (proj.y < size.y && proj.y > 0.));
}

static inline bool	rt_obj_cube_inside(
	t_rt_object *obj,
	t_vec3d hitpoint,
	t_vec3d opp,
	size_t face_id
)	{
	const t_rt_obj_quad	*quad = (t_rt_obj_quad *)(obj)->options + face_id;
	const t_vec3d		size = ft_vec3d(ft_vec3d_len(quad->wvect), ft_vec3d_len(quad->hvect), 0.);
	t_vec3d				proj;
	t_vec3d				v;
	
	if (face_id < 3)
		v = ft_vec3d_sub(hitpoint, obj->pos);
	else
		v = ft_vec3d_sub(hitpoint, opp);
	proj = ft_vec3d(ft_vec3d_dot(v, quad->wvect) / size.x, ft_vec3d_dot(v, quad->hvect) / size.y, 0.);
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
	d = ft_vec3d_dot(norm, ray.dir);
	if (ft_fabs(d) > EPSILON)
	{
		t = ft_vec3d_dot(ft_vec3d_sub(obj->pos, ray.origin), norm) / d;
		if (ft_fabs(t) > EPSILON && t >= 0)
		{
			hit->dist = t;
            hitpoint = ft_vec3d_add(ray.origin, ft_vec3d_mult(ray.dir, t));
			return (rt_obj_quad_inside(obj, hitpoint));
		}
	}
	return (false);
}

t_vec3d	rt_obj_quad_norm(
	t_rt_ray ray,
	t_rt_hit hit
)	{
	(void) ray;
	t_rt_obj_quad	*quad;

	quad = ((t_rt_obj_quad *)hit.obj->options);
	if (quad->face)
		quad += quad->face;
	return (quad->norm);
}

bool	rt_obj_cube_intersect(
	t_rt_ray ray,
	t_rt_object *obj,
	t_rt_hit *hit
)	{
	double			d_t[2];
    t_vec3d			hitpoint;
	t_rt_obj_quad	*faces;
	bool			result = false;
	bool			result2 = false;
	const t_vec3d	opp = ft_vec3d_add(obj->rotation, ft_vec3d_add(((t_rt_obj_quad *)obj->options)->hvect, ft_vec3d_add(((t_rt_obj_quad *)obj->options)->wvect, obj->pos)));

	faces = (t_rt_obj_quad *)obj->options - 1;
	while (++faces - (t_rt_obj_quad *)obj->options < 6)
	{
		d_t[0] = ft_vec3d_dot(faces->norm, ray.dir);
		if (ft_fabs(d_t[0]) > EPSILON)
		{
			if (faces - (t_rt_obj_quad *)obj->options < 3)
				d_t[1] = ft_vec3d_dot(ft_vec3d_sub(obj->pos, ray.origin), faces->norm) / d_t[0];
			else
				d_t[1] = ft_vec3d_dot(ft_vec3d_sub(opp, ray.origin), faces->norm) / d_t[0];
			if (ft_fabs(d_t[1]) > EPSILON && d_t[1] >= 0)
			{
				hitpoint = ft_vec3d_add(ray.origin, ft_vec3d_mult(ray.dir, d_t[1]));
				result2 = rt_obj_cube_inside(obj, hitpoint, opp, faces - (t_rt_obj_quad *)obj->options);
				result |= result2;
				if (result2 && d_t[1] < hit->dist)
				{
					hit->dist = d_t[1];
					((t_rt_obj_quad *)obj->options)->face = faces - (t_rt_obj_quad *)obj->options;
				}
			}
		}
	}
	return (result);
}