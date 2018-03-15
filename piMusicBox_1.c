
#include "piMusicBox_1.h"
#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include "kbhit.h"
#include <softTone.h>
#include "fsm.h"
#include "tmr.h"
volatile int flags = 0;

// espera hasta la prÛxima activaciÛn de reloj
void delay_until(unsigned int next){
	unsigned int now = millis();

	if(next > now){
		delay(next-now);
	}
}

int i;
int frecuenciaDespacito[160] = {0,1175,1109,988,740,740,740,740,740,740,988,988,988,988,880,988,784,0,784,784,784,784,784,988,988,988,988,1109,1175,880,0,880,880,880,880,880,1175,1175,1175,1175,1318,1318,1109,0,1175,1109,988,740,740,740,740,740,740,988,988,988,988,880,988,784,0,784,784,784,784,784,988,988,988,988,1109,1175,880,0,880,880,880,880,880,1175,1175,1175,1175,1318,1318,1109,0,1480,1318,1480,1318,1480,1318,1480,1318,1480,1318,1480,1568,1568,1175,0,1175,1568,1568,1568,0,1568,1760,1568,1480,0,1480,1480,1480,1760,1568,1480,1318,659,659,659,659,659,659,659,659,554,587,1480,1318,1480,1318,1480,1318,1480,1318,1480,1318,1480,1568,1568,1175,0,1175,1568,1568,1568,1568,1760,1568,1480,0,1480,1480,1480,1760,1568,1480,1318};
int tiempoDespacito[160] = {1200,600,600,300,300,150,150,150,150,150,150,150,150,300,150,300,343,112,150,150,150,150,150,150,150,150,300,150,300,300,150,150,150,150,150,150,150,150,150,300,150,300,800,300,600,600,300,300,150,150,150,150,150,150,150,150,300,150,300,343,112,150,150,150,150,150,150,150,150,300,150,300,300,150,150,150,150,150,150,150,150,150,300,150,300,450,1800,150,150,150,150,300,150,300,150,150,150,300,150,300,450,450,300,150,150,225,75,150,150,300,450,800,150,150,300,150,150,300,450,150,150,150,150,150,150,150,150,300,300,150,150,150,150,150,150,450,150,150,150,300,150,300,450,450,300,150,150,150,300,150,300,450,800,150,150,300,150,150,300,450};
int frecuenciaGOT[518] = {1568,0,1046,0,1244,0,1397,0,1568,0,1046,0,1244,0,1397,0,1175,0,1397,0,932,0,1244,0,1175,0,1397,0,932,0,1244,0,1175,0,1046,0,831,0,698,0,523,0,349,0,784,0,523,0,523,0,587,0,622,0,698,0,784,0,523,0,622,0,698,0,784,0,523,0,622,0,698,0,587,0,698,0,466,0,622,0,587,0,698,0,466,0,622,0,587,0,523,0,523,0,587,0,622,0,698,0,784,0,523,0,622,0,698,0,784,0,523,0,622,0,698,0,587,0,698,0,466,0,622,0,587,0,698,0,466,0,622,0,587,0,523,0,0,1568,0,0,1046,0,0,1244,0,0,1397,0,0,1568,0,0,1046,0,0,1244,0,0,1397,0,0,1175,0,587,0,622,0,587,0,523,0,587,0,784,0,880,0,932,0,1046,0,1175,0,0,1397,0,0,932,0,0,1244,0,0,1175,0,0,1397,0,0,932,0,0,1244,0,0,1175,0,0,1046,0,0,1568,0,0,1046,0,0,1244,0,0,1397,0,0,1568,0,0,1046,0,0,1244,0,0,1397,0,0,1175,0,880,0,784,0,932,0,1244,0,0,1397,0,0,932,0,0,1175,0,0,1244,0,0,1175,0,0,932,0,0,1046,0,0,2093,0,622,0,831,0,932,0,1046,0,622,0,831,0,1046,0,0,1865,0,622,0,784,0,831,0,932,0,622,0,784,0,932,0,0,1661,0,523,0,698,0,784,0,831,0,523,0,698,0,831,0,0,1568,0,1046,0,1244,0,1397,0,1568,0,1046,0,1244,0,1397,0,0,0,1661,0,1046,0,1175,0,1244,0,831,0,1175,0,1244,0,0,0,0,2489,0,0,0,0,2794,0,0,0,0,3136,0,0,2093,0,622,0,831,0,932,0,1046,0,622,0,831,0,1046,0,0,1865,0,622,0,784,0,831,0,932,0,622,0,784,0,932,0,0,1661,0,523,0,698,0,784,0,831,0,523,0,698,0,831,0,0,1568,0,1046,0,1244,0,1397,0,1568,0,1046,0,1244,0,1397,0,0,0,1661,0,1046,0,1175,0,1244,0,831,0,1175,0,1244,0,0,0,0,2489,0,1397,0,0,0,2350,0,0,0,2489,0,0,0,2350,0,0,0,0,2093,0,392,0,415,0,466,0,523,0,392,0,415,0,466,0,523,0,392,0,415,0,466,0,2093,0,1568,0,1661,0,1865,0,2093,0,1568,0,1661,0,1865,0,2093,0,1568,0,1661,0,1865};
int tiempoGOT[518] = {900,89,900,89,133,13,133,13,600,59,600,59,133,13,133,13,1400,1400,900,89,900,89,133,13,133,13,600,59,900,89,133,13,133,13,1200,116,267,28,267,28,267,28,900,89,900,89,1400,89,69,7,69,7,69,7,69,7,900,89,900,89,133,13,133,13,600,59,600,59,133,13,133,13,1800,1800,900,89,900,89,133,13,133,13,600,59,900,89,133,13,133,13,1200,2400,69,7,69,7,69,7,69,7,900,89,900,89,133,13,133,13,600,59,600,59,133,13,133,13,1800,1800,900,89,900,89,133,13,133,13,600,59,900,89,133,13,133,13,1200,2400,3600,900,89,900,900,89,900,133,13,150,133,13,150,600,59,600,600,59,600,133,13,150,133,13,150,1200,400,69,7,69,7,69,7,69,7,267,28,400,45,133,13,267,28,267,28,267,28,300,900,89,900,900,89,900,133,13,150,133,13,150,600,59,600,900,89,900,133,13,150,133,13,150,1200,1800,3600,900,89,900,900,89,900,133,13,150,133,13,150,600,59,600,600,59,600,133,13,150,133,13,150,1200,400,267,28,1200,400,133,13,133,13,150,900,89,900,900,89,900,600,59,600,267,28,300,600,59,600,267,28,300,1200,2400,3600,267,28,267,28,133,13,133,13,267,28,267,28,133,13,133,13,150,267,28,267,28,133,13,133,13,133,13,267,28,267,28,133,13,150,267,28,267,28,133,13,133,13,267,28,267,28,133,13,133,13,150,267,28,267,28,133,13,133,13,267,28,267,28,133,13,133,13,150,150,600,59,133,13,133,13,267,28,267,28,133,13,133,13,150,150,150,900,89,900,900,900,900,89,900,900,900,1200,2400,3600,267,28,267,28,133,13,133,13,267,28,267,28,133,13,133,13,150,267,28,267,28,133,13,133,13,267,28,267,28,133,13,133,13,150,267,28,267,28,133,13,133,13,267,28,267,28,133,13,133,13,150,267,28,267,28,133,13,133,13,267,28,267,28,133,13,133,13,150,150,600,59,133,13,133,13,267,28,267,28,133,13,133,13,150,150,150,600,212,133,13,150,150,267,28,300,300,400,45,450,450,133,13,150,150,150,267,28,267,28,133,13,133,13,267,28,267,28,133,13,133,13,267,28,267,28,133,13,2400,116,267,28,267,28,133,13,133,13,267,28,267,28,133,13,133,13,267,28,267,28,133,13,2400};
int frecuenciaTetris[55] = {1319,988,1047,1175,1047,988,880,880,1047,1319,1175,1047,988,988,1047,1175,1319,1047,880,880,0,1175,1397,1760,1568,1397,1319,1047,1319,1175,1047,988,988,1047,1175,1319,1047,880,880,0,659,523,587,494,523,440,415,659,523,587,494,523,659,880,831};
int tiempoTetris[55] = {450,225,225,450,225,225,450,225,225,450,225,225,450,225,225,450,450,450,450,450,675,450,225,450,225,225,675,225,450,225,225,450,225,225,450,450,450,450,450,450,900,900,900,900,900,900,1800,900,900,900,900,450,450,900,1800};
int frecuenciaStarwars[59] = {523,0,523,0,523,0,698,0,1046,0,0,880,0,784,0,1397,0,523,0,1760,0,0,880,0,784,0,1397,0,523,0,1760,0,0,880,0,784,0,1397,0,523,0,1760,0,0,880,0,1760,0,0,784,0,523,0,0,523,0,0,523,0};
int tiempoStarwars[59] = {134,134,134,134,134,134,536,134,536,134,134,134,134,134,134,536,134,402,134,134,429,357,134,134,134,134,536,134,402,134,134,429,357,134,134,134,134,536,134,402,134,134,429,357,134,134,134,429,357,1071,268,67,67,268,67,67,67,67,67};

