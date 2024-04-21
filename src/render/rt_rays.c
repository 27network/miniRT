/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt_rays.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 15:03:08 by rgramati          #+#    #+#             */
/*   Updated: 2024/04/21 21:34:26 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>
#include <ft/print.h>
#include <ft/mem.h>
#include <ft/math.h>
#include <ft/math/vector.h>
#include <rt/renderer.h>
#include <rt/object/sphere.h>
#include <rt/object/light.h>
#include <rt/object/camera.h>

double	rt_dist(t_vec3d p1, t_vec3d p2)
{
	return (ft_fabs(ft_fpow(p2.x - p1.x, 2)
		+ ft_fpow(p2.y - p1.y, 2)
		+ ft_fpow(p2.z - p1.z, 2)));
}

int	mapx(t_vec3d cam, double x, int start, int end)
{
	return (((x + 7.1f - cam.x) / 14.2f) * (end - start));
}

int	mapy(t_vec3d cam, double y, int start, int end)
{
	return (((8.0f - (y - cam.z)) / 8.0f) * (end - start));
}

void	*rt_getmlx(int what, t_rt_mlx_data *data)
{
	static t_rt_mlx_data	*mlx;

	if (data || !mlx)
		mlx = data;
	if (what == 0b0001)
		return mlx->rt_mlx;
	else if (what == 0b0010)
		return mlx->rt_win;
	else if (what == 0b0100)
		return mlx->rt_imgs[0];
	else if (what == 0b1000)
		return mlx->rt_imgs[1];
	return ((void *) mlx);
}

void	rt_trace_line(t_rt_scene *scene, t_vec2i start, t_vec2i end, t_color color)
{
	t_vec2i	deltas;
	t_vec3d	inc;
	t_vec3d	pos;
	double	steps;
	double	i;

	deltas = (t_vec2i){end.x - start.x, end.y - start.y};
	if (deltas.x > deltas.y)
		steps = deltas.x;
	else
		steps = deltas.y;
	inc.x = (double) deltas.x / steps;
	inc.y = (double) deltas.y / steps;
	i = 0.0f;
	pos = (t_vec3d){start.x, start.y, 0.0f};
	while (i < steps)
	{
		if (pos.x >= 0 && pos.x < scene->width && pos.y >= 0 && pos.y < scene->height && color.argb)
			mlx_set_image_pixel(rt_getmlx(0b0001, NULL), rt_getmlx(0b1000, NULL), pos.x, pos.y, color.argb);
		pos.x += inc.x;
		pos.y += inc.y;
		i += 1.0f;
	}
}

void	rt_render_ray(t_rt_scene *scene, t_rt_ray ray, t_rt_hit hit)
{
	t_vec2i	start;
	t_vec2i end;
	t_color	color;

	start = (t_vec2i){scene->width / 2, scene->height};
	color.argb = 0x00000000;
	if (hit.hit)
	{
		end = (t_vec2i){.x = mapx(scene->camera.position, hit.position.x, 0, scene->width),
						.y = mapy(scene->camera.position, hit.position.z, 0, scene->height)};
		color = hit.hit_object->color;
	}
	else
	{
		end = (t_vec2i){.x = mapx(scene->camera.position, (8.0f / ray.direction.x) * ray.direction.y, 0, scene->width - 1),
						.y = 0};
	}
	rt_trace_line(scene, end, start, color);
}

t_color	rt_check_lights(t_rt_scene *scene, t_rt_hit hit, t_vec3d norm, t_color amb)
{
	t_color		dcolor;
	t_rt_object	*light;
	t_vec3d		light_dir;
	double		dratio;

	light = (t_rt_object *) scene->lights;
	light_dir = ft_vec3d_sub(light->position, hit.position);
	dratio = ft_vec3d_dot(norm, light_dir) / RT_PI;
	if (dratio < 0.0f)
		dratio = 0.0f;
	dratio *= ((t_rt_obj_light *)light->options)->brightness;
	dcolor = hit.hit_object->color;
	dcolor.a = (uint8_t) (dratio * (dcolor.a * 0.75f + light->color.a * 0.25f));
	dcolor.r = (uint8_t) (dratio * (dcolor.r * 0.75f + light->color.r * 0.25f));
	dcolor.g = (uint8_t) (dratio * (dcolor.g * 0.75f + light->color.g * 0.25f));
	dcolor.b = (uint8_t) (dratio * (dcolor.b * 0.75f + light->color.b * 0.25f));
	return ((t_color){.a = (dcolor.a * 0.8f + amb.a * 0.2f),
		.r = (dcolor.r * 0.80f + amb.r * 0.2f),
		.g = (dcolor.g * 0.80f + amb.g * 0.2f),
		.b = (dcolor.b * 0.80f + amb.b * 0.2f)});
}

t_color	rt_get_ray(t_rt_scene *scene, t_rt_ray ray)
{
	size_t		i;
	t_rt_hit	hit;
	t_rt_hit	closest;
	t_color		acolor;
	double		dist;

	i = 0;
	hit = (t_rt_hit) {(t_vec3d) {0.0f, 0.0f, 0.0f}, NULL, false};
	closest = (t_rt_hit) {(t_vec3d) {0.0f, 0.0f, 0.0f}, NULL, false};
	dist = INFINITY;
	while (i < scene->objects_size)
	{
		if (scene->objects[i].intersect(ray, &scene->objects[i], &hit))
		{
			hit.hit = true;
			if (!closest.hit_object || (hit.hit_object && rt_dist(hit.position, scene->camera.position) < dist))
			{
				dist = rt_dist(hit.position, scene->camera.position);
				closest = hit;
			}
		}
		i++;
	}
	if (scene->rt_flags & RT_RAY_DEBUG)
		rt_render_ray(scene, ray, hit);
	acolor = (t_color){.argb = 0xFF424242};
	if (!closest.hit_object)
		return (ray.color);

	t_vec3d		inter = (t_vec3d) {0.0f, 0.0f, 0.0f};
	if (closest.hit_object->type == RT_OBJ_SPHERE)
	{
		inter = ft_vec3d_sub(closest.position, closest.hit_object->position);
		if (ft_vec3d_dot(ray.direction, inter) > 0.0f)
			inter = (t_vec3d) {-inter.x, -inter.y, -inter.z};
	}
	return (rt_check_lights(scene, hit, inter, acolor));
}

void	rt_init_ray(t_rt_scene *scene, t_rt_ray *ray, t_vec2i pixs, t_vec3d start)
{
	double	tmp;

	ray->bounces = 0;
	ray->color.argb = 0x00000000;
	ray->origin = start;
	ray->direction.x = pixs.x + 0.5f - scene->width * 0.5f;
	ray->direction.y = -(pixs.y + 0.5f - scene->height * 0.5f);
	tmp = 2.0 * tan((((t_rt_obj_camera *)scene->camera.options)->fov * RT_PI * 0.5) / 180);
	if (scene->width > scene->height)
		ray->direction.z = scene->width / tmp;
	else
		ray->direction.z = scene->height / tmp;
	ray->direction = ft_vec3d_norm(ray->direction);
}
