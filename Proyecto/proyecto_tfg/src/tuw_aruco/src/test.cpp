#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

vector <double> multiply (vector <vector<double>> a, vector <double> b){
    vector <double> result(4);
    result = {0,0,0,1};
    int i, j, k;

	for(i = 0; i < a.size(); ++i)
	{
        for(k=0; k < a.size(); ++k)
        {
            result[i] += a[i][k] * b[k];
        }
	}
    return result;

}

int main(){
    vector<vector<double>> matrix_A(4);  
    const double dx_A = -2;
    const double dy_A = -2;
    const double dz_A = 0;

    matrix_A[0] = {cos(0),    -sin(0),    0.0f,           dx};
    matrix_A[1] = {sin(0),    cos(0),     0.0f,           dy};
    matrix_A[2] = {0.0f,      0.0f,       1.0f,           dz};
    matrix_A[3] = {0.0f,      0.0f,       0.0f,           1.0f};

    vector<double> marker(4);
    marker = {1, 4, 0, 1};
    //cout << matrix[0][0] << " ";
    // for (int i=0; i< 4 ; i++){
    //     for (int j=0; j< 4 ; j++){
    //         cout << matrix[i][j] << " ";
    //     }
    //     cout << endl;
    // }
    vector<double> world;
    world = multiply(matrix, marker);

    for (int j=0; j< 3 ; j++)
        cout << world[j] << " ";
        

    return 0;
}