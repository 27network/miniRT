/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt_scene_init.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 07:48:48 by kiroussa          #+#    #+#             */
/*   Updated: 2024/04/23 22:12:52 by rgramati         ###   ########.fr       */
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
#include <rt/object/cylinder.h>
#include <rt/object/plane.h>
#include <rt/object/sphere.h>
#include <rt/object/light.h>

#define SPHERES_COUNT 4
#define LIGHTS_COUNT 1

t_rt_error	rt_scene_init(t_rt_scene *scene)
{
	ft_memset(scene, 0, sizeof(t_rt_scene));
	scene->pratio = 4;
	scene->objects_size = SPHERES_COUNT + 1;
	scene->lights_size = LIGHTS_COUNT;
	scene->objects = ft_calloc(SPHERES_COUNT + 1, sizeof(t_rt_object));
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
	obj->position = (t_vec3d) {0.0f, 0.0f, 0.0f};
	obj->rotation = (t_vec3d) {0.0f, 0.0f, 0.0f};
	obj->color = rt_color(0xFFFFFFFF);
	obj->options = NULL;
	if (type == RT_OBJ_LIGHT)
		obj->intersect = &rt_obj_light_intersect;
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
	t_rt_object	*camera;
	t_rt_object	*ambient;
	t_rt_object	*lights[LIGHTS_COUNT];
	t_rt_object	*spheres[SPHERES_COUNT];
	t_rt_object	*background;
	size_t		i;

	
	ambient = rt_object_init(&scene->ambient, RT_OBJ_LIGHT);
	ambient->options = rt_obj_light_init(0.2f);
	ambient->color = rt_color(0xFFFFFFFF);

	i = 0;
	while (i < SPHERES_COUNT)
	{
		spheres[i] = rt_object_init(&scene->objects[i + 1], RT_OBJ_SPHERE);
		rt_obj_set_pos(spheres[i], ((scene->objects_size / 2) * -2.0f + 1.0f) + 2.0f * i, 0.0f, 5.0f);
		spheres[i]->options = rt_obj_sphere_init(1.0f);
		spheres[i]->color = rt_get_random_color(0);
		// spheres[i]->color = rt_color(0xFFFFFFFF);
		i++;
	}

	background = rt_object_init(&scene->objects[0], RT_OBJ_PLANE);
	rt_obj_set_pos(background, 0.0f, -2.0f, 0.0f);
	background->options = rt_obj_plane_init((t_vec3d){0.0f, 1.0f, 0.0f});
	background->color = rt_get_random_color(0);
	// background->color = rt_color(0xFFFFFFFF);

	spheres[0] = rt_object_init(&scene->objects[1], RT_OBJ_CYLINDER);
	rt_obj_set_pos(spheres[0], 0.0f, 0.0f, 7.0f);
	spheres[0]->options = rt_obj_cylinder_init(1.5f, 10.0f, (t_vec3d){0.0f, 1.0f, 0.0f});
	spheres[0]->color = rt_get_random_color(0);

	rt_get_random_color(1);

	camera = rt_object_init(&scene->camera, RT_OBJ_CAMERA);
	camera->options = rt_obj_camera_init("Marvin");
	camera->rotation = (t_vec3d){0.0f, 0.0f, 0.0f};

	i = 0;
	while (i < scene->lights_size)
	{
		lights[i] = rt_object_init(&scene->lights[i], RT_OBJ_LIGHT);
		lights[i]->options = rt_obj_light_init(1.0f);
		lights[i]->color = rt_color(0xFFFFFFFF);
		i++;
	}
	rt_obj_set_pos(lights[0], 0.0f, 5.0f, 0.0f);

	return (rt_scene_guard(scene));
}
