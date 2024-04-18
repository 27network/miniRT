/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt_render.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 07:51:17 by kiroussa          #+#    #+#             */
/*   Updated: 2024/04/18 13:29:20 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <rt/renderer.h>

void	rt_setup_rendering(t_rt_scene *scene, t_rt_mlx_data *mlx)
{
	(void) scene;
	mlx->rt_width = 1920;
	mlx->rt_height = 1080;
}

void	rt_start_rendering(t_rt_scene *scene, t_rt_mlx_data *mlx)
{
	t_rt_renderer	renderer;

	renderer = (t_rt_renderer) {scene, mlx};
	mlx_on_event(mlx->rt_mlx, mlx->rt_win,
		MLX_WINDOW_EVENT, rt_window_event, mlx);
	mlx_on_event(mlx->rt_mlx, mlx->rt_win,
		MLX_KEYDOWN, rt_keydown_event, mlx);
	mlx_loop_hook(mlx->rt_mlx, rt_render_update, &renderer);
	rt_empty_scene(scene);
	mlx_loop(mlx->rt_mlx);
	mlx_destroy_image(mlx->rt_mlx, mlx->rt_imgs[0]);
	mlx_destroy_image(mlx->rt_mlx, mlx->rt_imgs[1]);
	mlx_destroy_window(mlx->rt_mlx, mlx->rt_win);
	mlx_destroy_display(mlx->rt_mlx);
}

void	rt_do_rendering(void *render)
{
	t_rt_renderer	*renderer;
	t_rt_scene		*scene;
	t_rt_mlx_data	*mlx;

	renderer = (t_rt_renderer *) render;
	scene = renderer->scene;
	mlx = renderer->mlx;
}

t_rt_error	rt_mlx_init(t_rt_mlx_data *mlx)
{
	mlx->rt_mlx = mlx_init();
	if (!mlx->rt_mlx)
		return (rt_errd(RT_ERROR_MLX, "MLX internal application failed."));
	mlx->rt_win = mlx_new_window(mlx->rt_mlx,
			mlx->rt_width, mlx->rt_height, "(mini)rt");
	if (!mlx->rt_win)
	{
		mlx_destroy_display(mlx->rt_mlx);
		return (rt_errd(RT_ERROR_MLX, "MLX window failed."));
	}
	mlx->rt_imgs[0] = mlx_new_image(mlx->rt_mlx, mlx->rt_width, mlx->rt_height);
	mlx->rt_imgs[1] = mlx_new_image(mlx->rt_mlx, mlx->rt_width, mlx->rt_height);
	if (!mlx->rt_imgs[0] || !mlx->rt_imgs[1])
	{
		mlx_destroy_window(mlx->rt_mlx, mlx->rt_win);
		mlx_destroy_display(mlx->rt_mlx);
		return (rt_errd(RT_ERROR_MLX, "MLX image allocation failed."));
	}
	return (rt_ok());
}

t_rt_error	rt_render(t_rt_scene *scene)
{
	t_rt_mlx_data	mlx_data;
	t_rt_error		err;

	(void)scene;
	rt_setup_rendering(scene, &mlx_data);
	err = rt_mlx_init(&mlx_data);
	if (err.type == RT_OK)
		rt_start_rendering(scene, &mlx_data);
	return (err);
}
