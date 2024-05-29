/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt_colors.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 21:05:47 by rgramati          #+#    #+#             */
/*   Updated: 2024/05/28 17:43:55 by rgramati         ###   ########.fr       */
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

	result.a = (uint8_t) ft_fmin(color.a * 255, 255);
	result.r = (uint8_t) ft_fmin(color.r * 255, 255);
	result.g = (uint8_t) ft_fmin(color.g * 255, 255);
	result.b = (uint8_t) ft_fmin(color.b * 255, 255);
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

t_color_norm	rt_color_add(t_color_norm c1, t_color_norm c2)
{
	t_color_norm	result;

	result.a = 1.;
	result.r = ft_fmin(c1.r + c2.r, 1.);
	result.g = ft_fmin(c1.g + c2.g, 1.);
	result.b = ft_fmin(c1.b + c2.b, 1.);
	// result.r = c1.r + c2.r;
	// result.g = c1.g + c2.g;
	// result.b = c1.b + c2.b;
	return (result);
}

t_color_norm	rt_color_mult(t_color_norm c1, t_color_norm c2)
{
	t_color_norm	result;

	result.a = 1.;
	result.r = c1.r * c2.r;
	result.g = c1.g * c2.g;
	result.b = c1.b * c2.b;
	return (result);
}

t_color_norm	rt_color_fact(t_color_norm c1, double f)
{
	t_color_norm	result;

	result.a = 1.;
	result.r = ft_fmin(c1.r * f, 1.);
	result.g = ft_fmin(c1.g * f, 1.);
	result.b = ft_fmin(c1.b * f, 1.);
	// result.r = c1.r * f;
	// result.g = c1.g * f;
	// result.b = c1.b * f;
	return (result);
}

typedef struct s_vec3
{
    float    x;
    float    y;
    float    z;
    float    w;
} __attribute__((aligned(16))) t_vec3;

double	map(double start, double end, double min, double max, double val)
{
	return (start + (end - start) * ((val - min) / (max - min)));
}

// 0 -> 0xFF0000FF

//1000 -> 0xFFFF0000

// static unsigned int rast_get_color(t_vec3 center, t_vec3 point)
// {
// 	const float			distance = sqrtf(powf(point.x - center.x, 2.) + powf(point.y - center.y, 2.) + powf(point.z - center.z, 2.));
// 	const float			ratio = 255 * distance / 1000;
// 	const unsigned int	color = 0 + ((int)ratio << 16) + ((int)ratio);
	
// 	return (color);
// }

#include <ft/math.h>
#include <ft/math/matrix.h>

// int main(void)
// {
// 	t_mat3d	mat = ft_mat3d_rot(0. ,RT_PI / 2. ,0.);

// 	ft_printf("%4f %4f %4f\n%4f %4f %4f\n%4f %4f %4f\n\n", mat.m[0][0], mat.m[0][1], mat.m[0][2], mat.m[1][0], mat.m[1][1], mat.m[1][2], mat.m[2][0], mat.m[2][1], mat.m[2][2]);
// 	// ft_mat3d_mult(mat, ft_mat3d_rot_z(1.2));
// 	// ft_printf("%4f %4f %4f\n%4f %4f %4f\n%4f %4f %4f\n", mat.m[0][0], mat.m[0][1], mat.m[0][2], mat.m[1][0], mat.m[1][1], mat.m[1][2], mat.m[2][0], mat.m[2][1], mat.m[2][2]);
// 	// rast_get_color((t_vec3){0., 0., 0., 0.}, (t_vec3){456., 24., 0., 0.});
// }


// bool	rt_color_occlusion(t_rt_scene *scene, t_rt_hit hit, t_vec3d light_dir, t_vec3d norm)
// {
// 	t_rt_ray	s_ray;
// 	t_rt_hit	s_hit;

