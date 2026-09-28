/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmesropy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/18 17:14:17 by rmesropy          #+#    #+#             */
/*   Updated: 2026/09/18 17:40:01 by rmesropy         ###   ########.fr       */
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

static int	find_burned_out_locked(t_env *env)
{
	int		i;
	long	elapsed;

	i = 0;
	while (i < env->cfg.number_of_coders)
	{
		elapsed = get_elapsed_ms(env) - env->coders[i].last_compile_start_ms;
		if (elapsed >= env->cfg.time_to_burnout)
			return (env->coders[i].id);
		i++;
	}
	return (0);
}

static void	update_simulation_state(t_env *env, int *burned_id, int *stop_all)
{
	pthread_mutex_lock(&env->state_lock);
	*burned_id = find_burned_out_locked(env);
	*stop_all = (!*burned_id && all_compiled_locked(env));
	if (!*burned_id && !*stop_all)
		try_grant(env);
	if (*burned_id || *stop_all)
	{
		env->stopped = 1;
		if (env->queue)
			env->queue->size = 0;
		pthread_cond_broadcast(&env->state_cond);
	}
	pthread_mutex_unlock(&env->state_lock);
}

void	*monitor_routine(void *arg)
{
	t_env	*env;
	int		burned_id;
	int		stop_all;

	env = (t_env *)arg;
	while (!simulation_stopped(env))
	{
		update_simulation_state(env, &burned_id, &stop_all);
		if (burned_id)
		{
			log_action(env, burned_id, "burned out");
			return (NULL);
		}
		if (stop_all)
			return (NULL);
		usleep(500);
	}
	return (NULL);
}
