/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt_scene_free.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 07:49:03 by kiroussa          #+#    #+#             */
/*   Updated: 2024/05/11 02:47:01 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <rt/scene.h>

void	rt_scene_free(t_rt_scene *scene)
{
	t_rt_object	*tmp;

	if (!scene->objects_size)
		return ;
	tmp = scene->objects;
	while (tmp - scene->objects < (long) scene->objects_size)
		free((tmp++)->options);
	tmp = scene->lights;
	while (tmp - scene->objects < (long) scene->lights_size)
		free((tmp++)->options);
	free(scene->objects);
}
