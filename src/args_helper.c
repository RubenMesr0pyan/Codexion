/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args_helper.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmesropy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/18 17:46:39 by rmesropy          #+#    #+#             */
/*   Updated: 2026/09/18 17:46:40 by rmesropy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <codexion.h>

static int	parse_scheduler(char *arg, t_scheduler *scheduler)
{
	if (strcmp(arg, "fifo") == 0)
		*scheduler = SCHED_FIFO_POLICY;
	else if (strcmp(arg, "edf") == 0)
		*scheduler = SCHED_EDF_POLICY;
	else
		return (0);
	return (1);
}

int	parse_args(int argc, char **argv, t_config *cfg)
{
	if (argc != 9)
		return (0);
	if (!parse_int_arg(argv[1], &cfg->number_of_coders, 1))
		return (0);
	if (!parse_times(argv, cfg))
		return (0);
	if (!parse_int_arg(argv[6], &cfg->number_of_compiles_required, 0))
		return (0);
	if (!parse_bounded(argv[7], &cfg->dongle_cooldown, 0))
		return (0);
	return (parse_scheduler(argv[8], &cfg->scheduler));
}
