/*Proyecto de termino medio.
Este programa recibe una tabla de verdad y devuelve dos funciones completas y reducidas.
Una de estas es para F=1 (SPL) y la otra para F=0. (PSL)*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define variables 3             //cantidad fija de entradas
#define filas (1 << variables)  //la cantidad de filas es 2^n para n = variables, esta linea hace eso

typedef struct{                 //Se define el tipo de estructura que contiene 
    int logic_in[variables];    //un array para las entradas y
    int logic_out;              //un entero para las salidas
} TablaDeVerdad;

void LeerTV(TablaDeVerdad *tabla) {
    char combinacion[variables + 1];        // Combinación actual (entradas)
    combinacion[variables] = '\0';  
    
    for (int i = 0; i < variables; i++) {   // Inicializa la combinación con valores de 0
        combinacion[i] = '0';
    }
    
    for (int fila_actual = 0; fila_actual < filas; fila_actual++) {  // Genera todas las combinaciones posibles y solicita las salidas al usuario
        for (int i = 0; i < variables; i++) {                        // Genera la combinación de entradas (valores 0 y 1)
            tabla[fila_actual].logic_in[i] = combinacion[i] - '0';
        }        
        printf("%s|", combinacion);
        char salida;
        scanf(" %c", &salida); 
        if (salida != '0' && salida != '1') {
            printf("Error: Por favor ingrese 0 o 1 como salida.\n");
            exit(EXIT_FAILURE);
        }
        tabla[fila_actual].logic_out = salida - '0';
        for (int i = variables - 1; i >= 0; i--) { // Incrementa la combinación para la siguiente fila
            if (combinacion[i] == '0') {
                combinacion[i] = '1';
                break;
            } else {
                combinacion[i] = '0';
            }
        }
    }
}

bool SonReducibles(char *termino1, char *termino2, bool sp_ps) {
    int diferencia = 0;
    if(sp_ps){       //compara terminos de spl
        for (int i = 0; termino1[i] != '\0' && termino2[i] != '\0'; i++) {
            if (termino1[i] != termino2[i]) {
                diferencia++;
                if (diferencia > 1) {
                    return false; 
                }
            }
        }
    }
    else{             //compara factores de psl
        for (int i = 1; termino1[i] != '\0' && termino2[i] != '\0'; i += 2) { // Iterar de a 2 para omitir los caracteres que no son variables
            if (termino1[i] != termino2[i]) {
                diferencia++;
                if (diferencia > 1) {
                    return false; 
                }
            }
        }
    }
    return true; // Si hay exactamente una diferencia, los términos son reducibles
}

void ReducirFunc(char *funcion, bool sp_ps) {//version casi bien
    char *terminos[filas];
    int num_terminos = 0;
    int pos_borrar = 0;
    char *token = strtok(funcion, sp_ps ? "+" : "*");
    while (token != NULL) {
        terminos[num_terminos++] = token;
        token = strtok(NULL, sp_ps ? "+" : "*");
    }

    char funcion_reducida[22];              //cadena más larga "(a+b)*(B+c)*(B+C)*(A+b)" (todas las salidas = 0)
    funcion_reducida[0] = '\0';
    bool termino_reducido[filas] = {false}; //array para identificar los ya reducidos

    for (int i = 0; i < num_terminos; i++) {
        if (!termino_reducido[i]) {
            bool termino_agregado = false;

            for (int j = i + 1; j < num_terminos; j++) {
                if (!termino_reducido[j] && SonReducibles(terminos[i], terminos[j], sp_ps)) {
                    char termino_comun[strlen(terminos[i])+1];     // Tamaño máximo para el término común
                    int index = 0;
                    for (int k = 0; terminos[i][k] != '\0'; k++) { //Arma el termino reducido
                        if (terminos[i][k] == terminos[j][k]) {    //si las variables son iguales quedan, se elimina la complemento
                            termino_comun[index++] = terminos[i][k];
                        }
                        else{  //al eliminar una variable en PSL siempre queda un signo de más, la posicion de dicho signo se marca a continuacion
                            if(k == 1 || k == 3){ 
                                pos_borrar = k;   //posicion del signo para la reduccion de A o B
                            }
                            else{ 
                                pos_borrar = k-1; //posicion del signo para la reduccion de C
                            }
                        }
                    }
                    if(!sp_ps){                   //solo queda el signo extra al reducir PSL
                    memmove(&termino_comun[pos_borrar], &termino_comun[pos_borrar + 1], strlen(termino_comun) - pos_borrar); //elimina el '+' errado 
                    }
                    termino_comun[index] = '\0';
                    strcat(funcion_reducida, termino_comun); //Agrega el termino reducido a la funcion
                    strcat(funcion_reducida, sp_ps ? "+" : "*"); 
                    termino_reducido[i] = true;             //marca los ya reducidos en el array
                    termino_reducido[j] = true;
                    termino_agregado = true;
                    break;
                }
            }

            if (!termino_agregado) {
                strcat(funcion_reducida, terminos[i]);    // Agregar el término no reducido a la función reducida              
                if (i != num_terminos - 1) {              // Agregar el delimitador solo si no es el último término
                    strcat(funcion_reducida, sp_ps ? "+" : "*");
                }
            }
        }
    }
    char ultimo_caracter = funcion_reducida[strlen(funcion_reducida) - 1];
    if (ultimo_caracter == '+' || ultimo_caracter == '*') {
        funcion_reducida[strlen(funcion_reducida) - 1] = '\0'; // Elimina el último signo
    }
    printf("Funcion reducida: %s\n", funcion_reducida);
}

int SPL(TablaDeVerdad *tabla, char *funcion_spl) {
    int contador_spl = 0;
    for (int i = 0; i < filas; i++) {
        if (tabla[i].logic_out == 1) {
            contador_spl++;
            for (int j = 0; j < variables; j++) {
                char var = 'A' + j;                  // Genera la letra correspondiente (A, B, C)
                if (tabla[i].logic_in[j] == 1) {
                    strncat(funcion_spl, &var, 1);   // Agrega la variable al final de la cadena
                } else {
                    var += ('a' - 'A');              // Convierte la letra a minúscula si la entrada es 0
                    strncat(funcion_spl, &var, 1);   // Agrega la variable al final de la cadena
                }
            }
            strcat(funcion_spl, "+");                // Agrega el singo separador
        }
    }
    if (contador_spl > 0) {
        funcion_spl[strlen(funcion_spl) - 1] = '\0'; // Elimina el último separador '+'
    }
    printf("\nPara la Suma de Productos Lógicos la función completa es:\n%s\n", funcion_spl);
    ReducirFunc(funcion_spl, true);
    return contador_spl;
}

int PSL(TablaDeVerdad *tabla, char *funcion_psl) {
    int contador_psl = 0;
    for (int i = 0; i < filas; i++) {
        if (tabla[i].logic_out == 0) {
            contador_psl++;
            strcat(funcion_psl, "(");                  // Agrega el paréntesis inicial
            for (int j = 0; j < variables; j++) {
                char var = 'A' + j;                    // Genera la letra correspondiente (A, B, C, ...)
                if (tabla[i].logic_in[j] == 1) {
                    strncat(funcion_psl, &var, 1);     // Agrega la variable al final de la cadena
                    strcat(funcion_psl, "+");          // Agrega el signo '+' entre variables
                } else {
                    var += ('a' - 'A');                // Convierte la letra a minúscula si la entrada es 0
                    strncat(funcion_psl, &var, 1);     // Agrega la variable al final de la cadena
                    strcat(funcion_psl, "+");          // Agrega el signo '+' entre variables
                }
            }
            funcion_psl[strlen(funcion_psl) - 1] = ')'; // Reemplaza el último '+' con el paréntesis de cierre
            strcat(funcion_psl, "*");                   // Agrega el signo '*' entre factores
        }
    }
    if (contador_psl > 0) {
        funcion_psl[strlen(funcion_psl) - 1] = '\0';    // Elimina el último signo '*' (no hay más factores)
    }
    printf("\nPara el Producto de Sumas Lógicas la función completa es:\n%s\n", funcion_psl);
    ReducirFunc(funcion_psl, false);
    return contador_psl;
}

int main(){
    char funcion_spl[(variables*filas)+3*(filas-1)];    //tamaño -> terminos + signos ' + '          
    funcion_spl[0] = '\0';                              // Inicializa la cadena vacía
    char funcion_psl[5*(variables*filas)+3*(filas-1)];  //tamaño -> factores + signos ' + ' + '( + + )'        
    funcion_psl[0] = '\0';                      
    TablaDeVerdad tabla[filas];                         //Cada elemento de tabla contiene logic_in[] y logic_out
    printf("Complete la tabla de verdad:\n");
    LeerTV(tabla);
    int spl =SPL(tabla, funcion_spl);
    int psl = PSL(tabla, funcion_psl);
    if (spl < psl){
        printf("\nConviene utilizar la Suma de Productos Logicos\n\n");
    }
    else if (psl > spl){
        printf("\nConviene utilizar el Producto de Suma Logicas\n");
    }
    else if (psl == spl){
        printf("\nAmbas funciones tienen la misma cantidad de terminos y factores\n");
    }
}
