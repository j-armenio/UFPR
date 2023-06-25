#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 100

typedef union{
  struct{
    char b1:1;
    char b2:1;
    char b3:1;
    char b4:1;
    char b5:1;
    char b6:1;
    char b7:1;
    char b8:1;
  };
  char B;
} manager;

char f1(char input){
  manager transformer;
  transformer.B = input;
  if (transformer.b3) transformer.b3 = 0;
  else transformer.b3 = 1;
  if (transformer.b4) transformer.b4 = 0;
  else transformer.b4 = 1;
  return transformer.B;
}

char f2(char input){
  manager transformer;
  transformer.B = input;
  if (transformer.b8) transformer.b8 = 0;
  else transformer.b8 = 1;
  return transformer.B;
}

char f3(char input){
  manager transformer;
  transformer.B = input;
  if (transformer.b1) transformer.b1 = 0;
  else transformer.b1 = 1;
  return transformer.B;
}

char f4(char input){
  manager transformer;
  transformer.B = input;
  if (transformer.b2) transformer.b2 = 0;
  else transformer.b2 = 1;
  if (transformer.b4) transformer.b4 = 0;
  else transformer.b4 = 1;
  return transformer.B;
}


int main(int argc, char** argv){
  unsigned int check_r, check_w;
  char buffer[BUFFER_SIZE];
  FILE *input, *output;
  char (*ponteiros[4])(char) = {f1, f2, f3, f4};
  
  if (argc != 3){
    printf("Para usar o programa execute: %s input output\n", argv[0]);
    return 1;
  }

  input = fopen(argv[1], "rb");
  if (!input){
    printf("O arquivo de entrada não foi aberto!\n");
    return 2;
  }
  
  output = fopen(argv[2], "wb+");
  if (!output){
    printf("O arquivo de saída não foi aberto!\n");
    return 3;
  }
  
  while(!feof(input)){
    check_r = fread(buffer, 1, 4, input);
    for (int i = 0; i < check_r; i++){
    	buffer[i] = ponteiros[i](buffer[i]);
    }
    check_w = fwrite(buffer, 1, check_r, output);
    if (check_w != check_r){
    	printf("Houve um problema de escrita!\n");
    	return 3;
    }
  }
  
}