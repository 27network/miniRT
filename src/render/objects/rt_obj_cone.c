/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt_obj_cone.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/26 16:01:40 by rgramati          #+#    #+#             */
/*   Updated: 2024/04/26 21:10:06 by rgramati         ###   ########.fr       */
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
	if (t_s[0] > t_s[1])
		rt_swap(t_s, t_s + 1);
	// return ();
}

bool    rt_obj_cone_intersect(
	t_rt_ray ray,
	t_rt_object *obj,
	t_rt_hit *hit
) {
	t_rt_obj_cone   *cone;
	// t_vec3d			tmp_vects[2];
	double			tmps[2];
	double			t_s[6];

	cone = (t_rt_obj_cone *)obj->options;
	t_s[5] = 1.0f;
	rt_obj_cone_cut(ray, obj, cone, t_s);
	if (t_s[5])
		return (false);
	hit->dist = t_s[0];
	tmps[0] = ft_vec3d_dot(ft_vec3d_sub(obj->position, ray.origin), cone->norm);
	tmps[1] = ft_vec3d_dot(ray.direction, cone->norm);
	t_s[2] = (tmps[0] + cone->height / 2.0f) / tmps[1];
	t_s[3] = (tmps[0] - cone->height / 2.0f) / tmps[1];
	if (t_s[2] > t_s[3])
		rt_swap(t_s + 2, t_s + 3);
	if (t_s[2] > t_s[1] || t_s[3] < t_s[0])
		return (false);
	t_s[4] = ft_fmax(t_s[0], t_s[2]);
	if (t_s[4] < 0)
		t_s[4] = ft_fmin(t_s[1], t_s[3]);
	if (t_s[4] <= 0)
		return (false);
	cone->last_hit_on_edge = (t_s[2] < t_s[0]);
	hit->dist = t_s[4];
	// return (true);
	return (ft_vec3d_dot(ft_vec3d_sub(ft_vec3d_add(ray.origin, ft_vec3d_mult(ray.direction, t_s[4])), obj->position), cone->norm) > 0);
}

t_vec3d	rt_obj_cone_norm(
	t_rt_ray ray,
	t_rt_hit hit
) {
	t_rt_obj_cone	*cone;
	t_vec3d			contact;
	t_vec3d			norm;
	double			t;

	cone = (t_rt_obj_cone *)hit.hit_object->options;
	// if (cone->last_hit_on_edge)
	// {
	// 	t = ft_vec3d_dot(cone->norm, ft_vec3d_sub(hit.position, hit.hit_object->position));
	// 	t = -t / ft_vec3d_dot(cone->norm, cone->norm);
	// 	contact = ft_vec3d_add(hit.hit_object->position, ft_vec3d_mult(cone->norm, -t));
	// 	norm = ft_vec3d_sub(hit.position, contact);
	// }
	// else
	// {
	// 	if (ft_vec3d_dot(ray.direction, cone->norm) > 0.0)
	// 		norm = ft_vec3d_mult(cone->norm, -1.0f);
	// 	else
	(void) contact;
	(void) t;
	(void) ray;
	(void) hit;
	norm = (t_vec3d){1.0f, 1.0f, 1.0f};
	// }
	return (norm);
}