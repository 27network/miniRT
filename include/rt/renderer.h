/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   renderer.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 07:50:09 by kiroussa          #+#    #+#             */
/*   Updated: 2024/04/18 13:26:28 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RENDERER_H
# define RENDERER_H

# include <mlx.h>
# include <rt/error.h>
# include <rt/scene.h>

typedef struct s_rt_mlx_data
{
	void	*rt_mlx;
	void	*rt_win;
	void	*rt_imgs[2];
	int		rt_width;
	int		rt_height;
}	t_rt_mlx_data;

typedef struct s_rt_renderer
{
	t_rt_scene		*scene;
	t_rt_mlx_data	*mlx;
}	t_rt_renderer;

void		rt_do_rendering(void *render);

void		rt_start_rendering(t_rt_scene *scene, t_rt_mlx_data *mlx_data);

void		rt_setup_rendering(t_rt_scene *scene, t_rt_mlx_data *mlx_data);

t_rt_error	rt_mlx_init(t_rt_mlx_data *mlx_data);

t_rt_error	rt_render(t_rt_scene *scene);

int			rt_render_update(void *mlx);

int			rt_window_event(int key, void *mlx);

int			rt_keydown_event(int key, void *mlx);

#endif // RENDERER_H
