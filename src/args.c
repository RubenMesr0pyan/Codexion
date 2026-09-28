/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmesropy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/18 17:14:03 by rmesropy          #+#    #+#             */
/*   Updated: 2026/09/18 18:04:31 by rmesropy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	is_all_digits(const char *s)
{
	int	i;

	if (!s[0])
		return (0);
	i = 0;
	while (s[i])
	{
		if (s[i] < '0' || s[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

static long	str_to_long(const char *s, int *ok)
{
	long	result;
	int		i;

	*ok = is_all_digits(s);
	if (!*ok)
		return (0);
	result = 0;
	i = 0;
	while (s[i])
	{
		if (result > (LONG_MAX - (s[i] - '0')) / 10)
		{
			*ok = 0;
			return (0);
		}
		result = result * 10 + (s[i] - '0');
		i++;
	}
	return (result);
}

int	parse_bounded(const char *s, long *out, long min_value)
{
	int		ok;
	long	value;

	value = str_to_long(s, &ok);
	if (!ok || value < min_value)
		return (0);
	*out = value;
	return (1);
}

int	parse_int_arg(const char *s, int *out, long min_value)
{
	long	value;

	if (!parse_bounded(s, &value, min_value) || value > INT_MAX)
		return (0);
	*out = (int)value;
	return (1);
}

int	parse_times(char **argv, t_config *cfg)
{
	if (!parse_bounded(argv[2], &cfg->time_to_burnout, 0))
		return (0);
	if (!parse_bounded(argv[3], &cfg->time_to_compile, 0))
		return (0);
	if (!parse_bounded(argv[4], &cfg->time_to_debug, 0))
		return (0);
	if (!parse_bounded(argv[5], &cfg->time_to_refactor, 0))
		return (0);
	return (1);
}
