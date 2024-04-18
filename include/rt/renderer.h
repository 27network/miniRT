/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   renderer.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 07:50:09 by kiroussa          #+#    #+#             */
/*   Updated: 2024/04/18 15:44:44 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RENDERER_H
# define RENDERER_H

# include <mlx.h>
# include <rt/error.h>
# include <rt/scene.h>

# define RENDER_FONT "assets/fonts/menu.ttf"

typedef enum e_rt_render_status
{
	RT_RS_NONE,
	RT_RS_HOME,
	RT_RS_SCENE,
	RT_RS_EDITOR
}	t_rt_render_status;

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
	t_rt_scene			*scene;
	t_rt_mlx_data		*mlx;
	t_rt_render_status	*status;
}	t_rt_renderer;

void		rt_do_rendering(t_rt_renderer *renderer);

void		rt_render_home(t_rt_renderer *renderer);

void		rt_render_scene(t_rt_renderer *renderer);

void		rt_render_editor(t_rt_renderer *renderer);

void		rt_start_rendering(t_rt_scene *scene, t_rt_mlx_data *mlx_data);

void		rt_setup_rendering(t_rt_scene *scene, t_rt_mlx_data *mlx_data);

t_rt_error	rt_mlx_init(t_rt_mlx_data *mlx_data);

t_rt_error	rt_render(t_rt_scene *scene);

int			rt_render_update(void *mlx);

int			rt_window_event(int key, void *mlx);

int			rt_keydown_event(int key, void *mlx);


#endif // RENDERER_H
