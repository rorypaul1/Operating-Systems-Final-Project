// C program for Merge Sort
#include <stdio.h>
#include <stdlib.h>

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
void mergeSort(int arr[], int l, int r)
{
    if (l < r) {
        int m = l + (r - l) / 2;

        // Sort first and second halves
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);

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
// Function to print an array
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
// Driver code
int main()
{
    char record_array[10][100];
    int key_array[10];
    read_file(record_array);
    int rows = 10;
    int columns = 100;
    //int arr[] = { 12, 11, 13, 5, 6, 7 };
    //int arr_size = sizeof(arr) / sizeof(arr[0]);

    printf("Given data is \n");
    printArray(record_array, rows, columns);

    createKeyArray(key_array,record_array);
    
    mergeSort(key_array, 0, rows - 1);

    printf("\nSorted array is \n");
    print1DArray(key_array, rows);
    return 0;
}