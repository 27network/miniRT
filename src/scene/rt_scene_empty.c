/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt_scene_empty.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 18:54:12 by rgramati          #+#    #+#             */
/*   Updated: 2024/04/18 19:52:15 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include <string.h>
#include <ft/mem.h>
#include <rt/error.h>
#include <rt/scene.h>

t_rt_object	*rt_init_obj(t_rt_object *obj, t_rt_object_type type)
{
	obj->type = type;
	obj->position = (t_vec3d) {0.0f, 0.0f, 0.0f};
	obj->rotation = (t_vec3d) {0.0f, 0.0f, 0.0f};
	obj->color.argb = 0xFFFFFFFF;
	obj->options = NULL;
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

	camera = rt_init_obj(&scene->objects[0], RT_OBJ_CAMERA);
	light = rt_init_obj(&scene->objects[1], RT_OBJ_SPHERE);
	sphere = rt_init_obj(&scene->objects[2], RT_OBJ_LIGHT);
	rt_obj_set_pos(light, 1.0f, 1.0f, 0.0f);
	rt_obj_set_pos(sphere, 1.0f, 0.0f, 0.0f);
	camera->options = rt_obj_camera_init("Marvin");
	light->options = rt_obj_light_init(1.0f);
	sphere->options = rt_obj_sphere_init(0.5f);
	return (rt_scene_guard(scene));
}
