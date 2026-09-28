/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pqueue.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmesropy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/18 17:14:18 by rmesropy          #+#    #+#             */
/*   Updated: 2026/09/18 18:28:42 by rmesropy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

t_pqueue	*pqueue_create(int capacity, int (*cmp)(t_request a, t_request b))
{
	t_pqueue	*pq;

	pq = malloc(sizeof(t_pqueue));
	if (!pq)
		return (NULL);
	pq->data = malloc(sizeof(t_request) * capacity);
	if (!pq->data)
	{
		free(pq);
		return (NULL);
	}
	pq->size = 0;
	pq->capacity = capacity;
	pq->cmp = cmp;
	return (pq);
}

static void	swap_req(t_request *a, t_request *b)
{
	t_request	tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

static void	sift_up(t_pqueue *pq, int i)
{
	int	parent;

	while (i > 0)
	{
		parent = (i - 1) / 2;
		if (!pq->cmp(pq->data[i], pq->data[parent]))
			break ;
		swap_req(&pq->data[i], &pq->data[parent]);
		i = parent;
	}
}

void	sift_down(t_pqueue *pq, int i)
{
	int	left;
	int	right;
	int	best;

	while (1)
	{
		left = 2 * i + 1;
		right = 2 * i + 2;
		best = i;
		if (left < pq->size && pq->cmp(pq->data[left], pq->data[best]))
			best = left;
		if (right < pq->size && pq->cmp(pq->data[right], pq->data[best]))
			best = right;
		if (best == i)
			break ;
		swap_req(&pq->data[i], &pq->data[best]);
		i = best;
	}
}

void	pqueue_push(t_pqueue *pq, t_request req)
{
	int	i;

	if (pq->size >= pq->capacity)
		return ;
	i = pq->size;
	pq->data[i] = req;
	pq->size++;
	sift_up(pq, i);
}
