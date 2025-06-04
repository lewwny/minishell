/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbrui.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lengarci <lengarci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 11:43:08 by lenygarcia        #+#    #+#             */
/*   Updated: 2025/06/04 10:11:54 by lengarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_putnbrui(int fd, unsigned int n)
{
	unsigned int	count;

	count = 0;
	if (n >= 10)
	{
		count += ft_putnbrui(fd, n / 10);
		count += ft_putnbrui(fd, n % 10);
	}
	else
		count += ft_putchari(fd, n + 48);
	return (count);
}