// 	s_ray = (t_rt_ray){.color = rt_color_from_norm(rt_color(0xFF000000)), .origin = hit.pos,
// 		.dir = light_dir, .bounces = 0};
// 	s_ray.origin = ft_vec3d_add(s_ray.origin, ft_vec3d_mult(norm, 0.1f));
// 	rt_ray_cast(scene, &s_ray, &s_hit);
// 	if (hit.obj == s_hit.obj)
// 		return (false);
// 	return (ft_vec3d_len(light_dir) - ft_vec3d_len(ft_vec3d_sub(hit.pos,
// 				s_hit.pos)) > 0.0f);
// }

static void	rt_color_diffuse(
	t_rt_scene *scene,
	t_rt_hit hit,
	t_vec3d norm,
	t_color_norm *c
) {
	t_rt_object		light;
	t_vec3d			light_dir;
	double			dratio;

	*c = rt_color_to_norm(rt_color(0xFF000000));
	light = (t_rt_object) scene->lights[0];
	light_dir = ft_vec3d_sub(light.pos, hit.pos);
	// if (rt_color_occlusion(scene, hit, light_dir, norm))
	// 	return ;
	light_dir = ft_vec3d_norm(light_dir);
	dratio = ft_vec3d_dot(norm, light_dir);
	if (dratio < 0.0f)
		dratio = 0.0f;
	dratio *= ((t_rt_obj_light *)light.options)->brightness;
	dratio = ft_fmin(1.0f, dratio);
	*c = light.mat.obj_color;
	*c = rt_color_mult(*c, (t_color_norm){1.0, dratio, dratio, dratio});
}

void	rt_color_ambient(t_rt_scene *scene, t_color_norm *c)
{
	t_color_norm	ambient;
	double			brightness;

	brightness = scene->ambient.mat.emi_strength;
	ambient = scene->ambient.mat.emi_color;
	// ft_printf("mon bon pixel la (%.2f %.2f %.2f) -> ", c->r, c->g, c->b);

	c->r = ft_fmin((ambient.r * brightness) + c->r, 1.0f);
	c->g = ft_fmin((ambient.g * brightness) + c->g, 1.0f);
	c->b = ft_fmin((ambient.b * brightness) + c->b, 1.0f);

	// ft_printf("(%.2f %.2f %.2f)\n", c->r, c->g, c->b);
}

t_vec3d	rt_color_specular(t_rt_ray ray, t_vec3d norm, t_vec3d light_dir)
{
	t_vec3d	reflect;

	reflect = ft_vec3d_norm(ft_vec3d_sub(ray.dir, ft_vec3d_mult(norm, 2.0 * ft_vec3d_dot(light_dir, norm))));
	// ft_printf("");
	return (reflect);
}

t_color	rt_obj_color(t_rt_scene *scene, t_rt_hit hit, t_rt_ray ray, t_vec3d norm)
{
	// t_vec3d			light_dir;
	t_color			result;
	t_color_norm	diff;
	t_color_norm	ambi;
	// t_vec3d			spec;

	(void) ray;	
	if (hit.obj->type != RT_OBJ_LIGHT)
		rt_color_diffuse(scene, hit, norm, &diff);
	rt_color_ambient(scene, &ambi);
	// light_dir = ft_vec3d_sub(scene->lights->pos, hit.pos);
	// spec = rt_color_specular(ray, norm, light_dir);
	// double specratio = ((t_rt_obj_light *)(scene->lights->options))->brightness * ft_vec3d_dot(ft_vec3d_mult(ray.dir, -1.0f), spec);
	// t_color_norm speccolor = rt_color_mult(rt_color_to_norm(scene->lights->color), (t_color_norm){1.0, specratio, specratio, specratio}, 0);
	// ft_printf("%6f %6f %6f\n", spec.r, spec.g, spec.b);
	// diff.a = ft_fmin(diff.a + ambi.a + spec.a, 1.0f);
	diff.r = ft_fmin(diff.r + ambi.r, 1.0f);
	diff.g = ft_fmin(diff.g + ambi.g, 1.0f);
	diff.b = ft_fmin(diff.b + ambi.b, 1.0f);

	result = rt_color_from_norm(rt_color_mult(diff, hit.obj->mat.obj_color));

	return (result);
}
