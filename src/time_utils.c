/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmesropy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/18 17:14:22 by rmesropy          #+#    #+#             */
/*   Updated: 2026/09/18 17:14:23 by rmesropy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long	get_elapsed_ms(t_env *env)
{
	struct timeval	now;
	long			sec_diff;
	long			usec_diff;

	gettimeofday(&now, NULL);
	sec_diff = now.tv_sec - env->start_time.tv_sec;
	usec_diff = now.tv_usec - env->start_time.tv_usec;
	return (sec_diff * 1000 + usec_diff / 1000);
}

void	sleep_ms(long ms)
{
	while (ms >= 1000)
	{
		usleep(1000000);
		ms -= 1000;
	}
	if (ms > 0)
		usleep((unsigned int)(ms * 1000));
}
