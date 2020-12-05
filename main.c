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
#include <sys/times.h>
int pidult=-100;
bool vistoelultimo=false;
char* mandatos[]={"cd","umask","time","read","exit"};


int size(char* string){
    int size;

    for(size=0;string[size];size++);
    return size+1;
}


bool comprobarmandato(char ***argvv ,int i){

    char **argv = argvv[i];
    int argc;
    int x;

        for (argc = 0; argv[argc]; argc++) {
            for (x = 0; x < 5; x++) {


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
    if(strcmp(mandato[0],"exit")==0){
        printf("El segundo argumento tiene un tamaño de %d\n",size(mandato[1]));

        exit(0);
    }

    char *buff = malloc(100);

    int termina;
    char *directorio;
    mode_t mask=umask(0);
    umask(mask);
    if(strcmp(mandato[0],"cd")==0){

        if(mandato[1]) {
            if(mandato[2]){
                fprintf(stderr,"Numero incorrecto de argumentos, se necesita 1");
                free(buff);return;
            }
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
        char **otrobuff = malloc(sizeof(otrobuff));
        if(mandato[1]){
            if(mandato[2]) {
                fprintf(stderr, "Numero incorrecto de argumentos, se necesita 1\n");
                free(buff);
                free(otrobuff);
                return;
            }
            else{
            mask=(int)strtol(mandato[1],otrobuff,8);

            if((mask==0&&atoi(mandato[1])!=0)||mask>01777||strcmp("",*otrobuff)!=0)
                fprintf(stderr,"La mascara asignada no es posible: %s\n",mandato[1]);
            else{

            printf("%o\n",umask(mask));}}
        }
        else
            printf("%o\n",mask);

        free(otrobuff);

    } else if(strcmp(mandato[0],"time")==0){
        struct tms *buff2=malloc(4*sizeof(long));
        times(buff2);
        long tksec=sysconf(_SC_CLK_TCK);
        clock_t utime=buff2->tms_cutime/tksec;
        clock_t stime=buff2->tms_cstime/tksec;
        clock_t rtime=(buff2->tms_cstime + buff2->tms_cutime)/tksec;
        if(!mandato[1]){



            printf("%d.%03du %d.%03ds %d.%03dr\n",(int)utime/1000,(int)utime%1000,(int)stime/1000,(int)stime%1000,(int)rtime/1000,(int)rtime%1000);
            free(buff2);
        }else{
            int x;
            int npalabras;

            for(x=1;mandato[x];x++){


            }
            npalabras=x-1;
            char * vex[npalabras+1];
            for(x=0;x<npalabras;x++){
                vex[x]=strdup(mandato[x+1]);
            }
            vex[npalabras]=NULL;
            pid_t pid=fork();
            if(pid==0){
                execvp(vex[0],vex);
                exit(0);
            }
            pidult=pid;
            while(!vistoelultimo)
                pause();
            vistoelultimo=false;
            times(buff2);
            utime=utime-buff2->tms_cutime/tksec;
            stime=stime-buff2->tms_cstime/tksec;
            rtime=rtime-(buff2->tms_cstime + buff2->tms_cutime)/tksec;
            printf("%d.%03du %d.%03ds %d.%03dr\n",(int)utime,(int)utime%1000,(int)stime,(int)stime%1000,(int)rtime,(int)rtime%1000);
            free(buff2);




        }
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
        int npalabra;
        char *palabra;
        int tampalabra;
        int nchar;
        int puntero1=0;
        int puntero2=0;
        bool hay=false;
        char charo;
        char** partes=(char**)malloc(1);
        char** punteropart;
        int contapartes=0;
        bool normal=false;

		for (argvc = 0; (argv = argvv[argvc]); argvc++) {
		    for(npalabra=0;(palabra=argv[npalabra]);npalabra++){
		        tampalabra=size(palabra);
		        for(nchar=0;nchar<=tampalabra;nchar++){



		            if(nchar==tampalabra){
		                charo='%';
		                if(normal){

                            char parte[nchar-puntero1];
                            for(x=0;x<nchar-puntero1;x++){
                                parte[x]=palabra[puntero1+x];
                            }
                            parte[nchar-puntero1]='\0';
                            contapartes++;
                            partes=realloc(partes,sizeof(partes)*(contapartes));
                            punteropart=partes+contapartes-1;
                            *punteropart=strdup(parte);
                            puntero1=nchar;
		                }



		            }
		            else
		                charo=palabra[nchar];


		            if(charo=='~'){
		                char *home=getenv("HOME");

                        contapartes++;
                        partes=realloc(partes,sizeof(partes)*(contapartes));
                        punteropart=partes+contapartes-1;
                        *punteropart=strdup(home);
                        puntero1=nchar;



		            }
		            else
		            if((charo=='$'&&!hay)){
		                char parte[nchar-puntero1+1];
		                for(x=0;x<nchar-puntero1;x++){
		                    parte[x]=palabra[puntero1+x];
		                }
		                parte[nchar-puntero1]='\0';
		                contapartes++;
		                partes=realloc(partes,sizeof(partes)*(contapartes));
		                punteropart=partes+contapartes-1;
		                *punteropart=strdup(parte);
		                puntero1=nchar;
		                hay=true;




                    } else if(hay){
		                if(!((charo>='0'&&charo<='9')||(charo>='a'&&charo<='z')||(charo>='A'&&charo<='Z')||charo=='_')){
		                    char var[nchar-puntero1];
		                    for(puntero2=1;puntero2<nchar-puntero1;puntero2++){
		                        var[puntero2-1]=palabra[puntero1+puntero2];
		                    }
		                    var[nchar-puntero1-1]='\0';
		                    hay=false;
		                    puntero1=nchar;
		                    normal=true;

		                    nchar--;
		                    char* varcambiada=getenv(var);
		                    if(varcambiada==NULL){
		                        printf("variable de entorno no encontrada\n");
		                        varcambiada=var;
		                    }
		                    contapartes++;
                            partes=realloc(partes,sizeof(partes)*(contapartes));
                            punteropart=partes+contapartes-1;
                            *punteropart=strdup(varcambiada);


		                }
		            }

		        }
		        if(contapartes>0) {
                    char *pa = malloc(100 * sizeof(pa));
                    char *aux = "";
                    for (x = 0; x < contapartes; x++) {
                        sprintf(pa, "%s%s", aux, *partes);
                        aux = strdup(pa);
                        if (x != contapartes - 1)
                            partes = partes + 1;
                    }
                    argv[npalabra] = strdup(pa);
                    free(pa);
                    partes=partes-contapartes+1;
                    free(partes);
                }


		    }
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
                            free(string);

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

                        if(!nohijo) {
                            if (execvp(argvv[i][0], argvv[i]) == -1)
                                printf("Mandato no encontrado\n");
                            exit(0);
                        }
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
