(1) struct timeval	tv;
-->chercher ce qu est la structure complete

#if 0
#ifndef __timeval_defined
#define __timeval_defined 1

#include <bits/types.h>

/* A time value that is accurate to the nearest
   microsecond but also has a range of years.  */
struct timeval
{
#ifdef __USE_TIME_BITS64
  __time64_t tv_sec;		/* Seconds.  */
  __suseconds64_t tv_usec;	/* Microseconds.  */
#else
  __time_t tv_sec;		/* Seconds.  */
  __suseconds_t tv_usec;	/* Microseconds.  */
#endif
};
#endif

#endif

(2) gettimeofday(&tv, NULL);

#if 0
/* Get the current time of day, putting it into *TV.
   If TZ is not null, *TZ must be a struct timezone, and both fields
   will be set to zero.
   Calling this function with a non-null TZ is obsolete;
   use localtime etc. instead.
   This function itself is semi-obsolete;
   most callers should use time or clock_gettime instead. */
#ifndef __USE_TIME_BITS64
extern int gettimeofday (struct timeval *__restrict __tv,
			 void *__restrict __tz) __THROW __nonnull ((1));
#else
# ifdef __REDIRECT_NTH
extern int __REDIRECT_NTH (gettimeofday, (struct timeval *__restrict __tv,
                                          void *__restrict __tz),
                           __gettimeofday64) __nonnull ((1));
# else
#  define gettimeofday __gettimeofday64
# endif
#endif

#endif

(3) typedef union :

#if 0

Différence clé avec une structure :

Si c'était une structure, la mémoire serait organisée ainsi :
[__data][__size][__align] (chaque champ a son propre espace)

Mais comme c'est une union, la mémoire est organisée ainsi :
[__data OU __size OU __align] (tous partagent le même espace)

La taille totale est celle du plus grand membre (ici probablement __size).

#endif

(4) Qu'est-ce qu'un mutex ?

#if 0
Un mutex (MUTual EXclusion) est un verrou qui permet de protéger
l'accès à des ressources partagées entre plusieurs threads.
C'est un outil fondamental de synchronisation en programmation concurrente.
____________________________________________________________________________
Analogie simple :
Imagine que les philosophes autour de la table sont des threads,
et les fourchettes sont des ressources partagées.

Le mutex est comme une règle qui dit :
"Une seule personne peut tenir une fourchette à la fois,
et pour l'utiliser, il faut d'abord demander la permission."

#endif

(4 b)Pourquoi initialiser un mutex ?
#if 0


pthread_mutex_init() sert à :

    Préparer le mutex pour son utilisation
    Configurer ses attributs (NULL signifie attributs par défaut)
    Garantir un état cohérent avant toute utilisation

#endif

(4 c)À quoi servent ces mutex spécifiques ?

#if 0
1. forks[] : Protège l'accès à chaque fourchette (ressource partagée)
    Un philosophe doit "lock" le mutex avant de prendre une fourchette
    Et "unlock" après l'avoir reposée
2. log_mutex : Protège l'affichage des messages
    Empêche que deux threads écrivent en même temps dans la sortie
    Évite que les messages se mélangent
3. death_mutex : Protège la vérification de l'état de simulation
    Garantit qu'un seul thread à la fois peut vérifier/modifier l'état de mort

#endif

(4 d)Comment fonctionne un mutex en pratique ?

#if 0
    pthread_mutex_lock(&mutex) : Bloque si le mutex est déjà pris, sinon le prend
    pthread_mutex_unlock(&mutex) : Relâche le mutex
    pthread_mutex_trylock(&mutex) : Essaie de prendre le mutex sans bloquer

Pourquoi c'est essentiel dans philo ?

Sans mutex, j'aurais des conditions de course (race conditions) où :

    Deux philosophes pourraient prendre la même fourchette
    Les messages d'état s'afficheraient de façon désordonnée
    La vérification de la mort pourrait donner des résultats incohérents

Les mutex garantissent que ces opérations se font de manière atomique (sans interruption).

#endif

(5)data->philos[i].left_fork = &data->forks[i];

#if 0
data->philos[i] accède au philosophe numéro i
left_fork est un pointeur (pthread_mutex_t *) dans la structure t_philo.
data->forks[i] est le mutex (type pthread_mutex_t à l'index i
                dans le tableau des fourchettes.
&data->forks[i] donne l'adresse de ce mutex.
L'assignation fait que left_fork pointe vers ce mutex

#endif