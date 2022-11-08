#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <utility>
#include <algorithm>
#include <fstream>
#include <set>
using namespace std;
/// unordered map ul cu tranzitiile
/**
CITIREA DIN FISIER SE FACE:
 prima linie - nr de stari ale automatului
 a doua linie - starile automatului
 a treia linie - numarul de tranzitii ale automatului (NRTRANZ)
 a patra linie - starea initiala
 a cincea linie - numarul de stari finale
 a sasea linie - starile finale
 pe urmatoarele NRTRANZ linii - o tranzitie de forma
             stare_din_care_pleaca   litera    stare_in_care_ajunge
 a urmatoarea linie - numarul de cuvinte ce vor fi verificate (NRCUV)
 pe urmatoarele NRCUV linii - cate un cuvant

 pentru tranzitia lambda in loc de litera se va scrie cuvantul lambda

 pentru transformare/minimizare nu se mai dau datele pentru cuvinte,
 dar se dau date despre alfabet:

 nrLitere
 litere alfabet
**/
//ifstream f("input.in");
ifstream f("D:\\c++\\input.in");
class Automat{
public:
    int nrStari,nrTranzitii,nrFinale,stareInit;
    vector<int> stariAutomat,stariFinale;
    unordered_map<int,vector<pair<string,int>>> tranzitii;
public:
    Automat(){
        int nod;
        f>>nrStari;
        for(int i=0;i<nrStari;++i){
            f>>nod;
            stariAutomat.push_back(nod);
        }
        f>>nrTranzitii>>stareInit>>nrFinale;
        for(int i=0;i<nrFinale;++i){
            f>>nod;
            stariFinale.push_back(nod);
        }
        pair<string,int> p;
        for(int i=0;i<nrTranzitii;++i){
            f>>nod;
            f>>p.first;
            f>>p.second;
            tranzitii[nod].push_back(p);
        }
    }
    Automat(int x){}
    void afis(){
        cout<<"Numar de stari: "<<nrStari<<endl<<"Stari automat: ";
        for(int i=0;i<nrStari;++i){
            cout<<stariAutomat[i]<<' ';
        }
        cout<<endl<<"Numar de tranzitii: "<<nrTranzitii<<endl<<"Starea initiala: "<<stareInit<<endl<<"Numar de stari finale: "<<nrFinale<<endl<<"Stari finale: ";
        for(int i=0;i<nrFinale;++i){
            cout<<stariFinale[i]<<' ';
        }
        cout<<"\nTranzitiile:\n";
        for(int i=0;i<nrTranzitii;++i){
            if (tranzitii.find(i)!=tranzitii.end())
                for(auto j=tranzitii[i].begin();j!=tranzitii[i].end();++j){
                    cout<<i<<" "<<j->first<<" "<<j->second<<endl;
                }
        }
    }
};
class lambdaNFA: public Automat{
    unordered_map<int,set<string>> vizitat;
    bool acceptat;
    vector<string> cuvinte;
    int nrCuv;

public:
    lambdaNFA():Automat(){
        string aux;
        f>>nrCuv;
        for(int i=0;i<nrCuv;++i){
            f>>aux;
            cuvinte.push_back(aux);
        }
        acceptat=false;
    }
    void afis(){
        Automat::afis();
        cout<<endl<<"Numar de cuvinte: "<<nrCuv<<endl<<"Cuvintele de verificat :\n";
        for(int i=0;i<nrCuv;++i){
            cout<<cuvinte[i]<<endl;
        }
    }
    void check(int stareCurenta,string cuvant){
        if(vizitat[stareCurenta].empty() || (!vizitat[stareCurenta].empty() && vizitat[stareCurenta].find(cuvant)==vizitat[stareCurenta].end())) {
            if (!cuvant.empty()) {
                if (tranzitii.find(stareCurenta) != tranzitii.end()) {
                    for (auto j = tranzitii[stareCurenta].begin(); j != tranzitii[stareCurenta].end(); ++j) {
                        if (j->first == "lambda") {
                            vizitat[stareCurenta].insert(cuvant);
                            check(j->second, cuvant);
                        }
                        else if (j->first == cuvant.substr(0, 1)) {
                            vizitat[stareCurenta].insert(cuvant);
                            check(j->second, cuvant.substr(1));
                        }
                    }
                }
            } else {
                if (find(stariFinale.begin(), stariFinale.end(), stareCurenta) != stariFinale.end())
                    acceptat = true;
                else {
                    if (tranzitii.find(stareCurenta) != tranzitii.end()) {
                        for (auto j = tranzitii[stareCurenta].begin(); j != tranzitii[stareCurenta].end(); ++j) {
                            if (j->first == "lambda") {
                                vizitat[stareCurenta].insert(cuvant);
                                check(j->second, cuvant);
                            }
                        }
                    }
                }
            }
        }


    }
    void checkwords(){
        for(int i=0;i<nrCuv;++i) {
            vizitat.clear();
            for(int i=0;i<nrStari;++i)
                vizitat[stariAutomat[i]]={};
            check(stareInit, cuvinte[i]);
            if (acceptat)
                cout << "Cuvantul "<<cuvinte[i]<<" este acceptat\n";
            else
                cout << "Cuvantul "<<cuvinte[i]<<" nu este acceptat\n";
            acceptat= false;
        }
    }

};
class NFA: public Automat{
public:
    vector<pair<pair<set<int>,string>,set<int>>> table;
    int nrLitere;
    vector<string> litere;
    NFA():Automat(){
        string c;
        f>>nrLitere;
        for(int i=0;i<nrLitere;++i) {
            f >> c;
            litere.push_back(c);
        }
    }
    void afis(){
        Automat::afis();
        cout<<"\nNumarul de litere: "<<nrLitere<<endl;
        for(int i=0;i<nrLitere;++i)
            cout<<litere[i]<<' ';
        cout<<endl;
    }
    void transformare(){
        pair<set<int>,string> plec;
        pair<pair<set<int>,string>,set<int>> ajung;
        vector<set<int>> stariNoi;
        int pozitie=0;
        bool exista;
        set<int> stare={stareInit};
        stariNoi.push_back(stare);
        while(pozitie<stariNoi.size()){
            for(int i=0;i<nrLitere;++i) {
                stare.clear();
                exista= false;
                if(!stariNoi[pozitie].empty()) {
                    for (auto j = stariNoi[pozitie].begin(); j != stariNoi[pozitie].end(); ++j) {
                        if (tranzitii.find(*j) != tranzitii.end()) {
                            for (auto k = tranzitii[*j].begin(); k != tranzitii[*j].end(); ++k) {
                                if (k->first == litere[i]) {
                                    stare.insert(k->second);
                                }
                            }
                        }
                    }
                    for(int verif=0;verif<stariNoi.size();++verif)
                        if(stariNoi[verif]==stare)
                            exista=true;
                    if(stare.empty() || !exista) {
                        stariNoi.push_back(stare);
                    }
                    plec.first = stariNoi[pozitie];
                    plec.second = litere[i];
                    ajung.first = plec;
                    ajung.second = stare;
                    table.push_back(ajung);
                }
            }
            pozitie++;
        }
        /// afisare tranzitii neredenumite
//        for(int i=0;i<table.size();++i){
//            for(auto j=table[i].first.first.begin();j!=table[i].first.first.end();++j)
//                cout<<*j;
//            cout<<' '<<table[i].first.second<<' ';
//            for(auto j=table[i].second.begin();j!=table[i].second.end();++j)
//                cout<<*j;
//            cout<<endl;
//        }
    }
};
class DFA: public Automat{
    int nrLitere;
    vector<string> litere;
public:
    DFA():Automat(){
        string c;
        f>>nrLitere;
        for(int i=0;i<nrLitere;++i) {
            f >> c;
            litere.push_back(c);
        }
    }
    DFA(const NFA &nfa):Automat(1){
        unordered_map<string,int> redenumire;
        int stare=1,stareDead=0;
        string s,t;
        for(int i=0;i<nfa.table.size();++i){
            s="";
            if(nfa.table[i].second.begin()==nfa.table[i].second.end())
                stareDead= 1;
            for(auto j=nfa.table[i].first.first.begin();j!=nfa.table[i].first.first.end();++j) {
                s+= to_string(*j);
            }
            if(redenumire.find(s) == redenumire.end()){
                redenumire[s]=stare;
                stare++;
            }
        }
        pair<string,int> p;
        nrLitere=nfa.nrLitere;
        for(int i=0;i<nrLitere;++i)
            litere.push_back(nfa.litere[i]);
        nrFinale=0;
        nrStari=stare-1+stareDead;
        stareInit=1;
        nrTranzitii=nfa.table.size()+nfa.nrLitere*stareDead;
        for(auto i=redenumire.begin();i!=redenumire.end();++i){
            stariAutomat.push_back(i->second);
            for(int j=0;j<nfa.stariFinale.size();++j){
                if(i->first.find(to_string(nfa.stariFinale[j])) != string::npos){
                    nrFinale++;
                    stariFinale.push_back(i->second);
                }
            }
        }
        sort(stariAutomat.begin(),stariAutomat.end());
        sort(stariFinale.begin(),stariFinale.end());
        if(stareDead)
            stariAutomat.push_back(-1);
        for(int i=0;i<nfa.table.size();++i){
            s="";
            t="";
            for(auto j=nfa.table[i].first.first.begin();j!=nfa.table[i].first.first.end();++j) {
                s+= to_string(*j);
            }
            if(!nfa.table[i].second.empty()) {
                for (auto j = nfa.table[i].second.begin(); j != nfa.table[i].second.end(); ++j) {
                    t += to_string(*j);
                }
            }
            p.first=nfa.table[i].first.second;
            if(t=="")
                p.second=-1;
            else
                p.second=redenumire[t];
            tranzitii[redenumire[s]].push_back(p);
        }
        if(stareDead){
            for (int i = 0; i < nfa.nrLitere; ++i) {
                p.first=nfa.litere[i];
                p.second=-1;
                tranzitii[-1].push_back(p);
            }
        }
    }
    void afis(){
        Automat::afis();
        if (tranzitii.find(-1) != tranzitii.end())
            for(auto j=tranzitii[-1].begin();j!=tranzitii[-1].end();++j)
                cout<<-1<<' '<<j->first<<' '<<j->second<<endl;
        cout<<"\nNumarul de litere: "<<nrLitere<<endl;
        for(int i=0;i<nrLitere;++i)
            cout<<litere[i]<<' ';
        cout<<endl;
    }
    void minimizare(){
        int k=0;
        vector<vector<set<int>>> echiv;
        vector<set<int>> auxiliar;
        set<int> aux;
        for(int i=0;i<nrStari;++i){
            if(find(stariFinale.begin(),stariFinale.end(),stariAutomat[i])==stariFinale.end()) {
                aux.insert(stariAutomat[i]);
            }
        }
        auxiliar.push_back(aux);
        echiv.push_back(auxiliar);
        auxiliar.clear();
        aux.clear();
        for(int i=0;i<nrFinale;++i)
            aux.insert(stariFinale[i]);
        echiv[0].push_back(aux);
        aux.clear();
        do{
            for (int i = 0; i < echiv[k].size(); ++i) {
                auxiliar.push_back({*echiv[k][i].begin()});
                if(echiv[k][i].size()>1) {
                    bool increment= true,adaugat;
                    for (auto j = echiv[k][i].begin(); j != echiv[k][i].end(); ++j) {
                        adaugat=false;
                        if(increment){
                            ++j;
                            increment=false;
                        }
                        for(int t=0;t<auxiliar.size() && !adaugat; ++t){
                            bool ok= true;
                            for(auto p=auxiliar[t].begin();p!=auxiliar[t].end() && ok;++p){
                                for(int h=0;h<tranzitii[*p].size() && ok;++h){
                                    for(int h1=0;h1<tranzitii[*j].size() && ok;++h1){
                                        if(tranzitii[*p][h].first==tranzitii[*j][h1].first){
                                            if(tranzitii[*p][h].second!=tranzitii[*j][h1].second && echiv[k][i].find(tranzitii[*j][h1].second)==echiv[k][i].end()){
                                                auxiliar.push_back({*j});
                                                ok=false;
                                                break;
                                            }
                                        }
                                    }
                                }
                            }
                            if (ok){
                                auxiliar[t].insert(*j);
                                adaugat= true;
                            }
                        }

                    }
                }
            }
            echiv.push_back(auxiliar);
            auxiliar.clear();
            k++;
        } while (echiv[k-1]!=echiv[k]);

        /// afisare stadiile de echivalenta
//        for(int k1=0;k1<=k;++k1) {
//            cout<<"\n\nEchiv "<<k1<<endl;
//            for (int i = 0; i < echiv[k1].size(); ++i) {
//                for (auto j = echiv[k1][i].begin(); j != echiv[k1][i].end(); ++j)
//                    cout << *j << ' ';
//                cout << endl;
//            }
//        }
        int nrStari1,nrTranzitii1,nrFinale1=0,stareInit1;
        vector<int> stariAutomat1,stariFinale1;
        unordered_map<int,vector<pair<string,int>>> tranzitii1;
        nrStari1=echiv[k].size();
        nrTranzitii1=nrStari1*nrLitere;
        bool ok;
        pair<string,int> p;
        int redenumire[10000];
        for(int i=0;i<echiv[k].size();++i){
            for(auto j=echiv[k][i].begin();j!=echiv[k][i].end();++j){
                redenumire[*j]=i+1;
            }
        }
        for(int i=0;i<echiv[k].size();++i){
            ok=false;
            for(auto j=echiv[k][i].begin();j!=echiv[k][i].end();++j){
                if(*j==stareInit)
                    stareInit1=i+1;
                if(find(stariFinale.begin(),stariFinale.end(),*j)!=stariFinale.end()) {
                    ok = true;
                }
            }
            stariAutomat1.push_back(i+1);
            if(ok){
                nrFinale1++;
                stariFinale1.push_back(i+1);
            }
            for(int lit=0;lit<nrLitere;lit++){
                p.first=litere[lit];
                for(auto index=tranzitii[*echiv[k][i].begin()].begin();index!=tranzitii[*echiv[k][i].begin()].end();++index){
                    if(index->first==p.first) {
                        p.second = redenumire[index->second];
                        break;
                    }
                }
                tranzitii1[i+1].push_back(p);
            }
        }
        nrStari=nrStari1;
        nrFinale=nrFinale1;
        stareInit=stareInit1;
        nrTranzitii=nrTranzitii1;
        stariAutomat.clear();
        stariFinale.clear();
        stariAutomat=stariAutomat1;
        stariFinale=stariFinale1;
        tranzitii.clear();
        for(int i=0;i<nrStari;++i){
            tranzitii[i+1]=tranzitii1[i+1];
        }
    }
};
int main(){
    ///a)
    lambdaNFA lNFA;
    lNFA.afis();
    lNFA.checkwords();

    ///b)
//    NFA nfa;
//    nfa.afis();
//    nfa.transformare();
//    DFA dfa(nfa);
//    dfa.afis();

    ///c)
//    DFA dfa2;
//    dfa2.afis();
//    dfa2.minimizare();
//    dfa2.afis();

    return 0;
}
