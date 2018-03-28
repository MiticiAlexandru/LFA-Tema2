#include <iostream>
#include <fstream>
#include <cstring>
using namespace std;

ifstream f("automat.in");

struct automat
{
    int nr_tranz, final;
    int urmator[100];
    char lit[100];
} A[100];

void initializare()
{
    int i;
    for(i=0;i<100;i++)
    {
        A[i].final=0;
        A[i].nr_tranz=0;
    }
}
int citire_automat()
{
    int i, n, m, x, y, init;
    char c;
    f >> n;
    for(i=1;i<=n;i++)
    {
        f >> x >> y >> c;
        A[x].nr_tranz++;
        A[x].urmator[A[x].nr_tranz]=y;
        A[x].lit[A[x].nr_tranz]=c;
    }
    f >> init >> m;
    for(i=1;i<=m;i++)
    {
        f >> x;
        A[x].final=1;
    }
    return init;
}
void verificare_citire(int init, char cuvant[])
{
    int i, j, n;
    cout << init << "\n";
    for(i=0;i<100;i++)
    {
        if(A[i].nr_tranz!=0)
            for(j=1;j<=A[i].nr_tranz;j++)
                cout << i << " " << A[i].urmator[j] << " " << A[i].lit[j] << "\n";
        if(A[i].final==1)
            cout << "Fin\n";
    }
    n=strlen(cuvant);
    for(i=0;i<n;i++)
        cout << cuvant[i];
    cout << "\n\n";
}
int backtracking(int x, char cuvant[], int s)
{
    if((s==strlen(cuvant))&&(A[x].final==1))
        return 1;
    int i, ok=0;
    for(i=0;i<100;i++)
        if(A[x].lit[i]==cuvant[s])
        {
            return backtracking(A[x].urmator[i], cuvant, s+1);
            ok=1;
        }
    if(ok==0)
        return 0;
}
void apartine_limbaj(int init, char cuvant[])
{
    if(backtracking(init, cuvant, 0)==1)
        cout << "Da.";
    else
        cout << "Nu.";
}

int main()
{
    int init, n, i;
    char cuvant[101];
    initializare();
    init=citire_automat();
    f.get();
    f.getline(cuvant, 100);
    //verificare_citire(init, cuvant);
    apartine_limbaj(init, cuvant);
}
