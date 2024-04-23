/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt_colors.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 21:05:47 by rgramati          #+#    #+#             */
/*   Updated: 2024/04/23 18:50:37 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>
#include <ft/math.h>
#include <rt/renderer.h>
#include <rt/object/light.h>
#include <ft/print.h>

t_color	rt_color(uint32_t argb)
{
	t_color	result;

	result.b = argb & 255;
	argb >>= 8;
	result.g = argb & 255;
	argb >>= 8;
	result.r = argb & 255;
	argb >>= 8;
	result.a = argb & 255;
	return (result);
}

uint32_t	rt_color_argb(t_color color)
{
	uint32_t	result;

	result = color.a;
	result <<= 8;
	result += color.r;
	result <<= 8;
	result += color.g;
	result <<= 8;
	result += color.b;
	return (result);
}

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

	result.a = (double)color.a / 255.0f;
	result.r = (double)color.r / 255.0f;
	result.g = (double)color.g / 255.0f;
	result.b = (double)color.b / 255.0f;
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
		result.a = sqrt(result.a);
		result.r = sqrt(result.r);
		result.g = sqrt(result.g);
		result.b = sqrt(result.b);
	}
	return (result);
}

void	rt_color_diffuse(t_rt_scene *scene, t_rt_hit hit, t_vec3d norm, t_color *c)
{
	t_color		dcolor;
	t_rt_object	light;
	t_vec3d		light_dir;
	double		dratio;

	light = (t_rt_object) scene->lights[0];
	light_dir = ft_vec3d_norm(ft_vec3d_sub(light.position, hit.position));
	dratio = ft_vec3d_dot(norm, light_dir);
	if (dratio < 0.0f)
		dratio = 0.0f;
	dratio *= ((t_rt_obj_light *)light.options)->brightness;
	dcolor.a = 255;
	dcolor.r = ((uint8_t) (dratio * light.color.r));
	dcolor.g = ((uint8_t) (dratio * light.color.g));
	dcolor.b = ((uint8_t) (dratio * light.color.b));
	*c = dcolor;
}

void	rt_color_ambient(t_rt_scene *scene, t_rt_hit hit, t_color *c)
{
	(void) scene;
	(void) hit;
	(void) c;

	t_color_norm	current;
	t_color_norm	obj_color;
	
	current = rt_color_to_norm(*c);
	obj_color = rt_color_to_norm(hit.hit_object->color);
	*c = rt_color_from_norm(rt_color_mult(current, obj_color, scene->rt_flags & RT_COL_GAMMA));
}


void	rt_color_specular(t_color *c)
{
	(void) c;
}

t_color	rt_obj_color(t_rt_scene *scene, t_rt_hit hit, t_vec3d norm)
{
	t_color	result;

	(void) norm;
	(void) scene;
	rt_color_diffuse(scene, hit, norm, &result);
	// rt_color_specular(&result);
	rt_color_ambient(scene, hit, &result);
	return (result);
}