#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;

ifstream f("transformare2.in");

// Programul noteaza lambda cu &, linia 79 include tranzitiile cu lambda

int nrst, init, indice=0, indice2=0;

/*
aAFN[i] si bAFD[i] reprezinta tranzitiile care incep din nodul i
nr_tranz = nr tranzitii
final = 1 sau 0 (e stare finala sau nu)
urmator[j] = capatul din dreapta a tranzitiei j
lit[j] = litera tranzitiei j
StariMultiple[0] = nr starilor din AFD compuse din mai multe stari ale AFN-ului
StariMultiple[j] = starea j AFN care apare in aceasta stare multipla
viz[j] = 1 sau 0 (daca litera tranzitiei din nodul i in j a fost deja folosita pentru tranzitiile din nodul i)
*/

struct automat
{
    int nr_tranz, final, verif;
    int urmator[100], viz[100];
    char lit[100];
    int StariMultiple[100];
} aAFN[100], bAFD[1000];

void initializare()
{
    int i;
    for(i=0;i<100;i++)
    {
        aAFN[i].final=0;
        aAFN[i].nr_tranz=0;
    }
    for(i=0;i<1000;i++)
    {
        bAFD[i].final=0;
        bAFD[i].nr_tranz=0;
        bAFD[i].StariMultiple[0]=0;
        bAFD[i].verif=0;
    }
}

void citire_automat()
{
    int i, n, x, y;
    char c;
    f >> n;
    for(i=1;i<=n;i++)
    {
        f >> x >> y >> c;
        aAFN[x].nr_tranz++;
        aAFN[x].urmator[aAFN[x].nr_tranz]=y;
        aAFN[x].lit[aAFN[x].nr_tranz]=c;
        aAFN[x].viz[aAFN[x].nr_tranz]=0;
    }
    f >> init >> nrst;
    for(i=1;i<=nrst;i++)
    {
        f >> x;
        aAFN[x].final=1;
    }
}

void SetTranzitii()
{
    int ok=0, i, j, k;
    for(i=0;i<100;i++)
        for(j=1;j<=aAFN[i].nr_tranz;j++)
            if(aAFN[i].viz[j]==0)
            {
                indice++;
                for(k=1;k<=aAFN[i].nr_tranz;j++)
                    if((aAFN[i].lit[k]==aAFN[i].lit[j])||(aAFN[i].lit[k]=='&')) //eroare la (aAFN[i].lit[k]==aAFN[i].lit[j])
                    {
                        aAFN[i].viz[k]=1;
                        bAFD[indice].StariMultiple[0]++;
                        bAFD[indice].StariMultiple[bAFD[indice].StariMultiple[0]]=k;
                    }
                bAFD[indice2].nr_tranz++;
                bAFD[indice2].urmator[bAFD[indice2].nr_tranz]=indice;
                strcpy(bAFD[indice2].lit,aAFN[i].lit);
                indice2=indice+1;
            }
    while(ok==0)
    {
        for(i=0;i<1000;i++)
            for(j=1;j<=bAFD[i].nr_tranz;j++)
                if(bAFD[i].viz[j]==0)
                {
                    indice++;
                    for(k=j;k<=bAFD[i].nr_tranz;j++)
                        if((bAFD[i].lit[k]==bAFD[i].lit[j])||(bAFD[i].lit[k]=='&'))
                        {
                            bAFD[i].viz[k]=1;
                            bAFD[indice].StariMultiple[0]++;
                            bAFD[indice].StariMultiple[bAFD[indice].StariMultiple[0]]=k;
                        }
                    bAFD[indice2].nr_tranz++;
                    bAFD[indice2].urmator[bAFD[indice2].nr_tranz]=indice;
                    strcpy(bAFD[indice2].lit,bAFD[i].lit);
                    indice2=indice+1;
                }
        //verificare iesire while:
        ok=1;
        for(i=0;i<indice;i++)
            if(((bAFD[i].verif==0)&&(bAFD[i].nr_tranz!=0))&&(indice<1000))
                ok=0;
    }
}

void transformare()
{
    int ok, i, j, k;
    //setare stare initiala:
    bAFD[0].StariMultiple[1]=init;
    bAFD[0].StariMultiple[0]=1;
    //setare tranzitii:
    SetTranzitii();
    //setare stari finale:
    for(i=0;i<1000;i++)
        if(bAFD[i].nr_tranz!=0)
        {
            ok=0;
            k=0;
            while(ok==0&&k<=bAFD[i].StariMultiple[0])
            {
                k++;
                for(j=0;j<100;j++)
                    if((aAFN[j].final==1)&&(j==bAFD[i].StariMultiple[k]))
                    {
                        bAFD[i].final=1;
                        ok=1;
                    }
            }
        }
}

void afisareStariMultiple(int i)
{
    cout << "{q";
    int j;
    cout << bAFD[i].StariMultiple[1];
    for(j=2;j<=bAFD[i].StariMultiple[0];j++)
        cout << ",q" << bAFD[i].StariMultiple[j];
    cout << "}";
}

void afisareAFD()
{
    int i, j, ok=0, k;
    cout << "Stare initiala este " << init << ".\n\n";
    cout << "Tranzitiile sunt:\n";
    for(i=0;i<1000;i++)
        if(bAFD[i].nr_tranz!=0)
            for(j=1;j<=bAFD[i].nr_tranz;j++)
            {
                afisareStariMultiple(i);
                cout << " ";
                afisareStariMultiple(bAFD[i].urmator[j]);
                cout << " " << bAFD[i].lit[j] << "\n";
            }
    cout << "\n";
    cout << "Cele " << nrst << " stari finale sunt: ";
    for(i=0;i<1000;i++)
        if(bAFD[i].nr_tranz!=0)
            if(bAFD[i].final==1)
            {
                if(ok==1)
                    cout << ", ";
                else
                    ok=1;
                afisareStariMultiple(i);
            }
    cout << ".\n";
}

int main()
{
    int init;
    char cuvant[101];
    initializare();
    citire_automat();
    f.close();
    transformare();
    afisareAFD();
}
