#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>

#include "timer.h"

#define NTHREADS 4000

void wait_threads(int, pthread_t *); // Définition du prototype de fonction pour l'utiliser

void *fn_thread(void *arg){
	sleep(10);
	pthread_exit((void *)0);
}

long create_threads(int n) {
	struct timespec vartime = timer_start(); /* Démarrage de la mesure temporelle */
	
    /* Création de n threads s'exécutant en parallèle */
	pthread_t TIDs[n];
    for (int i = 0; i < n; ++i) {
		pthread_create(&TIDs[i], NULL, fn_thread, NULL);
	}
	/* On mesure le temps écoulé pour la création des n threads */
	long time = timer_end(vartime);

    /* On attend pour pouvoir contaster la création des threads avec htop (mais ne compte pas dans le temps mesuré */
	sleep(10);

	/* On attend la fin des threads créés par mesure sociale et pour éviter un impact sur les prochaine mesures */
	wait_threads(n, TIDs);

	return time; /* Retourne le temps écoulé pour réaliser la création des n threads */
}

void wait_threads(int n, pthread_t *TIDs) {
    /* Mesure de salubrité sociale, le père attend la fin de tous ses fils */
    for (int i = 0; i < n; ++i)
		pthread_join(TIDs[i], NULL);
}

int main(int argc, char *argv[]) {
	int n = NTHREADS;

    for (int i = 1; i < argc; i++) {
		if (argv[i][0] != '-') {
			n = atoi(argv[i]);
		}
	}
	
	long time_threads = create_threads(n);
	
	printf("Time taken for creating %d threads (nanoseconds): %ld\n", n, time_threads);
	printf("Time taken for creating %d threads (milliseconds): %ld\n", n, time_threads / NANO_TO_MILLI);

    /* On flushe la sortie standard */
    fflush(stdout);

    /* Fin du père */
    exit(0);
}

