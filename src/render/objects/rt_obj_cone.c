/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt_obj_cone.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/26 16:01:40 by rgramati          #+#    #+#             */
/*   Updated: 2024/04/26 16:51:01 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>
#include <ft/math.h>
#include <ft/print.h>
#include <rt/object/cone.h>

bool    rt_obj_cone_delta(
	t_rt_ray ray,
	t_rt_hit *hit,
	t_rt_object *obj,
	double *eq_params
) {
	double      delta_root;
	t_vec3d     hitpos;
	double      t;

	if (ft_fabs(eq_params[3]) < EPSILON)
	{
		t = -eq_params[2] / (2 * eq_params[0]);
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
	hitpos = ft_vec3d_add(ray.origin, ft_vec3d_mult(ray.direction, t));
	return (ft_vec3d_dot(ft_vec3d_sub(hitpos, obj->position), ((t_rt_obj_cone *)obj->options)->norm) > 0);
}

bool    rt_obj_cone_intersect(
	t_rt_ray ray,
	t_rt_object *obj,
	t_rt_hit *hit
) {
	t_rt_obj_cone   *cone;
	t_vec3d         tmp_vect;
	double          tmps[5];
	double          eq_params[4];

	cone = (t_rt_obj_cone *)obj->options;
	tmp_vect = ft_vec3d_sub(ray.origin, obj->position); // -CO>
	tmps[0] = ft_vec3d_dot(ray.direction, cone->norm);
	tmps[1] = ft_vec3d_dot(tmp_vect, cone->norm);
	tmps[2] = ft_fpow(cos(cone->theta), 2);
	eq_params[0] = ft_fpow(tmps[0], 2) - tmps[2];
	eq_params[1] = 2.0f * (tmps[0] * tmps[1] - (ft_vec3d_dot(tmp_vect, ray.direction) * tmps[2]));
	eq_params[2] = ft_fpow(tmps[1], 2) - ft_vec3d_dot(tmp_vect, tmp_vect) * tmps[2];
	eq_params[3] = ft_fpow(eq_params[1], 2) - 4.0 * eq_params[0] * eq_params[2];
	if (eq_params[3] < 0)
		return (false);
	return (rt_obj_cone_delta(ray, hit, obj, eq_params));
}