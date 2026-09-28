/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logger.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmesropy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/18 17:14:14 by rmesropy          #+#    #+#             */
/*   Updated: 2026/09/18 17:14:15 by rmesropy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	log_action(t_env *env, int coder_id, const char *msg)
{
	long	ts;

	pthread_mutex_lock(&env->log_lock);
	ts = get_elapsed_ms(env);
	printf("%ld %d %s\n", ts, coder_id, msg);
	fflush(stdout);
	pthread_mutex_unlock(&env->log_lock);
}
