/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt_obj_cone.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/26 16:01:40 by rgramati          #+#    #+#             */
/*   Updated: 2024/05/05 19:26:02 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>
#include <ft/math.h>
#include <ft/print.h>
#include <rt/object/cone.h>

void    rt_obj_cone_cut(
	t_rt_ray ray,
	t_rt_object *obj,
	t_rt_obj_cone *cone,
	double *t_s
) {
	t_vec3d     tmp_vect;
	double      tmps[3];
	double      eq_params[4];
	double      delta_root;

	tmp_vect = ft_vec3d_sub(ray.origin, obj->position);
	tmps[0] = ft_vec3d_dot(ray.direction, cone->norm);
	tmps[1] = ft_vec3d_dot(tmp_vect, cone->norm);
	tmps[2] = ft_fpow(cos(cone->theta), 2);
	eq_params[0] = ft_fpow(tmps[0], 2) - tmps[2];
	eq_params[1] = 2.0f * (tmps[0] * tmps[1] - (ft_vec3d_dot(tmp_vect, ray.direction) * tmps[2]));
	eq_params[2] = ft_fpow(tmps[1], 2) - ft_vec3d_dot(tmp_vect, tmp_vect) * tmps[2];
	eq_params[3] = ft_fpow(eq_params[1], 2) - 4.0 * eq_params[0] * eq_params[2];
	if (eq_params[3] < 0.0f)
		return ;
	t_s[5] = 0.0f;
	delta_root = sqrt(eq_params[3]);
	t_s[0] = ((-eq_params[1] + delta_root) / (2 * eq_params[0]));
	t_s[1] = (-eq_params[1] - delta_root) / (2 * eq_params[0]);
}

bool    rt_obj_cone_intersect(
	t_rt_ray ray,
	t_rt_object *obj,
	t_rt_hit *hit
) {
	t_rt_obj_cone   *cone;
	t_vec3d			hitpos;
	double			t_s[6];

	cone = (t_rt_obj_cone *)obj->options;
	t_s[5] = 1.0f;
	rt_obj_cone_cut(ray, obj, cone, t_s);
	if (t_s[5])
		return (false);
	if (t_s[0] < 0.0f)
		rt_swap(t_s, t_s + 1);
	if (t_s[0] < 0.0f)
		return (false);
	hitpos = ft_vec3d_sub(ft_vec3d_add(ray.origin, ft_vec3d_mult(ray.direction, t_s[0])), obj->position);
	if (ft_vec3d_len(hitpos) > (cone->height / cos(cone->theta)))
		rt_swap(t_s, t_s + 1);
	hitpos = ft_vec3d_sub(ft_vec3d_add(ray.origin, ft_vec3d_mult(ray.direction, t_s[0])), obj->position);
	if (ft_vec3d_len(hitpos) > (cone->height / cos(cone->theta)))
		return (false);
	if (t_s[0] < 0.0f)
		return (false);
	hit->dist = t_s[0];
	cone->last_hit_on_edge = true;
	return (ft_vec3d_dot(hitpos, cone->norm) > 0);
}

t_vec3d	rt_obj_cone_norm(
	t_rt_ray ray,
	t_rt_hit hit
) {
	t_rt_obj_cone	*cone;
	t_vec3d			contact;
	t_vec3d			norm;
	double			t;

	(void) ray;
	cone = (t_rt_obj_cone *)hit.hit_object->options;
	// if (cone->last_hit_on_edge)
	// {
	t = ft_vec3d_dot(cone->norm, ft_vec3d_sub(hit.position, hit.hit_object->position));
	t = -t / ft_vec3d_dot(cone->norm, cone->norm);
	contact = ft_vec3d_add(hit.hit_object->position, ft_vec3d_mult(cone->norm, -t));
	norm = ft_vec3d_sub(hit.position, contact);
	// }
	// else
	// {
	// 	if (ft_vec3d_dot(ray.direction, cone->norm) < 0.0)
	// 		norm = ft_vec3d_mult(cone->norm, -1.0f);
	// 	else
	// 		norm = cone->norm;
	// }
	return (norm);
}