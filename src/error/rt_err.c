/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt_err.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 07:22:13 by kiroussa          #+#    #+#             */
/*   Updated: 2024/02/22 07:22:21 by kiroussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <rt/error.h>
#include <stddef.h>

t_rt_error	rt_err(t_rt_error_type type)
{
	return ((t_rt_error){.type = type, .data = NULL});
}
