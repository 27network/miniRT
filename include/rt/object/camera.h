/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 19:06:32 by rgramati          #+#    #+#             */
/*   Updated: 2024/04/18 19:21:46 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CAMERA_H
# define CAMERA_H

# include <ft/math/vector.h>

typedef struct s_rt_obj_camera
{
	char	*name;
}	t_rt_obj_camera;

t_rt_obj_camera	*rt_obj_camera_init(char *name);

#endif // CAMERA_H