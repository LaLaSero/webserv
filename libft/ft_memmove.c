/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutakagi <yutakagi@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/28 19:38:36 by yutakagi          #+#    #+#             */
/*   Updated: 2023/10/13 17:35:02 by yutakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	size_t			i;
	unsigned char	*p;
	unsigned char	*q;

	if (dst == NULL && src == NULL)
		return (NULL);
	p = (unsigned char *) dst;
	q = (unsigned char *) src;
	i = 0;
	if (dst < src)
		dst = ft_memcpy(dst, src, len);
	else
	{
		while (i < len)
		{
			p[len - i - 1] = q[len - i - 1];
			i++;
		}
	}
	return (dst);
}

// #include<stdio.h>
// #include<string.h>
// int main()
// {
// 	char	src[] = "ABCDDEFG";
// 	char	dst[] = "---------";
// 	void *p = (void *) src;
// 	void *q = (void *) dst;	
// 	printf("%s\n",memmove(p+2,p,2));
// 	printf("%s\n",ft_memmove(p+2,p,2));
// }