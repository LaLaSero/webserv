/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutakagi <yutakagi@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 17:24:49 by yutakagi          #+#    #+#             */
/*   Updated: 2023/10/12 14:10:29 by yutakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	int	len;

	len = ft_strlen(s);
	while (len >= 0)
	{
		if (s[len] == (char)c)
			return (&((char *)s)[len]);
		len--;
	}
	return (NULL);
}

// #include <string.h>
// #include <stdio.h>
// int main(void)
// {
// 	char str[] = "abcdefghijklmnabcdefghijklmn", *p, *q;
// 	char a = 'k';
// 	int c = (int)a;
// 	p = strrchr(str, c);
// 	if (p != NULL) {
// 		printf("%c,%d located\n", c, p-str);
// 		printf("%s\n", p);
// 	}
// 	else
// 		printf("%c not found", c);
// 	q = ft_strrchr(str, c);
// 	if (q != NULL) {
// 		printf("%c,%d located\n", c, q-str);
// 		printf("%s\n", q);
// 	}
// 	else
// 		printf("%c not found\n", c);
// 	return 0;
// }