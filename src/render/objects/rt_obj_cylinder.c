/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt_obj_cylinder.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 13:39:30 by rgramati          #+#    #+#             */
/*   Updated: 2024/06/04 15:19:04 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>
#include <ft/math.h>
#include <ft/print.h>
#include <rt/object/cylinder.h>

#if 1
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
	double				t_s[6];
	double				tmps[2];
	t_vec3d				tmp_vects[3];

	cyl = (t_rt_obj_cylinder *)obj->options;
	t_s[5] = 1.0f;
	rt_obj_cylinder_cut(ray, obj, t_s);
	if (t_s[5])
		return (false);
	tmp_vects[0] = ft_vec3d_mult(cyl->norm, cyl->height / 2);
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
#else

bool	rt_obj_cylinder_intersect(
	t_rt_ray ray,
	t_rt_object *obj,
	t_rt_hit *hit
)	{
	t_rt_obj_cylinder	*cyl;
	t_vec3d				bcap;
	t_vec3d				ba;
	t_vec3d				oc;
	double				vals[9];

	cyl = (t_rt_obj_cylinder *)obj->options;
	bcap = ft_vec3d_add(obj->pos, ft_vec3d_mult(cyl->norm, cyl->height));
	ba = ft_vec3d_sub(bcap, obj->pos);
	oc = ft_vec3d_sub(ray.origin, obj->pos);
	vals[0] = ft_vec3d_dot(ba, ba);
	vals[1] = ft_vec3d_dot(ba, ray.dir);
	vals[2] = ft_vec3d_dot(ba, oc);
	vals[3] = vals[0] - pow(vals[1], 2.);
	vals[4] = vals[0] * ft_vec3d_dot(oc, ray.dir) - vals[1] * vals[2];
	vals[5] = vals[0] * ft_vec3d_dot(oc, oc) - pow(vals[2], 2.) - pow(cyl->diameter / 2, 2.) * vals[0];
	vals[6] = pow(vals[4], 2) - vals[3] * vals[5];
	if (vals[6] < 0.0)
		return (false);
	vals[6] = sqrt(vals[6]);
	vals[7] = (-vals[4] - vals[6]) / vals[3];
	vals[8] = vals[2] + vals[7] * vals[1];
	if (vals[8] > 0.0 && vals[8] < vals[0] && vals[7] > 0.0)
	{
		hit->dist = vals[7];
		return (true);
	}
	vals[7] = ((vals[8] >= 0.0) * vals[0] - vals[2]) / vals[1];
	if (ft_fabs(vals[4] + vals[3] * vals[7]) < vals[6] && vals[7] > 0.0)
	{
		hit->dist = vals[7];
		return (true);
	}
	return (false);
}
#endif

t_vec3d	rt_obj_cylinder_norm(
	t_rt_ray ray,
	t_rt_hit hit
) {
	t_rt_obj_cylinder	*cyl;
	t_vec3d				bcap;
	t_vec3d 			pa;
    t_vec3d 			ba;
	float				h;

	(void) ray;
	cyl = (t_rt_obj_cylinder *)hit.obj->options;
	bcap = ft_vec3d_add(hit.obj->pos, ft_vec3d_mult(cyl->norm, cyl->height));
	pa = ft_vec3d_sub(hit.pos, hit.obj->pos);
	ba = ft_vec3d_sub(hit.pos, bcap);
    h = ft_vec3d_dot(pa,ba) / ft_vec3d_dot(ba,ba);
    return ft_vec3d_sub(pa, ft_vec3d_mult(ba, h));
}
