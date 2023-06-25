#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 4

int main()
{
	FILE *input, *output;
	unsigned int sizeRead;
	
	input = fopen("input.txt", "rb");
	output = fopen("output.txt", "wb+");

	if (! input || ! output){
		printf("Erro ao abrir arquivos de entrada.\n");
		return 1;
	}

	char *buffer; 
	
	buffer = malloc(sizeof(char) * BUFFER_SIZE);
	
	if (! buffer){
		printf("Erro ao alocar buffer\n");
		return 1;
	}

	while ((sizeRead = fread(buffer, 1, 4, input)) > 0)
	{
		printf("%s\n", buffer);

		int aux = buffer[0];
		aux += 1;
		fwrite(&aux, 1, 1, output);

		for (int i = 1; i < 4; i++)
			buffer[i] -= 32;
		fwrite(&buffer[1], 1, sizeRead-1, output);

		char lineBreak = '\n';
		fwrite(&lineBreak, 1, 1, output);

		fseek(input, 1, SEEK_CUR);
	}

	free(buffer);
	fclose(input);
	fclose(output);
	
	return 0;
}
