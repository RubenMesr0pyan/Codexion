/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmesropy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/18 17:14:08 by rmesropy          #+#    #+#             */
/*   Updated: 2026/09/18 17:38:14 by rmesropy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	begin_compile(t_env *env, t_coder *coder)
{
	pthread_mutex_lock(&env->state_lock);
	if (env->stopped)
	{
		pthread_mutex_unlock(&env->state_lock);
		return (0);
	}
	coder->last_compile_start_ms = get_elapsed_ms(env);
	log_action(env, coder->id, "is compiling");
	pthread_mutex_unlock(&env->state_lock);
	return (1);
}

static int	perform_compile(t_env *env, t_coder *coder)
{
	if (!acquire_dongles(env, coder))
		return (0);
	if (!begin_compile(env, coder))
	{
		release_dongles(env, coder);
		return (0);
	}
	sleep_ms(env->cfg.time_to_compile);
	mark_compile_done(env, coder);
	release_dongles(env, coder);
	return (1);
}

void	*coder_routine(void *arg)
{
	t_coder	*coder;
	t_env	*env;

	coder = (t_coder *)arg;
	env = coder->env;
	while (!simulation_stopped(env))
	{
		if (!perform_compile(env, coder))
			break ;
		if (simulation_stopped(env))
			break ;
		log_action(env, coder->id, "is debugging");
		sleep_ms(env->cfg.time_to_debug);
		if (simulation_stopped(env))
			break ;
		log_action(env, coder->id, "is refactoring");
		sleep_ms(env->cfg.time_to_refactor);
	}
	return (NULL);
}
