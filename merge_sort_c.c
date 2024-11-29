// C program for Merge Sort
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
struct thread_data{
    int l;
    int r;
    int arr_size;
    int thread_id;
    void* arr;
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

void* mergeSortParallelPre(void *thread_arg)
{
    struct thread_data *parallel_data;
    parallel_data = (struct thread_data *) thread_arg;
    if (parallel_data->l < parallel_data->r) {
        //find middle
        int m = parallel_data->l + (parallel_data->r - parallel_data->l) / 2;

        // Sort first and second halves
        mergeSortNotParallel(parallel_data->arr, parallel_data->l, m);
        mergeSortNotParallel(parallel_data->arr, m + 1, parallel_data->r);

        merge(parallel_data->arr, parallel_data->l, m, parallel_data->r);
    }
    return 0;
}

void createKeyArray(int* key_array, char** records_array, int data_rows){
    int i;
    for(i = 0;i < data_rows;++i){
        char character_to_int[4];
        int decimal_of_character;
        character_to_int[0] = records_array[i][0];
        character_to_int[1] = records_array[i][1];
        character_to_int[2] = records_array[i][2];
        character_to_int[3] = records_array[i][3];
        sscanf(character_to_int, "%d", &decimal_of_character);
        key_array[i] = decimal_of_character;
        
    }
}

void output_file(int * key_array_parallel, char** records_array, char * output_file, int rows){
    //Using sorted keys combine data with their keys in record_array_out
    int i;
    int j;
    int m;
    char* record_array_out[rows];
    for (m = 0; m < rows; m++)
        record_array_out[m] = (char*)malloc(100 * sizeof(char));

    for(i=0;i<rows;++i){
        for(j=0;j<rows;++j){
            char character_to_int[4];
            int decimal_of_character;
            character_to_int[0] = records_array[j][0];
            character_to_int[1] = records_array[j][1];
            character_to_int[2] = records_array[j][2];
            character_to_int[3] = records_array[j][3];
            sscanf(character_to_int, "%d", &decimal_of_character);
            if(key_array_parallel[i]==decimal_of_character){
                int p;
                for(p = 0; p < 100; ++p){
                    record_array_out[i][p] = records_array[j][p];
                }
            }
        }
    }
    
   //Write to output file
   FILE * file;
    file = fopen(output_file,"wb");
    if (file == NULL){
        printf("File cannot be opened \n");
        return;
    }
   int r;
   int c;
   for(r = 0; r < rows;++r){
    for(c = 0; c < 100; ++c){
        fprintf(file,"%c",record_array_out[r][c]);
    }
    fprintf(file, "\n");
   }
   fclose(file);


}


void printArray(char **record_array, int rows, int columns)
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
// Function to print an array
void print1DArrayAddress(int *arr, int rows)
{   
    int j;
        for (j = 0; j < rows; j++) {
            printf("%d ", arr[j]);
        }
        printf("\n");
}
void read_file(char** record_array, char *input_file){
    FILE * file;
    file = fopen(input_file,"rb");
    if (file == NULL){
        printf("File cannot be opened \n");
        return;
    }

    char record[100];
    int records_read = 0;
    while(fread(record,100,1,file) == 1)
    {
        //Store record in memory
        int j;
        for(j=0;j<100;++j){
            record_array[records_read][j]=record[j];
            int j;
            for(j=0;j<100;++j){
                printf("%d",record[j]);
            }
            printf("\n");
        }
        ++records_read;
    }
    printf("File Close\n");
    fclose(file);
}


int main(int argc, char **argv)
{
    // Enter the input file, and output file with the execution of the program for it to run: ./merge_sort_c data_to_sort.bin output
    char * input_file = argv[1];
    char * output_file_name = argv[2];
    printf("%s\n",input_file);
    printf("%s\n",output_file_name);
    
    //Get number of rows
    struct stat st;
    stat(input_file, &st);
    int size = st.st_size;
    int data_rows = size/100;
    
    int i;
    
    int c = 100;

    char* record_array[data_rows];
    for (i = 0; i < data_rows; i++)
        record_array[i] = (char*)malloc(c * sizeof(char));

    char* output_record_array[data_rows];
    for (i = 0; i < data_rows; i++)
        output_record_array[i] = (char*)malloc(c * sizeof(char));

    int *key_array_non_parallel = (int *)malloc((data_rows*sizeof(int)));
    int *key_array_parallel = (int *)malloc((data_rows*sizeof(int)));
    int *key_array_parallel_four = (int *)malloc((data_rows*sizeof(int)));
    int *output_key_array = (int *)malloc((data_rows*sizeof(int)));
    read_file(record_array, input_file);
    
    int rows = data_rows;
    createKeyArray(key_array_non_parallel,record_array, data_rows);
    createKeyArray(key_array_parallel,record_array, data_rows);
    createKeyArray(key_array_parallel_four,record_array, data_rows);
    //Time non parallel merge sort completion
    time_t start_not_parallel, end_not_parallel;
    long double time_not_parallel;
    start_not_parallel = clock();
    //after the user defined function does its work
    mergeSortNotParallel(key_array_non_parallel, 0, rows - 1);
    end_not_parallel = clock();
    time_not_parallel=(double)(end_not_parallel-start_not_parallel)/CLOCKS_PER_SEC; //clocks_per_sec
    

    printf("\nSorted array of non parallel is \n");
    print1DArray(key_array_non_parallel, rows);
    printf("Completion time for non parallel merge sort: %Lf\n", time_not_parallel);

    //2 THREADS
        time_t start_parallel, end_parallel;
        long double time_parallel;
        start_parallel = clock();
    
    
        int m = (rows-1) / 2;

        // Sort first and second halves
        pthread_t thread_id_left;
        pthread_t thread_id_right;

        //Left half 
        struct thread_data parallel_data_left;
        parallel_data_left.l = 0;
        parallel_data_left.r = m;
        
        parallel_data_left.arr = key_array_parallel;
        pthread_create(&thread_id_left, NULL, mergeSortParallelPre, (void *)&parallel_data_left);
        pthread_join(thread_id_left, NULL);
        pthread_cancel(thread_id_left);
        //Right half
        
        struct thread_data parallel_data_right;
        parallel_data_right.l = m+1;
        parallel_data_right.r = rows;
        parallel_data_right.arr = key_array_parallel;
        pthread_create(&thread_id_right, NULL, mergeSortParallelPre, (void *)&parallel_data_right);
        pthread_join(thread_id_right, NULL);
        pthread_cancel(thread_id_right);

        end_parallel = clock();
        time_parallel=(double)(end_parallel-start_parallel)/CLOCKS_PER_SEC; //clocks_per_sec
    

    //printf("\nSorted array for parallel merge is \n");
    print1DArray(key_array_parallel, rows);
    printf("Completion time for parallel merge sort (2 threads): %Lf\n", time_parallel);
    
    //Output to file
    output_file(key_array_parallel, record_array, output_file_name, rows);
    read_file(output_record_array,output_file_name);
    createKeyArray(output_key_array,output_record_array,data_rows);
    //4 THREADS
        time_t start_parallel_four, end_parallel_four;
        long double time_parallel_four;
        start_parallel_four = clock();
    
    
        int m_initial = (rows-1) / 2;
        int left_m = m_initial / 2;
        int right_m = m_initial + ((rows-1) - m_initial) / 2;
        // Sort first and second halves
        pthread_t thread_id_left_one;
        pthread_t thread_id_left_two;
        pthread_t thread_id_right_one;
        pthread_t thread_id_right_two;

        //Left half 
        struct thread_data parallel_data_left_one;
        parallel_data_left_one.l = 0;
        parallel_data_left_one.r = left_m;
        
        parallel_data_left_one.arr = key_array_parallel_four;
        pthread_create(&thread_id_left_one, NULL, mergeSortParallelPre, (void *)&parallel_data_left_one);
        pthread_join(thread_id_left_one, NULL);
        pthread_cancel(thread_id_left_one);

        struct thread_data parallel_data_left_two;
        parallel_data_left_two.l = left_m+1;
        parallel_data_left_two.r = m;
        
        parallel_data_left_two.arr = key_array_parallel_four;
        pthread_create(&thread_id_left_two, NULL, mergeSortParallelPre, (void *)&parallel_data_left_two);
        pthread_join(thread_id_left_two, NULL);
        pthread_cancel(thread_id_left_two);

        //Right half
        
        struct thread_data parallel_data_right_one;
        parallel_data_right_one.l = m+1;
        parallel_data_right_one.r = right_m;
        parallel_data_right_one.arr = key_array_parallel_four;
        pthread_create(&thread_id_right_one, NULL, mergeSortParallelPre, (void *)&parallel_data_right_one);
        pthread_join(thread_id_right_one, NULL);
        pthread_cancel(thread_id_right_one);

        struct thread_data parallel_data_right_two;
        parallel_data_right_two.l = right_m + 1;
        parallel_data_right_two.r = rows;
        parallel_data_right_two.arr = key_array_parallel_four;
        pthread_create(&thread_id_right_two, NULL, mergeSortParallelPre, (void *)&parallel_data_right_two);
        pthread_join(thread_id_right_two, NULL);
        pthread_cancel(thread_id_right_two);

        end_parallel_four = clock();
        time_parallel_four=(double) (end_parallel_four-start_parallel_four)/CLOCKS_PER_SEC; //clocks_per_sec
    

    
    printf("Completion time for parallel merge sort (4 threads): %Lf\n", time_parallel_four);
    
    printf("Checking output file\n");
    print1DArray(output_key_array, rows);
    //Deallocate Memory

    free(key_array_non_parallel);
    free(key_array_parallel);
    free(key_array_parallel_four);
    int f;
    for(f=0;f<rows;++f){
        free(record_array[f]);
    }
    int g;
    for(g=0;g<rows;++g){
        free(output_record_array[g]);
    }
    return 0;
}