#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void merge_sort(int left, int right);
void merge(int left, int mid, int right);
int compare_string(char* str1, char* str2);
int open_txt_file(const char file_name[]);
int write_to_txt(char *file_name);
void copy_file_to_buffer(FILE* file_ptr);
int is_text_file(const char* file_name);
int map_utf8_to_order(int utf8_code);

typedef struct Buffer
{
    char** data;
    size_t count_line;
}Buffer;

Buffer buffer;

int main(int argc, char* argv[]){

    //chech if argc bigger than 1
    if(argc < 2)
        return -1;

    if(is_text_file(argv[1]) == 0){
        
        open_txt_file(argv[1]);
        merge_sort(0, buffer.count_line - 1);
        write_to_txt(argv[1]);
    }

    for(size_t i = 0; i < buffer.count_line; i++) {
        free(buffer.data[i]);
    }
    free(buffer.data);

    return 0;
}
void merge(int left, int mid, int right){

    int n1 = mid - left + 1;
    int n2 = right - mid;

    char *L[n1], *R[n2];

    for (int x = 0; x < n1; x++) L[x] = buffer.data[left + x];
    for (int y = 0; y < n2; y++) R[y] = buffer.data[mid + 1 + y];

    int i = 0, j = 0, k = left;  //i left index, j right index, k buffer index

    while(i < n1 && j < n2){
        buffer.data[k++] = (compare_string(L[i], R[j]) <= 0) ? L[i++]: R[j++];
    }

    // Copy the remaining elements of L[],
    // if there are any
    while(i < n1){buffer.data[k++] = L[i++];}

    // Copy the remaining elements of R[],
    // if there are any
    while(j < n2){buffer.data[k++] = R[j++];}
}

void merge_sort(int left, int right){
    if(left < right){
        int mid = left + (right - left)/2;
        merge_sort(left, mid);
        merge_sort(mid + 1, right);

        merge(left, mid, right);
    }
}

int compare_string(char* str1, char* str2) {
    int str1_len = strlen(str1);
    int str2_len = strlen(str2);
    int max_index_count = (str1_len < str2_len) ? str1_len : str2_len;

    for (int i = 0; i < max_index_count; i++) {
        int c1 = map_utf8_to_order((unsigned char)str1[i]);
        int c2 = map_utf8_to_order((unsigned char)str2[i]);

        if (c1 < c2) return -1;  // str1 < str2
        if (c1 > c2) return 1;   // str1 > str2
    }

    // If all equal up to min length → shorter one is "smaller"
    if (str1_len < str2_len) return -1;
    if (str1_len > str2_len) return 1;

    return 0; // completely equal
}

int open_txt_file(const char file_name[]){
    FILE *file_ptr;
    file_ptr = fopen(file_name, "r");

    if (file_ptr == NULL) {
        exit(1);
        return -1;
    }
    copy_file_to_buffer(file_ptr);
    fclose(file_ptr);
    return 0;
}

int write_to_txt(char *file_name){
    FILE* file_ptr;
    file_ptr = fopen(file_name, "w");

    if (file_ptr == NULL) {
        exit(1);
        return -1;
    }

    for(int i = 0; i < buffer.count_line; i++){
        fprintf(file_ptr, "%s\n", buffer.data[i]);
    }

    fclose(file_ptr);
}

void copy_file_to_buffer(FILE* file_ptr){
    size_t text_line_count = 0;
    char str[256];

    //to count lines
    while (fgets(str, sizeof(str), file_ptr))
        text_line_count++;

    buffer.data = (char**)malloc(sizeof(char*)*text_line_count);
    buffer.count_line = text_line_count;

    rewind(file_ptr);
    
    //get input line by line
    size_t index = 0;
    while(fgets(str, sizeof(str), file_ptr) && index != text_line_count){
        str[strcspn(str, "\n")] = '\0'; //delete \n caracter
        size_t str_len = strlen(str);
        buffer.data[index] = (char*)malloc(sizeof(char)*str_len + 1);
        strcpy(buffer.data[index], str);
        index++;
    }
}

int is_text_file(const char* file_name){
    const char *dot = strrchr(file_name, '.');
    if (!dot || dot == file_name) return -1;
    if(strcmp(dot, ".txt") == 0)
        return 0;
    else 
        return 1;
}

//A B C Ç D E F G Ğ H I İ J K L M N O Ö P Q R S Ş T U Ü V W X Y Z
int map_utf8_to_order(int utf8_code){
    switch(utf8_code){
        case 'A': case 'a': return 1;
        case 'B': case 'b': return 2;
        case 'C': case 'c': return 3;
        case 0xC7: case 0xE7: return 4;    // Ç / ç
        case 'D': case 'd': return 5;
        case 'E': case 'e': return 6;
        case 'F': case 'f': return 7;
        case 'G': case 'g': return 8;
        case 0x11E: case 0x11F: return 9;  // Ğ / ğ
        case 'H': case 'h': return 10;
        case 'I': case 0x131: return 11;  // I / ı
        case 0x130: case 'i': return 12;  // İ / i
        case 'J': case 'j': return 13;
        case 'K': case 'k': return 14;
        case 'L': case 'l': return 15;
        case 'M': case 'm': return 16;
        case 'N': case 'n': return 17;
        case 'O': case 'o': return 18;
        case 0xD6: case 0xF6: return 19;   // Ö / ö
        case 'P': case 'p': return 20;
        case 'Q': case 'q': return 21;
        case 'R': case 'r': return 22;
        case 'S': case 's': return 23;
        case 0x15E: case 0x15F: return 24; // Ş / ş
        case 'T': case 't': return 25;
        case 'U': case 'u': return 26;
        case 0xDC: case 0xFC: return 27;  // Ü / ü
        case 'V': case 'v': return 28;
        case 'W': case 'w': return 29;
        case 'X': case 'x': return 30;
        case 'Y': case 'y': return 31;
        case 'Z': case 'z': return 32;
        default: return 33; // tanımsız karakter
    }
}