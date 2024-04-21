/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt_scene_empty.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 18:54:12 by rgramati          #+#    #+#             */
/*   Updated: 2024/04/21 20:51:44 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <ft/mem.h>
#include <ft/print.h>
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
	if (type == RT_OBJ_SPHERE)
		obj->intersect = &rt_obj_sphere_intersect;
	if (type == RT_OBJ_LIGHT)
		obj->intersect = &rt_obj_light_intersect;
	return (obj);
}

void	rt_obj_move(t_rt_object *obj, t_vec3d move)
{
	obj->position = ft_vec3d_add(obj->position, move);
}

void	rt_obj_rotate(t_rt_object *obj, t_vec3d angle)
{
	obj->rotation = ft_vec3d_add(obj->position, angle);
}

void	rt_obj_set_pos(t_rt_object *obj, double x, double y, double z)
{
	obj->position = (t_vec3d) {x, y, z};
}

void	rt_obj_set_rot(t_rt_object *obj, double x, double y, double z)
{
	obj->rotation = (t_vec3d) {x, y, z};
}

t_color	rt_get_random_color(void)
{
	int		urandom;
	char	buffer[3];
	t_color	color;

	urandom = open("/dev/random", O_RDONLY);
	read(urandom, buffer, 3);
	close(urandom);
	color.argb = 0xFF000000;
	color.argb += ((buffer[0] + 127) << 16);
	color.argb += ((buffer[1] + 127) << 8);
	color.argb += (buffer[2] + 127);
	return (color);
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
	t_rt_object	*spheres[4];
	int			i;

	camera = rt_object_init(&scene->camera, RT_OBJ_CAMERA);
	light = rt_object_init(scene->lights, RT_OBJ_LIGHT);
	rt_obj_set_pos(light, 0.0f, 3.0f, 0.0f);
	i = 0;
	while (i < 4)
	{
		spheres[i] = rt_object_init(&scene->objects[i], RT_OBJ_SPHERE);
		rt_obj_set_pos(spheres[i], -3.0f + 2.0f * i, 0.0f, 5.0f);
		spheres[i]->options = rt_obj_sphere_init(1.0f);
		spheres[i]->color = rt_get_random_color();
		ft_printf("%d, %d, %d\n", spheres[i]->color.r, spheres[i]->color.g, spheres[i]->color.b);
		i++;
	}
	camera->options = rt_obj_camera_init("Marvin");
	light->options = rt_obj_light_init(1);
	light->color.argb = 0xFFFF0000;
	return (rt_scene_guard(scene));
}
