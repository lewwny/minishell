/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbrhexmaj.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macauchy <macauchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 11:49:00 by lenygarcia        #+#    #+#             */
/*   Updated: 2025/06/10 11:15:34 by macauchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_putnbrhexmaj(int fd, int n)
{
	size_t		count;
	size_t		nb;
	char				*hex;

	hex = "0123456789ABCDEF";
	count = 0;
	if (n < 0)
		nb = 4294967295 + n + 1;
	else
		nb = n;
	if (nb >= 16)
	{
		count += ft_putnbrhexmaj(fd, nb / 16);
		count += ft_putnbrhexmaj(fd, nb % 16);
	}
	else
		count += ft_putchari(fd, hex[nb]);
	return (count);
}
/*
int	main(void)
{
	ft_putnbrhexmin(322006);
}*/
