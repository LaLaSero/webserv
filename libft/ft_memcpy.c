/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutakagi <yutakagi@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 16:34:52 by yutakagi          #+#    #+#             */
/*   Updated: 2023/10/12 14:09:10 by yutakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dst, const void *src, size_t len)
{
	size_t			i;
	unsigned char	*p;
	unsigned char	*q;

	if (dst == NULL && src == NULL)
		return (NULL);
	p = (unsigned char *) dst;
	q = (unsigned char *) src;
	i = 0;
	while (i < len)
	{
		p[i] = q[i];
		i++;
	}
	return (dst);
}
