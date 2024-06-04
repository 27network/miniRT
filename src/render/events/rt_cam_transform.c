/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt_cam_transform.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 17:07:34 by rgramati          #+#    #+#             */
/*   Updated: 2024/06/04 13:54:49 by rgramati         ###   ########.fr       */
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
	{
		rt_ray_cast(renderer->scene, &r, &h);
		renderer->scene->rt_flags |= RT_CLICKED;
	}
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
	t_toc_vec2i deltas;

	mlx_mouse_hide();
	if ((coords.x != pos.x || coords.y != pos.y))
	{
		deltas = toc_vec2i(coords.x - pos.x, coords.y - pos.y);
		mlx_mouse_move(renderer->mlx->rt_mlx, renderer->mlx->rt_win, pos.x, pos.y);
		ang->y -= deltas.x * 0.004;
		ang->x -= deltas.y * 0.004;
	}
}

// fcnt rotation_3D(coo_camera, object, delta/*decalage en pixel*/) {
// 	static coo_object;
// 	teta = 0;
// 	mu = 0;

// 	si (object != NULL)
// 		coo_object = object.{x, y, z}
// 	teta += delta.x
// 	mu += delta.y

// 	vecteur = coo_object - coo_camera;
// 	Rayon = vecteur.x ** 2 + vecteur.y ** 2 + vecteur.z ** 2;
// 	Nouveau point = {Rayon*sin(teta)*cos(mu) + coo_camera.x
// 					 Rayon*cos(mu) + coo_camera.y
// 					 Rayon*sin(teta)*sin(mu)	+ coo_camera.z }
// 	Nouveau vecteur directeur camera = coo_object - nouveau_point
// 	angle alpha_beta camera = fonction_angle_depuis_vecteur(Nv_V_Dir_Camera)
// }

void	rt_cam_object_related_rotate(
	t_rt_renderer *renderer,
	t_toc_vec2i coords,
	t_toc_vec2i pos,
	t_vec3d *ang
)	{
	static t_rt_ray	r = {0};
	static t_rt_hit	h = {0};
	t_toc_vec2i 	deltas;
	double			theta;
	double			mu;

	mlx_mouse_hide();
	rt_ray_init(renderer->scene, &r, \
		(t_toc_vec2i){coords.x, HEIGHT - coords.y}, NULL);
	if (!(renderer->scene->rt_flags & RT_CLICKED))
	{
		printf("ANCHOR CLICK\n");
		rt_ray_cast(renderer->scene, &r, &h);
		renderer->scene->rt_flags |= RT_CLICKED;
	}
	if ((coords.x != pos.x || coords.y != pos.y) \
	&& (renderer->scene->rt_flags & RT_CLICKED))
	{
		deltas = toc_vec2i(coords.x - pos.x, coords.y - pos.y);
		mlx_mouse_move(renderer->mlx->rt_mlx, renderer->mlx->rt_win, pos.x, pos.y);

		theta = deltas.x * 0.004;
		mu = deltas.y * 0.004;

		if (h.hit)
		{
			h.pos = ft_vec3d_add(r.origin, ft_vec3d_mult(r.dir, h.dist));
			printf("%6f  :  theta = %6f , mu = %6f\n", h.dist, theta, mu);
			t_vec3d	new_point = ft_vec3d(h.dist * sin(theta) * cos(mu), h.dist * cos(mu), h.dist * sin(theta) * sin(mu));
			printf("[%6f][%6f][%6f]\n", new_point.x, new_point.y, new_point.z);
			new_point = ft_vec3d_add(new_point, h.pos);
			renderer->scene->camera.pos = new_point;
			*ang = rt_obj_camera_angulation(ft_vec3d_sub(h.pos, new_point));
			printf("[%6f][%6f][%6f]\n", renderer->scene->camera.pos.x, renderer->scene->camera.pos.y, renderer->scene->camera.pos.z);
		}
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
		// else if (renderer->input_map[RT_MOUSE_RIGHT])
		// 	rt_cam_object_related_rotate(renderer, coords, pos, cam_angles);
	}
	else 
		mlx_mouse_show();
	if (!renderer->input_map[RT_MOUSE_RIGHT])
		mlx_mouse_get_pos(renderer->mlx->rt_mlx, &pos.x, &pos.y);
	if (!renderer->input_map[RT_MOUSE_LEFT] && !renderer->input_map[RT_MOUSE_RIGHT] && (renderer->scene->rt_flags & RT_CLICKED))
		renderer->scene->rt_flags &= ~RT_CLICKED;
}
