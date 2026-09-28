/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmesropy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/18 17:42:39 by rmesropy          #+#    #+#             */
/*   Updated: 2026/09/18 19:04:08 by rmesropy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <codexion.h>

static void	grant_to(t_env *env, int coder_id)
{
	int	idx;

	idx = coder_id - 1;
	env->dongles[idx].held = 1;
	env->dongles[(idx + 1) % env->cfg.number_of_coders].held = 1;
	env->coders[idx].granted = 1;
	log_action(env, coder_id, "has taken a dongle");
	log_action(env, coder_id, "has taken a dongle");
	pthread_cond_broadcast(&env->state_cond);
}

void	try_grant(t_env *env)
{
	int			scratch_n;
	t_request	req;
	long		now;

	if (env->stopped)
		return ;
	scratch_n = 0;
	while (!pqueue_is_empty(env->queue))
	{
		req = pqueue_pop(env->queue);
		now = get_elapsed_ms(env);
		if (now >= req.deadline_ms || blocked_by_earlier(env, req, scratch_n)
			|| !dongle_pair_free(env, req.coder_id))
			env->scratch[scratch_n++] = req;
		else
			grant_to(env, req.coder_id);
	}
	while (scratch_n > 0)
		pqueue_push(env->queue, env->scratch[--scratch_n]);
}

void	build_request(t_env *env, t_coder *coder, t_request *req)
{
	req->coder_id = coder->id;
	req->arrival_seq = env->next_request_seq++;
	req->deadline_ms = request_deadline(env, coder);
}

int	wait_for_grant(t_env *env, t_coder *coder)
{
	while (!coder->granted && !env->stopped)
		pthread_cond_wait(&env->state_cond, &env->state_lock);
	return (coder->granted);
}
