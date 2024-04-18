/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt_objects_init.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 19:21:15 by rgramati          #+#    #+#             */
/*   Updated: 2024/04/18 19:30:10 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft/mem.h>
#include <rt/object/box.h>
#include <rt/object/camera.h>
#include <rt/object/light.h>
#include <rt/object/plane.h>
#include <rt/object/sphere.h>

t_rt_obj_box	*rt_obj_box_init(double sx, double sy, double sz)
{
	t_rt_obj_box	*box;

	box = ft_calloc(1, sizeof(t_rt_obj_box));
	if (!box)
		return (NULL);
	box->size = (t_vec3d) {sx, sy, sz};
	return (box);
}

t_rt_obj_camera	*rt_obj_camera_init(char *name)
{
	t_rt_obj_camera	*camera;

	camera = ft_calloc(1, sizeof(t_rt_obj_camera));
	if (!camera)
		return (NULL);
	camera->name = name;
	return (camera);
}

t_rt_obj_light	*rt_obj_light_init(double brightness)
{
	t_rt_obj_light	*light;

	light = ft_calloc(1, sizeof(t_rt_obj_light));
	if (!light)
		return (NULL);
	light->brightness = brightness;
	return (light);
}

t_rt_obj_plane	*rt_obj_plane_init(double sx, double sy)
{
	t_rt_obj_plane	*plane;

	plane = ft_calloc(1, sizeof(t_rt_obj_plane));
	if (!plane)
		return (NULL);
	plane->size = (t_vec3d) {sx, sy, 0.0f};
	return (plane);
}

t_rt_obj_sphere	*rt_obj_sphere_init(double diameter)
{
	t_rt_obj_sphere	*sphere;

	sphere = ft_calloc(1, sizeof(t_rt_obj_sphere));
	if (!sphere)
		return (NULL);
	sphere->diameter = diameter;
	return (sphere);
}
