#include <iostream>
#include <ctime>
#include <iomanip>
#include <omp.h>

using namespace std;

int main()
{
    setlocale(LC_ALL, "Russian");
    srand(time(0));

    int N;
    unsigned int start_time, end_time, res_time, start_timeOMP, end_timeOMP, res_timeOMP;

    cout << "Введите размерность матрицы: ";
    cin >> N;

    int* ptrarrayA = new int[N * N];
    for (int count = 0; count < N * N; count++)
    {
        ptrarrayA[count] = rand() % 10 + 1;
    }
    int* ptrarrayB = new int[N * N];
    for (int count = 0; count < N * N; count++)
    {
        ptrarrayB[count] = rand() % 10 + 1;
    }
    //Матрица A
    int** ptrmatrixA = new int* [N];
    for (int count = 0; count < N; count++)
    {
        ptrmatrixA[count] = new int[N];
    }
    //Матрица B
    int** ptrmatrixB = new int* [N];
    for (int count = 0; count < N; count++)
    {
        ptrmatrixB[count] = new int[N];
    }
    //Результирующая матрица
    int** ptrresmatrix = new int* [N];
    for (int count = 0; count < N; count++)
    {
        ptrresmatrix[count] = new int[N];
    }

    start_time = clock();

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            ptrmatrixA[i][j] = ptrarrayA[i * N + j];
        }
    }

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            ptrmatrixB[i][j] = ptrarrayB[i * N + j];
        }
    }

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            ptrresmatrix[i][j] = 0;
            for (int k = 0; k < N; k++)
            {
                ptrresmatrix[i][j] += ptrmatrixA[i][k] * ptrmatrixB[k][j];
            }
        }
    }

    end_time = clock();

    /*cout << "\nПоследовательный вариант\n" << endl;
    cout << "\nМатрица A:\n";
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            cout << setw(4) << ptrmatrixA[i][j] << "   ";
        }
        cout << endl;
    }
    cout << "\nМатрица B:\n";
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            cout << setw(4) << ptrmatrixB[i][j] << "   ";
        }
        cout << endl;
    }
    cout << endl;
    cout << "\nПолученная матрица:\n";
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            cout << setw(4) << ptrresmatrix[i][j] << "   ";
        }
        cout << endl;
    }
    cout << endl;*/

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //Матрица A
    int** ptrmatrixAomp = new int* [N];
    for (int count = 0; count < N; count++)
    {
        ptrmatrixAomp[count] = new int[N];
    }
    //Матрица B
    int** ptrmatrixBomp = new int* [N];
    for (int count = 0; count < N; count++)
    {
        ptrmatrixBomp[count] = new int[N];
    }
    //Результирующая матрица
    int** ptrresmatrixomp = new int* [N];
    for (int count = 0; count < N; count++)
    {
        ptrresmatrixomp[count] = new int[N];
    }

    start_timeOMP = clock();

#pragma omp parallel
    {
#pragma omp for nowait schedule(dynamic, 250)
        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < N; j++)
            {
                ptrmatrixAomp[i][j] = ptrarrayA[i * N + j];
            }
        }

#pragma omp for nowait schedule(dynamic, 250)
        for (int i = 0; i < N; i++)
        {
#pragma omp parallel shared(i, N)
            {
#pragma omp for schedule(dynamic, 250)
                for (int j = 0; j < N; j++)
                {
                    ptrmatrixBomp[i][j] = ptrarrayB[i * N + j];
                }
            }
        }
    }

#pragma omp parallel
    {
#pragma omp for schedule(dynamic, 250)
        for (int i = 0; i < N; i++)
        {
#pragma omp parallel shared(i, N)
            {
#pragma omp for schedule(dynamic, 250)
                for (int j = 0; j < N; j++)
                {
                    ptrresmatrixomp[i][j] = 0;
                    for (int k = 0; k < N; k++)
                    {
                        ptrresmatrixomp[i][j] += ptrmatrixAomp[i][k] * ptrmatrixBomp[k][j];
                    }
                }
            }
        }
    }

    //#pragma omp parallel
    //    {
    //#pragma omp for
    //        for (int i = 0; i < N; i++)
    //        {
    //#pragma omp parallel shared(i, N)
    //            {
    //#pragma omp for
    //                for (int j = 0; j < N; j++)
    //                {
    //                    ptrresmatrixomp[i][j] = 0;
    //#pragma omp parallel shared(i, j, N)
    //                    {
    //#pragma omp for
    //                        for (int k = 0; k < N; k++)
    //                        {
    //                            ptrresmatrixomp[i][j] += ptrmatrixAomp[i][k] * ptrmatrixBomp[k][j];
    //                        }
    //                    }
    //                }
    //            }
    //        }
    //    }

    end_timeOMP = clock();

    /*cout << "\nПараллельный вариант\n" << endl;
    cout << "\nМатрица A:\n";
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            cout << setw(4) << ptrmatrixAomp[i][j] << "   ";
        }
        cout << endl;
    }
    cout << "\nМатрица B:\n";
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            cout << setw(4) << ptrmatrixBomp[i][j] << "   ";
        }
        cout << endl;
    }
    cout << endl;
    cout << "\nПолученная матрица:\n";
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            cout << setw(4) << ptrresmatrixomp[i][j] << "   ";
        }
        cout << endl;
    }
    cout << endl;*/

    res_time = end_time - start_time;
    cout << "\nВремя выполнения последовательной программы = " << res_time / 1000.0 << endl;
    res_timeOMP = end_timeOMP - start_timeOMP;
    cout << "\nВремя выполнения параллельно распределённой программы = " << res_timeOMP / 1000.0 << endl;

    delete[] ptrarrayA;
    delete[] ptrarrayB;
    for (int count = 0; count < N; count++)
    {
        delete[] ptrmatrixA[count];
    }
    for (int count = 0; count < N; count++)
    {
        delete[] ptrmatrixB[count];
    }
    for (int count = 0; count < N; count++)
    {
        delete[] ptrresmatrix[count];
    }
    for (int count = 0; count < N; count++)
    {
        delete[] ptrmatrixAomp[count];
    }
    for (int count = 0; count < N; count++)
    {
        delete[] ptrmatrixBomp[count];
    }
    for (int count = 0; count < N; count++)
    {
        delete[] ptrresmatrixomp[count];
    }

    cout << "\nОбщее время выполнения программы = " << clock() / 1000.0 << endl;
    cout << endl;
    return 0;
}