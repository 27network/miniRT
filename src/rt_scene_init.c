/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt_scene_init.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 07:48:48 by kiroussa          #+#    #+#             */
/*   Updated: 2024/04/18 13:01:06 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include <string.h>
#include <ft/mem.h>
#include <rt/error.h>
#include <rt/scene.h>

void	rt_empty_object(t_rt_object *obj, t_rt_object_type type, t_vec3d pos)
{
	obj->type = type;
	obj->position = pos;
	obj->options = NULL;
}

t_rt_error	rt_scene_init(t_rt_scene *scene)
{
	ft_memset(scene, 0, sizeof(t_rt_scene));
	scene->objects_size = 3;
	scene->objects = ft_calloc(3, sizeof(t_rt_object));
	if (!scene->objects)
		return (rt_errd(RT_ERROR_ALLOC, strerror(errno)));
	return (rt_ok());
}

t_rt_error	rt_empty_scene(t_rt_scene *scene)
{
	rt_empty_object(scene->objects,
		RT_OBJ_CAMERA, (t_vec3d){0.0, 0.0, 0.0});
	rt_empty_object(scene->objects + 1,
		RT_OBJ_SPHERE, (t_vec3d){1.0, 0.0, 0.0});
	rt_empty_object(scene->objects + 2,
		RT_OBJ_LIGHT, (t_vec3d){1.0, 1.0, 0.0});
	return (rt_ok());
}
