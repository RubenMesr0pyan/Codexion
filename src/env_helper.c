/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_helper.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmesropy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/18 17:49:06 by rmesropy          #+#    #+#             */
/*   Updated: 2026/09/18 18:34:15 by rmesropy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <codexion.h>

int	init_locks(t_env *env)
{
	if (pthread_mutex_init(&env->log_lock, NULL) != 0)
		return (0);
	env->log_lock_initialized = 1;
	if (pthread_mutex_init(&env->state_lock, NULL) != 0)
		return (0);
	env->state_lock_initialized = 1;
	if (pthread_cond_init(&env->state_cond, NULL) != 0)
		return (0);
	env->state_cond_initialized = 1;
	return (1);
}
