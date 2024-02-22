/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 07:37:43 by kiroussa          #+#    #+#             */
/*   Updated: 2024/02/22 07:39:35 by kiroussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include <rt/error.h>
# include <rt/scene.h>

t_rt_error	rt_scene_load(t_rt_scene *scene, const char *path);

#endif // PARSER_H
