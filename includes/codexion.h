/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmesropy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/18 17:13:26 by rmesropy          #+#    #+#             */
/*   Updated: 2026/09/18 19:13:29 by rmesropy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <pthread.h>
# include <sys/time.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <limits.h>

typedef enum e_scheduler
{
	SCHED_FIFO_POLICY,
	SCHED_EDF_POLICY
}	t_scheduler;

typedef struct s_config
{
	int			number_of_coders;
	long		time_to_burnout;
	long		time_to_compile;
	long		time_to_debug;
	long		time_to_refactor;
	int			number_of_compiles_required;
	long		dongle_cooldown;
	t_scheduler	scheduler;
}	t_config;

typedef struct s_dongle
{
	int				id;
	pthread_mutex_t	lock;
	int				held;
	long			released_at_ms;
	int				released;
}	t_dongle;

typedef struct s_request
{
	int		coder_id;
	long	arrival_seq;
	long	deadline_ms;
}	t_request;

typedef struct s_pqueue
{
	t_request	*data;
	int			size;
	int			capacity;
	int			(*cmp)(t_request a, t_request b);
}	t_pqueue;

typedef struct s_env	t_env;

typedef struct s_coder
{
	int			id;
	pthread_t	thread;
	long		last_compile_start_ms;
	int			compiles_done;
	int			granted;
	t_env		*env;
}	t_coder;

struct s_env
{
	t_config		cfg;
	t_coder			*coders;
	t_dongle		*dongles;
	struct timeval	start_time;
	pthread_mutex_t	log_lock;
	pthread_mutex_t	state_lock;
	pthread_cond_t	state_cond;
	t_pqueue		*queue;
	t_request		*scratch;
	long			next_request_seq;
	int				stopped;
	int				log_lock_initialized;
	int				state_lock_initialized;
	int				state_cond_initialized;
	int				dongles_initialized;
};

int			parse_args(int argc, char **argv, t_config *cfg);

int			init_env(t_env *env, t_config *cfg);
void		cleanup_env(t_env *env);
int			simulation_stopped(t_env *env);
void		stop_simulation(t_env *env);
void		mark_compile_done(t_env *env, t_coder *coder);

long		get_elapsed_ms(t_env *env);
void		sleep_ms(long ms);

void		log_action(t_env *env, int coder_id, const char *msg);

int			acquire_dongles(t_env *env, t_coder *coder);
void		release_dongles(t_env *env, t_coder *coder);
void		try_grant(t_env *env);

int			cmp_fifo(t_request a, t_request b);
int			cmp_edf(t_request a, t_request b);

void		*coder_routine(void *arg);

void		*monitor_routine(void *arg);

t_pqueue	*pqueue_create(int capacity, int (*cmp)(t_request a, t_request b));
void		pqueue_push(t_pqueue *pq, t_request req);
t_request	pqueue_pop(t_pqueue *pq);
int			pqueue_is_empty(t_pqueue *pq);
void		pqueue_destroy(t_pqueue *pq);

void		init_coder(t_coder *c, int id, t_env *env);
int			init_locks(t_env *env);
int			parse_int_arg(const char *s, int *out, long min_value);
int			parse_times(char **argv, t_config *cfg);
int			parse_bounded(const char *s, long *out, long min_value);
int			dongle_pair_free(t_env *env, int coder_id);
int			blocked_by_earlier(t_env *env, t_request req, int count);
void		build_request(t_env *env, t_coder *coder, t_request *req);
int			wait_for_grant(t_env *env, t_coder *coder);
long		request_deadline(t_env *env, t_coder *coder);
int			init_dongle(t_dongle *d, int id);
void		sift_down(t_pqueue *pq, int i);

#endif
