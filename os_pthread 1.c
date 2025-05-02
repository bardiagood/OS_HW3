#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

typedef struct
{
    int n;
    int (*sudoku)[9];
} sudoku;

int valid = 1;

void *check_row(void *arg)
{
    sudoku s = *(sudoku *)arg;
    int row = s.n;
    int sudo[9][9];
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            sudo[i][j] = s.sudoku[i][j];
        }
    }
    int seen[10] = {0};
    for (int i = 0; i < 9; i++)
    {
        if (!seen[sudo[row][i]])
        {
            seen[sudo[row][i]] = 1;
        }
        else
        {
            valid = 0;
            pthread_exit(NULL);
        }
    }
    pthread_exit(NULL);
    return NULL;
}
void *check_col(void *arg)
{
    sudoku s = *(sudoku *)arg;
    int col = s.n;
    int sudo[9][9];
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            sudo[i][j] = s.sudoku[i][j];
        }
    }
    int seen[10] = {0};
    for (int i = 0; i < 9; i++)
    {
        if (!seen[sudo[i][col]])
        {
            seen[sudo[i][col]] = 1;
        }
        else
        {
            valid = 0;
            pthread_exit(NULL);
        }
    }
    pthread_exit(NULL);
    return NULL;
}
void *check_grid(void *arg)
{
    sudoku s = *(sudoku *)arg;
    int grid = s.n + 1;
    int i, j;
    if (grid < 4)
    {
        i = 0;
    }
    else if (grid < 7)
    {
        i = 3;
    }
    else
    {
        i = 6;
    }
    if (grid % 3 == 1)
    {
        j = 0;
    }
    else if (grid % 3 == 2)
    {
        j = 3;
    }
    else
    {
        j = 6;
    }
    int a = i;
    int b = j;
    int sudo[9][9];
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            sudo[i][j] = s.sudoku[i][j];
        }
    }
    int seen[10] = {0};
    for (i; i < a + 3; i++)
    {
        for (j; j < b + 3; j++)
        {
            if (!seen[sudo[i][j]])
            {
                seen[sudo[i][j]] = 1;
            }
            else
            {
                valid = 0;
                pthread_exit(NULL);
            }
        }
    }
    pthread_exit(NULL);
    return NULL;
}

int main()
{
    pthread_t threads[27];
    sudoku s[27];
    int test[9][9] = {
        {5, 3, 4, 6, 7, 8, 9, 1, 2},
        {6, 7, 2, 1, 9, 5, 3, 4, 8},
        {1, 9, 8, 3, 4, 2, 5, 6, 7},
        {8, 5, 9, 7, 6, 1, 4, 2, 3},
        {4, 2, 6, 8, 5, 3, 7, 9, 1},
        {7, 1, 3, 9, 2, 4, 8, 5, 6},
        {9, 6, 1, 5, 3, 7, 2, 8, 4},
        {2, 8, 7, 4, 1, 9, 6, 3, 5},
        {3, 4, 5, 2, 8, 6, 1, 7, 9}};

    for (int i = 0; i < 9; i++)
    {
        s[i].sudoku = test;
        s[i + 9].sudoku = test;
        s[i + 18].sudoku = test;
        s[i].n = i;
        s[i + 9].n = i;
        s[i + 18].n = i;
    }
    int res;
    for (int i = 0; i < 9; i++)
    {
        res = pthread_create(&threads[i], NULL, check_row, &s[i]);
        res = pthread_create(&threads[i + 9], NULL, check_col, &s[i + 9]);
        res = pthread_create(&threads[i + 18], NULL, check_grid, &s[i + 18]);
    }
    for (int i = 0; i < 27; i++)
    {
        pthread_join(threads[i], NULL);
    }
    if (valid)
    {
        printf("valid");
    }
    else
    {
        printf("not valid");
    }
}