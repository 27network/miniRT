/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt_rays.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 15:03:08 by rgramati          #+#    #+#             */
/*   Updated: 2024/05/10 15:24:29 by rgramati         ###   ########.fr       */
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

void	rt_ray_init(t_rt_scene *scene, t_rt_ray *ray, t_vec2i pixs)
{
	t_vec3d	uv;
	double  phi;
	double  theta;

	uv = (t_vec3d){(2. * pixs.x - WIDTH) / HEIGHT, (2. * pixs.y - HEIGHT) / HEIGHT, 0.};
	uv = ft_vec3d_mult(uv, .5);
	uv.z = 1.;
	ray->origin = scene->camera.position;
	ray->direction = ft_vec3d_norm(ft_vec3d_add(uv, scene->camera.rotation));
	phi = atan2(scene->camera.rotation.y, scene->camera.rotation.x);
	theta = acos(scene->camera.rotation.z);
	scene->camera.rotation.x = cos(theta) * scene->camera.rotation.x + sin(theta) * scene->camera.rotation.z;
	scene->camera.rotation.z = cos(theta) * scene->camera.rotation.z - sin(theta) * scene->camera.rotation.x;
	scene->camera.rotation.x = cos(theta) * scene->camera.rotation.x - sin(theta) * scene->camera.rotation.y;
	scene->camera.rotation.y = sin(theta) * scene->camera.rotation.x + cos(theta) * scene->camera.rotation.y;
}
