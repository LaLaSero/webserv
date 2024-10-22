/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalpha.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutakagi <yutakagi@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 14:47:07 by yutakagi          #+#    #+#             */
/*   Updated: 2023/10/12 14:08:02 by yutakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isalpha(int c)
{
	if (('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z'))
		return (1);
	return (0);
}

// #include <ctype.h>
// #include<stdio.h>
// int main()
// {
// 	printf("%d\n",isalpha('A'));
// 	printf("%d\n",isalpha('3'));
// 	printf("%d\n",ft_isalpha('G'));
// 	printf("%d\n",ft_isalpha('3'));
// }
