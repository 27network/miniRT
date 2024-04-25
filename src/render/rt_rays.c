/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt_rays.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 15:03:08 by rgramati          #+#    #+#             */
/*   Updated: 2024/04/24 18:22:06 by rgramati         ###   ########.fr       */
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

void	rt_ray_cast(t_rt_scene *scene, t_rt_ray *ray, t_rt_hit *hit)
{
	size_t		i;
	t_rt_hit	closest;

	i = 0;
	hit->dist = INFINITY;
	while (i < scene->objects_size)
	{
		if (scene->objects[i].options && scene->objects[i].intersect(*ray, &scene->objects[i], &closest))
		{
			hit->hit = true;
			if (closest.dist < hit->dist)
			{
				hit->hit_object = &scene->objects[i];
				hit->dist = closest.dist;
				hit->position = ft_vec3d_add(ray->origin, ft_vec3d_mult(ray->direction, closest.dist));
			}
		}
		i++;
	}
}

void	rt_ray_cast_debug(t_rt_scene *scene, t_rt_ray *ray, t_rt_hit *hit)
{
	size_t		i;
	t_color		color;
	t_rt_hit	closest;

	i = 0;
	size_t ptdr = 0;
	hit->dist = INFINITY;
	ft_printf("hit at %p\n", hit);
	while (i < scene->objects_size)
	{
		if (scene->objects[i].options && scene->objects[i].intersect(*ray, &scene->objects[i], &closest))
		{
			hit->hit = true;
			if (closest.dist <= hit->dist)
			{
				hit->hit_object = &scene->objects[i];
				ptdr = i;
				hit->dist = closest.dist;
				hit->position = ft_vec3d_add(ray->origin, ft_vec3d_mult(ray->direction, hit->dist));
			}
			color = rt_obj_color(scene, *hit, hit->hit_object->norm(*ray, *hit));
			// ft_printf("rt_obj_color(%p, %p, (%p)->norm(%p, %p));\n", scene, hit, hit->hit_object, ray, hit);
			// ft_printf("\t%p[%2d] - closest at : {%6f, %6f, %6f} distance = %10f :: color = %X\n\n", scene->objects + i, i, hit->position.x, hit->position.y, hit->position.z, hit->dist, rt_color_argb(color));
		}
		i++;
	}
	if (hit->hit)
	{
		ft_printf("Final hit at %f on object #%d = %p\n", hit->dist, ptdr, hit->hit_object);
		color = rt_obj_color(scene, *hit, hit->hit_object->norm(*ray, *hit));
		// ft_printf("rt_obj_color(%p, %p, (%p)->norm(%p, %p));\n", scene, hit, hit->hit_object, ray, hit);
		// ft_printf("%p {OBJECT:%p} FINAL COLOR = %X :: %3d %3d %3d\n", hit, hit->hit_object, rt_color_argb(color), color.r, color.g, color.b);
	}
}

void	rt_ray_init(t_rt_scene *scene, t_rt_ray *ray, t_vec2i pixs)
{
	double	tmp;

	ray->bounces = 0;
	ray->color = rt_color(0xFFFFFFFF);
	ray->origin = scene->camera.position;
	ray->direction.x = pixs.x + 0.5f - scene->width * 0.5f;
	ray->direction.y = -(pixs.y + 0.5f - scene->height * 0.5f);
	tmp = 2.0 * tan((((t_rt_obj_camera *)scene->camera.options)->fov * RT_PI * 0.5) / 180);
	if (scene->width > scene->height)
		ray->direction.z = scene->width / tmp;
	else
		ray->direction.z = scene->height / tmp;
	ray->direction = ft_vec3d_norm(ray->direction);
}
