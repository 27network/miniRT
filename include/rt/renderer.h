/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   renderer.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 07:50:09 by kiroussa          #+#    #+#             */
/*   Updated: 2024/05/29 12:42:43 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RENDERER_H
# define RENDERER_H

#include <math.h>
# include <mlx.h>
# include <ft/math.h>
# include <ft/mem.h>
# include <ft/math/vector.h>
# include <ft/math/matrix.h>
# include <rt/error.h>
# include <rt/scene.h>
# include <stdbool.h>

# define RENDER_FONT "assets/fonts/menu.ttf"

# ifndef MAX_BOUNCES
#  define MAX_BOUNCES 200
# endif

# ifndef MAX_SEQ_PASSES
#  define MAX_SEQ_PASSES 20
# endif

# ifndef RAY_PER_PIXEL
#  define RAY_PER_PIXEL 2
# endif

# define EPSILON 0.00001
# define RT_PI 3.1415926535

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
	char				input_map[65535];
	t_color_norm		*image;
	t_color_norm		*calc;
	size_t				rendered;
}	t_rt_renderer;

typedef struct s_rt_hit
{
	t_vec3d		pos;
	t_rt_object	*obj;
	bool		hit;
	double		dist;
}	t_rt_hit;

typedef struct s_rt_ray
{
	t_color_norm	color;
	t_vec3d			origin;
	t_vec3d			dir;
	uint8_t			bounces;
	// t_rt_hit	*hits; //TODO: Optimiser avec une define constants MAX_REBONDS truc du genre
}	t_rt_ray;

void		*rt_getmlx(int what, t_rt_mlx_data *data);

void		rt_do_rendering(t_rt_renderer *renderer);

void		rt_render_home(t_rt_renderer *renderer);

void		rt_render_scene(t_rt_renderer *renderer);

void		rt_start_rendering(t_rt_mlx_data *m, t_rt_renderer *r);

void		rt_setup_rendering(t_rt_scene *scene, t_rt_mlx_data *mlx_data);




t_rt_error	rt_mlx_init(t_rt_scene *scene, t_rt_mlx_data *mlx_data);

t_rt_error	rt_render(t_rt_scene *scene, t_toc_vec2i size);

int			rt_render_update(void *mlx);




int			rt_window_event(int key, void *param);

int			rt_keydown_event(int key, void *param);

int			rt_keyup_event(int key, void *param);

int			rt_mousedown_event(int key, void *param);

int			rt_mouseup_event(int key, void *param);

t_color_norm	rt_ray_loop(t_rt_renderer *renderer, t_rt_ray ray);


void		rt_ray_init(t_rt_scene *scene, t_rt_ray *ray, t_toc_vec2i pixs);

void		rt_ray_cast(t_rt_scene *scene, t_rt_ray *ray, t_rt_hit *hit);

void		rt_ray_cast_debug(t_rt_scene *scene, t_rt_ray *ray, t_rt_hit *hit);

void		rt_trace_line(t_rt_renderer *renderer, t_toc_vec2i start, t_toc_vec2i end, t_color color);

void		rt_ray_render(t_rt_renderer *renderer, t_rt_ray ray, t_rt_hit hit,
				t_color color);

t_color_norm	rt_get_color_debug(t_rt_renderer *renderer, t_toc_vec2i coords);

bool		rt_color_occlusion(t_rt_scene *scene, t_rt_hit hit, t_vec3d light_dir, t_vec3d norm);

t_vec3d		ft_vec3d_random(long long *state);

double		ft_smoothstep(double start, double end, double x);

t_vec3d		ft_vec3d_lerp(t_vec3d a, t_vec3d b, double t);

void	rt_render_shoot_pixel(t_rt_renderer *renderer, t_toc_vec2i coords);

#endif // RENDERER_H
