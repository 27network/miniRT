/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 02:29:41 by kiroussa          #+#    #+#             */
/*   Updated: 2024/06/01 19:30:09 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <tocard/ui.h>
#include <ft/print.h>
#include <rt/error.h>
#include <rt/parse/parser.h>
#include <rt/renderer.h>
#include <rt/scene.h>


static void	init(t_toc_screen *screen)
{
	const t_toc_vec2i	buttons_size = toc_vec2i(250, 30);

	toc_screen_add(screen, toc_button(1, toc_vec2i(WIDTH / 2 - buttons_size.x / 2, HEIGHT / 2), buttons_size, "Exit"));
}

static bool	render(t_toc_screen *screen, __attribute__((unused)) t_toc_vec2i mouse_pos)
{
	static t_toc_vec2i	origin = {0, 0};
	const t_toc_vec2i	size = toc_vec2i(screen->width, screen->height);
	t_toc_color			color;

	if (screen->window->focused)
		color = toc_color(TOCARD_DEFAULT_BG);
	else
		color = toc_color(TOCARD_DEFAULT_BG_UNFOCUSED);
	toc_draw_rect(screen, origin, size, color);
	return (true);
}

static void	action_performed(
	t_toc_screen *screen,
	t_toc_elem *elem
) {
	if (elem->id == 1)
		toc_engine_exit(screen->window->engine);
}

t_toc_screen_def	rt_default_screen(void)
{
	static t_toc_screen_def	def = {0};
	static bool				initialized = false;

	if (!initialized)
	{
		def.id = "rt:main_menu";
		def.init = init;
		def.render = render;
		def.action = action_performed;
		initialized = true;
	}
	return (def);
}

int	main(int argc, char *argv[])
{
	// t_toc_engine	*engine;
	// t_toc_window	*window;
	t_rt_scene	scene;
	t_rt_error	err;

	if (argc != 2)
	{
		ft_dprintf(2, "Error\nUsage: %s <path/to/scene.rt>\n", argv[0]);
		return (-1);
	}
	err = rt_scene_init(&scene);
	err = rt_parse_scene(&scene, (const char *) argv[1]);
	// if (err.type == RT_OK)
	// {
	// 	engine = toc_engine_create();
	// 	if (!engine)
	// 		err = rt_err(RT_ERROR_ALLOC);
	// 	if (err.type == RT_OK)
	// 	{
	// 		window = toc_window_create(engine, "miniRT", WIDTH, HEIGHT);
	// 		if (!window)
	// 			err = rt_err(RT_ERROR_ALLOC);
	// 		toc_window_set_screen(window, rt_default_screen(), NULL);
	// 	}
	// 	if (err.type == RT_OK)
	// 		toc_engine_await(engine);
	// 	if (err.type == RT_OK)
	// 		toc_engine_destroy(engine);
	// }
	if (err.type == RT_OK)
		err = rt_render(&scene, (t_toc_vec2i){WIDTH, HEIGHT});
	if (err.type != RT_OK)
		rt_error_print(err);
	rt_scene_free(&scene);
	return (err.type);
}
