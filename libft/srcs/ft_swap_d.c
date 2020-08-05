/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_swap_d.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wkorande <wkorande@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/12 19:22:42 by wkorande          #+#    #+#             */
/*   Updated: 2020/01/12 19:22:51 by wkorande         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

void	ft_swap_d(double *d1, double *d2)
{
	double t;

	t = *d1;
	*d1 = *d2;
	*d2 = t;
}
