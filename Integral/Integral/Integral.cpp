#include <iostream>
#include <ctime>
#include <iomanip>
#include <omp.h>

using namespace std;

int main()
{
    setlocale(LC_ALL, "Russian");

    double x, dx, a, b, integral;
    double start_timeOMP, end_timeOMP, res_timeOMP;
    int N, idthread, numthreads;

    a = 0;
    b = 1;
    N = 10000000;
    dx = (b - a) / N;
    integral = 0;
    start_timeOMP = omp_get_wtime();

    ////Последовательный вариант
    //idthread = omp_get_thread_num();
    //numthreads = omp_get_num_threads();
    //cout << "id: " << idthread << endl;
    //cout << "num: " << numthreads << endl;
    //for (int i = idthread; i < N; i += numthreads)
    //{
    //    x = a + i * dx;
    //    integral += 4 * sqrt(1 - x * x) * dx;
    //}
    /////////////////////////////
    
    
    //Параллельный вариант
#pragma omp parallel for reduction (+:integral)
    for (int i = 0; i < N; i += 1)
    {
        x = a + i * dx;
        integral += 4 * sqrt(1 - x * x) * dx;
    }
    ///////////////////////////
    
    end_timeOMP = omp_get_wtime();
    res_timeOMP = end_timeOMP - start_timeOMP;

    cout << setprecision(4) << "Integral = " << integral << endl;
    cout << setprecision(4) << "Time: " << res_timeOMP;

    return 0;
}
