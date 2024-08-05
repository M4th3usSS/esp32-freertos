// Criando macro: 
#define setBit(word, bit)    (word |= (1<<bit))
#define clearBit(word, bit)  (word &= ~(1<<bit))
#define toogleBit(word, bit) (word ^= (1<<bit))
#define testBit(word, bit)   (word & (1<<bit))

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *binario(int numero);

int main(void)
{

    // Deslocamento de bits a esquerda
    // variável << (número de bits adicionados a esquerda)

    __uint8_t number1 = 0b00000111;

    printf("%s\n", binario(number1));      // 00000111
    printf("%s\n", binario(number1 << 1)); // 00001110
    printf("%s\n", binario(number1 << 2)); // 00011100
    printf("%s\n", binario(number1 << 3)); // 00111000
    printf("%s\n", binario(number1 << 4)); // 01110000

    // Deslocamento de bits a direita
    // variável >> (número de bits adicionados a esquerda)

    __uint8_t number2 = 0b11100000;

    printf("%s\n", binario(number2));      // 11100000
    printf("%s\n", binario(number2 >> 1)); // 01110000
    printf("%s\n", binario(number2 >> 2)); // 00111000
    printf("%s\n", binario(number2 >> 3)); // 00011100
    printf("%s\n", binario(number2 >> 4)); // 00001110

    /**
     * Ativando um bit:
     *
     * Para ativar um bit especifico de uma variável ou registrador, ou seja,
     * colocar nível lógico 1, utiliza-se o operador |.
     */

    __uint8_t word1 = 0b00000000;

    printf("\nAtivando um bit: %s\n", binario(word1));

    // Ativando o quinto bit:
    word1 |= (1 << 5); // (1<<5) é equivalente há: 00010000
    printf("Ativando um bit: %s\n", binario(word1));

    /**
     * Limpando um bit:
     *
     * Para limpar um bit específico de uma variável ou registrador, ou seja,
     * colocar nível lógico 0, utiliza-se o operador & (AND).
     */

    __uint8_t word2 = 0b00010000;

    printf("\nLimpando um bit: %s\n", binario(word2));
    word2 &= (~(1 << 4)); // 00010000 & 11101111
    printf("Limpando um bit: %s\n", binario(word2));

    /**
     * Invertendo o estado de um bit
     *
     * Para inverter o estado lógico de um bit utiliza-se o operador ^ (OU-Exclusivo).
     */

    __uint8_t word3 = 0b00000000;

    printf("\nInvertendo um bit: %s\n", binario(word3));
    word3 ^= (1 << 6); // 00000000 ^ 01000000
    printf("Invertendo um bit: %s\n", binario(word3));
    word3 ^= (1 << 6); // 01000000 ^ 01000000
    printf("Invertendo um bit: %s\n", binario(word3));

    /**
     * Testando valor de um bit
     *
     * Para teste de um bit também utilizaremos a lógica & (AND) para operação com a máscara
     * de bits.
     */

    __uint8_t word4 = 0b10101010;

    printf("\nTeste bit a bit:\n");

    for (__uint8_t i = 0; i < 8; i++)
    {
        if (word4 & (1 << i))
        {
            printf("Bit%d ", i);
        }
    }
    printf("\n\n");

    __uint8_t wordUsandoMacros = 0b00000000;
    
    printf("Estado 1 - %s\n", binario(wordUsandoMacros));
    
    printf("Estado 2 - Usando macro de setBit para setar bit 7: %s\n", binario(setBit(wordUsandoMacros, 7)));
    printf("Estado 3 - Usando macro de setBit para setar bit 6: %s\n", binario(setBit(wordUsandoMacros, 6)));
    printf("Estado 4 - Usando macro de setBit para setar bit 4: %s\n", binario(setBit(wordUsandoMacros, 4)));
    printf("Estado 5 - Usando macro de setBit para setar bit 7: %s\n", binario(setBit(wordUsandoMacros, 2)));
    printf("Estado 6 - Usando macro de setBit para setar bit 6: %s\n", binario(setBit(wordUsandoMacros, 1)));
    printf("Estado 7 - Usando macro de setBit para setar bit 4: %s\n", binario(setBit(wordUsandoMacros, 0)));

    printf("Estado 8 - Usando macro de clear para limpar bit 4: %s\n", binario(clearBit(wordUsandoMacros, 4)));
    printf("Estado 9 - Usando macro de clear para limpar bit 7: %s\n", binario(clearBit(wordUsandoMacros, 7)));

    printf("Estado 10 - Usando macro de toggle para mudar o bit 7: %s\n", binario(toogleBit(wordUsandoMacros, 7)));
    printf("Estado 11 - Usando macro de toggle para mudar o bit 7: %s\n", binario(toogleBit(wordUsandoMacros, 7)));

    testBit(wordUsandoMacros, 7) ? printf("Bit 7 = 1\n") : printf("Bit 7 = 0\n");
    toogleBit(wordUsandoMacros, 7); // Mudando o bit 7
    testBit(wordUsandoMacros, 7) ? printf("Bit 7 = 1\n") : printf("Bit 7 = 0\n");


    return 0;
}

// Função para converter um número inteiro em uma string binária
char *binario(int numero)
{
    // Calcula o número máximo de bits necessário
    int numBits = sizeof(__uint8_t) * 8; // Número de bits em um int (ex: 32 bits)

    // Aloca memória para a string binária mais o caractere nulo
    char *binario = (char *)malloc(numBits + 1);
    if (binario == NULL)
    {
        perror("Falha ao alocar memória");
        exit(EXIT_FAILURE);
    }

    // Define o caractere nulo
    binario[numBits] = '\0';

    // Converte o número para a representação binária
    for (int i = numBits - 1; i >= 0; i--)
    {
        if (numero & (1 << i))
        {
            binario[numBits - 1 - i] = '1';
        }
        else
        {
            binario[numBits - 1 - i] = '0';
        }
    }

    return binario;
}