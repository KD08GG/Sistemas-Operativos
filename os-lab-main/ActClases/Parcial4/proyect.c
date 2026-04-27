#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <pthread.h>

#define NUM_ROWS 17
#define NUM_COLS 10
#define MAX_LEN_WORD 10
#define MAX_LEN_DESC 100
#define ALARM_SECONDS 30

// ─────────────────────────────────────────────────────────────
// ESTRUCTURAS
// ─────────────────────────────────────────────────────────────

typedef struct {
    char word[MAX_LEN_WORD];
    char desc[MAX_LEN_DESC];
} Word;

typedef struct {
    int idx_h1, idx_h2, idx_h3;   // índice de variante activa (0-N)
    int idx_v1, idx_v2, idx_v3;
    int sol_h1, sol_h2, sol_h3;   // 1 = resuelta, 0 = no
    int sol_v1, sol_v2, sol_v3;
} Estado;

// ─────────────────────────────────────────────────────────────
// PALABRAS
// Restricciones de intersección:
//   H1(casa)[1]  == V1(col6,fila4)[3]  → letra 'a'
//   H2(perro)[0] == V2(col2,fila6)[0]  → letra 'p'
//   H3(barro)[4] == V3(col5,fila11)[0] → letra 'o'  (barro[4]='o', comino[0]='c') ← ver nota
//
// NOTA: barro termina en 'o' (col5 fila11) y comino empieza en 'c' (col5 fila11)
// → NO coinciden con las palabras base. Por eso elegimos variantes que sí coincidan.
// Las variantes de cada slot comparten la misma letra en la intersección.
// ─────────────────────────────────────────────────────────────

// H1: fila 4, columnas 5-8 (4 letras)
// Intersección con V1 en [4][6] → índice 1 de la palabra horizontal debe coincidir con V1[3]
// V1 empieza en fila 1 col 6, entonces fila4 = posición 3 de V1 → V1[3]
// Todas las variantes de H1 tienen letra 'a' en posición 1: casa✓ cara✓ capa✓ cama✓ caza✓
Word h1[5] = {
    {"casa", "Lugar donde viven las personas."},
    {"cara", "Parte del cuerpo donde estan los ojos, nariz y boca."},
    {"capa", "Prenda que se usa sobre la ropa, como la de los superheroes."},
    {"cama", "Mueble donde duermes y descansas."},
    {"caza", "Actividad de buscar y atrapar animales."}
};

// H2: fila 6, columnas 2-6 (5 letras)
// Intersección con V2 en [6][2] → índice 0 de H2 debe coincidir con V2[0]
// V2 empieza en fila 6 col 2 → posición 0 de V2
// Todas las variantes de H2 empiezan con 'p': perro✓ pacto✓ pasto✓ plato✓ plomo✓
Word h2[5] = {
    {"perro", "Animal domestico conocido como el mejor amigo del hombre."},
    {"pacto", "Acuerdo entre dos o mas personas."},
    {"pasto", "Planta verde que comen los animales como las vacas."},
    {"plato", "Utensilio donde se sirve la comida."},
    {"plomo", "Metal pesado de color gris."}
};

