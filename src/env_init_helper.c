/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_init_helper.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmesropy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/18 17:52:17 by rmesropy          #+#    #+#             */
/*   Updated: 2026/09/18 18:34:59 by rmesropy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <codexion.h>

void	init_coder(t_coder *c, int id, t_env *env)
{
	c->id = id;
	c->compiles_done = 0;
	c->last_compile_start_ms = 0;
	c->granted = 0;
	c->env = env;
}
