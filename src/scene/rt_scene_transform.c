/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt_scene_transform.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 18:54:12 by rgramati          #+#    #+#             */
/*   Updated: 2024/04/23 19:03:15 by rgramati         ###   ########.fr       */
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

void	rt_scene_translate(t_rt_scene *scene, t_vec3d move)
{
	size_t	i;

	i = 0;
	while (i < scene->objects_size)
	{
		scene->objects[i].position = ft_vec3d_add(scene->objects[i].position, move);
		i++;
	}
	i = 0;
	while (i < scene->lights_size)
	{
		scene->lights[i].position = ft_vec3d_add(scene->lights[i].position, move);
		i++;
	}
}