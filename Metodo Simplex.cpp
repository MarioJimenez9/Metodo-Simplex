#include <bits/stdc++.h>

using namespace std;

#define strdoub string, double

int n, m, x;
bool f = 0;

map<string, int> mp;
map<int, string> pm, cm;
map<string, double> values;
vector<vector<double>> simplex; //Matrix to represent simplex table
vector<string> restrictions;
vector<double> c;

//printing matrix
void printTable(int num){
    cout<<"TABLA SIMPLEX #"<<num<<endl<<endl;
    cout<<"xb"<<setw(13);
    for(int i = 1; i <= n; i++)
        cout<<'x'<<i<<setw(13);
    for(int i = 1; i <= m; i++)
        cout<<'s'<<i<<setw(13);
    cout<<"bj"<<endl;
    for(int i = 0; i <= m; i++){
        cout<<pm[i]<<setw(14);
        for(auto it2 : simplex[i]){
            cout<<it2<<setw(14);
        }
        cout<<"\n";
    }
    cout<<"\n\n";
    return;
}

int getPivCol(){
    vector<double> zCol = simplex.back();
    return min_element(zCol.begin(), zCol.end()) - zCol.begin();
}

int getPivRow(double pivCol){
    double mx = 1e9;
    int row = 0;
    int pivRow = 0;
    for(int i = 0; i < m; i++){
        double v = simplex[i].back() / simplex[i][pivCol];
        if(mx > v){
            mx = v;
            pivRow = row;
        }
        row++;
    }
    return pivRow;
}

vector<double> getInputRow(int r, int c){
    double pivElement = simplex[r][c];
    for(auto &it: simplex[r]){
        it /= pivElement;
    }
    return simplex[r];
}

void fixTable(int r, int c){

    vector<double> currRow = getInputRow(r, c); //getting the inputRow and updating
    for(int i = 0; i <= m; i++){
        if(i == r)continue;
        double cPivi = simplex[i][c]; //Pivot coefficient of the current row;
        for(int j = 0; j < simplex[i].size(); j++){
            simplex[i][j] = simplex[i][j] - (cPivi * currRow[j]);
        }
    }
}

//function to maximize
int maxZ(int num){
    printTable(num);
    double pivCol = getPivCol();
    if(simplex[m][pivCol] < 0){
        double pivRow = getPivRow(pivCol);
        fixTable(pivRow, pivCol);
        pm[pivRow] = cm[pivCol];

        return maxZ(num + 1);
    }
    else{
        double ans = 0;
        return ans;
    }
}

//Funcion to minimize
int minZ(int num){
    cout<<"Minimize"<<endl;
    getPivCol();
}

void build(){
    simplex = vector<vector<double>>(m + 1, vector<double>(n + m + 1, 0.0));
    int i = 0;
    //map all restrictions
    for(auto it : restrictions){
        string rest = it;
        istringstream ss(rest);
        string term;
        int pp = 1;
        int sign = 1;
        while(ss >> term){
            pp ^= 1;
            if(pp & 1){
                if(term == "+")sign = 1;
                else if(term == "-")sign = 0;
                else sign = 2;
                continue;
            }
            if(sign == 2){
                simplex[i].back() = stod(term);
                continue;
            }
            int len = term.size();
            double v = 1.0;
            if(len > 2) v = stod(term.substr(0, len - 2));
            string inc = term.substr(len - 2, 2);
            if(!sign && v != 0)v *= -1;
            simplex[i][mp[inc]] = v;
        }
        simplex[i][n + i] = 1;
        i++;
    }
    //map Z function
    for(int i = 0; i < n; i++){
        simplex[m][i] = c[i + 1] * (c[i + 1] != 0? -1 : 1);
    }
}

void getInput(){
    int x;
    double v;
    cout<<"########## METODO SIMPLEX ##########"<<endl<<endl;
    cout<<"¿Que quieres hacer? \n 1 = Maximizar. \n 2 = Minimizar. \n";
    cin>>x;
    if(x == 2)f = 1;


    //Asking and mapping coefficients
    cout<<fixed<<setprecision(4)<<"Numero de coeficientes de la ecuacion: ";
    cin>>n;
    c = vector<double>(n + 1);
    cout<<"\nIngresando "<< n <<" coeficientes: \n";
    for(int i = 1; i <= n; i++){
        string curr = 'x' + to_string(i);
        cout<<curr<<" = ";
        cin>>c[i];
        cout<<curr<<endl;
        mp[curr] = i - 1;
        cm[i - 1] = curr;
    }
    cout<<"Numero de restricciones: ";
    cin>>m;
    string rest;
    int s = 1;
    cin.ignore();
    for(int i = 0; i < m; i++){
        getline(cin, rest);
        restrictions.push_back(rest);
        pm[i] = 's' + to_string(i + 1);
    }
    pm[m] = "z ";
    return;
}


int main(){

    getInput();
    build();
    f ? minZ(1) : maxZ(1);
    return 0;
}
/*
Ejemplos:
1
2
10
20
3
4x1 + 2x2 <= 20
8x1 + 8x2 <= 20
2x2 <= 10

Ejemplos:
1
2
150
200
3
40x1 + 60x2 <= 7400
20x1 + 25x2 <= 3300
x1 + x2 <= 150

1
3
3
2
5
3
x1 + 2x2 + x3 <= 430
3x1 + 0x2 + 2x3 <= 460
x1 + 4x2 + 0x3 <= 420
*/
