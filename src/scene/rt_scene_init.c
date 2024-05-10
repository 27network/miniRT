/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt_scene_init.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 07:48:48 by kiroussa          #+#    #+#             */
/*   Updated: 2024/05/10 16:05:41 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

#define OBJ_COUNT 10
#define LIGHTS_COUNT 1

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
	obj->position = (t_vec3d) {0.0, 0.0, 0.0};
	obj->rotation = (t_vec3d) {0.0, 0.0, 0.0};
	obj->material.obj_color = rt_color_to_norm(rt_color(0xFFFFFFFF));
	obj->material.emi_color = rt_color_to_norm(rt_color(0xFFFFFFFF));
	obj->material.emi_strength = 0.0;
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
	return (obj);
}

void	rt_obj_set_pos(t_rt_object *obj, double x, double y, double z)
{
	obj->position = (t_vec3d) {.x = x, .y = y, .z = z};
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
	read(urandom, buffer, 3);
	color.r = (buffer[0] + 127);
	color.g = (buffer[1] + 127);
	color.b = (buffer[2] + 127);
	return (color);
}

t_rt_error	rt_scene_example(t_rt_scene *scene)
{
	t_rt_object	*ambient;
	ambient = rt_object_init(&scene->ambient, RT_OBJ_LIGHT);
	rt_obj_set_pos(ambient, 50., 50., 50.);
	ambient->options = rt_obj_light_init(0.2);
	ambient->material.obj_color = rt_color_to_norm(rt_color(0xFFFFFFFF));
	ambient->material.emi_color = rt_color_to_norm(rt_color(0xFFFFFFFF));
	ambient->material.emi_strength = 0.3;

	t_rt_object	*sphere;
	sphere = rt_object_init(&scene->objects[0], RT_OBJ_SPHERE);
	rt_obj_set_pos(sphere, 0, 0.0, 3.0);
	sphere->options = rt_obj_sphere_init(4);
	sphere->material.obj_color = rt_color_to_norm(rt_color(0xFFFFFFFF));
	sphere->material.emi_color = rt_color_to_norm(rt_color(0xFFFFFFFF));
	sphere->material.emi_strength = 100.0;
	
	t_rt_object	*sphere5;
	sphere5 = rt_object_init(&scene->objects[7], RT_OBJ_SPHERE);
	rt_obj_set_pos(sphere5, 4.0, 0.0, 3.0);
	sphere5->options = rt_obj_sphere_init(2.0);
	sphere5->material.obj_color = rt_color_to_norm(rt_color(0xFFFF0000));
	sphere5->material.emi_color = rt_color_to_norm(rt_color(0xFFFF0000));
	sphere5->material.emi_strength = 0.0;

	t_rt_object	*sphere6;
	sphere6 = rt_object_init(&scene->objects[8], RT_OBJ_SPHERE);
	rt_obj_set_pos(sphere6, 0.0, 3.0, 3.0);
	sphere6->options = rt_obj_sphere_init(1.0);
	sphere6->material.obj_color = rt_color_to_norm(rt_color(0xFF00FF00));
	sphere6->material.emi_color = rt_color_to_norm(rt_color(0xFF00FF00));
	sphere6->material.emi_strength = 0.0;

	t_rt_object	*sphere7;
	sphere7 = rt_object_init(&scene->objects[9], RT_OBJ_CYLINDER);
	rt_obj_set_pos(sphere7, -3.0, -5.0, 3.0);
	sphere7->options = rt_obj_cylinder_init(1., 10., (t_vec3d){0., 1., 0.});
	sphere7->material.obj_color = rt_color_to_norm(rt_color(0xFF0000FF));
	sphere7->material.emi_color = rt_color_to_norm(rt_color(0xFF0000FF));
	sphere7->material.emi_strength = 0.0;

	// BACKGROUND WALL
	t_rt_object	*wall;
	wall = rt_object_init(&scene->objects[1], RT_OBJ_PLANE);
	rt_obj_set_pos(wall, 0.0, 0.0, 5.);
	wall->options = rt_obj_plane_init((t_vec3d){0.0, 0.0, -1.0});
	wall->material.obj_color = rt_color_to_norm(rt_color(0xFFFFFFFF));
	wall->material.emi_color = rt_color_to_norm(rt_color(0xFFFFFFFF));
	wall->material.emi_strength = 0.0;

	// FRONT GROUND WALL ()
	t_rt_object	*backwall;
	backwall = rt_object_init(&scene->objects[2], RT_OBJ_PLANE);
	rt_obj_set_pos(backwall, 0.0, 0.0, -20.0);
	backwall->options = rt_obj_plane_init((t_vec3d){0.0, 0.0, 1.0});
	backwall->material.obj_color = rt_color_to_norm(rt_color(0xFFFFFFFF));
	backwall->material.emi_color = rt_color_to_norm(rt_color(0xFFFFFFFF));
	backwall->material.emi_strength = 0.0;

	t_rt_object	*rightwall;
	rightwall = rt_object_init(&scene->objects[3], RT_OBJ_PLANE);
	rt_obj_set_pos(rightwall, 5.0, 0.0, 0.0);
	rightwall->options = rt_obj_plane_init((t_vec3d){-1.0, 0.0, 0.0});
	rightwall->material.obj_color = rt_color_to_norm(rt_color(0xFF0000FF));
	rightwall->material.emi_color = rt_color_to_norm(rt_color(0xFF0000FF));
	rightwall->material.emi_strength = 0.0;

	t_rt_object	*leftwall;
	leftwall = rt_object_init(&scene->objects[4], RT_OBJ_PLANE);
	rt_obj_set_pos(leftwall, -5.0, 0.0, 0.0);
	leftwall->options = rt_obj_plane_init((t_vec3d){1.0, 0.0, 0.0});
	leftwall->material.obj_color = rt_color_to_norm(rt_color(0xFFFF0000));
	leftwall->material.emi_color = rt_color_to_norm(rt_color(0xFFFF0000));
	leftwall->material.emi_strength = 0.0;

	t_rt_object	*floor;
	floor = rt_object_init(&scene->objects[5], RT_OBJ_PLANE);
	rt_obj_set_pos(floor, 0.0, -5.0, 0.0);
	floor->options = rt_obj_plane_init((t_vec3d){0.0, 1.0, 0.0});
	floor->material.obj_color = rt_color_to_norm(rt_color(0xFF00FF00));
	floor->material.emi_color = rt_color_to_norm(rt_color(0xFF00FF00));
	floor->material.emi_strength = 0.0;

	t_rt_object	*ceil;
	ceil = rt_object_init(&scene->objects[6], RT_OBJ_PLANE);
	rt_obj_set_pos(ceil, 0.0, 5.0, 0.0);
	ceil->options = rt_obj_plane_init((t_vec3d){0.0, -1.0, 0.0});
	ceil->material.obj_color = rt_color_to_norm(rt_color(0xFFFFFFFF));
	ceil->material.emi_color = rt_color_to_norm(rt_color(0xFFFFFFFF));
	ceil->material.emi_strength = 0.0;

	// t_rt_object	*sun;
	// sun = rt_object_init(&scene->objects[4], RT_OBJ_SPHERE);
	// rt_obj_set_pos(sun, -300.0, 40.0, 1000.0);
	// sun->options = rt_obj_sphere_init(140.0);
	// sun->material.obj_color = rt_color_to_norm(rt_color(0xFFFFFFFF));
	// sun->material.emi_color = rt_color_to_norm(rt_color(0xFFFFFFFF));
	// sun->material.emi_strength = 0.;

	t_rt_object	*camera;
	camera = rt_object_init(&scene->camera, RT_OBJ_CAMERA);
	rt_obj_set_pos(camera, 0., 0., -19.);
	camera->options = rt_obj_camera_init("Marvin", 70);
	camera->rotation = (t_vec3d){0., 0., 1.0};
	camera->material.emi_color = rt_color_to_norm(rt_color(0x00000000));
	camera->material.emi_strength = 0.0;

	return (rt_scene_guard(scene));
}
