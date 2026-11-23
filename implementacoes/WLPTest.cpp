// Ulisses Paixão Cordeiro
// Versão Inicial - Teste do Word Level Parallelism

// A classe WLPTest testa as técnicas definidas na classe WLP 

#include <bits/stdc++.h>
#include "WLP.hpp"

using namespace std;

class WLPTest {
private:

    // Classe com as técnicas de Word Level Parallelism
    WLP* wlp;

    // Tamanho da chave
    int k;

    // Nível de debug do teste
    // É definido como sendo 1 somente para o DebugWLP == 0
    // Com o nível de DebugWLP igual 0 o teste ainda printa os resultados,
    // evitando prints duplicados nos outros níveis mas também sempre informando
    // ao testador o que está acontecendo.
    int TestDebug;
    
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

public:

    WLPTest(){
        wlp = new WLP();
        k = sqrt(W);
        TestDebug = (wlp->GetDebugLevel() == 0);
        
        cout << "Inicializado teste do Word Level Parallelism\n";
        cout << "O tamanho de palavra é: " << W << "\n";
        cout << "O tamanho das chaves k é: " << k << "\n";
        cout << "O nível de debug é: " << wlp->GetDebugLevel() << "\n";
        cout << "\n";
    }

    // Muda o tamanho da chave esperado para os testes
    void ChangeKeySize(){
        cout << "\n";
        cout << "Insira o novo tamanho k das chaves\n";
        cout << "O ambiente atual tem palavra de tamanho W = " << W << " portanto o k recomendado é " << sqrt(W) << "\n";

        int new_size;
        cout << "Input k: ";
        cin >> new_size;

        k = new_size;
    }

    // Muda o nível de debug das variáveis DebugWLP e TestDebug
    void TestChangeDebugLevel(int new_debug=0){
        if (new_debug){
            TestDebug = (new_debug == 0);
            wlp->ChangeDebugLevel(new_debug);
            return;
        }
        cout << "\n";
        cout << "Insira o novo nível de debug\n";
        cout << "0: Nenhum debug\n";
        cout << "1: Debug apenas do MSB\n";
        cout << "2: Debug de todas as operações\n";

        cout << "Input debug: ";
        cin >> new_debug;

        TestDebug = (new_debug == 0);
        wlp->ChangeDebugLevel(new_debug);
    }
    
    // Prepara o teste de Comparação Paralela
    void PrepareParallelComparison(){
        cout << "\n";
        cout << "TESTE COMPARAÇÃO PARALELA\n";
        
        string block(k, 'x');
        cout << "Insira as bitstrings A e B contendo chaves de tamanho " << k << " a serem comparadas.\n";
        cout << "O formato é: ";
        for (int i = 0; i < W/(k+1); i++) cout << block << " ";
        cout << "\n";

        cin.get(); // ignorar o \n após a op
        cout << "Input A: ";
        string A; getline(cin, A);
        
        cout << "Input B: ";
        string B; getline(cin, B);
        
        unsigned int R = wlp->ParallelComparison(BitStringToInt(A), BitStringToInt(B), k);
        
        if (TestDebug){
            cout << "R: ";
            wlp->PrintWord(bitset<W>(R),k+1);
        }
    }

    // Prepara o teste de Parallel Tile
    void PrepareParallelTile(){
        cout << "\n";
        cout << "TESTE PARALLEL TILE\n";
        
        string block(k, 'x');
        cout << "Insira a chave de tamanho " << k << " a ser extendida.\n";
        cout << "O formato é: " << block << "\n";
        
        cin.get(); // ignorar o \n após a op
        cout << "Input q: ";
        string q; getline(cin, q);
                
        unsigned int Q = wlp->ParallelTile(BitStringToInt(q), k);

        if (TestDebug){
            cout << "Q: ";
            wlp->PrintWord(bitset<W>(Q),k+1);
        }
    }

