/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmesropy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/18 17:14:21 by rmesropy          #+#    #+#             */
/*   Updated: 2026/09/18 17:14:22 by rmesropy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	cmp_fifo(t_request a, t_request b)
{
	if (a.arrival_seq != b.arrival_seq)
		return (a.arrival_seq < b.arrival_seq);
	return (a.coder_id < b.coder_id);
}

int	cmp_edf(t_request a, t_request b)
{
	if (a.deadline_ms != b.deadline_ms)
		return (a.deadline_ms < b.deadline_ms);
	return (a.coder_id < b.coder_id);
}
