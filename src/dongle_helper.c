/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_helper.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmesropy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/18 17:45:15 by rmesropy          #+#    #+#             */
/*   Updated: 2026/09/18 19:03:41 by rmesropy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <codexion.h>

int	acquire_dongles(t_env *env, t_coder *coder)
{
	t_request	req;

	pthread_mutex_lock(&env->state_lock);
	if (env->stopped)
	{
		pthread_mutex_unlock(&env->state_lock);
		return (0);
	}
	coder->granted = 0;
	build_request(env, coder, &req);
	pqueue_push(env->queue, req);
	try_grant(env);
	if (!wait_for_grant(env, coder))
	{
		pthread_mutex_unlock(&env->state_lock);
		return (0);
	}
	pthread_mutex_unlock(&env->state_lock);
	return (1);
}

void	release_dongles(t_env *env, t_coder *coder)
{
	int		idx;
	long	now;

	idx = coder->id - 1;
	pthread_mutex_lock(&env->state_lock);
	now = get_elapsed_ms(env);
	env->dongles[idx].held = 0;
	env->dongles[idx].released_at_ms = now;
	env->dongles[idx].released = 1;
	env->dongles[(idx + 1) % env->cfg.number_of_coders].held = 0;
	env->dongles[(idx + 1) % env->cfg.number_of_coders].released_at_ms = now;
	env->dongles[(idx + 1) % env->cfg.number_of_coders].released = 1;
	try_grant(env);
	pthread_mutex_unlock(&env->state_lock);
}
