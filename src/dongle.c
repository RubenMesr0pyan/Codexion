/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmesropy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/18 17:14:10 by rmesropy          #+#    #+#             */
/*   Updated: 2026/09/18 18:17:43 by rmesropy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long	request_deadline(t_env *env, t_coder *coder)
{
	long	last;

	last = coder->last_compile_start_ms;
	if (last > LONG_MAX - env->cfg.time_to_burnout)
		return (LONG_MAX);
	return (last + env->cfg.time_to_burnout);
}

static int	dongle_free(t_env *env, t_dongle *d)
{
	long	now;

	if (d->held)
		return (0);
	if (!d->released)
		return (1);
	now = get_elapsed_ms(env);
	return (now - d->released_at_ms >= env->cfg.dongle_cooldown);
}

int	dongle_pair_free(t_env *env, int coder_id)
{
	int	idx;

	idx = coder_id - 1;
	return (dongle_free(env, &env->dongles[idx])
		&& dongle_free(env,
			&env->dongles[(idx + 1) % env->cfg.number_of_coders]));
}

static int	shares_dongle(int coder_a, int coder_b, int count)
{
	int	a;
	int	b;
	int	a_right;
	int	b_right;

	a = coder_a - 1;
	b = coder_b - 1;
	a_right = (a + 1) % count;
	b_right = (b + 1) % count;
	return (a == b || a == b_right || a_right == b || a_right == b_right);
}

int	blocked_by_earlier(t_env *env, t_request req, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		if (shares_dongle(env->scratch[i].coder_id, req.coder_id,
				env->cfg.number_of_coders))
			return (1);
		i++;
	}
	return (0);
}
