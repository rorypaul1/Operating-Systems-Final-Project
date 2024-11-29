#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main() {
    FILE * file;

    // Creating a file
    file = fopen("data_to_sort.bin", "w");
    
    // Checking whether file is 
    // created or not
    if (file == NULL) {
    printf("Error in creating file");
    return 1;
    }
    printf("File created.\n");

    //Write to the file
    int num_records = 999999;
    int i;
    int record_size = 100;
    int key;

    //a char is 1 byte so 100 byte record = 100 bytes
    //key is 4 chars
    for(i = 0;i < num_records; ++i){
        //Initialize record to all zeros so we can overwrite with key and data after
        char record_to_write[record_size];
        memset(record_to_write, 0, record_size);

        //Randomly generate key
        key = rand() % num_records;
        sprintf(record_to_write,"%i", key);
        printf("Key from data creation: %d\n",key);
        //Write to file
        fwrite(record_to_write, record_size, 1, file);

    }
    fclose(file);

  return 0;
}