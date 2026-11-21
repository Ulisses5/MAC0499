// Ulisses Paixão Cordeiro
// Versão Inicial - Word Level Parallelism

// A classe WLP reúne diferentes técnicas de manipulação
// de bits para obter resultados interessantes em tempo constante

#include <bits/stdc++.h>

using namespace std;

constexpr int W = 20;

class WLP {
private:

   int k;
   int debug;

    // BlocksLastBitSet: para blocos de tamanho s, é uma palavra com os bits de índices
    // 0, s+1, ... , (n-1)(s+2) setados
    // Exemplo, para W = 20 e s = 5 é:
    // 00001 00001 00001 00001
    // (usualmente s = k+1, uma chave de tamanho k + um bit sentinela)
    unsigned int GetBlocksLastBitSet(int s){
        unsigned int R = 0;
        int n = W/s;
        for (int i = 0; i < n; i++){
            R = (R << s) | 1;
        }
        return R;
    }
    
    // BlocksFirstBitSet: para blocos de tamanho s, é uma palavra com os bits de índices
    // s, 2*s, ... , (n-1)*s setados
    // Exemplo, para W = 20 e s = 5 é:
    // 10000 10000 10000 10000
    // (usualmente s = k+1, uma chave de tamanho k + um bit sentinela)
    unsigned int GetBlocksFirstBitSet(int s){
        unsigned int R = 0;
        int n = W/s;
        for (int i = 0; i < n; i++){
            R = (R << s) | 1;
        }
        R = R << (s-1);
        return R;
    }

    // Mask: uma máscara de s bits setados como 1
    // Exemplo, para s = 5 é:
    // 11111
    unsigned int GetBitsMask(int s){
        unsigned int Mask = 0;
        for (int i = 0; i < s; i++)
            Mask = (Mask << 1) | 1;
        return Mask;
    }

    // P: array contendo as primeiras s potências de 2
    // separadas por um bit 0.
    // Potências 2^0, 2^1, ... , 2^(s-1)
    // (usualmente s = k = (sqrt(w)))
    unsigned int GetPowersOfTwo(int s){
        unsigned int P = 0; 
        for (int i = 0; i < s-1; i++){
            P = (P << s) | 1;
        }
        P = P << (s-2);
        return P;
    }

    // Input: Uma bitstring s de formato "XXXXXXXXXX"
    // onde X é 0 ou 1 ou um espaço, ' '. 
    // Os espaços serão substituídos por 0 por padrão, a flag desabilita isso.
    // Output: O inteiro que aquela bitstring representa
    unsigned int BitStringToInt(const string& s, bool flag=true){
        unsigned int ret = 0;
        for (char c : s) {
            if (c == ' '){ // caso tenha um espaço entre os bits
                if (flag) c = '0'; // tratar como um 0
                else continue; // ignorar o espaço
            } 
            ret = (ret << 1) | (c - '0');
        }
        return ret;
    }

    // Input: 
    // Uma palavra R de tamanho W com s blocos em que o primeiro bit de cada bloco nos interessa
    // Um inteiro s com o número de bits em cada bloco
    // Output: Uma palavra que contém apenas os bits de interesse
    unsigned int PerfectSketch(unsigned int R, int s){
        // O multiplicador perfeito "concatena" os s bits de interesse
        // então precisamos espaçá-los no número de bits que não nos interessa, s-1
        unsigned int SketchMultiplier = 0;
        for (int i = 0; i < s; i++){
            SketchMultiplier = (SketchMultiplier << s-1) | 1;
        }

        // A palavra apenas com os bits de interesse é de tamanho s+1
        unsigned int SketchMask = GetBitsMask(s);

        // Multiplicando, o resultado que queremos estará no bloco mais à direita,
        // como temos s bloco de tamanho s, fazemos um shift a direita de W-s.
        // Por fim, aplicamos a máscara para obter apenas os bits de interesse
        unsigned int C = (((R * SketchMultiplier) >> W-s) & SketchMask);

        return C;
    }


