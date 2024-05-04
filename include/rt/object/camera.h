/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 19:06:32 by rgramati          #+#    #+#             */
/*   Updated: 2024/04/30 14:31:31 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CAMERA_H
# define CAMERA_H

# include <ft/math/matrix.h>

typedef struct s_rt_obj_camera
{
	char	*name;
	double	fov;
}	t_rt_obj_camera;

t_rt_obj_camera	*rt_obj_camera_init(char *name, int fov);

#endif // CAMERA_H