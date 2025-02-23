/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt_objects_init.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 19:21:15 by rgramati          #+#    #+#             */
/*   Updated: 2024/04/30 14:31:34 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft/mem.h>
#include <rt/object/camera.h>
#include <rt/object/cone.h>
#include <rt/object/quad.h>
#include <rt/object/cylinder.h>
#include <rt/object/light.h>
#include <rt/object/plane.h>
#include <rt/object/sphere.h>

t_rt_obj_cylinder	*rt_obj_cylinder_init(
	double diameter,
	double height,
	t_vec3d norm
) {
	t_rt_obj_cylinder	*cylinder;

	cylinder = ft_calloc(1, sizeof(t_rt_obj_cylinder));
	if (!cylinder)
		return (NULL);
	cylinder->diameter = diameter;
	cylinder->height = height;
	cylinder->norm = norm;
	cylinder->last_hit_on_edge = false;
	return (cylinder);
}

t_rt_obj_camera	*rt_obj_camera_init(
	char *name,
	int fov
) {
	t_rt_obj_camera	*camera;

	camera = ft_calloc(1, sizeof(t_rt_obj_camera));
	if (!camera)
		return (NULL);
	camera->name = name;
	camera->fov = fov;
	return (camera);
}

t_rt_obj_light	*rt_obj_light_init(
	double brightness
) {
	t_rt_obj_light	*light;

	light = ft_calloc(1, sizeof(t_rt_obj_light));
	if (!light)
		return (NULL);
	light->brightness = brightness;
	return (light);
}

t_rt_obj_plane	*rt_obj_plane_init(
	t_vec3d norm
) {
	t_rt_obj_plane	*plane;

	plane = ft_calloc(1, sizeof(t_rt_obj_plane));
	if (!plane)
		return (NULL);
	plane->norm = ft_vec3d_norm(norm);
	return (plane);
}

t_rt_obj_sphere	*rt_obj_sphere_init(
	double diameter
) {
	t_rt_obj_sphere	*sphere;

	sphere = ft_calloc(1, sizeof(t_rt_obj_sphere));
	if (!sphere)
		return (NULL);
	sphere->diameter = diameter;
	return (sphere);
}

t_rt_obj_cone	*rt_obj_cone_init(
	double diameter,
	double height,
	t_vec3d norm,
	double theta
) {
	t_rt_obj_cone	*cone;

	cone = ft_calloc(1, sizeof(t_rt_obj_cone));
	if (!cone)
		return (NULL);
	cone->diameter = diameter;
	cone->height = height;
	cone->norm = norm;
	cone->theta = theta;
	cone->last_hit_on_edge = false;
	return (cone);
}

t_rt_obj_quad	*rt_obj_quad_init(
	t_vec3d norm,
	t_vec3d size
) {
	t_rt_obj_quad	*quad;

	quad = ft_calloc(1, sizeof(t_rt_obj_quad));
	if (!quad)
		return (NULL);
	quad->norm = ft_vec3d_norm(norm);
	quad->size = size;
	return (quad);
}