//------------------------------------------------------
// void InicializaMelodia (TipoMelodia *melodia)
//
// Descripcion: inicializa los parametros del objeto melodia.
// Ejemplo de uso:
//
// ...
//
// TipoMelodia melodia_demo;
//
// if ( InicializaMelodia (&melodia_demo, "STARWARS", frecuenciaStarwars, tiempoStarwars, 59) < 0 ) {
// 		printf("\n[ERROR!!!][InicializaMelodia]\n");
// 		fflush(stdout);
// 		}
//
// ...
//
//------------------------------------------------------
int InicializaMelodia (TipoMelodia *melodia, char *nombre, int *array_frecuencias, int *array_duraciones, int num_notas) {
/**melodia->nombre = *nombre;
 melodia->num_notas=num_notas;
for( i=0;i<num_notas;i++){
	melodia->frecuencias[i]=array_frecuencias[i];
	melodia->duraciones[i]=array_duraciones[i];
}
	return melodia->num_notas;
	*/
	int i;
		strncpy(melodia->nombre,nombre,20);
		for(i=0;i<num_notas;i++){
			melodia->frecuencias[i] = array_frecuencias[i];
			melodia->duraciones[i] = array_duraciones[i];
		}
		melodia->num_notas = num_notas;
		return melodia->num_notas;
}

