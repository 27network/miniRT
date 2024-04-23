/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt_scene_init.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 07:48:48 by kiroussa          #+#    #+#             */
/*   Updated: 2024/04/23 13:19:01 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <ft/mem.h>
#include <rt/error.h>
#include <rt/renderer.h>
#include <rt/object/camera.h>
#include <rt/object/sphere.h>
#include <rt/object/light.h>

#define SPHERES_COUNT 40
#define LIGHTS_COUNT 1

t_rt_error	rt_scene_init(t_rt_scene *scene)
{
	ft_memset(scene, 0, sizeof(t_rt_scene));
	scene->pratio = 4;
	scene->objects_size = SPHERES_COUNT;
	scene->lights_size = LIGHTS_COUNT;
	scene->objects = ft_calloc(SPHERES_COUNT, sizeof(t_rt_object));
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
	obj->color.argb = 0xFFFFFFFF;
	obj->options = NULL;
	if (type == RT_OBJ_SPHERE)
	{
		obj->intersect = &rt_obj_sphere_intersect;
		obj->norm = &rt_obj_sphere_norm;
	}
	if (type == RT_OBJ_LIGHT)
		obj->intersect = &rt_obj_light_intersect;
	return (obj);
}

void	rt_obj_set_pos(t_rt_object *obj, double x, double y, double z)
{
	obj->position = (t_vec3d) {x, y, z};
}

void	rt_obj_set_rot(t_rt_object *obj, double x, double y, double z)
{
	obj->rotation = (t_vec3d) {x, y, z};
}

t_color	rt_get_random_color(int toclose)
{
	static int	urandom = -1;
	char		buffer[3];
	t_color		color;

	color.argb = 0xFF000000;
	if (toclose)
	{
		close(urandom);
		return (color);
	}
	if (urandom == -1)
		urandom = open("/dev/random", O_RDONLY);
	read(urandom, buffer, 3);
	color.argb += ((buffer[0] + 127) << 16);
	color.argb += ((buffer[1] + 127) << 8);
	color.argb += (buffer[2] + 127);
	return (color);
}

t_rt_error	rt_scene_example(t_rt_scene *scene)
{
	t_rt_object	*camera;
	t_rt_object	*ambient;
	t_rt_object	*lights[LIGHTS_COUNT];
	t_rt_object	*spheres[SPHERES_COUNT];
	size_t		i;

	camera = rt_object_init(&scene->camera, RT_OBJ_CAMERA);
	ambient = rt_object_init(&scene->ambient, RT_OBJ_LIGHT);
	ambient->options = rt_obj_light_init(0.2f);
	ambient->color.argb = 0x35353535;
	i = 0;
	while (i < scene->objects_size)
	{
		spheres[i] = rt_object_init(&scene->objects[i], RT_OBJ_SPHERE);
		rt_obj_set_pos(spheres[i], ((scene->objects_size / 2) * -2.0f + 1.0f) + 2.0f * i, 0.0f, 5.0f);
		spheres[i]->options = rt_obj_sphere_init(1.0f);
		spheres[i]->color = rt_get_random_color(i == scene->objects_size);
		i++;
	}
	camera->options = rt_obj_camera_init("Marvin");
	camera->rotation = (t_vec3d){0.0f, 0.0f, 0.0f};
	i = 0;
	while (i < scene->lights_size)
	{
		lights[i] = rt_object_init(&scene->lights[i], RT_OBJ_LIGHT);
		lights[i]->options = rt_obj_light_init(1.0f);
		lights[i]->color = (t_color){.argb = 0xFFFFFFFF};
		i++;
	}
	rt_obj_set_pos(lights[0], 0.0f, 2.0f, 0.0f);
	return (rt_scene_guard(scene));
}