// H3: fila 11, columnas 0-5 (6 letras → usamos ganado/pasado/etc que son 6 letras)
// Intersección con V3 en [11][5] → índice 5 de H3 debe coincidir con V3[0]
// V3 empieza en fila 11 col 5 → posición 0 de V3
// Variantes de H3 terminan en 'o': ganado✗ → usamos palabras de 6 letras que en pos[5] tengan 'o'
// ganado[5]='o' ✓, pasado[5]='o' ✓, casado[5]='o' ✓
// V3[0] debe ser 'o' también → comino[0]='c' ✗
// Solución: usar variantes de V3 que empiecen con 'o' no existe fácilmente,
// entonces reposicionamos: H3 columnas 0-4 (5 letras) e intersección en col 5 separada
// Para simplicidad: H3 col 1-5, intersección con V3 en col 5 → H3[4] == V3[0]
// barro[4]='o', carro[4]='o', barco[4]='o' ✓ y V3 variantes empiezan con 'o': NO
// → Cambiamos V3 a palabras que empiecen con 'o':
// ovejas, océano... O cambiamos H3 para que col5 sea la misma letra inicial de V3
// DECISIÓN FINAL: V3 empieza en fila 12 col 5 (no en 11), e H3 termina en col 4
// Así no hay intersección H3-V3 directa y agregamos intersección V3 con H2:
// V3: filas 6-11 col 5, H2 fila 6 col 5 → H2[3] == V3[0]
// perro[3]='r', pacto[3]='t'... no uniforme.
// 
// DECISIÓN SIMPLE Y LIMPIA (para que compile y funcione bien):
// Mantenemos tu layout original y elegimos palabras que SÍ intersectan:
//
//   H3: col 1-5, fila 11 → "barro","carro","barco","jarro","campo" (todas tienen 'r' en [2])
//   V2: col 2, filas 6-11 → "planta","piedra","puerta","prueba","paloma" (todas tienen 'a' en [4])
//   Intersección V2∩H3: [11][2] → V2[5] y H3[1]
//   planta[5]='a', barro[1]='a' ✓ → ¡coinciden!
//
//   V3: col 5, filas 10-15 → "comino","conejo","tomate","sonido","modelo"
//   H3 col5 fila11 → H3[4] y V3[1]
//   barro[4]='o', comino[1]='o' ✓ → ¡coinciden!
//
// Resumen final de intersecciones válidas:
//   [4][6]  H1[1]='a'  V1[3]='a'  (casa[1]='a',  gascdo → gasdor → "gasado"... )
//   Reajuste V1: filas 1-6, col 6 → V1[3] es fila4
//   V1 variantes: todas tienen 'a' en pos[3]:
//   "gasado" no existe... usemos: planta✗(p-l-a-n-t-a, [3]='n')
//   puerta([3]='r')✗, piedra([3]='d')✗
//   → V1 debe tener 'a' en posición 3. Palabras de 6 letras con 'a' en pos3:
//   "amapas"... difícil. 
//
// SOLUCIÓN FINAL: Movemos H1 a col 5-8 pero la intersección es en col 6 = H1[1]
// V1 col 6 filas 1-6, intersección en fila 4 = V1[3]
// Necesitamos H1[1]='a' → casa✓,cara✓,capa✓,cama✓,caza✓ — todas tienen 'a' en [1] ✓
// Necesitamos V1[3]='a' → palabra de 6 letras con 'a' en posición 3:
// "pasada"✓(p-a-s-a-d-a,[3]='a'), "canada"✓, "lavanda"(7)✗, "parada"✓
// Usemos: parada, canada, pasada, casada, mirada → todas tienen 'a' en [3] ✓

Word h1_final[5] = {
    {"casa", "Lugar donde viven las personas."},
    {"cara", "Parte del cuerpo donde estan los ojos, nariz y boca."},
    {"capa", "Prenda que se usa sobre la ropa."},
    {"cama", "Mueble donde duermes y descansas."},
    {"caza", "Actividad de buscar y atrapar animales."}
};
// Todas tienen [1]='a' ✓

Word v1_final[5] = {
    {"parada", "Detencion o lugar donde algo se detiene."},
    {"canada", "Pais ubicado al norte de Estados Unidos."},
    {"pasada", "Algo que ya ocurrio o que ha transcurrido."},
    {"casada", "Mujer que tiene matrimonio."},
    {"mirada", "Accion de dirigir los ojos hacia algo."}
};
// Todas tienen [3]='a' ✓

// H2: fila 6, col 2-6, intersección con V2 en [6][2] → H2[0] == V2[0]
// V2 col 2, filas 6-11 → V2[0] es fila 6
// H2[0] y V2[0] deben coincidir → ambos 'p': perro✓
Word h2_final[5] = {
    {"perro", "Animal domestico conocido como el mejor amigo del hombre."},
    {"pacto", "Acuerdo entre dos o mas personas."},
    {"pasto", "Planta verde que comen los animales."},
    {"plato", "Utensilio donde se sirve la comida."},
    {"plomo", "Metal pesado de color gris."}
};
// Todas tienen [0]='p' ✓