    // Prepara o teste de Soma Paralela
    void PrepareParallelSum(){
        cout << "\n";
        cout << "TESTE SOMA PARALELA\n";
        
        string block(k, '0');
        cout << "Insira a palavra R a ter seus bits de interesses somados.\n";
        cout << "O formato é: ";
        for (int i = 0; i < W/(k+1); i++) cout << (char) (i+'a') << block << " ";
        cout << "\n";
        
        cin.get(); // ignorar o \n após a op
        cout << "Input R: ";
        string R; getline(cin, R);
                
        int sum = wlp->ParallelSum(BitStringToInt(R,false), k);
        
        if (TestDebug)
            cout << "Sum: " << sum << "\n";
    }

    // Prepara o teste de Rank Paralelo
    void PrepareParallelRank(){

        cout << "\n";
        cout << "TESTE RANK PARALELO\n";

        string block(k, 'x');
        cout << "Insira a bitstring X com as " << W/(k+1) << " chaves de tamanho " << k << " em ordem crescente.\n";
        cout << "O formato é: ";
        for (int i = 0; i < W/(k+1); i++) cout << block << " ";
        cout << "\n";

        cin.get(); // ignorar o \n após a op

        cout << "Input X: ";
        string X; getline(cin, X);

        cout << "Insira a chave q de tamanho " << k << " a ser comparada.\n";
        cout << "O formato é: " << block << "\n";

        cout << "Input q: ";
        string q; getline(cin, q);
                
        int rank = wlp->ParallelRank(BitStringToInt(X), BitStringToInt(q), k);
        
        if (TestDebug)
            cout << "Rank: " << rank << "\n";
    }

    // Prepara o teste de MSB
    void PrepareMostSignificantBit(){
        double s = sqrt(W);
        int int_s = (int) s;
        if (int_s*int_s != W){
            cout << "Para a operação de MSB, o W precisa ser um quadrado perfeito, mas o W atual é " << W << "\n";
            cout << "Altere seu valor em WLP.hpp para prosseguir\n";
            return;              
        }

        cout << "\n";
        cout << "TESTE BIT MAIS SIGNIFICATIVO\n";
        
        cout << "Insira a bitstring X de tamanho " << W << " com clusters de tamanho " << s << ".\n";
        cout << "O formato é: ";
        string block(s, 'x');
        for (int i = 0; i < W/s; i++) cout << block << " ";
        cout << "\n";
        
        cin.get(); // ignorar o \n após a op
        cout << "Input X: ";
        string X; getline(cin, X);
                
        int old_debug = wlp->GetDebugLevel();
        cout << "O nível de debug padrão para MSB é apenas suas operações.\n"
            << "Aperte enter para continuar ou qualquer outro input para o prompt de troca: ";
        string new_debug;
        getline(cin, new_debug);

        if (new_debug.empty()) TestChangeDebugLevel(1);
        else TestChangeDebugLevel();
        
        int MSB = wlp->MostSignificantBit(BitStringToInt(X,false));
        
        TestChangeDebugLevel(old_debug);
        
        if (TestDebug)
            cout << "MSB(X): " << MSB << "\n";
    }

