/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt_scene_init.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 07:48:48 by kiroussa          #+#    #+#             */
/*   Updated: 2024/04/20 23:27:46 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include <string.h>
#include <ft/mem.h>
#include <rt/error.h>
#include <rt/scene.h>

t_rt_error	rt_scene_init(t_rt_scene *scene)
{
	ft_memset(scene, 0, sizeof(t_rt_scene));
	scene->objects_size = 3;
	scene->objects = ft_calloc(3, sizeof(t_rt_object));
	if (!scene->objects)
		return (rt_errd(RT_ERROR_ALLOC, strerror(errno)));
	scene->lights = ft_calloc(1, sizeof(t_rt_object));
	if (!scene->lights)
	{
		free(scene->objects);
		return (rt_errd(RT_ERROR_ALLOC, strerror(errno)));
	}
	return (rt_ok());
}
