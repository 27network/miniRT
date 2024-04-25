/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt_colors.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 21:05:47 by rgramati          #+#    #+#             */
/*   Updated: 2024/04/24 17:50:05 by rgramati         ###   ########.fr       */
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
	t_rt_object		light;
	t_vec3d			light_dir;
	t_color			dcolor;
	double			dratio;
	t_color_norm	result;

	light = (t_rt_object) scene->lights[0];
	light_dir = ft_vec3d_norm(ft_vec3d_sub(light.position, hit.position));
	dratio = ft_vec3d_dot(norm, light_dir);
	if (dratio < 0.0f)
		dratio = 0.0f;
	dratio *= ((t_rt_obj_light *)light.options)->brightness;
	dratio = ft_fmin(1.0f, dratio);
	dcolor = light.color;
	result = rt_color_to_norm(dcolor);
	result = rt_color_mult(result, (t_color_norm){dratio, dratio, dratio, dratio}, 0);
	*c = rt_color_from_norm(result);
}

void	rt_color_ambient(t_rt_scene *scene, t_rt_hit hit, t_color *c, bool printshit)
{
	(void) scene;
	(void) hit;
	(void) c;

	t_color_norm	current;
	t_color_norm	obj_color;
	t_color_norm	ambient;
	t_color			color;
	double			brightness;

	brightness = ((t_rt_obj_light *)scene->ambient.options)->brightness;
	current = rt_color_to_norm(*c);
	ambient = rt_color_to_norm(scene->ambient.color);
	current.a = ft_fmin((ambient.a * brightness) + current.a, 1.0f);
	current.r = ft_fmin((ambient.r * brightness) + current.r, 1.0f);
	current.g = ft_fmin((ambient.g * brightness) + current.g, 1.0f);
	current.b = ft_fmin((ambient.b * brightness) + current.b, 1.0f);
	color = hit.hit_object->color;
	if (printshit)
		ft_printf("color from %p: %d %d %d\n", hit.hit_object, color.r, color.g, color.b);
	obj_color = rt_color_to_norm(color);
	*c = rt_color_from_norm(rt_color_mult(current, obj_color, scene->rt_flags & RT_COL_GAMMA));
}


void	rt_color_specular(t_color *c)
{
	(void) c;
}

t_color	rt_obj_color_debug(t_rt_scene *scene, t_rt_hit hit, t_vec3d norm)
{
	t_color	result;

	(void) norm;
	(void) scene;
	// if (hit.hit_object->type != RT_OBJ_LIGHT)
	rt_color_diffuse(scene, hit, norm, &result);
	// rt_color_specular(&result);
	rt_color_ambient(scene, hit, &result, true);
	return (result);
}

t_color	rt_obj_color(t_rt_scene *scene, t_rt_hit hit, t_vec3d norm)
{
	t_color	result;

	(void) norm;
	(void) scene;
	// if (hit.hit_object->type != RT_OBJ_LIGHT)
	rt_color_diffuse(scene, hit, norm, &result);
	// rt_color_specular(&result);
	rt_color_ambient(scene, hit, &result, false);
	return (result);
}