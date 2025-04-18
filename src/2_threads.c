/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   2_threads.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grohr <grohr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 19:11:28 by grohr             #+#    #+#             */
/*   Updated: 2025/04/18 19:11:45 by grohr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

//join_threads:
//- Attend la fin de tous les threads des philosophes avec pthread_join.
//- Attend la fin du thread superviseur.
//- pthread_join bloque jusqu'à ce qu'un thread se termine, garantissant que
//  le programme ne se termine pas avant la fin de la simulation.
//
void	join_threads(t_data *data, pthread_t supervisor)
{
	int	i;

	i = 0;
	while (i < data->num_philos)
	{
		pthread_join(data->philos[i].thread, NULL);
		i++;
	}
	pthread_join(supervisor, NULL);
}

//create_threads:
//- Crée un thread pour chaque philosophe avec pthread_create.
//- Crée un thread superviseur pour vérifier les morts.
//- pthread_create lance "philo_routine" ou "supervisor_routine"
//  dans un nouveau thread :
//			=> permet l'exécution parallèle des philosophes.
//- Retourne 0 en cas d'erreur, 1 sinon.
//
int	create_threads(t_data *data, pthread_t *supervisor)
{
	int	i;

	i = 0;
	while (i < data->num_philos)
	{
		if (pthread_create(&data->philos[i].thread, NULL, philo_routine,
				&data->philos[i]))
			return (0);
		i++;
	}
	if (pthread_create(supervisor, NULL, supervisor_routine, data))
		return (0);
	return (1);
}