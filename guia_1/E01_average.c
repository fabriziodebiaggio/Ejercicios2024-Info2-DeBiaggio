/* Escriba un programa que calcule e imprima el promedio de varios enteros.
Debe pedir al usuario que primero ingrese los valores que desee uno a uno. 
Cuando haya ingresado los numeros deseados debe ingresar el valor 9999, para asi proceder al calculo
De Biaggio Fabrizio 2024*/
#include <stdio.h>
#include <stdbool.h>
int main(){
    printf("Ingrese uno a uno los numero a promediar \n Ingrese 9999 para calcular\n");
    bool stop = true;
    int num_ing = 0, suma = 0, prom = 0, cont = 0;
    while (stop == true) {
        scanf("%d", &num_ing);
        if (num_ing != 9999){
            suma += num_ing;
            num_ing = 0;
            cont ++;
        }
        else {
            stop = false;
        }
    }
    if (cont != 0){
            prom = suma / cont;
    printf("Usted ingreso %d numeros\n El total de la suma es %d \n El promedio es %d", cont, suma, prom);
    }
    else {
        printf("Dale pa, no pongas 9999 si te estoy diciendo que es para calcular, mete algun otro valor, no seas porfiado");
    }
}