void InicializaPlayer (fsm_t* this) {
	TipoPlayer *p_player = (TipoPlayer *)(this->user_data);

	piLock(FLAGS_KEY);
	flags &= ~FLAG_PLAYER_START;
	flags &= ~FLAG_PLAYER_END;
	flags &= ~FLAG_PLAYER_STOP;
	flags &= ~FLAG_NOTA_TIMEOUT;
	piUnlock(FLAGS_KEY);

	piLock (FLAGS_KEY);
		flags |= FLAG_PLAYER_START;
	piUnlock (FLAGS_KEY);

	piLock(STD_IO_BUFFER_KEY);
	p_player->posicion_nota_actual = 0;
	p_player->frecuencia_nota_actual = p_player->melodia->frecuencias[p_player->posicion_nota_actual];
	p_player->duracion_nota_actual = p_player->melodia->duraciones[p_player->posicion_nota_actual];
	tmr_startms(p_player->tmr,p_player->duracion_nota_actual);
	piUnlock(STD_IO_BUFFER_KEY);

	piLock(STD_IO_BUFFER_KEY);
	printf("\n[PLAYER][InicializaPlayer][COMIENZA MELODIA]");
	piUnlock(STD_IO_BUFFER_KEY);
	flags=0;
}
void ActualizaPlayer (fsm_t* this) {
	TipoPlayer *p_player = (TipoPlayer *)(this->user_data);

	piLock(FLAGS_KEY);
	flags &= ~FLAG_PLAYER_START;
	flags &= ~FLAG_PLAYER_END;
	flags &= ~FLAG_PLAYER_STOP;
	flags &= ~FLAG_NOTA_TIMEOUT;
	piUnlock(FLAGS_KEY);

	piLock(STD_IO_BUFFER_KEY);
	if(p_player->posicion_nota_actual<p_player->melodia->num_notas){
		p_player->posicion_nota_actual++;
		p_player->frecuencia_nota_actual = p_player->melodia->frecuencias[p_player->posicion_nota_actual];
		p_player->duracion_nota_actual = p_player->melodia->duraciones[p_player->posicion_nota_actual];
		flags=0;
	}else {
		flags |= FLAG_PLAYER_END;
	}
	piUnlock(STD_IO_BUFFER_KEY);

	piLock(STD_IO_BUFFER_KEY);
	printf("\n[PLAYER][ActualizaPlayer][NUEVA NOTA (%d DE %d)]\n", p_player->posicion_nota_actual, p_player->melodia->num_notas);
	piUnlock(STD_IO_BUFFER_KEY);
}

