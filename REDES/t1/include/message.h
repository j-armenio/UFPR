#ifndef MESSAGE_H
#define MESSAGE_H

#include <stdint.h>

// Máscaras para setar campos da mensagem
#define TAM_MASK  0x003F  // 6 bits (0000000000111111)
#define SEQ_MASK  0x07C0  // 5 bits (0000011111000000)
#define TYPE_MASK 0xF800  // 5 bits (1111100000000000)

// Deslocamento para acessar campos da mensagem
#define SEQ_SHIFT 6
#define TYPE_SHIFT 11

// Atributos da mensagem
#define INIT_MARKER 0x7E
#define MAX_DATA_SIZE 63

// Tipos da mensagem
#define ACK 0x00
#define NACK 0x01
#define OK 0x02

#define BACKUP 0x04
#define RESTORE 0x05
#define VERIFY 0x06
#define EXIT 0x07

#define OKCHECKSUM 0x0D
#define OKSIZE 0x0E

#define SIZE 0x0F
#define DATA 0x10
#define END 0x11

#define ERROR 0x1F

//Códigos de erro
#define ERROR_ACCESS_DENIED 1
#define ERROR_NOT_ENOUGH_SPACE 2
#define ERROR_CANT_FIND_FILE 3

// Remover alinhamento para mensagem ter exatamente 67 bytes (536 bits)
#pragma pack(push, 1) // Alinha todos os membros da estrutura em 1 byte
typedef struct {
    uint8_t MI;       // 8 bits: Marcador de Inicio
    uint16_t Header;  // 16 bits (2 bytes): Header = Tam (6bits); Seq (5bits); Type (5bits)
    unsigned char Data[MAX_DATA_SIZE]; // até 63 bytes (504 bits)
    uint8_t CRC;      // 8 bits
} Message;
#pragma pack(pop)

void setTam(uint16_t *header, uint16_t tam);

void setSeq(uint16_t *header, uint16_t seq);

void setType(uint16_t *header, uint16_t type);

uint16_t getTam(uint16_t header);

uint16_t getSeq(uint16_t header);

uint16_t getType(uint16_t header);

#endif