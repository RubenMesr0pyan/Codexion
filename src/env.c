/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmesropy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/18 17:14:12 by rmesropy          #+#    #+#             */
/*   Updated: 2026/09/18 18:27:19 by rmesropy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	all_compiled_locked(t_env *env)
{
	int	i;

	i = 0;
	while (i < env->cfg.number_of_coders)
	{
		if (env->coders[i].compiles_done
			< env->cfg.number_of_compiles_required)
			return (0);
		i++;
	}
	return (1);
}

int	simulation_stopped(t_env *env)
{
	int	stopped;

	pthread_mutex_lock(&env->state_lock);
	stopped = env->stopped;
	pthread_mutex_unlock(&env->state_lock);
	return (stopped);
}

void	stop_simulation(t_env *env)
{
	if (!env->state_lock_initialized)
		return ;
	pthread_mutex_lock(&env->state_lock);
	env->stopped = 1;
	if (env->queue)
		env->queue->size = 0;
	pthread_cond_broadcast(&env->state_cond);
	pthread_mutex_unlock(&env->state_lock);
}

void	mark_compile_done(t_env *env, t_coder *coder)
{
	pthread_mutex_lock(&env->state_lock);
	coder->compiles_done++;
	if (all_compiled_locked(env))
	{
		env->stopped = 1;
		env->queue->size = 0;
		pthread_cond_broadcast(&env->state_cond);
	}
	pthread_mutex_unlock(&env->state_lock);
}

int	init_dongle(t_dongle *d, int id)
{
	d->id = id;
	d->held = 0;
	d->released_at_ms = 0;
	d->released = 0;
	return (pthread_mutex_init(&d->lock, NULL) == 0);
}
