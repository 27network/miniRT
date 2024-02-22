/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 02:33:41 by kiroussa          #+#    #+#             */
/*   Updated: 2024/02/22 07:48:39 by kiroussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SCENE_H
# define SCENE_H

typedef struct s_rt_scene
{
}	t_rt_scene;

void	rt_scene_init(t_rt_scene *scene);
void	rt_scene_free(t_rt_scene *scene);

#endif // SCENE_H
