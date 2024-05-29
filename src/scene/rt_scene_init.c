/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt_scene_init.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 07:48:48 by kiroussa          #+#    #+#             */
/*   Updated: 2024/05/29 12:36:32 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft/math/vector.h"
#include "rt/scene.h"
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <ft/mem.h>
#include <ft/print.h>
#include <rt/error.h>
#include <rt/renderer.h>
#include <rt/object/camera.h>
#include <rt/object/cone.h>
#include <rt/object/quad.h>
#include <rt/object/cylinder.h>
#include <rt/object/plane.h>
#include <rt/object/sphere.h>
#include <rt/object/light.h>

#define OBJ_COUNT 2
#define LIGHTS_COUNT 0

void	rt_swap(double *a, double *b)
{	
	double	tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

t_rt_error	rt_scene_init(t_rt_scene *scene)
{
	ft_memset(scene, 0, sizeof(t_rt_scene));
	scene->pratio = 1;
	scene->objects_size = OBJ_COUNT;
	scene->lights_size = LIGHTS_COUNT;
	scene->objects = ft_calloc(OBJ_COUNT, sizeof(t_rt_object));
	if (!scene->objects)
		return (rt_errd(RT_ERROR_ALLOC, strerror(errno)));
	scene->lights = ft_calloc(LIGHTS_COUNT, sizeof(t_rt_object));
	if (!scene->lights)
	{
		free(scene->objects);
		return (rt_errd(RT_ERROR_ALLOC, strerror(errno)));
	}
	return (rt_ok());
}

t_rt_error	rt_scene_guard(t_rt_scene *scene)
{
	t_rt_object	*tmp;

	tmp = scene->objects;
	while (tmp - scene->objects < (long) scene->objects_size && tmp->options)
		tmp++;
	if (tmp - scene->objects != (long) scene->objects_size)
		return (rt_errd(RT_ERROR_ALLOC, strerror(errno)));
	return (rt_ok());
}

/* ************************************************************************** */
/*                                  EXAMPLE SCENE							  */
/* ************************************************************************** */

t_rt_object	*rt_object_init(t_rt_object *obj, t_rt_object_type type)
{
	obj->type = type;
	obj->pos = (t_vec3d) {0.0, 0.0, 0.0};
	obj->rotation = (t_vec3d) {0.0, 0.0, 0.0};
	obj->mat.obj_color = rt_color_to_norm(rt_color(0xFFFFFFFF));
	obj->mat.emi_color = rt_color_to_norm(rt_color(0xFF000000));
	obj->mat.spe_color = rt_color_to_norm(rt_color(0xFFFFFFFF));
	obj->mat.emi_strength = 0.0;
	obj->mat.smoothness = 0.0;
	obj->mat.spe_prob = 0.0;
	obj->options = NULL;
	if (type == RT_OBJ_LIGHT)
	{
		obj->intersect = &rt_obj_light_intersect;
		obj->norm = &rt_obj_sphere_norm;
	}
	if (type == RT_OBJ_SPHERE)
	{
		obj->intersect = &rt_obj_sphere_intersect;
		obj->norm = &rt_obj_sphere_norm;
	}
	if (type == RT_OBJ_PLANE)
	{
		obj->intersect = &rt_obj_plane_intersect;
		obj->norm = &rt_obj_plane_norm;
	}
	if (type == RT_OBJ_CYLINDER)
	{
		obj->intersect = &rt_obj_cylinder_intersect;
		obj->norm = &rt_obj_cylinder_norm;
	}
	if (type == RT_OBJ_CONE)
	{
		obj->intersect = &rt_obj_cone_intersect;
		obj->norm = &rt_obj_cone_norm;
	}
	if (type == RT_OBJ_QUAD)
	{
		obj->intersect = &rt_obj_quad_intersect;
		obj->norm = &rt_obj_quad_norm;
	}
	if (type == RT_OBJ_CUBE)
	{
		obj->intersect = &rt_obj_cube_intersect;
		obj->norm = &rt_obj_quad_norm;
	}
	return (obj);
}

void	rt_obj_set_pos(t_rt_object *obj, double x, double y, double z)
{
	obj->pos = (t_vec3d) {.x = x, .y = y, .z = z};
}

void	rt_obj_set_rot(t_rt_object *obj, double x, double y, double z)
{
	obj->rotation = (t_vec3d) {.x = x, .y = y, .z = z};
}

t_color	rt_get_random_color(int toclose)
{
	static int	urandom = -1;
	char		buffer[3];
	t_color		color;

	color = rt_color(0xFF000000);
	if (toclose)
	{
		close(urandom);
		return (color);
	}
	if (urandom == -1)
		urandom = open("/dev/random", O_RDONLY);
	if (read(urandom, buffer, 3) < 0)
		return (color);
	color.r = (buffer[0] + 127);
	color.g = (buffer[1] + 127);
	color.b = (buffer[2] + 127);
	return (color);
}

