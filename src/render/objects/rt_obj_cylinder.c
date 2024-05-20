/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt_obj_cylinder.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 13:39:30 by rgramati          #+#    #+#             */
/*   Updated: 2024/05/13 23:17:30 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>
#include <ft/math.h>
#include <ft/print.h>
#include <rt/object/cylinder.h>

void	rt_obj_cylinder_delta(
	t_rt_obj_cylinder *cyl,
	t_rt_hit *hit,
	double *t_s
) {
	if (t_s[2] > t_s[1] || t_s[3] < t_s[0])
	{
		t_s[4] = -1;
		return ;
	}
	t_s[4] = ft_fmax(t_s[0], t_s[2]);
	if (t_s[4] < 0)
		t_s[4] = ft_fmin(t_s[1], t_s[3]);
	if (t_s[4] <= 0)
		return ;
	cyl->last_hit_on_edge = (t_s[2] < t_s[0]);
	hit->dist = t_s[4];
}

void	rt_obj_cylinder_cut(
	t_rt_ray ray,
	t_rt_object *obj,
	double *t_s
) {
	t_rt_obj_cylinder	*cyl;
	t_vec3d				tmp_vects[3];
	double				eq_params[5];
	double				delta_root;

	cyl = (t_rt_obj_cylinder *)obj->options;
	tmp_vects[0] = ft_vec3d_cross(ray.dir, cyl->norm);
	eq_params[0] = ft_vec3d_dot(tmp_vects[0], tmp_vects[0]);
	tmp_vects[1] = ft_vec3d_sub(ray.origin, obj->pos);
	tmp_vects[2] = ft_vec3d_cross(tmp_vects[1], cyl->norm);
	eq_params[1] = 2 * ft_vec3d_dot(tmp_vects[0], tmp_vects[2]);
	eq_params[2] = ft_vec3d_dot(tmp_vects[2], tmp_vects[2])
		 - ft_fpow(cyl->diameter / 2.0f, 2.);
	eq_params[4] = 2. * eq_params[0];
	eq_params[3] = ft_fpow(eq_params[1], 2.) - (2. * eq_params[4] * eq_params[2]);
	if (eq_params[3] < 0.0f)
		return ;
	t_s[5] = 0.0f;
	delta_root = sqrt(eq_params[3]);
	t_s[0] = ((-eq_params[1] - delta_root) / eq_params[4]);
	t_s[1] = ((-eq_params[1] + delta_root) / eq_params[4]);
	if (t_s[0] > t_s[1])
		rt_swap(t_s, t_s + 1);
}

bool	rt_obj_cylinder_intersect(
	t_rt_ray ray,
	t_rt_object *obj,
	t_rt_hit *hit
) {
	t_rt_obj_cylinder	*cyl;
	double	t_s[6];
	double	tmps[2];
	t_vec3d	tmp_vects[2];

	cyl = (t_rt_obj_cylinder *)obj->options;
	t_s[5] = 1.0f;
	rt_obj_cylinder_cut(ray, obj, t_s);
	if (t_s[5])
		return (false);
	tmp_vects[0] = ft_vec3d_mult(cyl->norm, cyl->height / 2.);
	tmp_vects[1] = ft_vec3d_add(obj->pos, tmp_vects[0]);
	tmps[0] = ft_vec3d_dot(ft_vec3d_sub(tmp_vects[1], ray.origin), cyl->norm);
	tmps[1] = ft_vec3d_dot(ray.dir, cyl->norm);
	t_s[2] = (tmps[0] + cyl->height / 2.) / tmps[1];
	t_s[3] = (tmps[0] - cyl->height / 2.) / tmps[1];
	if (t_s[2] > t_s[3])
		rt_swap(t_s + 2, t_s + 3);
	rt_obj_cylinder_delta(cyl, hit, t_s);
	return (t_s[4] > 0);
}

t_vec3d	rt_obj_cylinder_norm(
	t_rt_ray ray,
	t_rt_hit hit
) {
	t_rt_obj_cylinder	*cyl;
	t_vec3d				contact;
	t_vec3d				norm;
	double				t;

	cyl = (t_rt_obj_cylinder *)hit.obj->options;
	if (cyl->last_hit_on_edge)
	{
		t = ft_vec3d_dot(cyl->norm, ft_vec3d_sub(hit.pos, hit.obj->pos));
		t = -t / ft_vec3d_dot(cyl->norm, cyl->norm);
		contact = ft_vec3d_add(hit.obj->pos, ft_vec3d_mult(cyl->norm, -t));
		norm = ft_vec3d_sub(hit.pos, contact);
	}
	else
	{
		if (ft_vec3d_dot(ray.dir, cyl->norm) > 0.0)
			norm = ft_vec3d_mult(cyl->norm, -1.0f);
		else
			norm = cyl->norm;
	}
	return (norm);
}