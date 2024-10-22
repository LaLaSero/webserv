/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutakagi <yutakagi@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 13:08:46 by yutakagi          #+#    #+#             */
/*   Updated: 2023/10/12 14:07:54 by yutakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t number, size_t size)
{
	void	*str;
	size_t	total;

	if (size != 0)
	{
		if (number >= SIZE_MAX / size)
			return (NULL);
	}
	total = size * number;
	if (size == 0 || number == 0)
	{
		if (size == 0)
			size = 1;
		total = 1;
	}
	str = (void *) malloc(total);
	if (str == NULL)
		return (NULL);
	ft_bzero(str, total);
	return (str);
}
