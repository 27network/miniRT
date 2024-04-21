/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt_render.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 07:51:17 by kiroussa          #+#    #+#             */
/*   Updated: 2024/04/21 20:03:11 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft/print.h>
#include <rt/renderer.h>

void	rt_setup_rendering(t_rt_scene *scene, t_rt_mlx_data *mlx)
{
	scene->width = 1366;
	scene->height = 768;
	rt_getmlx(0, mlx);
}

void	rt_start_rendering(t_rt_scene *scene, t_rt_mlx_data *mlx)
{
	t_rt_renderer		renderer;
	t_rt_render_status	status;

	status = RT_RS_HOME;
	renderer = (t_rt_renderer) {scene, mlx, &status};
	mlx_on_event(mlx->rt_mlx, mlx->rt_win,
		MLX_WINDOW_EVENT, rt_window_event, mlx);
	mlx_on_event(mlx->rt_mlx, mlx->rt_win,
		MLX_KEYDOWN, rt_keydown_event, &renderer);
	mlx_loop_hook(mlx->rt_mlx, rt_render_update, &renderer);
	rt_empty_scene(scene);
	mlx_loop(mlx->rt_mlx);
	mlx_destroy_image(mlx->rt_mlx, mlx->rt_imgs[0]);
	mlx_destroy_image(mlx->rt_mlx, mlx->rt_imgs[1]);
	mlx_destroy_window(mlx->rt_mlx, mlx->rt_win);
	mlx_destroy_display(mlx->rt_mlx);
}

void	rt_do_rendering(t_rt_renderer *renderer)
{
	if (*renderer->status == RT_RS_HOME)
		rt_render_home(renderer);
	else if (*renderer->status == RT_RS_SCENE)
		rt_render_scene(renderer);
	else
		rt_render_editor(renderer);
}

t_rt_error	rt_mlx_init(t_rt_scene *scene, t_rt_mlx_data *mlx)
{
	mlx->rt_mlx = mlx_init();
	if (!mlx->rt_mlx)
		return (rt_errd(RT_ERROR_MLX, "MLX internal application failed."));
	mlx->rt_win = mlx_new_window(mlx->rt_mlx,
			scene->width, scene->height, "(mini)rt");
	if (!mlx->rt_win)
	{
		mlx_destroy_display(mlx->rt_mlx);
		return (rt_errd(RT_ERROR_MLX, "MLX window failed."));
	}
	mlx->rt_imgs[0] = mlx_new_image(mlx->rt_mlx, scene->width, scene->height);
	mlx->rt_imgs[1] = mlx_new_image(mlx->rt_mlx, scene->width, scene->height);
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
	err = rt_mlx_init(scene, &mlx_data);
	if (err.type == RT_OK)
		rt_start_rendering(scene, &mlx_data);
	return (err);
}
