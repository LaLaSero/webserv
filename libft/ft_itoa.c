/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutakagi <yutakagi@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/28 19:38:28 by yutakagi          #+#    #+#             */
/*   Updated: 2023/10/16 18:14:54 by yutakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	count_digit(int n)
{
	size_t	len;

	if (n == 0)
		return (1);
	len = 0;
	if (n < 0)
	{
		n *= -1;
		len++;
	}
	while (n > 0)
	{
		len++;
		n /= 10;
	}
	return (len);
}

static char	*int_to_str(int n, char *str, int len)
{
	int	i;
	int	digit;

	i = 0;
	if (n < 0)
	{
		str[i++] = '-';
		n *= -1;
	}
	if (n == 0)
		str[i++] = '0';
	else
	{
		i = 0;
		while (n > 0)
		{
			digit = n % 10;
			str[len - 1 - i] = digit + '0';
			n /= 10;
			i++;
		}
	}
	str[len] = '\0';
	return (str);
}

char	*ft_itoa(int n)
{
	char	*str;
	size_t	len;

	if (n == INT_MIN)
		return (ft_strdup("-2147483648"));
	len = count_digit(n);
	str = (char *) malloc(sizeof(char) * len + 1);
	if (str == NULL)
		return (NULL);
	ft_memset(str, '\0', len + 1);
	return (int_to_str(n, str, len));
}

// int main()
// {
// 	char *str = ft_itoa(-2147);
// 	printf("str =%s\n", str);
// 	free(str);
// }