Word v2_final[5] = {
    {"planta", "Ser vivo que crece en la tierra."},
    {"piedra", "Material duro que se encuentra en la naturaleza."},
    {"puerta", "Parte de una casa que sirve para entrar o salir."},
    {"prueba", "Evaluacion para medir conocimientos."},
    {"paloma", "Ave comun de color blanco o gris."}
};
// Todas tienen [0]='p' ✓

// H3: fila 11, col 1-5 (5 letras), intersección con V2 en [11][2] → H3[1] == V2[5]
// V2 filas 6-11, fila11 = posición 5 → V2[5]
// planta[5]='a', piedra[5]='a', puerta[5]='a', prueba[5]='a', paloma[5]='a' → V2[5]='a' siempre ✓
// H3[1] debe ser 'a': barro[1]='a'✓, carro[1]='a'✓, barco[1]='a'✓, jarro[1]='a'✓, campo[1]='a'✓
// Intersección con V3 en [11][5] → H3[4] == V3[1]
// V3 col 5, filas 10-15 → fila11 = posición 1 → V3[1]
// comino[1]='o', conejo[1]='o', tomate[1]='o', sonido[1]='o', modelo[1]='o' → V3[1]='o' siempre ✓
// H3[4]: barro[4]='o'✓, carro[4]='o'✓, barco[4]='o'✓, jarro[4]='o'✓, campo[4]='o'✓
Word h3_final[5] = {
    {"barro", "Mezcla de tierra y agua que se vuelve blanda."},
    {"carro", "Vehiculo con cuatro ruedas para transportarse."},
    {"barco", "Medio de transporte que navega por el agua."},
    {"jarro", "Recipiente usado para servir liquidos."},
    {"campo", "Terreno abierto fuera de la ciudad."}
};
// Todas tienen [1]='a' y [4]='o' ✓

Word v3_final[5] = {
    {"comino", "Semilla usada como condimento en la comida."},
    {"conejo", "Animal pequeno con orejas largas que salta."},
    {"tomate", "Fruta roja que se usa mucho en la comida."},
    {"sonido", "Vibracion que se puede escuchar con los oidos."},
    {"modelo", "Representacion que sirve como referencia."}
};
// Todas tienen [1]='o' ✓

// ─────────────────────────────────────────────────────────────
// POSICIONES EN EL TABLERO (índices 0-based)
// H1: fila 4, cols 5-8
// H2: fila 6, cols 2-6
// H3: fila 11, cols 1-5
// V1: col 6, filas 1-6
// V2: col 2, filas 6-11
// V3: col 5, filas 10-15
// ─────────────────────────────────────────────────────────────

// ─────────────────────────────────────────────────────────────
// VARIABLES GLOBALES
// ─────────────────────────────────────────────────────────────
char dashboard[NUM_ROWS][NUM_COLS];
pid_t padre_pid;
pid_t pid_cambios;
pid_t pid_input;
pthread_mutex_t mutex_pantalla = PTHREAD_MUTEX_INITIALIZER;

// ─────────────────────────────────────────────────────────────
// ARCHIVO tablero.dat
// ─────────────────────────────────────────────────────────────

void guardar_estado(Estado *e) {
    FILE *f = fopen("tablero.dat", "wb");
    if (!f) { perror("fopen guardar"); return; }
    fwrite(e, sizeof(Estado), 1, f);
    fclose(f);
}

void leer_estado(Estado *e) {
    FILE *f = fopen("tablero.dat", "rb");
    if (!f) { perror("fopen leer"); return; }
    fread(e, sizeof(Estado), 1, f);
    fclose(f);
}

// Lock/unlock con archivo de semáforo
void lock_archivo() {
    while (1) {
        FILE *f = fopen("tablero.lock", "wx"); // 'x' = falla si existe
        if (f) { fclose(f); return; }
        usleep(5000); // espera 5ms y reintenta
    }
}

void unlock_archivo() {
    remove("tablero.lock");
}

// ─────────────────────────────────────────────────────────────
// TABLERO: llenar y dibujar
// ─────────────────────────────────────────────────────────────

