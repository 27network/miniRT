/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt_hooks.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 11:10:38 by rgramati          #+#    #+#             */
/*   Updated: 2024/06/03 17:11:42 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft/math/vector.h"
#include "mlx.h"
#include <rt/renderer.h>
#include <rt/object/camera.h>
#include <SDL2/SDL_scancode.h>
#include <string.h>

void	rt_obj_translate(t_rt_object *obj, t_vec3d move)
{
	obj->pos.x += move.x;
	obj->pos.y += move.y;
	obj->pos.z += move.z;
}

static void	rt_input_handle(t_rt_renderer *renderer)
{
	float	speed;
	t_vec3d	*cam_angles = &((t_rt_obj_camera *)(renderer->scene->camera.options))->angle;
	double	trig[4] = {cos(cam_angles->x), sin(cam_angles->x), cos(cam_angles->y), sin(cam_angles->y)};
	t_vec3d camdir = ft_vec3d_norm(ft_vec3d(-trig[3] * trig[0], trig[1], trig[0] * trig[2]));
	t_vec3d	camright = ft_vec3d_norm(ft_vec3d_cross(camdir, ft_vec3d(0, 1, 0)));
	t_vec3d	camup = ft_vec3d_norm(ft_vec3d_cross(camdir, camright));

	speed = 0.1f;
	if (renderer->input_map[SDL_SCANCODE_LCTRL])
		speed = 0.4f;
	if (!renderer->input_map[RT_MOUSE_LEFT])
	{
		if (renderer->input_map[SDL_SCANCODE_D])
			rt_obj_translate(&(renderer->scene->camera), ft_vec3d_mult(camright, -speed));
		if (renderer->input_map[SDL_SCANCODE_A])
			rt_obj_translate(&(renderer->scene->camera), ft_vec3d_mult(camright, speed));
		if (renderer->input_map[SDL_SCANCODE_SPACE])
			rt_obj_translate(&(renderer->scene->camera), ft_vec3d_mult(camup, -speed));
		if (renderer->input_map[SDL_SCANCODE_LSHIFT])
			rt_obj_translate(&(renderer->scene->camera), ft_vec3d_mult(camup, speed));
		if (renderer->input_map[SDL_SCANCODE_W])
			rt_obj_translate(&(renderer->scene->camera), ft_vec3d_mult(camdir, speed));
		if (renderer->input_map[SDL_SCANCODE_S])
			rt_obj_translate(&(renderer->scene->camera), ft_vec3d_mult(camdir, -speed));
		if (renderer->input_map[SDL_SCANCODE_E])
			cam_angles->y -= 0.1;
		if (renderer->input_map[SDL_SCANCODE_Q])
			cam_angles->y += 0.1;
		if (renderer->input_map[SDL_SCANCODE_T])
			cam_angles->x -= 0.1;
		if (renderer->input_map[SDL_SCANCODE_G])
			cam_angles->x += 0.1;
	}
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
void	DEBUG_rt_ray_cast(t_rt_scene *scene, t_rt_ray *ray, t_rt_hit *hit);

int	rt_render_update(void *render)
{
	t_rt_renderer	*renderer;

	renderer = (t_rt_renderer *)render;
	// renderer->scene->pratio = 1;
	rt_input_handle(renderer);
	rt_clear_image(renderer->mlx->rt_mlx, renderer->mlx->rt_imgs[1], renderer->scene);
	if (renderer->input_map[SDL_SCANCODE_W]
		|| renderer->input_map[SDL_SCANCODE_Q]
		|| renderer->input_map[SDL_SCANCODE_E]
		|| renderer->input_map[SDL_SCANCODE_T]
		|| renderer->input_map[SDL_SCANCODE_G]
		|| renderer->input_map[SDL_SCANCODE_A]
		|| renderer->input_map[SDL_SCANCODE_S]
		|| renderer->input_map[SDL_SCANCODE_D]
		|| renderer->input_map[SDL_SCANCODE_SPACE]
		|| renderer->input_map[SDL_SCANCODE_LCTRL]
		|| renderer->input_map[SDL_SCANCODE_LSHIFT])
	{
		memset(renderer->image, 0, (renderer->scene->height * renderer->scene->width) * sizeof(t_color_norm));
		renderer->rendered = 0;
	}
	if (renderer->input_map[SDL_SCANCODE_F])
	{
		rt_clear_image(renderer->mlx->rt_mlx, renderer->mlx->rt_imgs[0], renderer->scene);
		renderer->scene->pratio = 1;
	}
	rt_cam_handle_transform(renderer);
	rt_do_rendering(renderer);
	return (0);
}

int	rt_window_event(int key, void *render)
{
	t_rt_renderer	*renderer;
	size_t			i;

	i = RT_MOUSE;
	renderer = (t_rt_renderer *)render;
	if (!key)
	{
		mlx_loop_end(renderer->mlx->rt_mlx);
		return (0);
	}
	if (key == 4)
		renderer->input_map[65534] = 1;
	if (key == 6 || key == 7)
	{
		while (++i <= RT_MOUSE_MID)
			renderer->input_map[i] = 0;
		renderer->input_map[65534] = 0;
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

int rt_mouseup_event(int key, void *render)
{
	t_rt_renderer	*renderer;
	t_toc_vec2i			coords;

	(void) key;
	renderer = (t_rt_renderer *)render;
	mlx_mouse_get_pos(renderer->mlx->rt_mlx, &coords.x, &coords.y);
	renderer->input_map[RT_MOUSE + key] = 0;
	return (0);
}

int rt_mousedown_event(int key, void *render)
{
	t_rt_renderer	*renderer;

	(void) key;
	renderer = (t_rt_renderer *)render;
	renderer->input_map[RT_MOUSE + key] = 1;
	return (0);
}
