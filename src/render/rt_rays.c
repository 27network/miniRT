/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt_rays.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 15:03:08 by rgramati          #+#    #+#             */
/*   Updated: 2024/05/12 16:05:34 by rgramati         ###   ########.fr       */
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
	const t_rt_obj_camera	*cam = (t_rt_obj_camera *)scene->camera.options;
	const t_vec3d			uv = (t_vec3d){tan(cam->fov * 0.5 * RT_PI / 180) * ((2. * pixs.x - WIDTH) / HEIGHT), tan(cam->fov * 0.5 * RT_PI / 180) * ((2. * pixs.y - HEIGHT) / HEIGHT), 1.};
	// const double			view_h = cam->near_clip * tan(cam->fov * 0.5 * RT_PI / 180) * 2.;
	// const t_vec3d			view_params = ft_vec3d(view_h, view_h * (WIDTH / HEIGHT), cam->near_clip);
	// const t_vec3d			view_point = ft_vec3d((uv.x - 0.5) * view_params.x, (uv.y - 0.5) * view_params.y, view_params.z);

	ray->origin = scene->camera.position;
	ray->direction = ft_vec3d_norm(ft_vec3d_add(uv, scene->camera.rotation));
	// phi = atan2(scene->camera.rotation.y, scene->camera.rotation.x);
	// theta = acos(scene->camera.rotation.z);
	// scene->camera.rotation.x = cos(theta) * scene->camera.rotation.x + sin(theta) * scene->camera.rotation.z;
	// scene->camera.rotation.z = cos(phi) * scene->camera.rotation.z - sin(phi) * scene->camera.rotation.x;
	// scene->camera.rotation.x = cos(theta) * scene->camera.rotation.x - sin(theta) * scene->camera.rotation.y;
	// scene->camera.rotation.y = sin(theta) * scene->camera.rotation.x + cos(theta) * scene->camera.rotation.y;
}
