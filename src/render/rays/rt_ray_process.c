/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt_ray_process.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 22:34:50 by rgramati          #+#    #+#             */
/*   Updated: 2024/05/28 17:52:34 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft/string/parse.h>
#include <rt/renderer.h>
#include <rt/object/quad.h>

static t_color_norm	rt_get_skybox(
	t_rt_renderer *renderer,
	t_rt_ray *ray
)	{
	const t_rt_skybox	\
	sky = renderer->scene->skybox;
	const t_vec3d		\
	skyvec = ft_vec3d_lerp(sky.usky, sky.dsky, \
	pow(ft_smoothstep(0, 0.4, ray->dir.y), 0.35));
	const double		\
	s[4] = {ft_smoothstep(-.01, 0, ray->dir.y), 0.2, 0.05, ft_fpow(\
	ft_fmax(0., ft_vec3d_dot(ray->dir, renderer->scene->ambient.pos)), \
	s[1]) * s[2]};
	const t_vec3d		\
	svec = ft_vec3d_mult(ft_vec3d(s[3], s[3], s[3]), (s[0] >= 1));
	const t_vec3d		\
	comp = ft_vec3d_add(ft_vec3d_lerp(sky.gcol, skyvec, s[0]), svec);

	return ((t_color_norm){.a = 1., .r = comp.x, .g = comp.y, .b = comp.z});
}

static void	rt_ray_colorize(
	t_rt_ray *ray,
	t_rt_hit *hit,
	t_color_norm *light,
	bool spec_bounce
)	{
	const t_rt_mat		\
	hit_mat = hit->obj->mat;
	const t_color_norm		\
	emittedlight = rt_color_fact(hit_mat.emi_color, hit_mat.emi_strength);
	const t_color_norm		\
	raylight = rt_color_mult(ray->color, emittedlight);

	*light = rt_color_add(*light, raylight);
	
	if (spec_bounce)
		ray->color = rt_color_mult(ray->color, hit_mat.spe_color);
	else
	{
		if (hit->obj->type != RT_OBJ_CUBE)
			ray->color = rt_color_mult(ray->color, hit_mat.obj_color);
		else
		{
			if (((t_rt_obj_quad *)hit->obj->options)->face == 0)
				ray->color = rt_color_mult(ray->color, rt_color_to_norm(rt_color(0xFFFF0000)));
			else if (((t_rt_obj_quad *)hit->obj->options)->face == 1)
				ray->color = rt_color_mult(ray->color, rt_color_to_norm(rt_color(0xFF00FF00)));
			else if (((t_rt_obj_quad *)hit->obj->options)->face == 2)
				ray->color = rt_color_mult(ray->color, rt_color_to_norm(rt_color(0xFF0000FF)));
			else if (((t_rt_obj_quad *)hit->obj->options)->face == 3)
				ray->color = rt_color_mult(ray->color, rt_color_to_norm(rt_color(0xFFFFFF00)));
			else if (((t_rt_obj_quad *)hit->obj->options)->face == 4)
				ray->color = rt_color_mult(ray->color, rt_color_to_norm(rt_color(0xFF00FFFF)));
			else if (((t_rt_obj_quad *)hit->obj->options)->face == 5)
				ray->color = rt_color_mult(ray->color, rt_color_to_norm(rt_color(0xFFFF00FF)));
		}
	}
}

static void	rt_ray_update(
	t_rt_renderer *renderer,
	t_rt_ray *ray,
	t_rt_hit *hit,
	bool spec_bounce
)	{
	static long long	\
	rng = 0;
	const size_t		\
	frame = renderer->rendered;
	const t_vec3d		\
	norm = hit->obj->norm(*ray, *hit);
	const t_vec3d		\
	diff_dir = ft_vec3d_norm(ft_vec3d_add(ft_vec3d_random(&rng), norm));
	const t_vec3d		\
	spec_dir = ft_vec3d_sub(ray->dir, ft_vec3d_mult(ft_vec3d_mult(\
	norm, ft_vec3d_dot(ray->dir, norm)), 2.));

	if (rng == 0)
		rng = ft_atoll(__TIME__);
	rng = rng + frame * 1471343;
	ray->dir = ft_vec3d_lerp(diff_dir, spec_dir, \
	hit->obj->mat.smoothness * spec_bounce);
	ray->origin = ft_vec3d_add(hit->pos, ft_vec3d_mult(ray->dir, 0.0001f));
}

t_color_norm	rt_ray_loop(
	t_rt_renderer *renderer,
	t_rt_ray ray
)	{
	t_rt_hit			hit;
	t_color_norm		light;
	static long long	rng = 0;
	bool				sbounce;

	if (rng == 0)
		rng = ft_atoll(__TIME__);
	light = (t_color_norm){.a = 1., .r = 0., .g = 0., .b = 0.};
	while (ray.bounces++ < MAX_BOUNCES)
	{
		hit = (t_rt_hit){(t_vec3d){0.0, 0.0, 0.0}, NULL, false, INFINITY};
		rt_ray_cast(renderer->scene, &ray, &hit);
		if (hit.hit && hit.obj && hit.dist < 100.)
		{
			sbounce = hit.obj->mat.spe_prob >= (double) ft_random_value(&rng);
			rt_ray_render(renderer, ray, hit, rt_color_from_norm(hit.obj->mat.obj_color));
			rt_ray_colorize(&ray, &hit, &light, sbounce);
			rt_ray_update(renderer, &ray, &hit, sbounce);
		}
		else
			light = rt_color_add(light, rt_color_mult(ray.color, \
			rt_get_skybox(renderer, &ray)));
		if (!hit.hit || !hit.obj || hit.obj->mat.emi_strength >= 1.)
			break ;
	}
	return (light);
}
