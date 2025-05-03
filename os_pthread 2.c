#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct
{
    int *array;
    int left;
    int right;
} SortArgs;

void *sort_thread(void *args);
void *merge_thread(void *args);
void merge(int arr[], int l, int m, int r);
void print_array(int arr[], int size);

int *arr1, *arr2;
int size1, size2;
int *merged_array;

int main()
{
    int arr[] = {12, 11, 13, 5, 6, 7, 2, 9, 1, 4, 8, 3, 10};
    int arr_size = sizeof(arr) / sizeof(arr[0]);

    int mid = arr_size / 2;

    pthread_t tid1, tid2, tid3;

    SortArgs args1 = {arr, 0, mid - 1};
    SortArgs args2 = {arr, mid, arr_size - 1};

    pthread_create(&tid1, NULL, sort_thread, &args1);
    pthread_create(&tid2, NULL, sort_thread, &args2);

    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);

    arr1 = arr;
    size1 = mid;
    arr2 = arr + mid;
    size2 = arr_size - mid;
    merged_array = (int *)malloc(arr_size * sizeof(int));

    pthread_create(&tid3, NULL, merge_thread, NULL);

    pthread_join(tid3, NULL);

    print_array(merged_array, arr_size);

    free(merged_array);

    return 0;
}

void *sort_thread(void *args)
{
    SortArgs *sort_args = (SortArgs *)args;
    int *array = sort_args->array;
    int left = sort_args->left;
    int right = sort_args->right;

    for (int i = left; i <= right; i++)
    {
        for (int j = left; j < right - (i - left); j++)
        {
            if (array[j] > array[j + 1])
            {

                int temp = array[j];
                array[j] = array[j + 1];
                array[j + 1] = temp;
            }
        }
    }

    pthread_exit(NULL);
}

void *merge_thread(void *args)
{
    int i = 0, j = 0, k = 0;

    while (i < size1 && j < size2)
    {
        if (arr1[i] <= arr2[j])
        {
            merged_array[k++] = arr1[i++];
        }
        else
        {
            merged_array[k++] = arr2[j++];
        }
    }

    while (i < size1)
    {
        merged_array[k++] = arr1[i++];
    }

    while (j < size2)
    {
        merged_array[k++] = arr2[j++];
    }

    pthread_exit(NULL);
}

void print_array(int arr[], int size)
{
    for (int i = 0; i < size; i++)
    {
        printf("%d ", arr[i]);
    }
    printf("\n");
}