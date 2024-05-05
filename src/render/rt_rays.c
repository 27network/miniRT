/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt_rays.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 15:03:08 by rgramati          #+#    #+#             */
/*   Updated: 2024/05/05 18:17:32 by rgramati         ###   ########.fr       */
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
	size_t		ptdr = 0;
	t_rt_hit	closest;

	i = 0;
	hit->dist = INFINITY;
	while (i < scene->objects_size)
	{
		if (scene->objects[i].options && scene->objects[i].intersect(*ray, &scene->objects[i], &closest))
		{
			ft_printf("Testing intersection on object %d:\n", i);
			ft_printf("\n\tObject #%d::%p hit !\n", i, scene->objects[i]);
			hit->hit = true;
			if (closest.dist <= hit->dist)
			{
				hit->hit_object = &scene->objects[i];
				ptdr = i;
				hit->dist = closest.dist;
				hit->position = ft_vec3d_add(ray->origin, ft_vec3d_mult(ray->direction, hit->dist));
				ft_printf("\tUpdating closest hit: Object #%d::%p closest distance = %f, closest hit position {%6f, %6f, %6f}\n", i, hit->hit_object, hit->dist, hit->position.x, hit->position.y, hit->position.z);
			}
			ft_printf("\n");
		}
		i++;
	}
	if (hit->hit)
		ft_printf("FINAL hit : Object #%d::%p distance = %f\n", ptdr, hit->hit_object, hit->dist);
}

void	rt_ray_init(t_rt_scene *scene, t_rt_ray *ray, t_vec2i pixs)
{
	double	tmp;

	ray->color = rt_color(0xFFFFFFFF);
	ray->origin = scene->camera.position;
	// rat->hits = ft_calloc()
	tmp = 2.0 * tan(((t_rt_obj_camera *)scene->camera.options)->fov * 0.5 * RT_PI / 180);
	ray->direction.x = pixs.x + 0.5f - scene->width * 0.5f;
	ray->direction.y = -(pixs.y + 0.5f - scene->height * 0.5f);
	if (scene->width > scene->height)
		ray->direction.z = scene->width / tmp;
	else
		ray->direction.z = scene->height / tmp;
	ray->direction = ft_vec3d_norm(ray->direction);
}
