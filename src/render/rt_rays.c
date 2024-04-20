/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt_rays.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 15:03:08 by rgramati          #+#    #+#             */
/*   Updated: 2024/04/20 23:51:31 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>
#include <ft/print.h>
#include <ft/mem.h>
#include <ft/math.h>
#include <ft/math/vector.h>
#include <rt/renderer.h>
#include <rt/object/sphere.h>
#include <rt/object/camera.h>

bool	rt_obj_intersect(t_rt_ray ray, t_rt_object *obj, t_rt_hit *hit)
{
	// if (obj->type == RT_OBJ_BOX)
	// 	return (rt_obj_box_intersect(ray, obj, hit));
	if (obj->type == RT_OBJ_SPHERE)
		return (rt_obj_sphere_intersect(ray, obj, hit));
	return (false);
}

double	rt_dist(t_vec3d p1, t_vec3d p2)
{
	return (ft_fabs(ft_fpow(p2.x - p1.x, 2)
		+ ft_fpow(p2.y - p1.y, 2)
		+ ft_fpow(p2.z - p1.z, 2)));
}

t_color	rt_get_ray(t_rt_scene *scene, t_rt_ray ray)
{
	size_t		i;
	t_rt_hit	hit;
	t_rt_hit	closest;
	double		dist;

	i = 0;
	hit = (t_rt_hit) {(t_vec3d) {0.0f, 0.0f, 0.0f}, NULL};
	closest = (t_rt_hit) {(t_vec3d) {0.0f, 0.0f, 0.0f}, NULL};
	dist = INFINITY;
	while (i < scene->objects_size && !rt_obj_intersect(ray, &scene->objects[i], &hit))
	{
		if (!closest.hit_object || (hit.hit_object && rt_dist(hit.position, scene->camera.position) < dist))
		{
			dist = rt_dist(hit.position, scene->camera.position);
			closest.hit_object = hit.hit_object;
			closest.position = hit.position;
		}
		i++;
	}
	if (i == scene->objects_size || !closest.hit_object)
		return ((t_color){.argb = 0});
	return (closest.hit_object->color);
}

void	rt_init_ray(t_rt_scene *scene, t_rt_ray *ray, int x, int y)
{
	double	tmp;

	ray->bounces = 0;
	ray->color.argb = 0x00000000;
	ray->origin = scene->camera.position;
	ray->direction.x = x + 0.5f - scene->width * 0.5f;
	ray->direction.y = y + 0.5f - scene->height * 0.5f;
	tmp = 2.0 * tan((((t_rt_obj_camera *)scene->camera.options)->fov * RT_PI * 0.5) / 180);
	if (scene->width > scene->height)
		ray->direction.z = scene->height / tmp;
	else
		ray->direction.z = scene->width / tmp;
	ray->direction = ft_vec3d_norm(ray->direction);
	// phi = atan2(camrot.y, camrot.x);
	// theta = acos(camrot.z);
	// camrot.x = cos(theta) * camrot.x + sin(theta) * camrot.z;
	// camrot.z = cos(theta) * camrot.z - sin(theta) * camrot.x;
	// camrot.x = cos(theta) * camrot.x - sin(theta) * camrot.y;
	// camrot.y = sin(theta) * camrot.x + cos(theta) * camrot.y;
}
