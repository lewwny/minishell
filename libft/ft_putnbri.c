/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbri.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lengarci <lengarci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 20:29:22 by lenygarcia        #+#    #+#             */
/*   Updated: 2025/06/04 10:10:59 by lengarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_putnbri(int fd, int n)
{
	unsigned int	count;

	count = 0;
	if (n == -2147483648)
	{
		count += ft_putchari(fd, '-');
		count += ft_putchari(fd, '2');
		n = 147483648;
	}
	if (n < 0)
	{
		count += ft_putchari(fd, '-');
		n = n * -1;
	}
	if (n >= 10)
	{
		count += ft_putnbri(fd, n / 10);
		count += ft_putnbri(fd, n % 10);
	}
	else
		count += ft_putchari(fd, n + 48);
	return (count);
}
