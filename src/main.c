/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmesropy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/18 17:14:15 by rmesropy          #+#    #+#             */
/*   Updated: 2026/09/18 17:14:16 by rmesropy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	spawn_threads(t_env *env, pthread_t *monitor,
	int *coders_created, int *monitor_created)
{
	int	i;

	*coders_created = 0;
	*monitor_created = 0;
	i = 0;
	while (i < env->cfg.number_of_coders)
	{
		if (pthread_create(&env->coders[i].thread, NULL,
				coder_routine, &env->coders[i]) != 0)
			return (0);
		*coders_created = i + 1;
		i++;
	}
	if (pthread_create(monitor, NULL, monitor_routine, env) != 0)
		return (0);
	*monitor_created = 1;
	return (1);
}

static void	join_threads(t_env *env, pthread_t monitor,
	int coders_created, int monitor_created)
{
	int	i;

	i = 0;
	while (i < coders_created)
	{
		pthread_join(env->coders[i].thread, NULL);
		i++;
	}
	if (monitor_created)
		pthread_join(monitor, NULL);
}

static int	run_threads(t_env *env)
{
	pthread_t	monitor;
	int			coders_created;
	int			monitor_created;

	if (!spawn_threads(env, &monitor, &coders_created, &monitor_created))
	{
		stop_simulation(env);
		join_threads(env, monitor, coders_created, monitor_created);
		return (0);
	}
	join_threads(env, monitor, coders_created, monitor_created);
	return (1);
}

static void	print_usage(const char *prog)
{
	fprintf(stderr, "Usage: %s number_of_coders time_to_burnout "
		"time_to_compile time_to_debug time_to_refactor "
		"number_of_compiles_required dongle_cooldown scheduler(fifo|edf)\n",
		prog);
}

int	main(int argc, char **argv)
{
	t_config	cfg;
	t_env		env;

	if (!parse_args(argc, argv, &cfg))
	{
		print_usage(argv[0]);
		return (1);
	}
	if (!init_env(&env, &cfg))
	{
		fprintf(stderr, "Error: failed to initialize simulation.\n");
		return (1);
	}
	if (!run_threads(&env))
	{
		fprintf(stderr, "Error: failed to create threads.\n");
		cleanup_env(&env);
		return (1);
	}
	cleanup_env(&env);
	return (0);
}
