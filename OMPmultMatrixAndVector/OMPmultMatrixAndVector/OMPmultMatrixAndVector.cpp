#include <iostream>
#include <ctime>
#include <iomanip>
#include <omp.h>

using namespace std;

int main()
{
    setlocale(LC_ALL, "Russian");
    srand(time(0));

    int rows = 0,
        columns = 0,
        N = 0;
    unsigned int start_time, end_time, res_time, start_timeOMP, end_timeOMP, res_timeOMP;

    cout << "Введите количество строк матрицы: ";
    cin >> rows;

    cout << "Введите количество столбцов матрицы: ";
    cin >> columns;
    
    if (rows == 0) 
    {
        N = columns;
    }
    else
    {
        N = rows * columns;
    }

    int* ptrarray = new int[N];
    for (int count = 0; count < N; count++)
    {
        ptrarray[count] = rand() % 10 + 1;
    }
    //Вектор
    int* ptrvector = new int[columns];
    //Матрица
    int** ptrmatrix = new int* [rows];
    for (int count = 0; count < rows; count++)
    {
        ptrmatrix[count] = new int[columns];
    }
    //Результирующий вектор
    int* ptrresvector = new int[rows];

    start_time = clock();
    
    for (int count = 0; count < columns; count++)
    {
        ptrvector[count] = ptrarray[count];
    }

    for (int count_row = 0; count_row < rows; count_row++)
    {
        for (int count_column = 0; count_column < columns; count_column++)
        {
            ptrmatrix[count_row][count_column] = ptrarray[count_row * columns + count_column];
        }
    }

    for (int count_row = 0; count_row < rows; count_row++)
    {
        ptrresvector[count_row] = 0;
        for (int count_column = 0; count_column < columns; count_column++)
        {
            ptrresvector[count_row] += ptrmatrix[count_row][count_column] * ptrvector[count_column];
        }
    }

    /*cout << "\nМатрица:\n";
    for (int count_row = 0; count_row < rows; count_row++)
    {
        for (int count_column = 0; count_column < columns; count_column++)
        {
            cout << setw(4) << ptrmatrix[count_row][count_column] << "   ";
        }
        cout << endl;
    }
    cout << "\nВектор:\n";
    for (int count = 0; count < columns; count++)
    {
        cout << setw(4) << ptrvector[count] << "    ";
    }
    cout << endl;
    cout << "\nПолученный вектор:\n";
    for (int count = 0; count < rows; count++)
    {
        cout << setw(4) << ptrresvector[count] << "    ";
    }
    cout << endl;*/

    end_time = clock();

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //Вектор
    int* ptrvectorOMP = new int[columns];
    //Матрица
    int** ptrmatrixOMP = new int* [rows];
    for (int count = 0; count < rows; count++)
    {
        ptrmatrixOMP[count] = new int[columns];
    }
    //Результирующий вектор
    int* ptrresvectorOMP = new int[rows];

    start_timeOMP = clock();

    #pragma omp parallel
    {
        #pragma omp for nowait
        for (int count = 0; count < columns; count++)
        {
            ptrvectorOMP[count] = ptrarray[count];
        }
        #pragma omp for nowait
        for (int count_row = 0; count_row < rows; count_row++)
        {
            for (int count_column = 0; count_column < columns; count_column++)
            {
                ptrmatrixOMP[count_row][count_column] = ptrarray[count_row * columns + count_column];
            }
        }
        #pragma omp for nowait
        for (int count_row = 0; count_row < rows; count_row++)
        {
            #pragma omp parallel shared(count_row, rows)
            {
                #pragma omp for
                for (int count_column = 0; count_column < columns; count_column++)
                {
                    ptrmatrixOMP[count_row][count_column] = ptrarray[count_row * columns + count_column];
                }
            }
        }
    }

    #pragma omp parallel
    {
        #pragma omp for
        for (int count_row = 0; count_row < rows; count_row++)
        {
            ptrresvectorOMP[count_row] = 0;
            #pragma omp parallel shared(count_row, rows)
            {
                #pragma omp for
                for (int count_column = 0; count_column < columns; count_column++)
                {
                    ptrresvectorOMP[count_row] += ptrmatrixOMP[count_row][count_column] * ptrvectorOMP[count_column];
                }
            }
        }
    }


//#pragma omp parallel for default(shared)
//        //#pragma omp for
//        for (int count_row = 0; count_row < rows; count_row++)
//        {
//            ptrresvectorOMP[count_row] = 0;
//#pragma omp parallel shared(count_row, rows)
//            {
//#pragma omp for
//                for (int count_column = 0; count_column < columns; count_column++)
//                {
//                    ptrresvectorOMP[count_row] += ptrmatrixOMP[count_row][count_column] * ptrvectorOMP[count_column];
//                }
//            }
//        }

    /*cout << "\nМатрица:\n";
    for (int count_row = 0; count_row < rows; count_row++)
    {
        for (int count_column = 0; count_column < columns; count_column++)
        {
            cout << setw(4) << ptrmatrixOMP[count_row][count_column] << "   ";
        }
        cout << endl;
    }
    cout << "\nВектор:\n";
    for (int count = 0; count < columns; count++)
    {
        cout << setw(4) << ptrvectorOMP[count] << "    ";
    }
    cout << endl;
    cout << "\nПолученный вектор:\n";
    for (int count = 0; count < rows; count++)
    {
        cout << setw(4) << ptrresvectorOMP[count] << "    ";
    }
    cout << endl;*/

    end_timeOMP = clock();
    res_time = end_time - start_time;
    cout << "\nВремя выполнения последовательной программы = " << res_time / 1000.0 << endl;
    res_timeOMP = end_timeOMP - start_timeOMP;
    cout << "\nВремя выполнения параллельно распределённой программы = " << res_timeOMP / 1000.0 << endl;


    delete[] ptrarray;
    delete[] ptrvector;
    for (int count = 0; count < rows; count++)
    {
        delete[] ptrmatrix[count];
    }
    delete[] ptrresvector;
    delete[] ptrvectorOMP;
    for (int count = 0; count < rows; count++)
    {
        delete[] ptrmatrixOMP[count];
    }
    delete[] ptrresvectorOMP;

    cout << "\nОбщее время выполнения программы = " << clock() / 1000.0 << endl;
    cout << endl;
    return 0;
}