void llenar_dashboard(Estado *e) {
    // Limpiar
    for (int i = 0; i < NUM_ROWS; i++)
        for (int j = 0; j < NUM_COLS; j++)
            dashboard[i][j] = ' ';

    char *p;

    // H1: fila 4, cols 5-8
    p = h1_final[e->idx_h1].word;
    for (int j = 5; j <= 8; j++)
        dashboard[4][j] = e->sol_h1 ? p[j-5] : '_';

    // H2: fila 6, cols 2-6
    p = h2_final[e->idx_h2].word;
    for (int j = 2; j <= 6; j++)
        dashboard[6][j] = e->sol_h2 ? p[j-2] : '_';

    // H3: fila 11, cols 1-5
    p = h3_final[e->idx_h3].word;
    for (int j = 1; j <= 5; j++)
        dashboard[11][j] = e->sol_h3 ? p[j-1] : '_';

    // V1: col 6, filas 1-6
    p = v1_final[e->idx_v1].word;
    for (int i = 1; i <= 6; i++)
        dashboard[i][6] = e->sol_v1 ? p[i-1] : '_';

    // V2: col 2, filas 6-11
    p = v2_final[e->idx_v2].word;
    for (int i = 6; i <= 11; i++)
        dashboard[i][2] = e->sol_v2 ? p[i-6] : '_';

    // V3: col 5, filas 10-15
    p = v3_final[e->idx_v3].word;
    for (int i = 10; i <= 15; i++)
        dashboard[i][5] = e->sol_v3 ? p[i-10] : '_';
}

void imprimir_dashboard(Estado *e) {
    system("clear");
    printf("\n=== LA CASA DE HOJAS ===\n\n");

    for (int i = 0; i < NUM_ROWS; i++) {
        for (int j = 0; j < NUM_COLS; j++) {
            if (dashboard[i][j] == ' ')
                printf("   ");
            else
                printf("[%c]", dashboard[i][j]);
        }
        printf("\n");
    }

    printf("\n--- PISTAS ---\n");
    printf("Horizontales:\n");
    printf("  H1 (fila 5):  %s\n", e->sol_h1 ? "[RESUELTA]" : h1_final[e->idx_h1].desc);
    printf("  H2 (fila 7):  %s\n", e->sol_h2 ? "[RESUELTA]" : h2_final[e->idx_h2].desc);
    printf("  H3 (fila 12): %s\n", e->sol_h3 ? "[RESUELTA]" : h3_final[e->idx_h3].desc);
    printf("Verticales:\n");
    printf("  V1 (col 7):   %s\n", e->sol_v1 ? "[RESUELTA]" : v1_final[e->idx_v1].desc);
    printf("  V2 (col 3):   %s\n", e->sol_v2 ? "[RESUELTA]" : v2_final[e->idx_v2].desc);
    printf("  V3 (col 6):   %s\n", e->sol_v3 ? "[RESUELTA]" : v3_final[e->idx_v3].desc);

    printf("\nIngresa el numero de palabra (H1/H2/H3/V1/V2/V3): ");
    fflush(stdout);
}

int todas_resueltas(Estado *e) {
    return e->sol_h1 && e->sol_h2 && e->sol_h3 &&
           e->sol_v1 && e->sol_v2 && e->sol_v3;
}

// ─────────────────────────────────────────────────────────────
// SEÑALES DEL PADRE
// ─────────────────────────────────────────────────────────────

void padre_sigusr1(int sig) {
    // hijo_cambios aviso: cambio de palabra
    Estado e;
    lock_archivo();
    leer_estado(&e);
    unlock_archivo();

    llenar_dashboard(&e);
    imprimir_dashboard(&e);
    printf("\n*** La casa ha cambiado... una palabra se transformo ***\n");
    fflush(stdout);

    if (todas_resueltas(&e)) {
        printf("\n¡Felicidades! Resolviste todo el crucigrama.\n");
        kill(pid_cambios, SIGTERM);
        kill(pid_input,   SIGTERM);
        waitpid(pid_cambios, NULL, 0);
        waitpid(pid_input,   NULL, 0);
        remove("tablero.dat");
        remove("tablero.lock");
        exit(0);
    }
}

