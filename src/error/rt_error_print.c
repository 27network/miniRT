/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt_error_print.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 07:52:04 by kiroussa          #+#    #+#             */
/*   Updated: 2024/02/23 00:32:32 by kiroussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <rt/error.h>
#include <ft/print.h>
#include <stdlib.h>

static void	rt_error_printd(t_rt_error error)
{
	ft_putstr_fd(2, ": ");
	ft_putstr_fd(2, (char *) error.data);
	if (error.type == RT_ERROR_SCENE_PARSE)
		free(error.data);
}

void	rt_error_print(t_rt_error error)
{
	static char	*error_messages[] = {
	[RT_ERROR_ALLOC] = "Memory allocation error",
	[RT_ERROR_FILE_PATH] = "File path error",
	[RT_ERROR_FILE_ACCESS] = "File access error",
	[RT_ERROR_SCENE_PARSE] = "An error occured while parsing the scene file",
	[RT_ERROR_SCENE_VALIDATION] = "Scene validation error",
	[RT_ERROR_SCENE_RENDER] = "Scene rendering error",
	[RT_ERROR_MLX] = "MLX error",
	[RT_ERROR_NULLPTR] = "java.lang.NullPointerException :trollface:"
	};

	if (error.type == RT_OK)
		return ;
	ft_putstr_fd(2, "Error\n");
	if (error.type < RT_OK || error.type > RT_ERROR_NULLPTR)
		ft_putstr_fd(2, "Unknown error type\n");
	else
		ft_putstr_fd(2, error_messages[error.type]);
	if (error.data)
		rt_error_printd(error);
	ft_putstr_fd(2, "\n");
}