void ComienzaNuevaNota (fsm_t* this) {
	TipoPlayer *p_player = (TipoPlayer *)(this->user_data);

	piLock (FLAGS_KEY);
	flags &= ~FLAG_PLAYER_START;
	flags &= ~FLAG_PLAYER_STOP;
	flags &= ~FLAG_PLAYER_END;
	flags &= ~FLAG_NOTA_TIMEOUT;
	flags &= ~FLAG_NOTA_TIMEOUT;
	piUnlock (FLAGS_KEY);

	int frecuencia = p_player->frecuencia_nota_actual;
	tmr_startms(p_player->tmr,p_player->duracion_nota_actual);
	softToneWrite(18,frecuencia);

	piLock (STD_IO_BUFFER_KEY);
	printf("\n[PLAYER][ComienzaNuevaNota][NOTA %d][FREC %d][DURA %d]\n", p_player->posicion_nota_actual, p_player->frecuencia_nota_actual, p_player->duracion_nota_actual);
	piUnlock (STD_IO_BUFFER_KEY);
	flags=0;
}

void StopPlayer (fsm_t* fsm) {

	piLock(FLAGS_KEY);
	flags &= ~FLAG_PLAYER_START;
	flags &= ~FLAG_PLAYER_END;
	flags &= ~FLAG_PLAYER_STOP;
	flags &= ~FLAG_NOTA_TIMEOUT;
	piUnlock(FLAGS_KEY);

	piLock(STD_IO_BUFFER_KEY);
	softToneWrite(18,0);
	piUnlock(STD_IO_BUFFER_KEY);

	piLock(STD_IO_BUFFER_KEY);
	printf("\n[PLAYER][StopPlayer][MELODIA INTERRUMPIDA]");
	piUnlock(STD_IO_BUFFER_KEY);
	flags=0;

}

void FinalMelodia (fsm_t *fsm) {
	TipoPlayer *p_player = (TipoPlayer *)(fsm->user_data);

	piLock (FLAGS_KEY);
	flags &= ~FLAG_PLAYER_START;
	flags &= ~FLAG_PLAYER_STOP;
	flags &= ~FLAG_PLAYER_END;
	flags &= ~FLAG_NOTA_TIMEOUT;
	piUnlock (FLAGS_KEY);

	p_player->frecuencia_nota_actual=0;
	p_player->posicion_nota_actual=0;


	piLock (STD_IO_BUFFER_KEY);
	printf("FINAL MELODIA\n");
	piUnlock (STD_IO_BUFFER_KEY);
	flags=0;
}

//Funcion de atencion a la interrupcion del temporizador
void timer_isr(union sigval arg){
	piLock (FLAGS_KEY);
	flags |= FLAG_NOTA_TIMEOUT;
	piUnlock (FLAGS_KEY);
	piLock (STD_IO_BUFFER_KEY);
	printf("Ha saltado el temporizador");
	piUnlock (STD_IO_BUFFER_KEY);
	//softToneWrite(18,0);
}

//Funciones de entrada o comprobaciÛn de la m·quina de estados
int CompruebaPlayerStart(fsm_t* this){
	int result;
	piLock(FLAGS_KEY);
	result = (flags & FLAG_PLAYER_START);
	piUnlock(FLAGS_KEY);
	return result;
}

int CompruebaPlayerStop(fsm_t* this){
	int result;
	piLock(FLAGS_KEY);
	result = (flags & FLAG_PLAYER_STOP);
	piUnlock(FLAGS_KEY);
	return result;
}

int CompruebaFinalMelodia(fsm_t* this){
	int result;
	piLock(FLAGS_KEY);
	result = (flags & ~FLAG_PLAYER_END);
	piUnlock(FLAGS_KEY);
	return result;
}

int CompruebaNotaTimeout (fsm_t* this) {
	int result;
	piLock (FLAGS_KEY);
	result = (flags & FLAG_NOTA_TIMEOUT);
	piUnlock (FLAGS_KEY);
	return result;
}
int CompruebaNuevaNota(fsm_t* this){
	int result;
	piLock(FLAGS_KEY);
	result = (flags & FLAG_PLAYER_END);
	piUnlock(FLAGS_KEY);
	return result;
}

//------------------------------------------------------
// FUNCIONES DE INICIALIZACION
//------------------------------------------------------

