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
#include <stdbool.h>
#include <sys/stat.h>
#include <memory.h>

int pidult=-100;
bool vistoelultimo=false;
char* mandatos[]={"cd","umask","time","read"};
bool comprobarmandato(char ***argvv ,int i){

    char **argv = argvv[i];
    int argc;
    int x;

        for (argc = 0; argv[argc]; argc++) {
            for (x = 0; x < 4; x++) {


                if (strcmp(argv[argc], mandatos[x]) == 0) {
                    return true;



            }
        }

    }


    return false;
}

void esperarzombies(){
    int s;
    int pid=wait(&s);
    if(pid==pidult) {
        vistoelultimo = true;



    }



}
void mandato(char * mandato[]){
    char *buff = malloc(100);
    char **otrobuff = NULL;
    int termina;
    char *directorio;
    mode_t mask=umask(0);
    umask(mask);
    if(strcmp(mandato[0],"cd")==0){

        if(mandato[1]) {

            else{
            directorio=mandato[1];
            termina=chdir(mandato[1]);
        }}
        else{

            directorio=getenv("HOME");
            termina=chdir(getenv("HOME"));
            }
            if (termina==-1){
                fprintf(stderr,"el directorio no es correcto: %s\n",directorio);}
            else{
            getcwd(buff,100);
                printf("%s\n", buff);
        }
    }


    else if(strcmp(mandato[0],"umask")==0){
        if(mandato[1]){
            mask=(int)strtol(mandato[1],otrobuff,8);

            if(mask==0&&atoi(mandato[1])!=0&&mask<01700)
                fprintf(stderr,"La mascara asignada no es posible: %s\n",mandato[1]);
            else
            printf("%o\n",umask(mask));
        }
        else
            printf("%o\n",mask);


    } else if(strcmp(mandato[0],"times")==0){



    }
   free(buff);
}



extern int obtain_order();		/* See parser.y for description */

int main(void)
{	int x;

    int fdstock[3];
    for(x=0;x<3;x++){
        fdstock[x]=dup(x);
    }

	char ***argvv = NULL;
	int argvc;
	char **argv = NULL;
	char *filev[3] = { NULL, NULL, NULL };
	int bg;
	int ret;

    mode_t mascara=0666;
    mode_t mask=0000;
    umask(mask);


	setbuf(stdout, NULL);			/* Unbuffered */
	setbuf(stdin, NULL);
    int vuelta=0;

    signal(SIGCHLD,esperarzombies);
    struct sigaction accion;
    accion.sa_handler=SIG_IGN;
    accion.sa_flags=0;

    sigaction(SIGQUIT,&accion,NULL);
    sigaction(SIGINT,&accion,NULL);

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

        bool nohijo=false;
		for( i=0;i<nmandatos;i++) {
		    if(i==nmandatos-1) {
		        nohijo=comprobarmandato(argvv,i);


            }
            if(!nohijo) {

            pid[i] = fork();}
            else {
                nmandatos--;
                pid[i]=0;
            }
            if(i==nmandatos-1)
                pidult=pid[i];
            if(pid[i]==0||pid[i]==-1) {




                for(x=0;x<nmandatos-1;x++){
                    if(i==x) {
                        dup2(pipes[x][1], 1);
                    }
                    else
                        close(pipes[x][1]);
                    if(x==i-1)
                        dup2(pipes[x][0],0);
                    else
                        close(pipes[x][0]);


                }

                break;}
            }


            if(i==nmandatos&&!nohijo)
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
                        pid2 = fork();
                        if (pid2 != 0) {
                            printf("[%d]\n", pid2);
                            char *string=(char*)malloc(7*sizeof(string)+sizeof(int));
                            sprintf(string,"bgpid=%d",pid2);
                            putenv(string);

                        }}
                    else{if(!nohijo){
                        struct sigaction accion2;
                        accion.sa_handler=SIG_DFL;
                        accion.sa_flags=0;

                        sigaction(SIGQUIT,&accion2,&accion);
                        sigaction(SIGINT,&accion2,&accion);
                        }}

                    if (pid2 != 0) {
                        exit(0);
                    } else {

                        bool hayfich=false;

                        for(x=0;x<3;x++){


                            if(filev[x]) {
                                if (x == 0 && i == 0) {

                                    files[x] = open(filev[x], O_RDONLY,mascara);
                                    hayfich = true;
                                } else if (x == 1 && (i == nmandatos - 1||nohijo)) {
                                    files[x] = open(filev[x], O_WRONLY | O_TRUNC | O_CREAT,mascara);
                                    hayfich = true;
                                }
                                if (hayfich) {
                                    if (files[x] == -1) {
                                        fprintf(stderr, "Hubo un error al abrir el archivo '%s' , con el error: %d\n",
                                                filev[x], errno);
                                        exit(errno);
                                    } else {


                                        if (dup2(files[x],x) == -1) {
                                            fprintf(stderr, "Hubo un error en el dup2\n");
                                            exit(errno);
                                        }
                                        if (close(files[x]) == -1) {
                                            fprintf(stderr, "Hubo un error en el close\n");
                                            exit(errno);

                                        }
                                    }
                                    hayfich = false;
                                }

                            }
                            }
                        }

                    if (comprobarmandato(argvv,i)){
                            mandato(argvv[i]);
                            if(!nohijo)
                            exit(0);
                            else{
                                for(x=0;x<3;x++){
                                    dup2(fdstock[x],x);
                                }
                            }


                        }else

                        if(!nohijo)
                        execvp(argvv[i][0], argvv[i]);

                        }

            while(!vistoelultimo&&!nohijo)
                pause();
            vistoelultimo=false;



/*
 * FIN DE LA PARTE A ELIMINAR
 */
#endif
	}
	exit(0);
	return 0;
}