    // Input: 
    // Uma palavra X de tamanho W
    // O tamanho s dos seus blocos
    // Output: Uma palavra de tamanho s com o sumario dos blocos de X
    unsigned int NonEmptyClusters(unsigned int X, int s){
        // F tem apenas os primeiros bits de cada bloco setado
        unsigned int F = GetBlocksFirstBitSet(s);

        // Obtemos R_1, que indica com seus bits sentinelas
        // se o primeiro bit de cada bloco está setado
        unsigned int R_1 = X & F;

        if (debug){
            cout << "R_1: ";
            PrintWord(bitset<W>(R_1),s);
        }

        // Obtemos R_2, que contém os últimos k-1 bits de cada bloco original de X , 
        // mas separados por um bit 0.
        unsigned int R_2 = X ^ R_1;

        // Com isso, podemos fazer a comparação paralela de R_2 com F,
        // que nessa perspectiva é composto por várias bitstrings com valor zero
        // intercaladas com 1 para a comparação.
        // O resultado dessa comparação nos diz se cada um dos blocos de R_2 
        // tem valor maior que 0, ou seja, se tem pelo menos um bit 1
        unsigned int R_3 = ParallelComparison(F,R_2,s-1);
                
        // Queremos contudo indicar com um bit 1 os blocos que tem pelo menos
        // um de seus últimos s-1 bits setados, então fazemos um XOR com F 
        R_3 = R_3 ^ F;

        if (debug){
            cout << "R_3: ";
            PrintWord(bitset<W>(R_3),s);
        }

        // Os bits sentinelas de R_1 indicam se o primeiro bit de cada bloco de X está setado
        // Os bits sentinelas de R_3 indicam se pelo menos um dos últimos s-1 bits de cada bloco X está setado
        // Fazendo R_1 OR R_3 obtemos R informando no bit sentinela se cada bloco de X está vazio ou não
        unsigned int R = R_1 | R_3;

        if (debug){
            cout << "R: ";
            PrintWord(bitset<W>(R),s);
        }

        // Por fim, fazemos um Sketching para reunir apenas os bits sentinelas.
        // Eles estão separados por s-1 bits desinteressantes, os bits restantes de cada bloco.
        // O resultado C terá tamanho s
        unsigned int C = PerfectSketch(R, s);

        if (debug){
            cout << "C: ";
            PrintWord(bitset<W>(C),s);
        }

        return C;
    }

    // Imprime os blocos de uma palavra de tamanho W
    // separa os bits em blocos exatos de tamanho s
    void PrintWord(bitset<W> bits, int s){
        for (int i = W/s*s - 1; i >= 0; i--){
            if (i%s == 0) cout << bits[i] << " ";
            else cout << bits[i];
        }
        cout << endl;
    }

public:
    
    WLP(){
        k = sqrt(W);
        debug = 2;

        cout << "Inicializado teste do Word Level Parallelism\n";
        cout << "O tamanho de palavra é: " << W << "\n";
        cout << "O tamanho das chaves k é: " << k << "\n";
        cout << "O nível de debug é: " << debug << "\n";
        cout << "\n";
    }

    void ChangeKeySize(){
        cout << "\n";
        cout << "Insira o novo tamanho k das chaves\n";
        cout << "O ambiente atual tem palavra de tamanho W = " << W << " portanto o k recomendado é " << sqrt(W) << "\n";
        
        int new_size;
        cout << "Input k: ";
        cin >> new_size;

        k = new_size;
    }

    void ChangeDebugLevel(){
        cout << "\n";
        cout << "Insira o novo nível de debug\n";
        cout << "0: Nenhum debug\n";
        cout << "1: Debug apenas do MSB\n";
        cout << "2: Debug de todas as operações\n";

        int new_debug;
        cout << "Input debug: ";
        cin >> new_debug;

        debug = new_debug;
    }

