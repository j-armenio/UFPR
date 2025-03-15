#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <linux/if_packet.h>
#include <net/ethernet.h>
#include <net/if.h>

#include "utils.h"
#include "message.h"

// Função para definir o valor de Tam (bits 0 a 5)
void setTam(uint16_t *header, uint16_t tam) {
    *header &= ~TAM_MASK; // Limpa os bits de Tam
    *header |= (tam & TAM_MASK);  // Define Tam
}

// Função para definir o valor de Seq (bits 6 a 10)
void setSeq(uint16_t *header, uint16_t seq) {
    *header &= ~SEQ_MASK; // Limpa os bits de Seq
    *header |= (seq << SEQ_SHIFT) & SEQ_MASK;  // Define Seq
}

// Função para definir o valor de Type (bits 11 a 15)
void setType(uint16_t *header, uint16_t type) {
    *header &= ~TYPE_MASK; // Limpa os bits de Type
    *header |= (type << TYPE_SHIFT) & TYPE_MASK; // Define Type
}

// Função para extrair o valor de Tam (bits 0 a 5)
uint16_t getTam(uint16_t header) {
    return header & TAM_MASK;
}

// Função para extrair o valor de Seq (bits 6 a 10)
uint16_t getSeq(uint16_t header) {
    return (header & SEQ_MASK) >> SEQ_SHIFT;
}

// Função para extrair o valor de Type (bits 11 a 15)
uint16_t getType(uint16_t header) {
    return (header & TYPE_MASK) >> TYPE_SHIFT;
}
