/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt_hooks.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 11:10:38 by rgramati          #+#    #+#             */
/*   Updated: 2024/04/30 15:00:43 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft/print.h>
#include <rt/renderer.h>
#include <SDL2/SDL_scancode.h>

static void	rt_input_handle(t_rt_renderer *renderer)
{
	float	speed;

	speed = 0.2f;
	if (renderer->input_map[SDL_SCANCODE_LCTRL])
		speed = 0.7f;
	if (renderer->input_map[SDL_SCANCODE_D])
		rt_scene_translate(renderer->scene, (t_vec3d){-speed, 0.0f, 0.0f});
	if (renderer->input_map[SDL_SCANCODE_A])
		rt_scene_translate(renderer->scene, (t_vec3d){speed, 0.0f, 0.0f});
	if (renderer->input_map[SDL_SCANCODE_SPACE])
		rt_scene_translate(renderer->scene, (t_vec3d){0.0f, -speed, 0.0f});
	if (renderer->input_map[SDL_SCANCODE_LSHIFT])
		rt_scene_translate(renderer->scene, (t_vec3d){0.0f, speed, 0.0f});
	if (renderer->input_map[SDL_SCANCODE_W])
		rt_scene_translate(renderer->scene, (t_vec3d){0.0f, 0.0f, -speed});
	if (renderer->input_map[SDL_SCANCODE_S])
		rt_scene_translate(renderer->scene, (t_vec3d){0.0f, 0.0f, speed});
}

void	rt_clear_image(void *mlx, void *img, t_rt_scene *scene)
{
	int	x;
	int	y;

	x = 0;
	while (x < scene->width / 4)
	{
		y = 0;
		while (y < scene->height / 4)
			mlx_set_image_pixel(mlx, img, x, y++, 0xBF000000);
		x++;
	}
}

int	rt_render_update(void *render)
{
	t_rt_renderer	*renderer;

	renderer = (t_rt_renderer *)render;
	rt_input_handle(renderer);
	rt_clear_image(renderer->mlx->rt_mlx, renderer->mlx->rt_imgs[1], renderer->scene);
	mlx_clear_window(renderer->mlx->rt_mlx, renderer->mlx->rt_win);
	if (renderer->input_map[SDL_SCANCODE_W]
		|| renderer->input_map[SDL_SCANCODE_A]
		|| renderer->input_map[SDL_SCANCODE_S]
		|| renderer->input_map[SDL_SCANCODE_D]
		|| renderer->input_map[SDL_SCANCODE_LCTRL]
		|| renderer->input_map[SDL_SCANCODE_LSHIFT])
	{
		renderer->scene->pratio = 16;
	}
	if (renderer->input_map[SDL_SCANCODE_F])
		renderer->scene->pratio = 4;
	rt_do_rendering(renderer);
	return (0);
}

int	rt_window_event(int key, void *mlx)
{
	t_rt_mlx_data	*mlx_data;

	mlx_data = (t_rt_mlx_data *) mlx;
	if (!key)
	{
		mlx_loop_end(mlx_data->rt_mlx);
		return (0);
	}
	return (0);
}


int rt_keyup_event(int key, void *param)
{
	t_rt_renderer	*renderer;

	renderer = (t_rt_renderer *) param;
	renderer->input_map[key] = 0;
	return (0);
}

int	rt_keydown_event(int key, void *render)
{
	
	t_rt_renderer	*renderer;

	renderer = (t_rt_renderer *)render;
	// rt_clear_image(renderer->mlx->rt_mlx, renderer->mlx->rt_imgs[1], renderer->scene);
	if (renderer->status == RT_RS_NONE)
		renderer->status = RT_RS_HOME;
	if (renderer->status == RT_RS_HOME && key == SDL_SCANCODE_RETURN)
	{
		mlx_clear_window(renderer->mlx->rt_mlx, renderer->mlx->rt_win);
		renderer->status = RT_RS_SCENE;
	}
	if (key == SDL_SCANCODE_ESCAPE)
	{
		mlx_loop_end(renderer->mlx->rt_mlx);
		return (0);
	}

	//pixels !
	if (key == SDL_SCANCODE_PAGEUP)
		renderer->scene->pratio += (renderer->scene->pratio < 32);
	if (key == SDL_SCANCODE_PAGEDOWN)
		renderer->scene->pratio -= (renderer->scene->pratio != 1);
	
	renderer->input_map[key] = 1;

	//debug rays
	if (key == SDL_SCANCODE_R)
		renderer->scene->rt_flags ^= RT_RAY_DEBUG;
	if (key == SDL_SCANCODE_G)
		renderer->scene->rt_flags ^= RT_COL_GAMMA;
	return (0);
}

bool g_debug = false;

t_color	rt_get_random_color(int toclose);

int rt_mousedown_event(int key, void *render)
{
	t_rt_renderer	*renderer;
	t_rt_ray		dray;
	t_rt_hit		dhit;
	int				params[2];

	renderer = (t_rt_renderer *)render;
	mlx_mouse_get_pos(renderer->mlx->rt_mlx, params, params + 1);
	if (key == 1)
	{
		rt_ray_init(renderer->scene, &dray, (t_vec2i){.x = *params, .y = *(params + 1)});
		for (int i = 0; i < 211; i++)
			ft_printf("-");
		ft_printf("\nDEBUG RAY:\n");
		for (int i = 0; i < 211; i++)
			ft_printf("-");
		g_debug = true;
		rt_ray_cast_debug(renderer->scene, &dray, &dhit);
		for (int i = 0; i < 211; i++)
			ft_printf("-");
		ft_printf("\nSHADOW RAY:\n");
		for (int i = 0; i < 211; i++)
			ft_printf("-");
		// rt_color_occlusion(renderer->scene, dhit, ft_vec3d_sub(((t_rt_object) renderer->scene->lights[0]).position, dhit.position));
		g_debug = false;
		dhit.hit_object->color = rt_get_random_color(0);
	}
	if (key == 3)
	{
		rt_ray_init(renderer->scene, &dray, (t_vec2i){.x = *params, .y = *(params + 1)});
		rt_ray_cast_debug(renderer->scene, &dray, &dhit);
		if (dhit.hit_object)
		{
			free(dhit.hit_object->options);
			dhit.hit_object->options = NULL;
		}
	}
	return (0);
}
