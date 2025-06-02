/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saalarco <saalarco@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/25 09:19:59 by saalarco          #+#    #+#             */
/*   Updated: 2024/01/25 09:20:07 by saalarco         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// #include <stdio.h>
// ft_strlen
// ft_strlcpy

int	calc_slen(int number)
{
	int	len;

	len = 0;
	if (number == INT_MIN)
		return (11);
	if (number < 0)
	{
		len++;
		number = -number;
	}
	while (number >= 10)
	{
		number /= 10;
		len++;
	}
	len = len + 1;
	return (len);
}

/*
Casos 0 e INT_MIN
*/

char	*case_limits(int number, char *str)
{
	if (number == 0)
		str[0] = '0';
	else if (number == INT_MIN)
		ft_strlcpy (str, "-2147483648", 12);
	return (str);
}

/*
Si no son los casos límites (0 o MIN)
construye de atrás alante el string usando %
*/

char	*inttonumber(int n, int slen, char *s)
{
	int	npaso;

	npaso = 0;
	if (n < 0)
	{
		n = -n;
		s[0] = '-';
	}
	npaso = n;
	s[slen] = '\0';
	if (n == 0 || n == INT_MIN)
		return (case_limits(n, s));
	while (npaso > 0)
	{
		s[slen - 1] = (char)(npaso % 10) + '0';
		npaso = npaso / 10;
		slen--;
	}
	return (s);
}

/*
Reserva memoria y devuelve
inttonumber
*/

char	*ft_itoa(int n)
{
	char	*s;
	int		slen;

	slen = calc_slen(n);
	s = (char *) malloc((slen + 1) * sizeof(char));
	if (s == NULL)
		return (NULL);
	return (inttonumber(n, slen, s));
}

/*
Devuelve: 
La string que represente el número.
NULL si falla la reserva de memoria
*/
/*
Descripción: 
Utilizando malloc(3), genera una string que
represente el valor entero recibido como argumento.
Los números negativos tienen que gestionarse.
*/
// int main()
// {
// 	puts(ft_itoa(-2147483648LL));
// 	puts(ft_itoa(-218));
// 	return 0;
// }
