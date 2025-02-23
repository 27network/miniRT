/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt_render.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 07:51:17 by kiroussa          #+#    #+#             */
/*   Updated: 2024/04/27 13:50:35 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include <string.h>
#include <ft/print.h>
#include <ft/mem.h>
#include <rt/error.h>
#include <rt/renderer.h>

void	rt_start_rendering(t_rt_scene *s, t_rt_mlx_data *m, t_rt_renderer *r)
{
	mlx_on_event(m->rt_mlx, m->rt_win,
		MLX_WINDOW_EVENT, rt_window_event, m);
	mlx_on_event(m->rt_mlx, m->rt_win,
		MLX_KEYDOWN, rt_keydown_event, r);
	mlx_on_event(m->rt_mlx, m->rt_win,
		MLX_KEYUP, rt_keyup_event, r);
	mlx_on_event(m->rt_mlx, m->rt_win,
		MLX_MOUSEDOWN, rt_mousedown_event, r);
	mlx_set_fps_goal(m->rt_mlx, 120);
	mlx_loop_hook(m->rt_mlx, rt_render_update, r);
	rt_scene_example(s);
	mlx_loop(m->rt_mlx);
	mlx_destroy_image(m->rt_mlx, m->rt_imgs[0]);
	mlx_destroy_image(m->rt_mlx, m->rt_imgs[1]);
	mlx_destroy_window(m->rt_mlx, m->rt_win);
	mlx_destroy_display(m->rt_mlx);
}

void	rt_do_rendering(t_rt_renderer *renderer)
{
	if (renderer->status == RT_RS_HOME)
		rt_render_home(renderer);
	else if (renderer->status == RT_RS_SCENE)
		rt_render_scene(renderer);
	else
		rt_render_editor(renderer);
}

t_rt_error	rt_mlx_init(t_rt_scene *s, t_rt_mlx_data *m)
{
	m->rt_mlx = mlx_init();
	if (!m->rt_mlx)
		return (rt_errd(RT_ERROR_MLX, "MLX internal application failed."));
	m->rt_win = mlx_new_window(m->rt_mlx,
			s->width, s->height, "(mini)rt");
	if (!m->rt_win)
	{
		mlx_destroy_display(m->rt_mlx);
		return (rt_errd(RT_ERROR_MLX, "MLX window failed."));
	}
	m->rt_imgs[0] = mlx_new_image(m->rt_mlx, s->width, s->height);
	m->rt_imgs[1] = mlx_new_image(m->rt_mlx, s->width, s->height);
	if (!m->rt_imgs[0] || !m->rt_imgs[1])
	{
		mlx_destroy_window(m->rt_mlx, m->rt_win);
		mlx_destroy_display(m->rt_mlx);
		return (rt_errd(RT_ERROR_MLX, "MLX image allocation failed."));
	}
	return (rt_ok());
}

t_rt_error	rt_renderer_init(t_rt_scene *s, t_rt_mlx_data *m, t_rt_renderer *r)
{
	ft_memset(r->input_map, 0, sizeof(r->input_map));
	r->scene = s;
	r->mlx = m;
	r->status = RT_RS_HOME;
	r->img = ft_calloc(s->width * s->height, sizeof(t_color));
	if (!r->img)
		return (rt_errd(RT_ERROR_ALLOC, strerror(errno)));
	return (rt_ok());
}

t_rt_error	rt_render(t_rt_scene *scene, t_vec2i size)
{
	t_rt_mlx_data	mlx_data;
	t_rt_renderer	renderer;
	t_rt_error		err;

	scene->width = size.x;
	scene->height = size.y;
	err = rt_mlx_init(scene, &mlx_data);
	rt_getmlx(0, &mlx_data);
	err = rt_renderer_init(scene, &mlx_data, &renderer);
	if (err.type == RT_OK)
		rt_start_rendering(scene, &mlx_data, &renderer);
	return (err);
}
