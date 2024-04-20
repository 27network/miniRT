/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 07:16:58 by kiroussa          #+#    #+#             */
/*   Updated: 2024/04/18 19:35:50 by kiroussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERROR_H
# define ERROR_H

typedef enum e_rt_error_type
{
	RT_OK = 0,
	RT_ERROR_ALLOC,
	RT_ERROR_FILE_PATH,
	RT_ERROR_FILE_ACCESS,
	RT_ERROR_SCENE_PARSE,
	RT_ERROR_SCENE_VALIDATION,
	RT_ERROR_SCENE_RENDER,
	RT_ERROR_MLX,
	RT_ERROR_NULLPTR,
	RT_ERROR_UNIMPLEMENTED
}	t_rt_error_type;

typedef struct s_rt_error
{
	t_rt_error_type	type;
	void			*data;
}	t_rt_error;

void		rt_error_print(t_rt_error error);

t_rt_error	rt_ok(void);
t_rt_error	rt_err(t_rt_error_type type);
t_rt_error	rt_errd(t_rt_error_type type, void *data);

#endif // ERROR_H
