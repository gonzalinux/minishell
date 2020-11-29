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
#include <fcntl.h>
#include <errno.h>

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
	int x;

	setbuf(stdout, NULL);			/* Unbuffered */
	setbuf(stdin, NULL);
    int vuelta=0;
	while (1) {
	    vuelta++;
		fprintf(stderr, "%s", "msh>");	/* Prompt */
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
        int nmandatos=0;
		for (argvc = 0; (argv = argvv[argvc]); argvc++) {
			for (argc = 0; argv[argc]; argc++);
				//printf("%s", argv[argc]);
			//printf("\n");
			nmandatos++;
		}
		//if (filev[0]) printf("< %s\n", filev[0]);//IN
		//if (filev[1]) printf("> %s\n", filev[1]);//OUT
		//if (filev[2]) printf(">& %s\n", filev[2]);//ERR
		//if (bg) printf("&\n");
        //printf("-------------------------------\n");
        if(nmandatos==0)
            continue;

        int files[3];

		pid_t pid[nmandatos];
        pid_t pid2=0;

        int pipes[nmandatos-1][2];
        for( x=0;x<nmandatos-1;x++){
            pipe(pipes[x]);
        }

        int i;

		for( i=0;i<nmandatos;i++) {


            pid[i] = fork();
            if(pid[i]==0||pid[i]==-1) {
                for(x=0;x<nmandatos-1;x++){
                    if(i==0) {
                        if(x==0){
                        close(pipes[x][0]);
                        dup2(pipes[x][1], 1);
                    }
                        else{
                        close(pipes[x][0]);
                        close(pipes[x][1]);
                    }}
                    else {
                        if (i == nmandatos - 1) {
                            if (x == nmandatos - 2) {
                                dup2(pipes[x][0], 0);
                                close(pipes[x][1]);
                            } else {
                                close(pipes[x][0]);
                                close(pipes[x][0]);
                            }
                        }
                        else
                            if(i==x){
                                dup2(pipes[x][0],0);
                                close(pipes[x][1]);

                            }else
                                if(i==x+1){
                                    dup2(pipes[x][1],1);
                                    close(pipes[x][0]);
                                }
                    }
                }


                break;}
            }


            if(i==nmandatos)
                for( x=0;x<nmandatos-1;x++){
                    close(pipes[x][0]);
                    close(pipes[x][1]);

                }
            else
            switch (pid[i]) {
                case -1:
                    perror("Ha habido un error con el fork");
                    exit(1);

                case 0:

                    if (bg) {
                        pid2 = fork();}

                    if (pid2 != 0) {
                        exit(0);
                    } else {


                        for(x=0;x<3&&i==nmandatos-1;x++){
                            if(filev[x]){
                                files[x]=open(filev[x],O_WRONLY | O_CREAT | O_TRUNC, 0600);
                                if(files[x]==-1){
                                    fprintf(stderr,"Hubo un error al abrir el archivo '%s' , con el error: %d",filev[x],errno);
                                    exit(errno);}
                                else{
                                    if(dup2(files[x],x)==-1) {
                                        fprintf(stderr, "Hubo un error en el dup2");
                                        exit(errno);
                                    }
                                    if(close(files[x])==-1){
                                        fprintf(stderr, "Hubo un error en el close");
                                        exit(errno);

                                    }
                                }


                            }
                        }

                        if(bg)
                            sleep(3);
                        execvp(argvv[i][0], argvv[i]);
                        }



            }

        int s;

		for(int j=0;j<nmandatos;j++) {
            wait(&s);
            if (WIFEXITED(s));
          //      printf("ha retornado %d\n", s);
        }







/*
 * FIN DE LA PARTE A ELIMINAR
 */
#endif
	}
	exit(0);
	return 0;
}
