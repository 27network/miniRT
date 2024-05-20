/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt_objects_init.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 19:21:15 by rgramati          #+#    #+#             */
/*   Updated: 2024/05/19 17:41:55 by rgramati         ###   ########.fr       */
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
)	{
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

#include <ft/print.h>

t_vec3d	rt_obj_camera_angulation(t_vec3d look)
{
	// Chopper les angles a partir du vecteur directeur de la cam.
	const double	alpha = -acos(look.z / sqrt(pow(look.x, 2.) + pow(look.z, 2.)));
	const double	beta = atan2(look.y, look.x);
	// (void)look;

	// const double alpha = 0;
	// const double beta = RT_PI;
	ft_printf("a = %6f  b = %6f\n", alpha, beta);
	return (ft_vec3d(beta, alpha, 0.));
}

t_rt_obj_camera	*rt_obj_camera_init(
	char *name,
	int fov,
	t_vec3d look
)	{
	t_rt_obj_camera	*camera;

	(void) look;
	camera = ft_calloc(1, sizeof(t_rt_obj_camera));
	if (!camera)
		return (NULL);
	camera->name = name;
	camera->fov = fov;
	camera->angle = rt_obj_camera_angulation(look);
	return (camera);
}

t_rt_obj_light	*rt_obj_light_init(
	double brightness
)	{
	t_rt_obj_light	*light;

	light = ft_calloc(1, sizeof(t_rt_obj_light));
	if (!light)
		return (NULL);
	light->brightness = brightness;
	return (light);
}

t_rt_obj_plane	*rt_obj_plane_init(
	t_vec3d norm
)	{
	t_rt_obj_plane	*plane;

	plane = ft_calloc(1, sizeof(t_rt_obj_plane));
	if (!plane)
		return (NULL);
	plane->norm = ft_vec3d_norm(norm);
	return (plane);
}

t_rt_obj_sphere	*rt_obj_sphere_init(
	double diameter
)	{
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
)	{
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
	t_vec3d wvect,
	t_vec3d hvect
)	{
	t_rt_obj_quad	*quad;

	quad = ft_calloc(1, sizeof(t_rt_obj_quad));
	if (!quad)
		return (NULL);
	quad->norm = ft_vec3d_norm(norm);
	quad->wvect = wvect;
	quad->hvect = hvect;
	return (quad);
}
