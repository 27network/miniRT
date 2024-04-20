/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt_scene_empty.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 18:54:12 by rgramati          #+#    #+#             */
/*   Updated: 2024/04/21 00:25:12 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include <string.h>
#include <ft/mem.h>
#include <rt/error.h>
#include <rt/scene.h>
#include <rt/renderer.h>
#include <rt/object/camera.h>
#include <rt/object/sphere.h>
#include <rt/object/light.h>

t_rt_object	*rt_object_init(t_rt_object *obj, t_rt_object_type type)
{
	obj->type = type;
	obj->position = (t_vec3d) {0.0f, 0.0f, 0.0f};
	obj->rotation = (t_vec3d) {0.0f, 0.0f, 0.0f};
	obj->color.argb = 0xFFFFFFFF;
	obj->options = NULL;
	return (obj);
}

void	rt_obj_move(t_rt_object *obj, t_vec3d move)
{
	obj->position = ft_vec3d_add(obj->position, move);
}

void	rt_obj_set_pos(t_rt_object *obj, double x, double y, double z)
{
	obj->position = (t_vec3d) {x, y, z};
}

void	rt_obj_set_rot(t_rt_object *obj, double x, double y, double z)
{
	obj->rotation = (t_vec3d) {x, y, z};
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

t_rt_error	rt_empty_scene(t_rt_scene *scene)
{
	t_rt_object	*camera;
	t_rt_object	*light;
	t_rt_object	*sphere;
	t_rt_object *sphere2;
	t_rt_object *sphere3;

	camera = rt_object_init(&scene->camera, RT_OBJ_CAMERA);
	light = rt_object_init(scene->lights, RT_OBJ_LIGHT);
	sphere = rt_object_init(&scene->objects[1], RT_OBJ_SPHERE);
	sphere2 = rt_object_init(&scene->objects[0], RT_OBJ_SPHERE);
	sphere3 = rt_object_init(&scene->objects[2], RT_OBJ_SPHERE);
	rt_obj_set_pos(light, 1.0f, 1.0f, 0.0f);
	rt_obj_set_pos(sphere, 0.0f, 0.0f, 2.0f);
	rt_obj_set_pos(sphere2, 0.5f, 0.2f, 3.0f);
	rt_obj_set_pos(sphere3, 0.3f, -0.3f, 1.0f);
	camera->options = rt_obj_camera_init("Marvin");
	light->options = rt_obj_light_init(1);
	sphere->options = rt_obj_sphere_init(2.0f);
	sphere->color.argb = 0xFFFFFFFF;
	sphere2->options = rt_obj_sphere_init(1.0f);
	sphere2->color.argb = 0xFF00FF00;
	sphere3->options = rt_obj_sphere_init(0.6f);
	sphere3->color.argb = 0xFFFF0000;
	return (rt_scene_guard(scene));
}
