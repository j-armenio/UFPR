#include <stdio.h>
#include <stdint.h>
#include "meuAlocador.h"

int main (long int argc, char** argv) {
  void *a,*b,*c,*d,*e;
  int64_t estado;

  iniciaAlocador();    
  imprimeMapa();       // OK
  // 0) estado inicial

  a=(void *) alocaMem(100);
  // 1 OK
  imprimeMapa();
  b=(void *) alocaMem(130);
  // 2 OK
  imprimeMapa(); 
  c=(void *) alocaMem(120);
  // 3 OK
  imprimeMapa();
  d=(void *) alocaMem(110);
  // 4 OK
  imprimeMapa();
  // 1) Espero ver quatro segmentos ocupados
  // OK

  liberaMem(b);
  // 5 OK
  imprimeMapa();
  liberaMem(d);
  imprimeMapa(); 
  // 6 OK
  // 2) Espero ver quatro segmentos alternando
  //    ocupados e livres
    
  estado = *((int64_t*)b-2);
  b=(void *) alocaMem(50);
  // 7
  imprimeMapa();
  d=(void *) alocaMem(90);
  // 8
  imprimeMapa();
  e=(void *) alocaMem(40);
  // 9
  imprimeMapa();
  // 3) Deduzam
	
  liberaMem(c);
  // 10
  imprimeMapa(); 
  liberaMem(a);
  // 11
  imprimeMapa();
  liberaMem(b);
  // 12
  imprimeMapa();
  liberaMem(d);
  // 13
  imprimeMapa();
  liberaMem(e);
  imprimeMapa();
  // 14
   // 4) volta ao estado inicial

  finalizaAlocador();
}