    // Input: Duas palavras A e B. A contém números a_0, ... , a_n 
    // e cada número tem s bits. Entre cada número há um bit 0 de espaço.
    // O mesmo vale para B, com b_0, ... , b_n.
    // Output: Uma palavra R com o resultado de a_i >= b_i nos bits que 
    // ocupavam o espaço entre cada número nos arrays de input
    unsigned int ParallelComparison(unsigned int A, unsigned int B, int s){
        // Pegamos uma palavra que tem o primeiro bit de cada bloco setado
        // O tamanho de um bloco eh o tamanho das chaves + o bit sentinela 
        unsigned int BlocksFirstBitSet = GetBlocksFirstBitSet(s+1);
        
        // Setamos os bits de espaço de A como 1.
        unsigned int newA = A | BlocksFirstBitSet;

        bitset<W> bitsA(newA);
        bitset<W> bitsB(B);

        if (debug > 1){
            cout << "A: ";
            PrintWord(bitsA,s+1);
            cout << "B: ";
            PrintWord(bitsB,s+1);
            string line(W+s+2, '-');
            cout << line << "\n";
        }

        // Fazemos a subtração de A por B, obtendo em cada bit sentinela
        // o resultado de a_i >= b_i. 
        // Aplicamos uma máscara para que os outros bits sejam zerados.
        unsigned int R = (newA - B) & BlocksFirstBitSet;


        if (debug > 1){
            cout << "R: ";
            PrintWord(bitset<W>(R),s+1);
        }

        return R;
    }

    void PrepareParallelComparison(){

        cout << "\n";
        cout << "COMPARAÇÃO PARALELA\n";

        string block(k, 'x');
        cout << "Insira as bitstrings A e B contendo chaves de tamanho " << k << " a serem comparadas. O formato é: ";
        for (int i = 0; i < W/(k+1); i++) cout << block << " ";
        cout << "\n";
        
        cin.get(); // ignorar o \n após a op

        cout << "Input A: ";
        string A; getline(cin, A);

        if (A.empty()) A = "0100 1010 0101 0010";

        cout << "Input B: ";
        string B; getline(cin, B);

        if (B.empty()) B = "0010 0100 1000 0001";

        unsigned int R = ParallelComparison(BitStringToInt(A), BitStringToInt(B),k);

        cout << "R: ";
        PrintWord(bitset<W>(R),k+1);
    }

    // Input: Uma chave q de tamanho s
    // Output: Uma palavra Q de tamanho W em que q se repete n vezes,
    // onde n = W/(s+1), com as repetições interespaçados por um bit 1.
    unsigned int ParallelTile(unsigned int q, int s){
        unsigned int BlocksFirstBitSet = GetBlocksFirstBitSet(s+1);
        unsigned int BlocksLastBitSet = GetBlocksLastBitSet(s+1);
        
        unsigned int Q = q * BlocksLastBitSet | BlocksFirstBitSet;

        if (debug > 1){
            cout << "Q: ";
            PrintWord(bitset<W>(Q),s+1);
        }

        return Q;
    }

    void PrepareParallelTile(){

        cout << "\n";
        cout << "PARALLEL TILE\n";
        string block(k, 'x');
        cout << "Insira a chave de tamanho " << k << " a ser extendida. O formato é: " << block << "\n";

        cin.get(); // ignorar o \n após a op
        cout << "Input q: ";
        string q; getline(cin, q);

        if (q.empty()) q = "1100";

        unsigned int Q = ParallelTile(BitStringToInt(q),k);

        cout << "Q: ";
        PrintWord(bitset<W>(Q),k+1);
    }

