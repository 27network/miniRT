/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt_cam_transform.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 17:07:34 by rgramati          #+#    #+#             */
/*   Updated: 2024/06/03 17:26:23 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <rt/renderer.h>
#include <rt/object/camera.h>
#include <SDL2/SDL_scancode.h>
#include <string.h>

void	rt_cam_viewport_parallel_inter(
	t_rt_renderer *renderer,
	t_rt_hit h,
	t_rt_ray r,
	t_vec3d camdir
)	{
	t_vec3d	offsets;
	double	dist;

	dist = ft_vec3d_dot(ft_vec3d_mult(camdir, -1), r.dir);
	if (ft_fabs(dist) > EPSILON)
	{
		dist = ft_vec3d_dot(ft_vec3d_sub(h.pos, r.origin), \
						ft_vec3d_mult(camdir, -1)) / dist;
		if (ft_fabs(dist) > EPSILON && dist >= 0)
		{
			offsets = ft_vec3d_sub(h.pos, \
		ft_vec3d_add(r.origin, ft_vec3d_mult(r.dir, dist)));
			rt_obj_translate(&(renderer->scene->camera), offsets);
		}
	}
}

void	rt_cam_viewport_parallel_move(
	t_rt_renderer *renderer,
	t_toc_vec2i coords,
	t_toc_vec2i pos,
	t_vec3d ang
)	{
	const double	tr[4] = {cos(ang.x), sin(ang.x), cos(ang.y), sin(ang.y)};
	const t_vec3d	camdir = ft_vec3d_norm(ft_vec3d(-tr[3] * tr[0], \
		tr[1], \
		tr[0] * tr[2]));
	static t_rt_ray	r = {0};
	static t_rt_hit	h = {0};

	rt_ray_init(renderer->scene, &r, \
		(t_toc_vec2i){coords.x, HEIGHT - coords.y}, NULL);
	if (!(renderer->scene->rt_flags & RT_CLICKED))
		rt_ray_cast(renderer->scene, &r, &h);
	if (!(renderer->scene->rt_flags & RT_CLICKED))
		renderer->scene->rt_flags |= 0b10000;
	if ((coords.x != pos.x || coords.y != pos.y) \
	&& (renderer->scene->rt_flags & RT_CLICKED))
		rt_cam_viewport_parallel_inter(renderer, h, r, camdir);
}

void	rt_cam_self_related_rotate(
	t_rt_renderer *renderer,
	t_toc_vec2i coords,
	t_toc_vec2i pos,
	t_vec3d *ang
)	{
	mlx_mouse_hide();
	if ((coords.x != pos.x || coords.y != pos.y))
	{
		t_toc_vec2i deltas = toc_vec2i(coords.x - pos.x, coords.y - pos.y);
		mlx_mouse_move(renderer->mlx->rt_mlx, renderer->mlx->rt_win, pos.x, pos.y);
		ang->y -= deltas.x * 0.004;
		ang->x -= deltas.y * 0.004;
	}
}

void	rt_cam_handle_transform(t_rt_renderer *renderer)
{
	static t_toc_vec2i	pos;
	t_toc_vec2i			coords;
	t_vec3d				*cam_angles = &((t_rt_obj_camera *)(renderer->scene->camera.options))->angle;

	if (renderer->input_map[RT_MOUSE_LEFT] || renderer->input_map[RT_MOUSE_RIGHT])
	{
		mlx_mouse_get_pos(renderer->mlx->rt_mlx, &coords.x, &coords.y);
		if (renderer->input_map[RT_MOUSE_LEFT])
			rt_cam_viewport_parallel_move(renderer, coords, pos, *cam_angles);
		else if (renderer->input_map[RT_MOUSE_RIGHT])
			rt_cam_self_related_rotate(renderer, coords, pos, cam_angles);
	}
	else 
		mlx_mouse_show();
	if (!renderer->input_map[RT_MOUSE_RIGHT])
		mlx_mouse_get_pos(renderer->mlx->rt_mlx, &pos.x, &pos.y);
	if (!renderer->input_map[RT_MOUSE_LEFT] && (renderer->scene->rt_flags & RT_CLICKED))
		renderer->scene->rt_flags &= 0b01111;
}
