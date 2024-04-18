/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt_scene_free.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 07:49:03 by kiroussa          #+#    #+#             */
/*   Updated: 2024/04/18 19:46:37 by rgramati         ###   ########.fr       */
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
	free(scene->objects);
}
