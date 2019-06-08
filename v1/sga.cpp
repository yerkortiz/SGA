/* versión 1 del sga, mutación con bitflip, 
cromosoma como un vector de 0's y 1's */
/* Compilar usando c++17, sl2 */
#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
#include <random>
#include <fstream>
#include <string>

using namespace std;

typedef vector<int> vctr;
typedef pair<int, vctr> individual;
typedef vector<individual> pop;

const int POP_SIZE = 400;
const double MUT_RATE = 0.1;
const int CROSS_RATE = 50;
const int GEN_SIZE = 5;
const int MAX_IT = 50;
const vctr TARGET {1,1,1,1,1};
/*Los 1's y 0's son parte de la función fitness con la
que se hará la comparación, los 2 son considerados simbolos 
de indiferencia en la evaluación del individuo.*/
void print_population(pop &population){
    for(int i(0); i < POP_SIZE; ++i){
        cout<<"Gen "<<i + 1<<": [";
        for(int j(0); j < GEN_SIZE; ++j){
            cout<<population[i].second[j];
            cout<<(j == GEN_SIZE - 1? "] ": ", ");
        }
        cout<<"Valor: "<<population[i].first<<'\n';
    }
}
int fitness_evaluation(vctr &v){
    int c(0);
    for(int i(0); i < GEN_SIZE; ++i){
        if(TARGET[i] == 2) continue;
        if(v[i] != TARGET[i]) c++;
    }
    return c;
}
int rnd_int(int lo, int hi){
    random_device rd; mt19937 e(rd());
    static uniform_int_distribution<> dis(lo, hi);
    return dis(e);
}
float rnd_float(float lo, float hi){
    random_device rd; mt19937 e(rd());
    static uniform_real_distribution<> dis(lo, hi);
    return dis(e);
}
void mutate_gen(vctr &v){
    int index(rnd_int(0, GEN_SIZE - 1));
    if (v[index] == 1) v[index] = 0;
    else v[index] = 1;
    //~ es el simbolo del operador logico NOT.
}
void crossover(vctr &p1, vctr &p2, vctr &offspring){
    for(int i(0); i < GEN_SIZE; ++i){
        float dice = rnd_float(0, 1);
        if (dice < 0.5) offspring[i] = p1[i];
        else offspring[i] = p2[i];
    }
}
string new_generation(pop &population, int it){
    
    //print_population(population);
    pop auxpop;
    /* 
    int best_individual = (10*POP_SIZE)/100;
    for(int i(0); i < best_individual; ++i){
        auxpop.push_back(population[i]);
    }*/
    string s = "";
    int c(0);
    int fittest_individual = (CROSS_RATE*POP_SIZE)/100;
    for(int i(0); i < POP_SIZE; ++i){
        int index1 = rnd_int(0, fittest_individual);
        int index2 = rnd_int(0, fittest_individual);
        vctr offspring(GEN_SIZE, 0);
        crossover(population[index1].second, population[index2].second, offspring);
        float dice = rnd_float(0,1);
        if (dice < MUT_RATE){
            mutate_gen(offspring);
        }
        int f_value = fitness_evaluation(offspring);
        if(f_value == 0) c++;
        auxpop.push_back(make_pair(f_value, offspring));
    }
    population = auxpop;
    s = to_string(it);
    s += ", ";
    s += to_string(c);
    s += '\n';
    return s;
}
int main(){
    for(int k(0); k < 30; ++k){
    fstream file;
    string s_index = to_string(k + 1);
    file.open("/Users/Yerko/codes/sga/experiments/n_bacterias/400/simu" + s_index + ".txt", fstream::out);
    pop population(POP_SIZE);
    int c(0);
    for(int i(0); i < POP_SIZE; ++i){
        vctr v(GEN_SIZE);
        for(int j(0); j < GEN_SIZE; ++j) v[j] = rnd_int(0, 1);
        int f_value = fitness_evaluation(v);
        if (f_value == 0) c++;
        population[i] = make_pair(f_value, v);
    }
    string s = "0, " + to_string(c) + "\n";
    file<<s;
    //print_population(population);
    for(int i(0); i < MAX_IT; ++i){
        sort(population.begin(), population.end());
        file<<new_generation(population, i + 1);
    } 
    sort(population.begin(), population.end());
    file.close();
    }
    return 0;
}