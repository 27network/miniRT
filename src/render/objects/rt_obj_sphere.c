/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt_obj_sphere.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 14:49:44 by rgramati          #+#    #+#             */
/*   Updated: 2024/04/24 16:25:31 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>
#include <ft/print.h>
#include <ft/math.h>
#include <rt/object/sphere.h>

bool	rt_obj_sphere_delta(
	t_rt_hit *hit,
	double *eq_params)
{
	double			delta_root;
	double			t;

	t = -eq_params[2] / (2 * eq_params[0]);
	if (fabs(eq_params[3]) < EPSILON)
	{
		hit->dist = t;
		return (true);
	}
	delta_root = sqrt(eq_params[3]);
	t = ((-eq_params[1] + delta_root) / (2 * eq_params[0]));
	if (t > ((-eq_params[1] - delta_root) / (2 * eq_params[0])))
		t = ((-eq_params[1] - delta_root) / (2 * eq_params[0]));
	if (t < 0)
		return (false);
	hit->dist = t;
	return (true);
}

bool	rt_obj_sphere_intersect(
	t_rt_ray ray,
	t_rt_object *obj,
	t_rt_hit *hit)
{
	t_vec3d			c_to_r;
	double			eq_params[4];

	eq_params[0] = ft_vec3d_dot(ray.direction, ray.direction);
	c_to_r = ft_vec3d_sub(ray.origin, obj->position);
	eq_params[1] = 2 * ft_vec3d_dot(c_to_r, ray.direction);
	eq_params[2] = ft_vec3d_dot(c_to_r, c_to_r)
		- ft_fpow(((t_rt_obj_sphere *)obj->options)->diameter / 2.0f, 2);
	eq_params[3] = ft_fpow(eq_params[1], 2) - (4 * eq_params[0] * eq_params[2]);
	if (eq_params[3] < 0 || fabs(eq_params[0]) < EPSILON)
		return (false);
	return (rt_obj_sphere_delta(hit, eq_params));
}

t_vec3d	rt_obj_sphere_norm(
	t_rt_ray ray,
	t_rt_hit hit)
{
	t_vec3d	norm;

	norm = ft_vec3d_sub(hit.position, hit.hit_object->position);
	if (ft_vec3d_dot(ray.direction, norm) > 0.0f)
		norm = (t_vec3d) {-norm.x, -norm.y, -norm.z};
	return (norm);
}
