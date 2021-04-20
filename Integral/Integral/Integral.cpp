#include <iostream>
#include <ctime>
#include <iomanip>
#include <omp.h>

using namespace std;

int main()
{
    setlocale(LC_ALL, "Russian");

    double a = 0, b = 1, integral = 0, x, dx;
    double start_timeOMP, end_timeOMP, res_timeOMP;
    int n = 1000000, idthread, numthreads;

    dx = (b - a) / n;

    start_timeOMP = omp_get_wtime();

    ////Последовательный вариант
    //idthread = omp_get_thread_num();
    //numthreads = omp_get_num_threads();
    //cout << "Id thread: " << idthread << endl;
    //cout << "Num threads: " << numthreads << endl;
    //for (int i = 0; i < n; i += 1)
    //{
    //    x = a + i * dx;
    //    integral += 4 * sqrt(1 - x * x);
    //}
    //integral *= dx;
    /////////////////////////////
    
    
    //Параллельный вариант
#pragma omp parallel
    {
        idthread = omp_get_thread_num();
        numthreads = omp_get_num_threads();
        if (idthread == 0)
        {
            cout << "Num threads: " << numthreads << endl;
        }
    }
    
#pragma omp parallel for reduction (+:integral)
    for (int i = 0; i < n; i += 1)
    {
        x = a + i * dx;
        integral += 4 * sqrt(1 - x * x);
    }
    integral *= dx;
    ///////////////////////////
    
    end_timeOMP = omp_get_wtime();
    res_timeOMP = end_timeOMP - start_timeOMP;

    cout << setprecision(4) << "Definite integral on (a, b): 4 * sqrt(1 - x * x) * dx = " << integral << endl;
    cout << setprecision(4) << "Time: " << res_timeOMP;

    return 0;
}
