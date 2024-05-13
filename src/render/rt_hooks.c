/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt_hooks.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 11:10:38 by rgramati          #+#    #+#             */
/*   Updated: 2024/05/12 19:05:07 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft/print.h>
#include <rt/renderer.h>
#include <SDL2/SDL_scancode.h>

// static void	rt_obj_rotate_y(t_vec3d *direction, double angle)
// {
// 	const double	c = cos(angle);
// 	const double	s = sin(angle);
	
// 	direction->x = direction->x * c + direction->z * s;
// 	direction->z = direction->x * -s + direction->z * c;
// }

// static void	rt_obj_rotate_x(t_vec3d *direction, double angle)
// {
// 	const double	c = cos(angle);
// 	const double	s = sin(angle);
	
// 	direction->x = direction->x * c + direction->y * s;
// 	direction->y = direction->x * -s + direction->y * c;
// }

// static void	rt_obj_rotate_z(t_vec3d *direction, double angle)
// {
// 	const double	c = cos(angle);
// 	const double	s = sin(angle);
	
// 	direction->y = direction->y * c + direction->z * s;
// 	direction->z = direction->y * -s + direction->z * c;
// }

static void	rt_obj_translate(t_rt_object *obj, t_vec3d move)
{
	obj->position.x += move.x;
	obj->position.y += move.y;
	obj->position.z += move.z;
	ft_printf("CAM POS = [%4f %4f %4f]\n", obj->position.x, obj->position.y, obj->position.z);
}

static void	rt_input_handle(t_rt_renderer *renderer)
{
	float	speed;

	speed = 0.1f;
	if (renderer->input_map[SDL_SCANCODE_LCTRL])
		speed = 0.4f;
	if (renderer->input_map[SDL_SCANCODE_D])
		rt_obj_translate(&(renderer->scene->camera), (t_vec3d){speed, 0.0f, 0.0f});
	if (renderer->input_map[SDL_SCANCODE_A])
		rt_obj_translate(&(renderer->scene->camera), (t_vec3d){-speed, 0.0f, 0.0f});
	if (renderer->input_map[SDL_SCANCODE_SPACE])
		rt_obj_translate(&(renderer->scene->camera), (t_vec3d){0.0f, speed, 0.0f});
	if (renderer->input_map[SDL_SCANCODE_LSHIFT])
		rt_obj_translate(&(renderer->scene->camera), (t_vec3d){0.0f, -speed, 0.0f});
	if (renderer->input_map[SDL_SCANCODE_W])
		rt_obj_translate(&(renderer->scene->camera), (t_vec3d){0.0f, 0.0f, speed});
	if (renderer->input_map[SDL_SCANCODE_S])
		rt_obj_translate(&(renderer->scene->camera), (t_vec3d){0.0f, 0.0f, -speed});
	if (renderer->input_map[SDL_SCANCODE_E])
		renderer->scene->camera.rotation.x += 0.1;
	if (renderer->input_map[SDL_SCANCODE_Q])
		renderer->scene->camera.rotation.x -= 0.1;
	// renderer->scene->camera.rotation.x = fmod(renderer->scene->camera.rotation.x, RT_PI * 2.);
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
	// renderer->scene->pratio = 1;
	rt_input_handle(renderer);
	rt_clear_image(renderer->mlx->rt_mlx, renderer->mlx->rt_imgs[1], renderer->scene);
	if (renderer->input_map[SDL_SCANCODE_W]
		|| renderer->input_map[SDL_SCANCODE_A]
		|| renderer->input_map[SDL_SCANCODE_S]
		|| renderer->input_map[SDL_SCANCODE_D]
		|| renderer->input_map[SDL_SCANCODE_LCTRL]
		|| renderer->input_map[SDL_SCANCODE_LSHIFT])
	{
		memset(renderer->image, 0, (renderer->scene->height * renderer->scene->width) * sizeof(t_color_norm));
		renderer->rendered = 0;
		// renderer->scene->pratio = 5;
	}
	if (renderer->input_map[SDL_SCANCODE_F])
	{
		rt_clear_image(renderer->mlx->rt_mlx, renderer->mlx->rt_imgs[0], renderer->scene);
		renderer->scene->pratio = 1;
	}
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
	if (key == SDL_SCANCODE_K)
		renderer->scene->rt_flags ^= RT_SEQ_RENDER;
	if (key == SDL_SCANCODE_J)
		renderer->scene->rt_flags ^= RT_NO_RENDER;
	if (key == SDL_SCANCODE_G)
		renderer->scene->rt_flags ^= RT_COL_GAMMA;
	return (0);
}

int g_debug = false;

int rt_mousedown_event(int key, void *render)
{
	t_rt_renderer	*renderer;
	t_vec2i			coords;

	(void) key;
	renderer = (t_rt_renderer *)render;
	mlx_mouse_get_pos(renderer->mlx->rt_mlx, &coords.x, &coords.y);
	if (key == 1)
	{
		g_debug = true;
		rt_render_shoot_pixel(renderer, coords);
		g_debug = false;
	}
	return (0);
}
