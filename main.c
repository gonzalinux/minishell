/*-
 * main.c
 * Minishell C source
 * Shows how to use "obtain_order" input interface function.
 *
 * Copyright (c) 1993-2002-2019, Francisco Rosales <frosal@fi.upm.es>
 * Todos los derechos reservados.
 *
 * Publicado bajo Licencia de Proyecto Educativo Práctico
 * <http://laurel.datsi.fi.upm.es/~ssoo/LICENCIA/LPEP>
 *
 * Queda prohibida la difusión total o parcial por cualquier
 * medio del material entregado al alumno para la realización
 * de este proyecto o de cualquier material derivado de este,
 * incluyendo la solución particular que desarrolle el alumno.
 *
 * DO NOT MODIFY ANYTHING OVER THIS LINE
 * THIS FILE IS TO BE MODIFIED
 */

#include <stddef.h>			/* NULL */
#include <stdio.h>			/* setbuf, printf */
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <sched.h>

extern int obtain_order();		/* See parser.y for description */

int main(void)
{
	char ***argvv = NULL;
	int argvc;
	char **argv = NULL;
	int argc;
	char *filev[3] = { NULL, NULL, NULL };
	int bg;
	int ret;

	setbuf(stdout, NULL);			/* Unbuffered */
	setbuf(stdin, NULL);
    int vuelta=0;
	while (1) {
	    vuelta++;
		fprintf(stderr, "%s,%d>>>", "msh> ",vuelta);	/* Prompt */
		ret = obtain_order(&argvv, filev, &bg);
		if (ret == 0) break;		/* EOF */
		if (ret == -1) continue;	/* Syntax error */
		argvc = ret - 1;		/* Line */
		if (argvc == 0) continue;	/* Empty line */
#if 1
/*
 * LAS LINEAS QUE A CONTINUACION SE PRESENTAN SON SOLO
 * PARA DAR UNA IDEA DE COMO UTILIZAR LAS ESTRUCTURAS
 * argvv Y filev. ESTAS LINEAS DEBERAN SER ELIMINADAS.
 */
		for (argvc = 0; (argv = argvv[argvc]); argvc++) {
			for (argc = 0; argv[argc]; argc++)
				printf("%s", argv[argc]);
			printf("\n");
		}
		if (filev[0]) printf("< %s\n", filev[0]);//IN
		if (filev[1]) printf("> %s\n", filev[1]);//OUT
		if (filev[2]) printf(">& %s\n", filev[2]);//ERR
		if (bg) printf("&\n");
        printf("-------------------------------\n");


		pid_t pid;


		pid=fork();
        switch (pid) {
            case -1:
                perror("Ha habido un error con el fork");
                exit(1);

            case 0:
                if (!bg) {
                    pid_t pid2 =fork();
                    if(pid2==0){
                        exit(0);
                    }
                    else {
                     sleep(5);
                        execvp(argvv[0][0], argvv[0]);
                    }
                }


        }
        int s;
        wait(&s);
        if(WIFEXITED(s))
        printf("ha retornado %d\n",s);








/*
 * FIN DE LA PARTE A ELIMINAR
 */
#endif
	}
	exit(0);
	return 0;
}