// int systemSetup (void): procedimiento de configuracion del sistema.
// Realizar√°, entra otras, todas las operaciones necesarias para:
// configurar el uso de posibles librer√≠as (e.g. Wiring Pi),
// configurar las interrupciones externas asociadas a los pines GPIO,
// configurar las interrupciones peri√≥dicas y sus correspondientes temporizadores,
// crear, si fuese necesario, los threads adicionales que pueda requerir el sistema
int systemSetup (TipoSistema *p_sistema) {
	/*
	wiringPiSetupGpio();
	pinMode(18,OUTPUT);
	pinMode(24,INPUT);
*/
	int x = 0;

		piLock (STD_IO_BUFFER_KEY);

		// sets up the wiringPi library
		if (wiringPiSetupGpio () < 0) {
			printf ("Unable to setup wiringPi\n");
			piUnlock (STD_IO_BUFFER_KEY);
			return -1;
	    }

		// Lanzamos thread para exploracion del teclado convencional del PC
		x = piThreadCreate (thread_explora_teclado);

		if (x != 0) {
			printf ("it didn't start!!!\n");
			piUnlock (STD_IO_BUFFER_KEY);
			return -1;
	    }

		piUnlock (STD_IO_BUFFER_KEY);

		p_sistema->player.tmr = tmr_new(timer_isr);

		softToneCreate(18);

		return 1;
}

void fsm_setup(fsm_t* fsm){
    piLock (FLAGS_KEY);
    flags=0;
    piUnlock (FLAGS_KEY);
    StopPlayer(fsm);

}



PI_THREAD (thread_explora_teclado) {
	int teclaPulsada;

	while(1) {
		delay(10); // Wiring Pi function: pauses program execution for at least 10 ms

		piLock (STD_IO_BUFFER_KEY);

		if(kbhit()) {
			teclaPulsada = kbread();

			switch(teclaPulsada) {
				case 's':
					piLock (FLAGS_KEY);
					flags |= FLAG_PLAYER_START;
					piUnlock (FLAGS_KEY);
					break;

				case 't':
					piLock (FLAGS_KEY);
					flags |= FLAG_PLAYER_STOP;
					piUnlock (FLAGS_KEY);
					break;

				case 'q':
					exit(0);
					break;

				default:
					printf("INVALID KEY!!!\n");
					break;
			}
		}

		piUnlock (STD_IO_BUFFER_KEY);
	}
}
int main (){

	unsigned int next;
	TipoSistema sistema;
    //TipoMelodia melodia_demo;

    //Maquina de estados: lista de transiciones
    fsm_trans_t reproductor[] ={
    		{ WAIT_START, CompruebaPlayerStart, WAIT_PUSH, InicializaPlayer },
    		{ WAIT_PUSH, CompruebaPlayerStop, WAIT_START, StopPlayer },
    		{ WAIT_END, CompruebaFinalMelodia, WAIT_START, FinalMelodia },
    		{ WAIT_END, CompruebaNuevaNota, WAIT_PUSH, ComienzaNuevaNota },
    		{ WAIT_PUSH, CompruebaNotaTimeout, WAIT_END, ActualizaPlayer },
    		{-1, NULL, -1, NULL }
    };

	// Configuracion e inicializacion del sistema
	// A completar por el alumno...
    systemSetup(&sistema);
    		sistema.player.melodia = &(sistema.tarjetas_activas[0].melodia);
    		if ( InicializaMelodia (sistema.player.melodia, "TETRIS", frecuenciaTetris, tiempoTetris, 55) < 0 ) {
    						printf("\n[ERROR!!!][InicializaMelodia]\n");
    						fflush(stdout);
    		}
/*

	 systemSetup(&sistema);
	 InicializaMelodia(&melodia_demo,"STARWARS",frecuenciaStarwars,tiempoStarwars,59);
	 if ( InicializaMelodia (&melodia_demo, "STARWARS", frecuenciaStarwars, tiempoStarwars, 59) < 0 ) {
		 		printf("\n[ERROR!!!][InicializaMelodia]\n");
		 		fflush(stdout);
	 }
	 */
	 fsm_t* player_fsm = fsm_new(WAIT_START,reproductor,&(sistema.player));
	 fsm_setup(player_fsm);
	 next = millis();
	 while(1) {
		 fsm_fire (player_fsm);
		 next += CLK_MS;
		 delay_until(next);
	 }
	 fsm_destroy(player_fsm);
	 //sistema.player.melodia=&(melodia_demo);
	 //sistema.estado=WAIT_START;

return 0;
}

