/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt_scene_init.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 07:48:48 by kiroussa          #+#    #+#             */
/*   Updated: 2024/04/26 21:49:39 by rgramati         ###   ########.fr       */
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
#include <rt/object/cylinder.h>
#include <rt/object/plane.h>
#include <rt/object/sphere.h>
#include <rt/object/light.h>

#define SPHERES_COUNT 5
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
	t_rt_object	*ambient;
	ambient = rt_object_init(&scene->ambient, RT_OBJ_LIGHT);
	ambient->options = rt_obj_light_init(0.1f);
	ambient->color = rt_color(0xFFFFFFFF);

	t_rt_object	*floor;
	floor = rt_object_init(&scene->objects[0], RT_OBJ_PLANE);
	rt_obj_set_pos(floor, 0.0f, -2.0f, 0.0f);
	floor->options = rt_obj_plane_init((t_vec3d){0.0f, 1.0f, 0.0f});
	floor->color = rt_get_random_color(0);

	t_rt_object	*ceiling;
	ceiling = rt_object_init(&scene->objects[5], RT_OBJ_PLANE);
	rt_obj_set_pos(ceiling, 0.0f, 4.0f, 0.0f);
	ceiling->options = rt_obj_plane_init((t_vec3d){0.0f, -1.0f, 0.0f});
	ceiling->color = rt_get_random_color(0);

	t_rt_object	*left_wall;
	left_wall = rt_object_init(&scene->objects[1], RT_OBJ_PLANE);
	rt_obj_set_pos(left_wall, -5.0f, 0.0f, 0.0f);
	left_wall->options = rt_obj_plane_init((t_vec3d){1.0f, 0.0f, 0.0f});
	left_wall->color = rt_get_random_color(0);

	t_rt_object	*right_wall;
	right_wall = rt_object_init(&scene->objects[2], RT_OBJ_PLANE);
	rt_obj_set_pos(right_wall, 5.0f, 0.0f, 0.0f);
	right_wall->options = rt_obj_plane_init((t_vec3d){-1.0f, 0.0f, 0.0f});
	right_wall->color = rt_get_random_color(0);

	t_rt_object	*back_wall;
	back_wall = rt_object_init(&scene->objects[3], RT_OBJ_PLANE);
	rt_obj_set_pos(back_wall, 0.0f, 0.0f, 10.0f);
	back_wall->options = rt_obj_plane_init((t_vec3d){0.0f, 0.0f, -1.0f});
	back_wall->color = rt_get_random_color(0);

	// t_rt_object	*cone;
	// cone = rt_object_init(&scene->objects[4], RT_OBJ_CONE);
	// rt_obj_set_pos(cone, 0.0f, 0.0f, 7.0f);
	// cone->options = rt_obj_cone_init(1.0f, 4.0f, ft_vec3d_norm((t_vec3d){0.0f, -1.0f, 0.0f}), RT_PI / 8.0f);
	// cone->color = rt_get_random_color(0);

	t_rt_object	*cylinder;
	cylinder = rt_object_init(&scene->objects[4], RT_OBJ_CYLINDER);
	rt_obj_set_pos(cylinder, 0.0f, 0.0f, 7.0f);
	cylinder->options = rt_obj_cylinder_init(1.0f, 4.0f, ft_vec3d_norm((t_vec3d){0.0f, -1.0f, 0.0f}));
	cylinder->color = rt_get_random_color(0);

	// t_rt_object	*sphere;
	// sphere = rt_object_init(&scene->objects[4], RT_OBJ_SPHERE);
	// rt_obj_set_pos(sphere, 0.0f, 0.0f, 8.0f);
	// sphere->options = rt_obj_sphere_init(1.0f);
	// sphere->color = rt_get_random_color(0);
	
	// t_rt_object	*sphere2;
	// sphere2 = rt_object_init(&scene->objects[5], RT_OBJ_SPHERE);
	// rt_obj_set_pos(sphere2, 0.0f, 0.0f, 6.0f);
	// sphere2->options = rt_obj_sphere_init(1.0f);
	// sphere2->color = rt_get_random_color(0);

	// rt_get_random_color(1);

	t_rt_object	*camera;
	camera = rt_object_init(&scene->camera, RT_OBJ_CAMERA);
	camera->options = rt_obj_camera_init("Marvin");
	camera->rotation = (t_vec3d){0.0f, 0.0f, 0.0f};

	t_rt_object	*light;
	light = rt_object_init(scene->lights, RT_OBJ_LIGHT);
	light->options = rt_obj_light_init(0.8f);
	light->color = rt_color(0xFFFFFFFF);
	rt_obj_set_pos(light, -3.0f, 2.0f, 5.0f);

	return (rt_scene_guard(scene));
}
