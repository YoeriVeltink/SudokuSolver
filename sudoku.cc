#include <iostream>
using namespace std;

class cell{
    public:
    int waarde;
    int i;
    int j;
    bool mogelijk[9];
    cell(int waarde, int i, int j); //0-9
    void forceerZet(int waarde, cell* veld[][9]);
    void checkMogelijkheden(cell* veld[][9]);
    int telMogelijkheden();
    int indexMogelijk();
    void setBuurMogelijkheid(int waarde, int i, int j, cell* veld[][9]);
    cell(cell &t);
};

class spel{
    public:
    cell* veld[9][9];
    bool opgelost();
    bool determined(); //true if next step is deducable
    void printVeld();
    cell* minsteMogelijkheden();
    spel* ouder = nullptr;
    spel* kinderen[9] = {nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr};
    spel();
    spel(spel &t, int zetWaarde);
};  

cell::cell(cell &t){
    waarde = t.waarde;
    i = t.i;
    j = t.j;
    for(int i =0; i<9; i++){
        mogelijk[i] = t.mogelijk[i];
    }
}

spel::spel(){

}

spel::spel(spel &t, int zetWaarde){

    cout << "hier copy 1" << endl;
    for(int i =0; i < 9; i++){
        for(int j = 0; j < 9; j++){
            veld[i][j] = new cell(*t.veld[i][j]);
        }
    }
    cout << "hier copy 2" << endl;

    cell* branchpoint = t.minsteMogelijkheden();

    veld[branchpoint->i][branchpoint->j]->forceerZet(zetWaarde, veld);
            printVeld();
            do{
                for(int i = 0; i < 9; i++){
                    for(int j = 0; j < 9; j++){
                        veld[i][j]->checkMogelijkheden(veld);
                    }
                }
                printVeld();
            }while(!opgelost() && determined());
    cout << "hier copy 3" << endl;

    if(opgelost()){
        cout << "klaar 2" << endl;
        exit(0);
    }

    //anders...

    cell* branchpoint2 = minsteMogelijkheden();
    if(branchpoint2 == nullptr)
        return;
        

    cout << branchpoint2->i << " " << branchpoint2->j << endl;
    cout << "branchpoint2" << endl;
    cout << "hier copy 4" << endl;

    for(int k = 0; k< 9; k++){
    cout << "hier copy 5" << endl;

        if(branchpoint2->mogelijk[k]){
            cout << "NESTED KIND " << k+1 << endl;
            kinderen[k] = new spel(*this, k+1);

        }


    }

}

cell::cell(int initWaarde, int i, int j){
    waarde = initWaarde;
    this->i = i;
    this->j = j;
    if(initWaarde == 0)
        for(int i = 0; i < 9; i++)
            mogelijk[i] = true;
    else
        for(int i = 0; i < 9; i++)
            mogelijk[i] = (initWaarde == i+1);
}

void cell::checkMogelijkheden(cell* veld[][9]){
    int counter = 0;

    for(int i = 0; i<9; i++)
        counter+=mogelijk[i];

    if(counter == 1){
        waarde = indexMogelijk()+1;
        setBuurMogelijkheid(waarde, i, j, veld);
    }
}

void cell::forceerZet(int waarde, cell* veld[][9]){
    for(int i =0; i<9; i++){
        mogelijk[i]=(i==waarde-1);
    }
    checkMogelijkheden(veld);
}

int cell::indexMogelijk(){ //only call if 1 mogelijk
    for(int i = 0; i < 9; i++){
        if(mogelijk[i])
            return i;
        
    }
    return -1;
}

int cell::telMogelijkheden(){
    int counter = 0;

    for(int i = 0; i<9; i++){
        counter+=mogelijk[i];
    }

    return counter;
}

