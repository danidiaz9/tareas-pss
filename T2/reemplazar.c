#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "reemplazar.h"

char *reemplazo(char *s, char c, char *pal) {
    int largo_pal = strlen(pal);
    int contador = 0;
    char *recorre_s = s;
    //contamos las veces que aparece c en s.
    while (*recorre_s) {
        if (*recorre_s == c) {
            contador += largo_pal;
        }
        else {
            contador++;
        }
        recorre_s++;
    }
    //calculamos el largo del string resultante.
    //usamos malloc para pedir memoria para el nuevo string
    char *res = (char *)malloc((contador + 1) * sizeof(char));
    //recorremos s y pegamos los caracteres en res
    char *recorre_res = res;
    char *recorre_s_final = s;
    while (*recorre_s_final) {
        if (*recorre_s_final == c) {
            strcpy(recorre_res, pal);
            recorre_res += largo_pal;    
        }
        else {
            *recorre_res++ = *recorre_s_final;
        }
        ++recorre_s_final;
    }
    //agregamos la terminación '\0' del string
    *recorre_res = '\0';
    return res;
}

void reemplazar(char *s, char c, char *pal) {   
    int contador = 0;
    char *recorre_s = s;
    int largo_pal = strlen(pal);
    while (*recorre_s) {
        if (*recorre_s == c) {   
            contador += largo_pal;           
        }
        else {
            contador++;
        }
        recorre_s++;
    }
    char *recorre_s_final = s;
    char *recorre_res_final = s;
    //largo de pal a lo mas uno, se recorre s de principio a fin.
    if (strlen(pal) <= 1) {
        while (*recorre_s_final) {
            if (*recorre_s_final == c) {
                if (*pal != '\0') {
                    *recorre_res_final++ = *pal;
                }              
            }
            else {
                *recorre_res_final++ = *recorre_s_final;
            }
            recorre_s_final++;
        }
        *recorre_res_final = '\0';
    }
    //de lo contrario se recorre s en orden inverso.
    else {
        recorre_s_final += strlen(s);
        *recorre_s_final-- = ' ';
        recorre_res_final += contador;
        *recorre_res_final-- = '\0';
        while (recorre_s_final >= s) {
            if (*recorre_s_final == c) {
                char *recorre_pal_final = pal + (largo_pal - 1);
                while (recorre_pal_final >= pal) {
                    *recorre_res_final-- = *recorre_pal_final--;
                }
                recorre_s_final--;    
            }
            else {
                *recorre_res_final-- = *recorre_s_final--;
            }           
        }           
    }
}