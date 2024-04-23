/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt_rays.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 15:03:08 by rgramati          #+#    #+#             */
/*   Updated: 2024/04/22 22:23:56 by rgramati         ###   ########.fr       */
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
	size_t	i;

	i = 0;
	while (i < scene->objects_size)
	{
		if (scene->objects[i].intersect(*ray, &scene->objects[i], hit))
			hit->hit = true;
		i++;
	}
}

void	rt_ray_init(t_rt_scene *scene, t_rt_ray *ray, t_vec2i pixs)
{
	double	tmp;

	ray->bounces = 0;
	ray->color.argb = 0xFFFFFFFF;
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
