/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dot_vec2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wkorande <wkorande@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/07 15:35:49 by wkorande          #+#    #+#             */
/*   Updated: 2020/03/07 15:36:06 by wkorande         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vector.h"

double	ft_dot_vec2(t_vec2 v1, t_vec2 v2)
{
	double dot;

	dot = (v1.x * v2.x) + (v1.y * v2.y);
	return (dot);
}