void	new_sphere(t_vec3d pos, double diameter, t_rt_object *location)
{
	rt_object_init(location, RT_OBJ_SPHERE);
	rt_obj_set_pos(location, pos.x, pos.y, pos.z);
	location->options = rt_obj_sphere_init(diameter);
}

void	new_quad(t_vec3d pos, t_vec3d norm, t_vec3d wvect, t_vec3d hvect, t_rt_object *location)
{
	rt_object_init(location, RT_OBJ_QUAD);
	rt_obj_set_pos(location, pos.x, pos.y, pos.z);
	location->options = rt_obj_quad_init(norm, wvect, hvect);
}

t_rt_error	rt_scene_example(t_rt_scene *scene)
{
	t_color_norm	sky[3] = {rt_color_to_norm(rt_color(0xFFFFFFFF)),
								rt_color_to_norm(rt_color(0xFF87CEEB)),
								rt_color_to_norm(rt_color(0xFF8C8391))};

	scene->skybox.usky = ft_vec3d(sky[0].r, sky[0].g, sky[0].b);
	scene->skybox.dsky = ft_vec3d(sky[1].r, sky[1].g, sky[1].b);
	scene->skybox.gcol = ft_vec3d(sky[2].r, sky[2].g, sky[2].b);

	t_rt_object	*ambient;
	ambient = rt_object_init(&scene->ambient, RT_OBJ_LIGHT);
	rt_obj_set_pos(ambient, 100., 100., 100.);
	ambient->options = rt_obj_light_init(1.);
	ambient->mat.obj_color = rt_color_to_norm(rt_color(0xFFFFFFFF));
	ambient->mat.emi_color = rt_color_to_norm(rt_color(0xFFFFFFFF));
	ambient->mat.emi_strength = 0.0;
	ambient->mat.smoothness = 0.0;

	// new_quad(ft_vec3d(-1., -1., 0.), ft_vec3d_norm(ft_vec3d(2., 1., 0.)), ft_vec3d(-1., 2., 0.), ft_vec3d(0., 0., 2.), &scene->objects[0]);
	// new_quad(ft_vec3d(-1., -1., 0.), ft_vec3d_norm(ft_vec3d(-1., 2., 0.)), ft_vec3d(0., 0., 2.), ft_vec3d(2., 1., 0.), &scene->objects[2]);
	new_quad(ft_vec3d(-1., 0., -1.), ft_vec3d_norm(ft_vec3d(0., 1., 0.)), ft_vec3d(2., 0., 0.), ft_vec3d(0., 0., 2.), &scene->objects[1]);

	// new_sphere(ft_vec3d(0., 0., 0.), 1., &scene->objects[0]);
	t_rt_object	*cylinder;
	cylinder = rt_object_init(&scene->objects[0], RT_OBJ_CYLINDER);
	rt_obj_set_pos(cylinder, 0., 0., 0.);
	rt_obj_set_rot(cylinder, 0., 0., 0.);
	cylinder->options = rt_obj_cylinder_init(2., 5., ft_vec3d(0., 1., 0.), cylinder->rotation);
	cylinder->mat.obj_color = rt_color_to_norm(rt_color(0xFFFF0000));
	cylinder->mat.smoothness = 1.0;
	cylinder->mat.spe_prob = 0.5;

	// new_sphere(ft_vec3d(0., 0., 0.), 1., &scene->objects[0]);
	// new_sphere(ft_vec3d(1., 0., 0.), 1., &scene->objects[1]);
	// new_sphere(ft_vec3d(2., 0., 0.), 1., &scene->objects[2]);
	// new_sphere(ft_vec3d(3., 0., 0.), 1., &scene->objects[3]);
	// new_sphere(ft_vec3d(4., 0., 0.), 1., &scene->objects[4]);
	// new_sphere(ft_vec3d(5., 0., 0.), 1., &scene->objects[5]);
	// new_sphere(ft_vec3d(6., 0., 0.), 1., &scene->objects[6]);
	// new_sphere(ft_vec3d(7., 0., 0.), 1., &scene->objects[7]);

	// new_quad(ft_vec3d(-2., 4., -2.), ft_vec3d(0., -1., 0.), ft_vec3d(4., 0., 0.), ft_vec3d(0., 0., 4.), &scene->objects[1]);
	// scene->objects[1].mat.emi_strength = 1.0;
	// scene->objects[1].mat.emi_color = rt_color_to_norm(rt_color(0xFFFFFFFF));

	t_rt_object	*camera;
	camera = rt_object_init(&scene->camera, RT_OBJ_CAMERA);
	rt_obj_set_pos(camera, 0., 0., -4.);
	camera->options = rt_obj_camera_init("Marvin", 90, ft_vec3d(0., 0., 1.));
	camera->mat.emi_color = rt_color_to_norm(rt_color(0x00000000));
	camera->mat.emi_strength = 0.0;

	return (rt_scene_guard(scene));
}