    // Input: Uma palavra R com W/(s+1) bits de interesse separados por s bits 0.
    // Output: A soma dos bits de interesse.
    // Exemplo: R = a0000 b0000 c0000 d0000
    //          Sum = a + b + c + d
    int ParallelSum(unsigned int R, int s){
        unsigned int BlocksLastBitSet = GetBlocksLastBitSet(s+1);
        unsigned int SSizedMask = GetBitsMask(s);

        // Multiplicamos R pelo multiplicador adequado que "sobrepõe" os
        // bits de interesse, os somando nos índices dos últimos bits da palavra.
        // Fazemos então um shift para levá-los para direita e por fim
        // aplicamos uma máscara para obter apenas eles.
        int sum = (((R * BlocksLastBitSet) >> W/(s+1)*(s+1)-1) & SSizedMask);

        if (debug > 1)
            cout << "Sum: " << sum << "\n";

        return sum;
    }

    void PrepareParallelSum(){

        cout << "\n";
        cout << "SOMA PARALELA\n";

        string block(k, '0');
        cout << "Insira a palavra R a ter seus bits de interesses somados. O formato é: ";
        for (int i = 0; i < W/(k+1); i++) cout << (char) (i+'a') << block << " ";
        cout << "\n";

        cin.get(); // ignorar o \n após a op
        cout << "Input R: ";
        string R; getline(cin, R);

        if (R.empty()) R = "10000 10000 00000 10000";

        int sum = ParallelSum(BitStringToInt(R,false),k);

        cout << "Sum: " << sum << "\n";
    }

    // Input:
    // Um array de inteiros X = x1 , ... , xn de s bits ordenados,
    // reunidos numa palavra com um bit de espaço entre eles.
    // Uma query q tambem de tamanho s.
    // Um inteiro s, o tamanho das chaves e da query.
    // Output: O número de elementos no array que é menor ou igual a q.
    int ParallelRank(unsigned int X, unsigned int q, int s){
        unsigned int Q = ParallelTile(q,s);
        unsigned int R = ParallelComparison(Q, X,s);
        int rank = ParallelSum(R,s);
        
        if (debug > 1)
            cout << "Rank: " << rank << "\n";

        return rank;
    }

    void PrepareParallelRank(){
        
        cout << "\n";
        cout << "RANK PARALELO\n";
        
        string block(k, 'x');
        cout << "Insira a bitstring X com as " << W/(k+1) << " chaves de tamanho " << k << " em ordem crescente. O formato é: ";
        for (int i = 0; i < W/(k+1); i++) cout << block << " ";
        cout << "\n";
        cout << "Insira a chave q de tamanho " << k << " a ser comparada. O formato é: " << block << "\n";

        cin.get(); // ignorar o \n após a op
        
        cout << "Input X: ";
        string X; getline(cin, X);

        if (X.empty()) X = "0001 0101 1011 1101";

        cout << "Input q: ";
        string q; getline(cin, q);

        if (q.empty()) q = "1100";

        int rank = ParallelRank(BitStringToInt(X), BitStringToInt(q),k);

        cout << "Rank: " << rank << "\n";
    }

