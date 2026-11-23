// Ulisses Paixão Cordeiro
// Versão Inicial - vEB

#include <bits/stdc++.h>

using namespace std;

const uint W = 8;

class vEB {
private:
    // w = lg u
    int w;
    // u = 2^w
    int u;

    vector<vEB*> clusters;
    vEB* summary;
    int min;
    int max;

    // se estamos no ultimo nivel da arvore, usamos um bit-vector diretamente
    bool last_level = false;
    vector<int> bit_vector;

    // c = floor ( x / sqrt(u) )
    int low(int x){
        return x >> w/2;
    }

    // i = x mod sqrt(u)
    int high(int x){
        return x & ((1 << w/2) - 1);
    }

    // x = c * sqrt(u) + i 
    int get_x(int c, int i){
        return (c << w/2)|i;
    }

public:
    
    vEB(int tam_palavra){
        w = tam_palavra;
        u = pow(2, w);
        min = -1;
    
        // crio clusters e summary somente se o universo atual eh maior que o tamanho inicial da palavra
        if (u > W){
            int next_level_size = sqrt(u); 
            clusters.resize(next_level_size);   
            for (int i = 0; i < next_level_size; i++)
                clusters[i] = new vEB(w/2);
            summary = new vEB(w/2);
        }    
        // se o universo eh pequeno, usamos um bit vector
        // considerando que W eh uma constante, as operacoes no bit vector terao tempo constante
        else {
            last_level = true;
            bit_vector.resize(u);
        }
    }

    void Inserir(int x){
        if (last_level) {
            if (x < min || min == -1) min = x;
            if (x > max) max = x;
            bit_vector[x] = 1;
            return;
        }

        if (min == -1){
            min = x;
            max = x;
            return;
        }
        if (x <= min)
            swap(min, x);
        if (x > max)
            max = x;

        int c = low(x);
        int i = high(x);

        // cout << "x = ⟨c,i⟩ | " << x << " = ⟨" << c << "," << i << "⟩" << endl;
        
        if (clusters[c]->Vazio())
            summary->Inserir(c);
        clusters[c]->Inserir(i);
    }

    void Remover(int x){
        int c = low(x);
        int i = high(x);

        // cout << "x = ⟨c,i⟩ | " << x << " = ⟨" << c << "," << i << "⟩" << endl;

        if (last_level){
            bit_vector[x] = 0;
            if (x == min){
                min = -1;
                for (int y = x; y < u; y++)
                    if (bit_vector[y]) {
                        min = get_x(c,y);
                        break;
                    }
            }
            if (x == max){
                max = min;
                for (int y = x; y < u; y++)
                    if (bit_vector[y]) 
                        max = get_x(c,y);
            }
            return;
        }

        if (x == min){
            c = summary->min;
            if (c == -1){
                min = -1;
                return;
            }
            i = clusters[c]->min;
            min = get_x(c,i); // pegamos um novo minimo e vamos remover seu c e i da estrutura
        }
        clusters[c]->Remover(i);
        if (clusters[c]->min == -1)
            summary->Remover(c);
        if (summary->min == -1)
            max = min;
        else {
            int c_ = summary->max;
            max = get_x(c_, clusters[c_]->max);
        }

    }

    int Predecessor(int x){
        if (x <= min)
            return -1;
        
        int c = low(x);
        int i = high(x);
        
        // cout << "x = ⟨c,i⟩ | " << x << " = ⟨" << c << "," << i << "⟩" << endl;

        if (last_level){
            for (int y = x-1; y >= 0; y--)
                if (bit_vector[y]) return y;
            return -1;
        }
        
        if (!clusters[c]->Vazio() && i > clusters[c]->min){
            int i_ = clusters[c]->Predecessor(i);
            if (i_ == -1) return -1; // isso sequer eh possivel acontecer? nao cai sempre no min antes de varrer o bit-vector?
            return get_x(c,i_);
        }
        else {
            int c_ = summary->Predecessor(c);

            // --- caso especial para o min, nao armazenado recursivamente ---
            // se nao temos predecessor para esse cluster no sumario, seu predecessor eh o min da arvore
            if (c_ == -1)
                return min;

            return get_x(c_, clusters[c_]->max);
        }
    }

    int Sucessor(int x){
        if (x < min)
            return min;
        
        int c = low(x);
        int i = high(x);

        // cout << "x = ⟨c,i⟩ | " << x << " = ⟨" << c << "," << i << "⟩" << endl;

        if (last_level){
            for (int y = x+1; y < u; y++)
                if (bit_vector[y]) return y;
            return -1;
        }

        if (!clusters[c]->Vazio() && i < clusters[c]->max){
            int i_ = clusters[c]->Sucessor(i);
            if (i_ == -1) return -1;
            return get_x(c,i_);
        }
        else {
            int c_ = summary->Sucessor(c);
            if (c_ == -1) return -1;
            return get_x(c_, clusters[c_]->min);
        }
    }

    bool Vazio(){
        return (min == -1);
    }

    void Print(const std::string& indent = "   "){
        if (last_level){
            for (int i = 0; i < u; i++)
                cout << bit_vector[i] << " ";
            cout << endl;
            return;
        }
        cout << "vEB de w = " << w << " e u = " << u << ". Min = " << min << endl;
        if (min == -1) return; // nao printamos arvores vazias
        cout << indent << "Summary: ";
        summary->Print(indent + "   ");
        cout << endl;

        for (int c = 0; c < clusters.size(); c++){
            cout << indent << "cluster[" << c << "]: ";
            clusters[c]->Print(indent + "   ");
        }
        cout << endl;
    }
};

void vEBPrintPrompt(){
    cout << "--------------------------------\n";
    cout << "1: Criar uma nova àrvore de van Emde Boas\n";
    cout << "2 X: Inserir o elemento X\n";
    cout << "3 X: Remover o elemento X\n";
    cout << "4 X: Consultar o prededecessor de X\n";
    cout << "5 X: Consultar o sucessor de X\n";
    cout << "6: Imprimir a àrvore\n";
    cout << "7: Sair\n";
    cout << "Input operação: ";
}


int main(int argc, char** argv){

    vEB* V;

    int op;
    int x;

    bool exit = false;

    vEBPrintPrompt();

    while(!exit && cin >> op){
        switch (op)
        {
        case 1:
            V = new vEB(W);
            break;
        case 2:
            cin >> x; 
            V->Inserir(x);
            break;
        case 3:
            cin >> x;
            V->Remover(x);
            break;
        case 4:
            cin >> x;
            cout << V->Predecessor(x) << endl;
            break;
        case 5:
            cin >> x;
            cout << V->Sucessor(x) << endl;
            break;
        case 6:
            V->Print();
            break;
        case 7:
            exit = true;
            break;
        } 
        if (!exit) cout << "Input operação: ";
    }
}