    // Realiza testes padronizados para os métodos de Word Level Parallelism
    void PrepareDefaultTest(){
    
        cout << "\n";
        cout << "TESTE PADRÃO WLP\n";

        cout << "Obs: Para o teste padrão do MSB, W precisa ser 16. Para os demais testes, W precisa ser 20.\n";
        cout << "O W atual é " << W << ". Altere seu valor em WLP.hpp\n";
        cout << "1: Comparação Paralela\n";
        cout << "2: Parallel Tile\n";
        cout << "3: Soma Paralela\n";
        cout << "4: Rank Paralelo\n";
        cout << "5: Bit Mais Significativo\n";
        cout << "6: Teste geral sem MSB\n";   
        cout << "7: Sair\n"; 
        cout << "Input teste: ";

        int test; cin >> test;

        if (test == 7)
            return;

        if (test == 1 || test == 2 || test == 3 || test == 4 || test == 6 && W != 20){
            cout << "O teste padrão para esse(s) teste(s) é W = 20, mas o W atual é " << W << "\n";
            cout << "Altere seu valor em WLP.hpp para prosseguir\n";
            return;
        }
        if (test == 5 && W != 16){
            cout << "O teste padrão para o teste do MSB é W = 16, mas o W atual é " << W << "\n";
            cout << "Altere seu valor em WLP.hpp para prosseguir\n";
            return;
        }

        int k = 4;

        if (test == 1 || test == 6){
            cout << "\n";
            cout << "TESTE PADRÃO COMPARAÇÃO PARALELA\n";

            string A = "0100 1010 0101 0010";
            string B = "0010 0100 1000 0001";

            cout << "Input A: " << A << "\n";
            cout << "Input B: " << B << "\n";

            unsigned int R_AB = wlp->ParallelComparison(BitStringToInt(A), BitStringToInt(B), k);

            if (TestDebug){
                cout << "R: ";
                wlp->PrintWord(bitset<W>(R_AB),k+1);
            }
        }
        if (test == 2 || test == 6){
            cout << "\n";
            cout << "TESTE PADRÃO PARALLEL TILE\n";

            string q = "1100";

            cout << "Input q: " << q << "\n";

            unsigned int Q = wlp->ParallelTile(BitStringToInt(q), k);

            if (TestDebug){
                cout << "Q: ";
                wlp->PrintWord(bitset<W>(Q),k+1);
            }
        }
        if (test == 3 || test == 6){
            cout << "\n";
            cout << "TESTE PADRÃO SOMA PARALELA\n";

            string R = "10000 10000 00000 10000";

            cout << "Input R: " << R << "\n";
            
            int sum = wlp->ParallelSum(BitStringToInt(R,false), k);
            
            if (TestDebug)
                cout << "Sum: " << sum << "\n";
        }
        if (test == 4 || test == 6){

            cout << "\n";
            cout << "TESTE PADRÃO RANK PARALELO\n";

            string X = "0001 0101 1011 1101";
            string query = "1100";

            cout << "Input X: " << X << "\n";
            cout << "Input q: " << query << "\n";
        
            int rank = wlp->ParallelRank(BitStringToInt(X), BitStringToInt(query), k);
        
            if (TestDebug)
                cout << "Rank: " << rank << "\n";
        }
        if (test == 5){

            cout << "\n";
            cout << "TESTE PADRÃO BIT MAIS SIGNIFICATIVO\n";

            string MSB_X = "0101 0000 1000 1101";

            cout << "Input X: " << MSB_X << "\n";
        
            int old_debug = wlp->GetDebugLevel();
            cout << "O nível de debug padrão para MSB é apenas suas operações.\n"
                << "Aperte enter para continuar ou qualquer outro input para o prompt de troca: ";
            cin.get(); // ignorar o \n após o test
            string new_debug; getline(cin, new_debug);

            if (new_debug.empty()) TestChangeDebugLevel(1);
            else TestChangeDebugLevel();
            
            int MSB = wlp->MostSignificantBit(BitStringToInt(MSB_X,false));
            
            TestChangeDebugLevel(old_debug);
            
            if (TestDebug)
                cout << "MSB(X): " << MSB << "\n";
        }
    }

    void PrintPrompt(){
        cout << "--------------------------------\n";
        cout << "1: Comparação Paralela\n";
        cout << "2: Parallel Tile\n";
        cout << "3: Soma Paralela\n";
        cout << "4: Rank Paralelo\n";
        cout << "5: Bit Mais Significativo\n";
        cout << "6: Teste padrão\n";
        cout << "7: Mudar o tamanho das chaves k\n";
        cout << "8: Mudar o nível de debug\n";
        cout << "9: Sair\n";
        cout << "Input operação: ";
    }

};

int main(int argc, char** argv){

    int op;
    string A, bitstring;
    bool exit = false;

    WLPTest* test = new WLPTest();

    test->PrintPrompt();

    while(!exit && cin >> op){
        switch (op)
        {
        case 1:
            test->PrepareParallelComparison();
            break;
        case 2:
            test->PrepareParallelTile();
            break;
        case 3:
            test->PrepareParallelSum();
            break;
        case 4:
            test->PrepareParallelRank();
            break;
        case 5:
            test->PrepareMostSignificantBit();
            break;    
        case 6:
            test->PrepareDefaultTest();
            break;
        case 7:
            test->ChangeKeySize();
            break;
        case 8:
            test->TestChangeDebugLevel();
            break;
        case 9:
            exit = true;
            break;    
        }  
        if (!exit) test->PrintPrompt();
    }
}