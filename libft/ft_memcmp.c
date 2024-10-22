/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutakagi <yutakagi@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 17:25:48 by yutakagi          #+#    #+#             */
/*   Updated: 2023/10/12 14:09:07 by yutakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	size_t			i;
	unsigned char	*p;
	unsigned char	*q;

	p = (unsigned char *) s1;
	q = (unsigned char *) s2;
	i = 0;
	while (i < n)
	{
		if (p[i] != q[i])
			return (p[i] - q[i]);
		i++;
	}
	return (0);
}

// #include <string.h>
// #include <stdio.h>
// int main()
// {
// 	char buf1[] = "ABCD5EFGH";
// 	char buf2[] = "ABCF23456";
// 	size_t i = 4;
// 	printf("%d\n",ft_memcmp(buf1,buf2,i));
// 	printf("%d\n",memcmp(buf1,buf2,i));
// }