void padre_sigusr2(int sig) {
    // hijo_input aviso: el usuario acerto una palabra
    Estado e;
    lock_archivo();
    leer_estado(&e);
    unlock_archivo();

    llenar_dashboard(&e);
    imprimir_dashboard(&e);
    printf("\n*** ¡Correcto! Palabra fijada. ***\n");
    fflush(stdout);

    if (todas_resueltas(&e)) {
        printf("\n¡Felicidades! Resolviste todo el crucigrama.\n");
        kill(pid_cambios, SIGTERM);
        kill(pid_input,   SIGTERM);
        waitpid(pid_cambios, NULL, 0);
        waitpid(pid_input,   NULL, 0);
        remove("tablero.dat");
        remove("tablero.lock");
        exit(0);
    }
}

// ─────────────────────────────────────────────────────────────
// HIJO 1: hijo_cambios
// ─────────────────────────────────────────────────────────────

void sigalrm_handler(int sig) {
    Estado e;

    lock_archivo();
    leer_estado(&e);

    // Rotar solo las palabras no resueltas
    if (!e.sol_h1) e.idx_h1 = (e.idx_h1 + 1) % 5;
    if (!e.sol_h2) e.idx_h2 = (e.idx_h2 + 1) % 5;
    if (!e.sol_h3) e.idx_h3 = (e.idx_h3 + 1) % 5;
    if (!e.sol_v1) e.idx_v1 = (e.idx_v1 + 1) % 5;
    if (!e.sol_v2) e.idx_v2 = (e.idx_v2 + 1) % 5;
    if (!e.sol_v3) e.idx_v3 = (e.idx_v3 + 1) % 5;

    guardar_estado(&e);
    unlock_archivo();

    kill(padre_pid, SIGUSR1);
    alarm(ALARM_SECONDS);
}

void hijo_cambios_func() {
    signal(SIGALRM, sigalrm_handler);
    signal(SIGTERM, SIG_DFL);
    alarm(ALARM_SECONDS);
    while (1) pause();
}

// ─────────────────────────────────────────────────────────────
// HIJO 2: hijo_input (con dos hilos)
// ─────────────────────────────────────────────────────────────

volatile int notificar = 0;

// Hilo B: recibe aviso de cambio y lo muestra sin bloquear al hilo A
void *hilo_notificaciones(void *arg) {
    while (1) {
        if (notificar) {
            pthread_mutex_lock(&mutex_pantalla);
            printf("\n[!] La casa cambio mientras escribias. El tablero se actualizo.\n");
            printf("Ingresa el numero de palabra (H1/H2/H3/V1/V2/V3): ");
            fflush(stdout);
            pthread_mutex_unlock(&mutex_pantalla);
            notificar = 0;
        }
        usleep(100000); // revisa cada 100ms
    }
    return NULL;
}

// Señal para avisarle al hilo_notificaciones
void hijo_input_sigusr1(int sig) {
    notificar = 1;
}

