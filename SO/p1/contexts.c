// PingPongOS - PingPong Operating System
// Prof. Carlos A. Maziero, DINF UFPR
// Versão 1.4 -- Janeiro de 2022

// Demonstração das funções POSIX de troca de contexto (ucontext.h).

// operating system check
#if defined(_WIN32) || (!defined(__unix__) && !defined(__unix) && (!defined(__APPLE__) || !defined(__MACH__)))
#warning Este codigo foi planejado para ambientes UNIX (LInux, *BSD, MacOS). A compilacao e execucao em outros ambientes e responsabilidade do usuario.
#endif

/* para compilar no MacOS */
#define _XOPEN_SOURCE 600

// Importações de declarações de bibliotecas
#include <stdio.h>
#include <stdlib.h>
#include <ucontext.h>

/* tamanho de pilha das threads */
#define STACKSIZE 64*1024

// Variáveis globais de contexto
ucontext_t ContextPing, ContextPong, ContextMain ;

/*****************************************************/

// Função chamada quando o contextPing é inicializado
void BodyPing (void * arg)
{
   // Inicializa um int i
   int i ;

   // Printa que deu inicio ao contexto BodyPing
   printf ("%s: inicio\n", (char *) arg) ;

   // Loop que acontece 4 vezes
   for (i=0; i<4; i++)
   {
      // Printa a string do contexto e a iteração que o loop está
      printf ("%s: %d\n", (char *) arg, i) ;
      // Carrega ContextPong salvando o contexto atual em ContextPing
      swapcontext (&ContextPing, &ContextPong) ;
   }
   // Printa que chegou ao final da execução do ContextPing
   printf ("%s: fim\n", (char *) arg) ;

   // Salva o contexto atual em ContextPing e carrega ContextMain
   swapcontext (&ContextPing, &ContextMain) ;
}

/*****************************************************/

// Função chamada quando o contextPong é inicializado
void BodyPong (void * arg)
{
   int i ;

   printf ("%s: inicio\n", (char *) arg) ;

   for (i=0; i<4; i++)
   {
      printf ("%s: %d\n", (char *) arg, i) ;
      // Carrega ContextPing salvando o contexto atual em ContextPong
      swapcontext (&ContextPong, &ContextPing) ;
   }
   printf ("%s: fim\n", (char *) arg) ;

   // Salva o contexto atual em ContextPong e carrega ContextMain
   swapcontext (&ContextPong, &ContextMain) ;
}

/*****************************************************/

int main (int argc, char *argv[])
{
   // Ponteiro para stack do contexto. É em char pois char tem 1 byte
   char *stack ;

   // Print que sinaliza inicio do contexto Main
   printf ("main: inicio\n") ;

   // Salva o contexto atual da execução na variável ContextPing
   getcontext (&ContextPing) ;

   // Aloca memória para a pilha do contexto
   stack = malloc (STACKSIZE) ;
   if (stack)
   { // Se a alocação deu certo
      ContextPing.uc_stack.ss_sp = stack ; // ss_sp é o stack pointer (base da pilha)
      ContextPing.uc_stack.ss_size = STACKSIZE ; // ss_size é o tamanho da pilha
      ContextPing.uc_stack.ss_flags = 0 ; // ss_flags são flags de stack
      ContextPing.uc_link = 0 ; //uc_link é um ponteiro para outro contexto que será retornado quando o atual terminar sua execução
   }
   else
   { // Se a alocação deu
      perror ("Erro na criação da pilha: ") ;
      exit (1) ;
   }

   // Ajusta o ContextPing para executar a função BodyPing com o parâmetro "   Ping" quando inicializado. 1 é a quantidade de argumentos de BodyPing
   makecontext (&ContextPing, (void*)(*BodyPing), 1, "    Ping") ;

   // Salva o contexto atual em ContextPong
   getcontext (&ContextPong) ;

   // A variável stack é reutilizada, mas cada contexto deve ter sua própria pilha, logo ele perde referência a pilha alocada em ContextPing
   stack = malloc (STACKSIZE) ;
   if (stack)
   {
      ContextPong.uc_stack.ss_sp = stack ;
      ContextPong.uc_stack.ss_size = STACKSIZE ;
      ContextPong.uc_stack.ss_flags = 0 ;
      ContextPong.uc_link = 0 ;
   }
   else
   {
      perror ("Erro na criação da pilha: ") ;
      exit (1) ;
   }

   // Ajusta o ContextPong com a função BodyPong com parâmetro "     Pong"
   makecontext (&ContextPong, (void*)(*BodyPong), 1, "        Pong") ;

   // Salva o contexto atual em ContextMain e carrega ContextPing, que vai executar BodyPing
   swapcontext (&ContextMain, &ContextPing) ;
   swapcontext (&ContextMain, &ContextPong) ;

   // Printa o fim do programa
   printf ("main: fim\n") ;

   exit (0) ;
}

