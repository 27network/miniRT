/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt_scene_free.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 07:49:03 by kiroussa          #+#    #+#             */
/*   Updated: 2024/04/18 13:25:21 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <rt/scene.h>

void	rt_scene_free(t_rt_scene *scene)
{
	if (!scene->objects_size)
		return ;
	free(scene->objects);
}
