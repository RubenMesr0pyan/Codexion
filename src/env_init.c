/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmesropy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/18 17:47:50 by rmesropy          #+#    #+#             */
/*   Updated: 2026/09/18 17:48:22 by rmesropy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <codexion.h>

static int	alloc_size_ok(int count, size_t size)
{
	return ((size_t)count <= (size_t)-1 / size);
}

static int	alloc_env(t_env *env)
{
	int	(*cmp)(t_request a, t_request b);

	if (!alloc_size_ok(env->cfg.number_of_coders, sizeof(t_coder))
		|| !alloc_size_ok(env->cfg.number_of_coders, sizeof(t_dongle))
		|| !alloc_size_ok(env->cfg.number_of_coders, sizeof(t_request)))
		return (0);
	env->coders = malloc(sizeof(t_coder) * env->cfg.number_of_coders);
	env->dongles = malloc(sizeof(t_dongle) * env->cfg.number_of_coders);
	env->scratch = malloc(sizeof(t_request) * env->cfg.number_of_coders);
	if (!env->coders || !env->dongles || !env->scratch)
		return (0);
	if (env->cfg.scheduler == SCHED_FIFO_POLICY)
		cmp = cmp_fifo;
	else
		cmp = cmp_edf;
	env->queue = pqueue_create(env->cfg.number_of_coders, cmp);
	return (env->queue != NULL);
}

static int	init_objects(t_env *env)
{
	int	i;

	i = 0;
	while (i < env->cfg.number_of_coders)
	{
		if (!init_dongle(&env->dongles[i], i))
		{
			env->dongles_initialized = i;
			return (0);
		}
		env->dongles_initialized = i + 1;
		init_coder(&env->coders[i], i + 1, env);
		i++;
	}
	return (1);
}

int	init_env(t_env *env, t_config *cfg)
{
	memset(env, 0, sizeof(*env));
	env->cfg = *cfg;
	env->stopped = (cfg->number_of_compiles_required == 0);
	gettimeofday(&env->start_time, NULL);
	if (!init_locks(env) || !alloc_env(env) || !init_objects(env))
	{
		cleanup_env(env);
		return (0);
	}
	return (1);
}

void	cleanup_env(t_env *env)
{
	int	i;

	if (!env)
		return ;
	i = 0;
	while (i < env->dongles_initialized)
	{
		pthread_mutex_destroy(&env->dongles[i].lock);
		i++;
	}
	if (env->state_cond_initialized)
		pthread_cond_destroy(&env->state_cond);
	if (env->state_lock_initialized)
		pthread_mutex_destroy(&env->state_lock);
	if (env->log_lock_initialized)
		pthread_mutex_destroy(&env->log_lock);
	pqueue_destroy(env->queue);
	free(env->scratch);
	free(env->coders);
	free(env->dongles);
	memset(env, 0, sizeof(*env));
}
