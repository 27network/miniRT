/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt_obj_sphere.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 14:49:44 by rgramati          #+#    #+#             */
/*   Updated: 2024/05/13 23:17:30 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>
#include <ft/print.h>
#include <ft/math.h>
#include <rt/object/sphere.h>

bool	rt_obj_sphere_delta(
	t_rt_hit *hit,
	double *eq_params
) {
	const double	delta_root = sqrt(eq_params[3]);
	const double	double_a = 2 * eq_params[0];
	double			t;

	if (ft_fabs(eq_params[3]) < EPSILON)
	{
		t = -eq_params[2] / double_a;
		hit->dist = t;
		return (true);
	}
	t = ((-eq_params[1] - delta_root) / double_a);
	if (t < 0)
		t = ((-eq_params[1] + delta_root) / double_a);
	if (t < 0)
		return (false);
	hit->dist = t;
	return (true);
}

bool	rt_obj_sphere_intersect(
	t_rt_ray ray,
	t_rt_object *obj,
	t_rt_hit *hit
) {
	const t_vec3d	c_to_r = ft_vec3d_sub(ray.origin, obj->pos);
	double			eq_params[4];

	eq_params[0] = ft_vec3d_dot(ray.dir, ray.dir);
	eq_params[1] = 2 * ft_vec3d_dot(c_to_r, ray.dir);
	eq_params[2] = ft_vec3d_dot(c_to_r, c_to_r)
		- ft_fpow(((t_rt_obj_sphere *)obj->options)->diameter / 2.0, 2.);
	eq_params[3] = ft_fpow(eq_params[1], 2) - (4 * eq_params[0] * eq_params[2]);
	if (eq_params[3] < 0 || ft_fabs(eq_params[0]) < EPSILON)
		return (false);
	return (rt_obj_sphere_delta(hit, eq_params));
}

t_vec3d	rt_obj_sphere_norm(
	t_rt_ray ray,
	t_rt_hit hit
) {
	t_vec3d	norm;

	norm = ft_vec3d_sub(hit.pos, hit.obj->pos);
	if (ft_fabs(ft_vec3d_dot(ray.dir, norm)) < EPSILON)
		norm = ft_vec3d_mult(norm, -1.);
	return (ft_vec3d_norm(norm));
}
