/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt_objects_init.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 19:21:15 by rgramati          #+#    #+#             */
/*   Updated: 2024/05/28 18:30:58 by rgramati         ###   ########.fr       */
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
	t_vec3d norm,
	t_vec3d rot
)	{
	t_rt_obj_cylinder	*cylinder;

	cylinder = ft_calloc(1, sizeof(t_rt_obj_cylinder));
	if (!cylinder)
		return (NULL);
	cylinder->diameter = diameter;
	cylinder->height = height;
	cylinder->norm = ft_vec3d_norm(ft_vec3d_mult_mat3d(norm, ft_mat3d_rot(rot.x, rot.y, rot.z)));
	cylinder->last_hit_on_edge = false;
	return (cylinder);
}

t_vec3d	rt_obj_camera_angulation(t_vec3d look)
{
	const double	alpha = -acos(look.z / sqrt(pow(look.x, 2.) + pow(look.z, 2.)));
	const double	beta = atan2(look.y, look.x);

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

t_rt_obj_quad	*rt_obj_cube_init(
	t_vec3d xvect, // (1, 0, 0)
	t_vec3d yvect, // (0, 1, 0)
	t_vec3d zvect  // (0, 0, 1)
)	{
	t_rt_obj_quad	*cube;

	cube = ft_calloc(6, sizeof(t_rt_obj_quad));
	if (!cube)
		return (NULL);
	cube[0].norm = ft_vec3d_norm(xvect);;
	cube[0].wvect = zvect;
	cube[0].hvect = yvect;
	cube[1].norm = ft_vec3d_norm(yvect);
	cube[1].wvect = zvect;
	cube[1].hvect = xvect;
	cube[2].norm = ft_vec3d_norm(zvect);
	cube[2].wvect = xvect;
	cube[2].hvect = yvect;
	
	cube[3].norm = ft_vec3d_norm(xvect);
	cube[3].wvect = ft_vec3d_mult(zvect, -1.);
	cube[3].hvect = ft_vec3d_mult(yvect, -1.);
	cube[4].norm = ft_vec3d_norm(yvect);
	cube[4].wvect = ft_vec3d_mult(zvect, -1.);
	cube[4].hvect = ft_vec3d_mult(xvect, -1.);
	cube[5].norm = ft_vec3d_norm(zvect);
	cube[5].wvect = ft_vec3d_mult(xvect, -1.);
	cube[5].hvect = ft_vec3d_mult(yvect, -1.);
	return (cube);
}
