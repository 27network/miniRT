/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 02:29:41 by kiroussa          #+#    #+#             */
/*   Updated: 2024/04/21 18:48:11 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft/print.h>
#include <rt/error.h>
#include <rt/parse/parser.h>
#include <rt/renderer.h>
#include <rt/scene.h>

int	main(int argc, char *argv[])
{
	t_rt_scene	scene;
	t_rt_error	err;

	if (argc != 2)
	{
		ft_dprintf(2, "Error\nUsage: %s <path/to/scene.rt>\n", argv[0]);
		return (-1);
	}
	err = rt_scene_init(&scene);
	err = rt_parse_scene(&scene, (const char *) argv[1]);
	if (err.type == RT_OK)
		err = rt_render(&scene);
	if (err.type != RT_OK)
		rt_error_print(err);
	rt_scene_free(&scene);
	return (err.type);
}
