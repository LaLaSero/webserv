/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutakagi <yutakagi@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 17:21:03 by yutakagi          #+#    #+#             */
/*   Updated: 2023/10/12 14:09:21 by yutakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *b, int c, size_t len)
{
	size_t			i;
	unsigned char	*p;

	i = 0;
	p = (unsigned char *) b;
	while (i < len)
	{
		p[i] = (unsigned char) c;
		i++;
	}
	return (b);
}

// #include <stdio.h>
// #include <string.h>
// int main() {
// 	char str[20];
// 	size_t len = 10;
// 	int value = 'A';
// 	ft_memset(str, value, len);
// 	printf("ft_memset result: %s\n", str);
// 	memset(str, value, len);
// 	printf("memset result: %s\n", str);
// 	if (memcmp(str, str, len) == 0) {
// 		printf("match.\n");
// 	} else {
// 		printf("not match.\n");
// 	}
// 	return (0);
// }