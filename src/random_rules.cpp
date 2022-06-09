#include "random_rules.hpp"

auto seed = chrono::high_resolution_clock::now().time_since_epoch().count();
mt19937 generator(seed);

vector<int> generate_birth_survive_cond(){
    vector<int> conds;
    for(int i=0; i < int(generator()%9+1); ++i){
        conds.push_back(generator()%10);
    }
    return conds;
}

int generate_range(){
    return (generator() % 10 + 1);
}

bool generate_middle_included(){
    return (generator() % 2 == 1 ? true : false);
}

char generate_neighbourhood(){
    return (generator() % 2 == 1 ? 'm' : 'n');
}

int generate_number_of_states(){
    return (generator() % 254 + 2);
}
