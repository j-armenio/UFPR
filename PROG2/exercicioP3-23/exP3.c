#include <stdio.h>

typedef unsigned char byte;

byte invertBits1(byte c) {
    return c ^ 0x30; // 0011 0000 : Quinto e sexto
}

byte invertBits2(byte c) {
    return c ^ 0x80; // 1000 0000 : Mais significativo
}

byte invertBits3(byte c) {
    return c ^ 0x01; // 0000 0001 : Menos significativo
}

byte invertBits4(byte c) {
    return c ^ 0x0A; // 0000 1010 : Segundo e quarto bit
}

int main() {
    byte (*encodeFuncs[4])(byte) = { 
        invertBits1,
        invertBits2,
        invertBits3,
        invertBits4
    };

    const char* inputFileName = "input.txt";
    const char* outputFileName = "output.txt";

    FILE* inputFile = fopen(inputFileName, "rb");
    FILE* outputFile = fopen(outputFileName, "wb+");

    byte buffer[4];

    unsigned int checkR, checkW;

    while (! feof(inputFile)) 
    {
        checkR = fread(buffer, 1, 4, inputFile);

        for (int i = 0; i < sizeof(buffer); i++){
            byte encodedChar = encodeFuncs[i](buffer[i]);
        }
        checkW = fwrite(buffer, 1, checkR, outputFile);
    }

    fclose(inputFile);
    fclose(outputFile);

    printf("Arquivo codificado com sucesso.\n");

    FILE* encodedFile = fopen(outputFileName, "rb");
    FILE* decodedFile = fopen("decoded.txt", "wb");

    while (fread(buffer, sizeof(byte), sizeof(buffer), encodedFile)) {
        for (int i = 0; i < sizeof(buffer); i++) {
            byte decodedChar = encodeFuncs[i](buffer[i]);
            fwrite(&decodedChar, sizeof(byte), 1, decodedFile);
        }
    }

    fclose(encodedFile);
    fclose(decodedFile);

    printf("Arquivo decodificado com sucesso.\n");

    return 0;
}