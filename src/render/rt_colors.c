/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt_colors.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 21:05:47 by rgramati          #+#    #+#             */
/*   Updated: 2024/04/23 13:18:18 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>
#include <ft/math.h>
#include <rt/renderer.h>
#include <rt/object/light.h>
#include <ft/print.h>

t_color	rt_color_add(t_color c1, t_color c2)
{
	t_color	result;

	result.argb = 0xFF000000;
	result.argb += ft_min(c1.r + c2.r, 255) << 16;
	result.argb += ft_min(c1.g + c2.g, 255) << 8;
	result.argb += ft_min(c1.b + c2.b, 255);
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
	dcolor.argb = 0x00000000;
	dcolor.argb += ((uint8_t) (dratio * light.color.a)) << 24;
	dcolor.argb += ((uint8_t) (dratio * light.color.r)) << 16;
	dcolor.argb += ((uint8_t) (dratio * light.color.g)) << 8;
	dcolor.argb += ((uint8_t) (dratio * light.color.b));
	*c = dcolor;
}

void	rt_color_ambient(t_rt_scene *scene, t_rt_hit hit, t_color *c)
{
	double	t;
	t_color	amb;

	t = ((t_rt_obj_light *)scene->ambient.options)->brightness;
	amb.argb = 0x00000000;
	amb.argb += ((uint8_t) ((1.0 - t) * hit.hit_object->color.a)) << 24;
	amb.argb += ((uint8_t) ((1.0 - t) * hit.hit_object->color.r)) << 16;
	amb.argb += ((uint8_t) ((1.0 - t) * hit.hit_object->color.g)) << 8;
	amb.argb += ((uint8_t) ((1.0 - t) * hit.hit_object->color.b));
	amb.argb += ((uint8_t) (t * scene->ambient.color.a)) << 24;
	amb.argb += ((uint8_t) (t * scene->ambient.color.r)) << 16;
	amb.argb += ((uint8_t) (t * scene->ambient.color.g)) << 8;
	amb.argb += ((uint8_t) (t * scene->ambient.color.b));
	*c = rt_color_add(*c, amb);
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
	// rt_color_ambient(scene, hit, &result);
	rt_color_specular(&result);
	return (result);
}