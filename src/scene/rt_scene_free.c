/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt_scene_free.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 07:49:03 by kiroussa          #+#    #+#             */
/*   Updated: 2024/05/16 17:32:05 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <rt/scene.h>

void	rt_scene_free(t_rt_scene *scene)
{
	t_rt_object	*tmp;

	tmp = scene->objects;
	if (scene->objects_size)
	{
		while (tmp - scene->objects < (long) scene->objects_size)
			free((tmp++)->options);
	}
	free(scene->objects);
	tmp = scene->lights;
	if (scene->lights_size)
	{
		while (tmp - scene->objects < (long) scene->lights_size)
			free((tmp++)->options);
	}
	free(scene->lights);
	if (scene->ambient.options)
		free(scene->ambient.options);
	if (scene->camera.options)
		free(scene->camera.options);
}
