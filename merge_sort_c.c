// C program for Merge Sort
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

struct thread_data{
    int l;
    int r;
    int arr_size;
    int thread_id;
    int arr[10];
};

// Merges two subarrays of arr[].
// First subarray is arr[l..m]
// Second subarray is arr[m+1..r]
void merge(int arr[], int l, int m, int r)
{
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;

    // Create temp arrays
    int L[n1], R[n2];

    // Copy data to temp arrays L[] and R[]
    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];

    // Merge the temp arrays back into arr[l..r
    i = 0;
    j = 0;
    k = l;
    while (i < n1 && j < n2) {
    // This is the actual sorting of the elements, will only happen when L = R -> 1 element in array
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        }
        else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    // Copy the remaining elements of L[],
    // if there are any
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    // Copy the remaining elements of R[],
    // if there are any
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

// l is for left index and r is right index of the
// sub-array of arr to be sorted

void *mergeSortParallel(void *thread_arg)
{
    
    struct thread_data *parallel_data;
    parallel_data = (struct thread_data *) thread_arg;
    printf("In merge sort parallel, thread_id: %u\n",parallel_data->thread_id);
    if (parallel_data->l < parallel_data->r) {
        //find middle
        int m = parallel_data->l + (parallel_data->r - parallel_data->l) / 2;

        // Sort first and second halves
        pthread_t thread_id_left;
        pthread_t thread_id_right;

        struct thread_data parallel_data_left;
        parallel_data_left.l = parallel_data->l;
        parallel_data_left.r = m;
        parallel_data_left.thread_id = thread_id_left;
        parallel_data_left.arr_size = parallel_data->arr_size;
        printf("parallel_data->arr_size: %d\n",parallel_data->arr_size);
        
        //memcpy(parallel_data_left.arr, parallel_data->arr, parallel_data->arr_size+4);
        int i;
        for(i=0;i < ((parallel_data->arr_size)/4);++i){
            parallel_data_left.arr[i] = (parallel_data->arr)[i];
            
        }
        pthread_create(&thread_id_left, NULL, mergeSortParallel, (void *)&parallel_data_left);
        /*
        struct thread_data parallel_data_right;
        parallel_data_right.l = m+1;
        parallel_data_right.r = parallel_data->r;
        parallel_data_right.thread_id = thread_id_right;
        memcpy(parallel_data_right.arr, parallel_data->arr, parallel_data->arr_size);
        pthread_create(&thread_id_right, NULL, mergeSortParallel, (void *)&parallel_data_right);
        */
        //mergeSort(arr, m + 1, r);

        //merge(parallel_data->arr, parallel_data->l, m, parallel_data->r);
    }
}

void mergeSortNotParallel(int arr[], int l, int r)
{
    if (l < r) {
        //find middle
        int m = l + (r - l) / 2;

        // Sort first and second halves
        mergeSortNotParallel(arr, l, m);
        mergeSortNotParallel(arr, m + 1, r);

        merge(arr, l, m, r);
    }
}

void createKeyArray(int key_array[10], char records_array[10][100]){
    int i;
    for(i = 0;i < 10;++i){
        printf("createKeyArray(): %c \n",records_array[i][0]);
        key_array[i] = (records_array[i][0]-'0');
    }
}


void printArray(char record_array[10][100], int rows, int columns)
{   
    int i;
    int j;
    for (i = 0; i < rows; i++) {
        for (j = 0; j < columns; j++) {
            printf("%c ", record_array[i][j]);
        }
        printf("\n");
    }
}

// Function to print an array
void print1DArray(int arr[], int rows)
{   
    int j;
        for (j = 0; j < rows; j++) {
            printf("%d ", arr[j]);
        }
        printf("\n");
}

void read_file(char record_array[10][100]){
    FILE * file;
    file = fopen("data_to_sort.bin","rb");
    if (file == NULL){
        printf("File cannot be opened \n");
        return;
    }

    char record[100];
    int records_read = 0;
    while(fread(record,100,1,file) == 1)
    {
        printf("%s\n", record);
        //Store record in memory
        int j;
        for(j=0;j<100;++j){
            record_array[records_read][j]=record[j];
        }
        ++records_read;
    }
    fclose(file);
}

int main()
{
    char record_array[10][100];
    int key_array_non_parallel[10];
    int key_array_parallel[10];
    read_file(record_array);
    int rows = 10;
    int columns = 100;

    printf("Given data is \n");
    printArray(record_array, rows, columns);

    createKeyArray(key_array_non_parallel,record_array);
    createKeyArray(key_array_parallel,record_array);
    
    //Time non parallel merge sort completion
    time_t start_not_parallel, end_not_parallel;
    long double time_not_parallel;
    start_not_parallel = clock();
    //after the user defined function does its work
    mergeSortNotParallel(key_array_non_parallel, 0, rows - 1);
    end_not_parallel = clock();
    time_not_parallel=(end_not_parallel-start_not_parallel); //clocks_per_sec
    

    printf("\nSorted array is \n");
    print1DArray(key_array_non_parallel, rows);
    printf("Completion time for not parallel merge sort: %Lf\n", time_not_parallel);

    //Time non parallel merge sort completion
    time_t start_parallel, end_parallel;
    long double time_parallel;
    
    //after the user defined function does its work
    struct thread_data parallel_data;
    parallel_data.l = 0;
    parallel_data.r = rows-1;
    parallel_data.arr_size = sizeof(key_array_parallel);
    printf("size: %d",parallel_data.arr_size);
    parallel_data.thread_id = 1;

    start_parallel = clock();

    int i;
    for(i=0;i < sizeof(key_array_parallel)/4;++i){
            parallel_data.arr[i] = key_array_parallel[i];
        }
    //int j;
    //for(j=0;j < sizeof(key_array_parallel)/4;++j){
       // printf("main: %d",parallel_data.arr[j]);
    //}
    mergeSortParallel(&parallel_data);
    end_parallel = clock();
    time_parallel=(end_parallel-start_parallel); //clocks_per_sec
    

    printf("\nSorted array is \n");
    print1DArray(key_array_parallel, rows);
    printf("Completion time for parallel merge sort: %Lf\n", time_parallel);

    return 0;
}