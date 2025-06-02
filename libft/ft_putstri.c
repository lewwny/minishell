/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstri.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lenygarcia <marvin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 11:29:57 by lenygarcia        #+#    #+#             */
/*   Updated: 2025/04/30 14:27:11 by lenygarcia       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_putstri(char const *s)
{
	int	i;
	int	tmp;

	i = 0;
	if (!s)
		s = "(null)";
	while (s[i])
		i++;
	tmp = write(1, s, i);
	if (tmp < 0)
		return (-1);
	return (i);
}
