/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt_colors.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 21:05:47 by rgramati          #+#    #+#             */
/*   Updated: 2024/04/27 16:40:41 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>
#include <ft/math.h>
#include <rt/renderer.h>
#include <rt/object/light.h>
#include <ft/print.h>

t_color	rt_color_from_norm(t_color_norm color)
{
	t_color	result;

	result.a = color.a * 255;
	result.r = color.r * 255;
	result.g = color.g * 255;
	result.b = color.b * 255;
	return (result);
}

t_color_norm	rt_color_to_norm(t_color color)
{
	t_color_norm	result;

	result.a = (double)color.a / 255.0;
	result.r = (double)color.r / 255.0;
	result.g = (double)color.g / 255.0;
	result.b = (double)color.b / 255.0;
	return (result);
}

t_color_norm	rt_color_mult(t_color_norm c1, t_color_norm c2, bool gamma)
{
	t_color_norm	result;

	result.a = c1.a * c2.a;
	result.r = c1.r * c2.r;
	result.g = c1.g * c2.g;
	result.b = c1.b * c2.b;
	if (gamma)
	{
		result.r = sqrt(result.r);
		result.g = sqrt(result.g);
		result.b = sqrt(result.b);
	}
	return (result);
}

bool	rt_color_occlusion(t_rt_scene *scene, t_rt_hit hit, t_vec3d light_dir, t_vec3d norm)
{
	t_rt_ray	s_ray;
	t_rt_hit	s_hit;
	size_t		i;

	i = 0;
	s_ray = (t_rt_ray){.color = rt_color(0xFF000000), .origin = hit.position,
		.direction = light_dir, .bounces = 0};
	s_ray.origin = ft_vec3d_add(s_ray.origin, ft_vec3d_mult(norm, 0.1f));
	rt_ray_cast(scene, &s_ray, &s_hit);
	if (hit.hit_object == s_hit.hit_object)
		return (false);
	return (ft_vec3d_len(light_dir) - ft_vec3d_len(ft_vec3d_sub(hit.position,
				s_hit.position)) > 0.0f);
}

static void	rt_color_diffuse(
	t_rt_scene *scene,
	t_rt_hit hit,
	t_vec3d norm,
	t_color_norm *c
) {
	t_rt_object		light;
	t_vec3d			light_dir;
	t_color			dcolor;
	double			dratio;

	*c = rt_color_to_norm(rt_color(0xFF000000));
	light = (t_rt_object) scene->lights[0];
	light_dir = ft_vec3d_sub(light.position, hit.position);
	if (rt_color_occlusion(scene, hit, light_dir, norm))
		return ;
	light_dir = ft_vec3d_norm(light_dir);
	dratio = ft_vec3d_dot(norm, light_dir);
	if (dratio < 0.0f)
		dratio = 0.0f;
	dratio *= ((t_rt_obj_light *)light.options)->brightness;
	dratio = ft_fmin(1.0f, dratio);
	dcolor = light.color;
	*c = rt_color_to_norm(dcolor);
	*c = rt_color_mult(*c, (t_color_norm){1.0, dratio, dratio,
			dratio}, 0);
}

static void	rt_color_ambient(t_rt_scene *scene, t_rt_hit hit, t_color_norm *c)
{
	t_color_norm	ambient;
	double			brightness;

	(void) hit;
	brightness = ((t_rt_obj_light *)scene->ambient.options)->brightness;
	*c = rt_color_to_norm(rt_color(0xFF000000));
	ambient = rt_color_to_norm(scene->ambient.color);
	c->a = ft_fmin((ambient.a * brightness) + c->a, 1.0f);
	c->r = ft_fmin((ambient.r * brightness) + c->r, 1.0f);
	c->g = ft_fmin((ambient.g * brightness) + c->g, 1.0f);
	c->b = ft_fmin((ambient.b * brightness) + c->b, 1.0f);
}

t_vec3d	rt_color_specular(t_rt_ray ray, t_vec3d norm, t_vec3d light_dir)
{
	t_vec3d	reflect;

	reflect = ft_vec3d_norm(ft_vec3d_sub(ray.direction, ft_vec3d_mult(norm, 2.0 * ft_vec3d_dot(light_dir, norm))));
	// ft_printf("");
	return (reflect);
}

t_color	rt_obj_color(t_rt_scene *scene, t_rt_hit hit, t_rt_ray ray, t_vec3d norm)
{
	t_vec3d			light_dir;
	t_color			result;
	t_color_norm	diff;
	t_color_norm	ambi;
	// t_vec3d			spec;

	(void) ray;	
	if (hit.hit_object->type != RT_OBJ_LIGHT)
		rt_color_diffuse(scene, hit, norm, &diff);
	rt_color_ambient(scene, hit, &ambi);
	light_dir = ft_vec3d_sub(scene->lights->position, hit.position);
	// spec = rt_color_specular(ray, norm, light_dir);
	// double specratio = ((t_rt_obj_light *)(scene->lights->options))->brightness * ft_vec3d_dot(ft_vec3d_mult(ray.direction, -1.0f), spec);
	// t_color_norm speccolor = rt_color_mult(rt_color_to_norm(scene->lights->color), (t_color_norm){1.0, specratio, specratio, specratio}, 0);
	// ft_printf("%6f %6f %6f\n", spec.r, spec.g, spec.b);
	// diff.a = ft_fmin(diff.a + ambi.a + spec.a, 1.0f);
	diff.r = ft_fmin(diff.r + ambi.r, 1.0f);
	diff.g = ft_fmin(diff.g + ambi.g, 1.0f);
	diff.b = ft_fmin(diff.b + ambi.b, 1.0f);

	result = rt_color_from_norm(rt_color_mult(diff, rt_color_to_norm(hit.hit_object->color), scene->rt_flags & RT_COL_GAMMA));

	return (result);
}
