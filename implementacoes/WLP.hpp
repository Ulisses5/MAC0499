// Ulisses Paixão Cordeiro
// Versão Inicial - Word Level Parallelism

// A classe WLP reúne diferentes técnicas de manipulação
// de bits para obter resultados interessantes em tempo constante

#include <bits/stdc++.h>

using namespace std;

constexpr int W = 16;

class WLP {
private:

    // Nível de debug, pode ser 0, 1 ou 2
    // 0: Nenhum debug
    // 1: Debug apenas do MSB
    // 2: Debug de todas as operações
    int DebugWLP;

    // BlocksLastBitSet: para blocos de tamanho s, é uma palavra com os bits de índices
    // 0, s+1, ... , (n-1)(s+2) setados
    // Exemplo, para W = 20 e s = 5 é:
    // 00001 00001 00001 00001
    // (usualmente s = k+1, uma chave de tamanho k + um bit sentinela)
    unsigned int GetBlocksLastBitSet(int s);
    
    // BlocksFirstBitSet: para blocos de tamanho s, é uma palavra com os bits de índices
    // s, 2*s, ... , (n-1)*s setados
    // Exemplo, para W = 20 e s = 5 é:
    // 10000 10000 10000 10000
    // (usualmente s = k+1, uma chave de tamanho k + um bit sentinela)
    unsigned int GetBlocksFirstBitSet(int s);

    // Mask: uma máscara de s bits setados como 1
    // Exemplo, para s = 5 é:
    // 11111
    unsigned int GetBitsMask(int s);

    // P: array contendo as primeiras s potências de 2
    // separadas por um bit 0.
    // Potências 2^0, 2^1, ... , 2^(s-1)
    // (usualmente s = k = (sqrt(w)))
    unsigned int GetPowersOfTwo(int s);

    // Input: 
    // Uma palavra R de tamanho W com s blocos em que o primeiro bit de cada bloco nos interessa
    // Um inteiro s com o número de bits em cada bloco
    // Output: Uma palavra que contém apenas os bits de interesse
    unsigned int PerfectSketch(unsigned int R, int s);

    // Input: 
    // Uma palavra X de tamanho W
    // O tamanho s dos seus blocos
    // Output: Uma palavra de tamanho s com o sumario dos blocos de X
    unsigned int NonEmptyClusters(unsigned int X, int s);

public:
    
    WLP();
    
    // Retorna o atual nível de debug
    int GetDebugLevel();

    // Input: Um inteiro new_debug, pode assumir os valores 0, 1 ou 2
    // Muda o nível de debug para o nível escolhido
    // 0: Nenhum debug
    // 1: Debug apenas do MSB
    // 2: Debug de todas as operações
    void ChangeDebugLevel(int new_debug);

    // Imprime os blocos de uma palavra de tamanho W
    // separa os bits em blocos exatos de tamanho s
    void PrintWord(bitset<W> bits, int s);

    // Input: Duas palavras A e B. A contém números a_0, ... , a_n 
    // e cada número tem s bits. Entre cada número há um bit 0 de espaço.
    // O mesmo vale para B, com b_0, ... , b_n.
    // Output: Uma palavra R com o resultado de a_i >= b_i nos bits que 
    // ocupavam o espaço entre cada número nos arrays de input
    unsigned int ParallelComparison(unsigned int A, unsigned int B, int s);

    // Input: Uma chave q de tamanho s
    // Output: Uma palavra Q de tamanho W em que q se repete n vezes,
    // onde n = W/(s+1), com as repetições interespaçados por um bit 1.
    unsigned int ParallelTile(unsigned int q, int s);

    // Input: Uma palavra R com W/(s+1) bits de interesse separados por s bits 0.
    // Output: A soma dos bits de interesse.
    // Exemplo: R = a0000 b0000 c0000 d0000
    //          Sum = a + b + c + d
    int ParallelSum(unsigned int R, int s);

    // Input:
    // Um array de inteiros X = x1 , ... , xn de s bits ordenados,
    // reunidos numa palavra com um bit de espaço entre eles.
    // Uma query q tambem de tamanho s.
    // Um inteiro s, o tamanho das chaves e da query.
    // Output: O número de elementos no array que é menor ou igual a q.
    int ParallelRank(unsigned int X, unsigned int q, int s);

    // Input: Uma palavra X de tamanho W
    // Output: O índice do bit 1 mais a esquerda em X
    int MostSignificantBit(unsigned int X);

};
