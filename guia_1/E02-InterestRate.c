/*Calculo de interes compuesto en un periodo de 10 años con 
tasas de interes del 5%, 6%, 8%, 9% y 10%
De Biaggio Fabrizio*/
#include <stdio.h>
#include <math.h>
int main()
{
    double m_final, m_inicial= 1000;
    int anio;
    printf("%4s %4s %21s \n", "Tasa" "   Anio","   Monto del deposito" );
    float interes[5]={0.05, 0.06, 0.08, 0.09, 0.1};
    for (int i=0; i<5; i++){
        for(anio = 1; anio < 11; anio++){
            m_final = m_inicial*pow(1+interes[i],anio);
            printf("%1.2f %4d %15.2f \n", interes[i], anio, m_final);
        }
        printf("-------------------------\n");
    }
}