// Hilo A: lee input del usuario y valida
void *hilo_lectura(void *arg) {
    char opcion[4];
    char respuesta[MAX_LEN_WORD];

    while (1) {
        pthread_mutex_lock(&mutex_pantalla);
        // Leer opción
        pthread_mutex_unlock(&mutex_pantalla);

        if (scanf("%3s", opcion) != 1) continue;

        pthread_mutex_lock(&mutex_pantalla);
        printf("Ingresa tu respuesta: ");
        fflush(stdout);
        pthread_mutex_unlock(&mutex_pantalla);

        if (scanf("%9s", respuesta) != 1) continue;

        // Convertir a minúsculas
        for (int i = 0; respuesta[i]; i++)
            if (respuesta[i] >= 'A' && respuesta[i] <= 'Z')
                respuesta[i] += 32;

        Estado e;
        lock_archivo();
        leer_estado(&e);

        char *correcta = NULL;
        int ya_resuelta = 0;

        if      (strcmp(opcion,"H1")==0||strcmp(opcion,"h1")==0) { correcta=h1_final[e.idx_h1].word; ya_resuelta=e.sol_h1; }
        else if (strcmp(opcion,"H2")==0||strcmp(opcion,"h2")==0) { correcta=h2_final[e.idx_h2].word; ya_resuelta=e.sol_h2; }
        else if (strcmp(opcion,"H3")==0||strcmp(opcion,"h3")==0) { correcta=h3_final[e.idx_h3].word; ya_resuelta=e.sol_h3; }
        else if (strcmp(opcion,"V1")==0||strcmp(opcion,"v1")==0) { correcta=v1_final[e.idx_v1].word; ya_resuelta=e.sol_v1; }
        else if (strcmp(opcion,"V2")==0||strcmp(opcion,"v2")==0) { correcta=v2_final[e.idx_v2].word; ya_resuelta=e.sol_v2; }
        else if (strcmp(opcion,"V3")==0||strcmp(opcion,"v3")==0) { correcta=v3_final[e.idx_v3].word; ya_resuelta=e.sol_v3; }

        if (!correcta) {
            unlock_archivo();
            pthread_mutex_lock(&mutex_pantalla);
            printf("Opcion invalida. Usa H1,H2,H3,V1,V2,V3.\n");
            printf("Ingresa el numero de palabra (H1/H2/H3/V1/V2/V3): ");
            fflush(stdout);
            pthread_mutex_unlock(&mutex_pantalla);
            continue;
        }

        if (ya_resuelta) {
            unlock_archivo();
            pthread_mutex_lock(&mutex_pantalla);
            printf("Esa palabra ya fue resuelta.\n");
            printf("Ingresa el numero de palabra (H1/H2/H3/V1/V2/V3): ");
            fflush(stdout);
            pthread_mutex_unlock(&mutex_pantalla);
            continue;
        }

        if (strcmp(respuesta, correcta) == 0) {
            // Marcar como resuelta
            if      (strcmp(opcion,"H1")==0||strcmp(opcion,"h1")==0) e.sol_h1=1;
            else if (strcmp(opcion,"H2")==0||strcmp(opcion,"h2")==0) e.sol_h2=1;
            else if (strcmp(opcion,"H3")==0||strcmp(opcion,"h3")==0) e.sol_h3=1;
            else if (strcmp(opcion,"V1")==0||strcmp(opcion,"v1")==0) e.sol_v1=1;
            else if (strcmp(opcion,"V2")==0||strcmp(opcion,"v2")==0) e.sol_v2=1;
            else if (strcmp(opcion,"V3")==0||strcmp(opcion,"v3")==0) e.sol_v3=1;

            guardar_estado(&e);
            unlock_archivo();

            kill(padre_pid, SIGUSR2);
        } else {
            unlock_archivo();
            pthread_mutex_lock(&mutex_pantalla);
            printf("Incorrecto. Intenta de nuevo.\n");
            printf("Ingresa el numero de palabra (H1/H2/H3/V1/V2/V3): ");
            fflush(stdout);
            pthread_mutex_unlock(&mutex_pantalla);
        }
    }
    return NULL;
}

void hijo_input_func() {
    signal(SIGUSR1, hijo_input_sigusr1); // padre redirige para notificar cambio
    signal(SIGTERM, SIG_DFL);

    pthread_t hilo_a, hilo_b;
    pthread_mutex_init(&mutex_pantalla, NULL);

    pthread_create(&hilo_b, NULL, hilo_notificaciones, NULL);
    pthread_create(&hilo_a, NULL, hilo_lectura, NULL);

    pthread_join(hilo_a, NULL);
    pthread_join(hilo_b, NULL);
}

// ─────────────────────────────────────────────────────────────
// MAIN — proceso padre
// ─────────────────────────────────────────────────────────────

int main() {
    padre_pid = getpid();

    // Estado inicial
    Estado e;
    memset(&e, 0, sizeof(Estado));
    remove("tablero.lock"); // limpiar lock viejo si existe
    guardar_estado(&e);

    // Dibujar tablero inicial
    llenar_dashboard(&e);
    imprimir_dashboard(&e);

    // Registrar señales del padre
    signal(SIGUSR1, padre_sigusr1);
    signal(SIGUSR2, padre_sigusr2);

    // Fork hijo_cambios
    pid_cambios = fork();
    if (pid_cambios == 0) {
        hijo_cambios_func();
        exit(0);
    }

    // Fork hijo_input
    pid_input = fork();
    if (pid_input == 0) {
        hijo_input_func();
        exit(0);
    }

    // Padre espera señales
    while (1) pause();

    // Limpieza (no se llega aquí normalmente, se sale desde el handler)
    waitpid(pid_cambios, NULL, 0);
    waitpid(pid_input,   NULL, 0);
    remove("tablero.dat");
    remove("tablero.lock");
    return 0;
}