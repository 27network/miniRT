/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt_parse_error.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 22:29:10 by kiroussa          #+#    #+#             */
/*   Updated: 2024/05/27 18:42:58 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft/colors.h>
#include <ft/print.h>
#include <ft/string.h>
#include <rt/parse/parser.h>

#define B_LBLUE "\e[1;94m"

/**
 * An error occured while parsing the scene file: 
 *  --> scenes/invalid/invalid-char.rt:3:4
 *   |
 * 3 | cy A50.0,0.0,20.6 0,0,1.0 14.2 21.42 10,0,255
 *   |    ^ Invalid character, expected a number
 */
t_rt_error	rt_parse_error(
		const char *file,
		t_toc_vec2i pos,
		const char *line,
		const char *message
) {
	char	*lines[4];
	char	*final;
	int		line_len;
	int		n_line;
	int		n_col;

	n_line = pos.y;
	n_col = pos.x;
	line_len = ft_lllen(n_line);
	lines[0] = ft_format(B_LBLUE "\n --> "RESET"%s:%d:%d", file, n_line, n_col);
	lines[1] = ft_format(B_LBLUE "%*s |" RESET, line_len, "");
	lines[2] = ft_format(B_LBLUE "%d |" RESET " %s", n_line, line);
	lines[3] = ft_format(B_LBLUE "%*s |" RESET B_RED " %*s^ %s\n" RESET,
			line_len, "", n_col, "", message);
	final = ft_strjoins(4, "\n", 0b1111, lines[0], lines[1], lines[2], lines[3]);
	return (rt_errd(RT_ERROR_SCENE_PARSE, final));
}
