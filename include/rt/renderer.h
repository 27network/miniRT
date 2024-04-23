/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   renderer.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 07:50:09 by kiroussa          #+#    #+#             */
/*   Updated: 2024/04/23 20:10:43 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RENDERER_H
# define RENDERER_H

# include <mlx.h>
# include <rt/error.h>
# include <rt/scene.h>

# define RENDER_FONT "assets/fonts/menu.ttf"
# define MAX_BOUNCES 1
# define EPSILON 0.00001f
# define RT_PI 3.14159265358979323846

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
}	t_rt_mlx_data;

typedef struct s_rt_renderer
{
	t_rt_scene			*scene;
	t_rt_mlx_data		*mlx;
	t_rt_render_status	status;
	t_color				*img;
}	t_rt_renderer;

typedef struct s_rt_hit
{
	t_vec3d		position;
	t_rt_object	*hit_object;
	bool		hit;
	double		dist;
}	t_rt_hit;

typedef struct s_rt_ray
{
	t_color	color;
	t_vec3d	origin;
	t_vec3d	direction;
	uint8_t	bounces;
}	t_rt_ray;

void		*rt_getmlx(int what, t_rt_mlx_data *data);

void		rt_do_rendering(t_rt_renderer *renderer);

void		rt_render_home(t_rt_renderer *renderer);

void		rt_render_scene(t_rt_renderer *renderer);

void		rt_render_editor(t_rt_renderer *renderer);

void		rt_start_rendering(t_rt_scene *s, t_rt_mlx_data *m, t_rt_renderer *r);

void		rt_setup_rendering(t_rt_scene *scene, t_rt_mlx_data *mlx_data);

t_rt_error	rt_mlx_init(t_rt_scene *scene, t_rt_mlx_data *mlx_data);

t_rt_error	rt_render(t_rt_scene *scene, t_vec2i size);

int			rt_render_update(void *mlx);

int			rt_window_event(int key, void *mlx);

int			rt_keydown_event(int key, void *mlx);


void		rt_ray_init(t_rt_scene *scene, t_rt_ray *ray, t_vec2i pixs);

void		rt_ray_cast(t_rt_scene *scene, t_rt_ray *ray, t_rt_hit *hit);

bool	rt_hit_update(double t, t_rt_ray ray, t_rt_object *obj, t_rt_hit *hit);

#endif // RENDERER_H
