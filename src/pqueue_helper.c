/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pqueue_helper.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmesropy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/18 17:50:39 by rmesropy          #+#    #+#             */
/*   Updated: 2026/09/18 17:50:42 by rmesropy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <codexion.h>

t_request	pqueue_pop(t_pqueue *pq)
{
	t_request	top;

	top = pq->data[0];
	pq->size--;
	pq->data[0] = pq->data[pq->size];
	if (pq->size > 0)
		sift_down(pq, 0);
	return (top);
}

int	pqueue_is_empty(t_pqueue *pq)
{
	return (pq->size == 0);
}

void	pqueue_destroy(t_pqueue *pq)
{
	if (!pq)
		return ;
	free(pq->data);
	free(pq);
}