    // Input: Uma palavra X de tamanho W
    // Output: O índice do bit 1 mais a esquerda em X
    int MostSignificantBit(unsigned int X){
        // Dividiremos X em clusters de tamanho sqrt(W)
        // então agora vamos passar a usar s = sqrt(W),
        // independente de qual k foi escolhido antes
        int s = sqrt(W);
        // Identificar os clusters não-vazios
        unsigned int C = NonEmptyClusters(X,s);

        // Encontrar o indice c do primeiro bloco não vazio

        // A ideia: P contém as primeiras s potências de 2.
        // Queremos fazer o rank de C com P e assim descobrir qual o número 
        // de potências menores ou iguais a C, o que nos dá o índice do seu
        // primeiro bit setado, c.
        int c;

        // Problema: Cada potência ocupa s bits e precisamos espaçá-las
        // por um bit. O tamanho da palavra será W + sqrt(W), passando do limite.

        // A solução: Fazemos um caso especial separado para o primeiro bit de C,
        // ou seja para a última potência de 2 a ser usada, 2^s.

        // P então tem apenas as potências 2^0, 2^1, ... , 2^(s-1)
        unsigned int P = GetPowersOfTwo(s);
        
        if (debug){
            cout << "P: ";
            PrintWord(bitset<W>(P),s+1);
        }

        // Caso especial
        unsigned int FirstPower = (1 << s-1); // 2^s

        if (C & FirstPower) 
            c = s - 1;
        else 
            c = ParallelRank(P,C,s) - 1;

        if (debug)
            cout << "c: " << c << "\n";
        
        // Encontrar o indice d do primeiro bit 1 no cluster

        // Pegamos o cluster X_c fazendo um shift em c*k para a direita
        // Assim, os bits do cluster que queremos são apenas os s bits mais à direita
        // Aplicamos então uma máscara de tamanho s para capturar apenas eles
        unsigned int ClusterMask = GetBitsMask(s); // mascara de tamanho s = sqrt(W), tamanho de um cluster
        unsigned int X_c = ((X >> (c*s)) & ClusterMask); // cluster c de X
        
        if (debug){
            cout << "X_c: ";
            PrintWord(bitset<W>(X_c),s);
        }

        // Novamente temos a questão do tamanho das potências de 2, 
        // então aplicamos o caso especial 
        int d;

        if (X_c & FirstPower)
            d = s - 1;
        else
            d = ParallelRank(P,X_c,s) - 1;

        if (debug)
            cout << "MSB(X_c): " << d << "\n";

        // O bit mais signifcativo de x é dado por c*sqrt(w) + d
        int MSB = c * s + d;

        return MSB;
    }

    void PrepareMostSignificantBit(){

        cout << "\n";
        cout << "BIT MAIS SIGNIFICATIVO\n";
        cout << "Insira a bitstring X de tamanho w, onde w é um quadrado perfeito.\n";
        int int_sqrt = (int) sqrt(W);
        cout << "Para o ambiente atual, a maior possível é w = " << int_sqrt * int_sqrt << "\n";

        cin.get(); // ignorar o \n após a op
        cout << "Input X: ";
        string X; getline(cin, X);

        if (X.empty()) X = "0101 0000 1000 1101";

        int old_debug = debug;
        cout << "O nível de debug padrão para MSB é apenas suas operações.\n"
            << "Aperte enter para continuar ou qualquer outro input para o prompt de troca: ";
        string new_debug;
        getline(cin, new_debug);
        
        if (new_debug.empty()) debug = 1;
        else ChangeDebugLevel();

        int MSB = MostSignificantBit(BitStringToInt(X,false));

        debug = old_debug;

        cout << "MSB(X): " << MSB << "\n";
    }
};

void PrintPrompt(){
    cout << "--------------------------------\n";
    cout << "1: Mudar o tamanho das chaves k\n";
    cout << "2: Mudar o nível de debug\n";
    cout << "3: Comparação Paralela\n";
    cout << "4: Parallel Tile\n";
    cout << "5: Soma Paralela\n";
    cout << "6: Rank Paralelo\n";
    cout << "7: Bit Mais Significativo\n";
    cout << "8: Sair\n";
    cout << "Input operação: ";
}


int main(int argc, char** argv){

    WLP* wlp;

    int op;
    string A, bitstring;
    bool exit = false;

    wlp = new WLP();

    PrintPrompt();

    while(!exit && cin >> op){
        switch (op)
        {
        case 1:
            wlp->ChangeKeySize();
            break;
        case 2:
            wlp->ChangeDebugLevel();
            break;
        case 3:
            wlp->PrepareParallelComparison();
            break;
        case 4:
            wlp->PrepareParallelTile();
            break;
        case 5:
            wlp->PrepareParallelSum();
            break;
        case 6:
            wlp->PrepareParallelRank();
            break;
        case 7:
            wlp->PrepareMostSignificantBit();
            break;    
        case 8:
            exit = true;
            break;    
        }  
        if (!exit) PrintPrompt();
    }
}