void cell::setBuurMogelijkheid(int waarde, int i, int j, cell* veld[][9]){
    for(int x = 0; x < 9; x++){
        if(x != veld[i][j]->i)
            veld[x][j]->mogelijk[waarde-1] = false;
        if(x != veld[i][j]->j)
            veld[i][x]->mogelijk[waarde-1] = false;   
    }

    int linksbovenI = 3*(i/3); 
    int linksbovenJ = 3*(j/3);

    for(int x = 0; x < 3; x++){
        for(int y =0; y< 3; y++){
            if(!(x == i && y == j))
                veld[linksbovenI+x][linksbovenJ+y]->mogelijk[waarde-1] = false;
        }
    }
}

void spel::printVeld(){
    for (int i = 0; i<9; i++){
        for (int j = 0; j < 9; j++){
            if(veld[i][j]-> waarde == 0)
                cout << "  ";
            else
                cout << veld[i][j]->waarde << ' ';
        }
        cout << endl;
    }
    cout << endl;
}

bool spel::opgelost(){
    for (int i = 0; i<9; i++){
        for (int j = 0; j < 9; j++){
            if(veld[i][j]-> waarde == 0)
                return false;
        }
    }
    return true;
}

bool spel::determined(){
    for (int i = 0; i<9; i++){
        for (int j = 0; j < 9; j++){
            if((veld[i][j]->telMogelijkheden() == 1) && (veld[i][j]->waarde ==0)){
                cout << "veld " << i << " " << j << endl;
                cout << veld[i][j]->indexMogelijk()+1 << endl;
                return true;
            }

        }
    }
    return false;
}

cell* spel::minsteMogelijkheden(){
    cell* result = nullptr;
    int laagste = 10;
    for (int i = 0; i<9; i++){
        for (int j = 0; j < 9; j++){
            if((veld[i][j]->telMogelijkheden()<laagste) && (veld[i][j]->telMogelijkheden()>1)){
                result = veld[i][j];
            }
        }
    }
    return result;
}

int main(){
    int init[9][9] = {
        /*{0, 0, 8, 0, 0, 0, 0, 0, 0},
        {4, 9, 0, 0, 0, 5, 0, 0, 0},
        {1, 0, 7, 0, 3, 9, 0, 0, 2},
        {8, 2, 0, 0, 0, 1, 0, 0, 7},
        {6, 0, 0, 5, 9, 0, 3, 0, 0},
        {0, 0, 4, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 6, 0, 1, 0, 0},
        {3, 0, 9, 0, 7, 0, 8, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 7, 0}*/
        {6, 9, 0, 2, 0, 0, 0, 5, 1},
        {0, 0, 0, 0, 0, 8, 3, 0, 4},
        {0, 8, 3, 9, 0, 1, 0, 0, 0},
        {5, 1, 0, 8, 0, 2, 0, 0, 7},
        {0, 0, 0, 0, 9, 0, 1, 0, 6},
        {0, 4, 2, 0, 0, 7, 0, 8, 0},
        {0, 2, 5, 1, 0, 6, 4, 3, 0},
        {0, 0, 4, 0, 8, 0, 0, 7, 0},
        {7, 0, 0, 5, 0, 0, 6, 0, 0}
    };

    spel hoofdSpel;

    for(int i = 0; i < 9; i++){
        for(int j = 0; j < 9; j++){
            hoofdSpel.veld[i][j] = new cell(init[i][j], i, j);
        }
    }

    

    do{
        for(int i = 0; i < 9; i++){
            for(int j = 0; j < 9; j++){
                hoofdSpel.veld[i][j]->checkMogelijkheden(hoofdSpel.veld);
            }
        }
        hoofdSpel.printVeld();
    }while(!hoofdSpel.opgelost() && hoofdSpel.determined());

    if(hoofdSpel.opgelost()){
        cout << "klaar 1" << endl;
        return 0;
    }

    cell* branchpoint = hoofdSpel.minsteMogelijkheden();
    cout << branchpoint->i << " " << branchpoint->j << endl;
    cout << branchpoint->telMogelijkheden() << endl;

    for(int k = 0; k< 9; k++){
        cout << "KIND " << k << endl;
        if(branchpoint->mogelijk[k])
            hoofdSpel.kinderen[k] = new spel(hoofdSpel, k+1);
    }

    return 0;
}