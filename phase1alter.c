#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdlib.h>

int does_exist(const char * path){
    if (access(path, F_OK) == -1)
        return 0;
    else
        return 1;
}

char * get_path(){
    char * path = (char *)calloc(100000, sizeof(char));
    char c;
    scanf("%c", &c);

    if (c == 34){
        for(int i = 0; ; i++){
            scanf("%c", &c);
            if (c != 34)
                path[i] = c;
            else
                break;
        }
    }

    else{
        path[0] = c;
        for(int i = 1; ;i++){
            scanf("%c", &c);
            if (c != 32 && c != '\n')
                path[i] = c;
            else
                break;
        }
    }
    return path;
}

char * get_string(){
    char * string = (char *)calloc(100000, sizeof(char));
    char c;
    scanf("%c", &c);

    if (c == 34){
        for(int i = 0; ; i++){
            scanf("%c", &c);
            if (c != 34)
                string[i] = c;
            else if(c == 34 && string[i-1] == 92)
                string[i] = c;
            else
                break;
        }
    }

    else{
        string[0] = c;
        for(int i = 1; ;i++){
            scanf("%c", &c);
            if (c != 32)
                string[i] = c;
            else
                break;
        }
    }
    return string;
}

char * dashstr(){
    char dashstr[10];
    char space;
    scanf("%s%c", dashstr, &space);
    return dashstr;
}

char * dashfile(){
    char dashfile[10];
    char space;
    scanf("%s%c", dashfile, &space);
    return dashfile;
}

int isDir(const char* fileName){

    struct stat path;
    stat(fileName, &path);

    return S_ISREG(path.st_mode);
}

int match(char *first, char *second) {
    if (*first == '\0' && *second == '\0')
        return 1;

    if (*first == '*') {
        while (*(first + 1) == '*') {
            first++;
        }
    }

    if (*first == '*' && *(first + 1) != '\0' && *second == '\0')
        return 0;

    if (*first == *second)
        return match(first + 1, second + 1);

    if (*first == '*')
        return match(first + 1, second) || match(first, second + 1);

    return 0;
}

int createfile(char * path){

    char * path2 = (char *)calloc(100000, sizeof(char));

    if (path[0] == 34){
        for (int i = 0; i<300; i++){
            if (path[i+1] != 34)
                path[i] = path[i+1];
            else{
                path[i] = NULL;
                path[i+1] = NULL;
                break;
            }
        }
    }

    if (does_exist(path) == 1){
        printf("file already exists\n");
        return 0;
    }

    char * token = {NULL};
    token = strtok(path, "/");

    while(token != NULL){
        strcat(path2, token);

        token = strtok(NULL, "/");
        if (token != NULL){

            if (does_exist(path2) == 0)
                mkdir(path2);

            strcat(path2, "/");
        }

        else{
            FILE * filepointer = fopen(path2, "w");
            fclose(filepointer);
            return 1;
        }
    }
}

void undo_saver(char * path){

    char * path2 = (char *)calloc(100000, sizeof(char));

    strcpy(path2, "undo/");
    strcat(path2, path);

    if(does_exist(path2) == 0)
        createfile(path2);

    FILE * open_undo = fopen(path2, "w");

    FILE * open_source = fopen(path, "r");

    char copier = fgetc(open_source);

    while(copier != EOF){
        fputc(copier, open_undo);
        copier = fgetc(open_source);
    }

    fclose(open_source);
    fclose(open_undo);
}

void create_copy(){
    char * path = (char *) calloc(1000, sizeof(char));
    strcpy(path, "copy/copy.txt");
    if(does_exist(path) == 0)
        createfile(path);
}

void cat(){
    dashfile();
    char * path = get_path();
    if(does_exist(path) == 0){
        printf("file does not exist\n");
        return;
    }

    FILE * filepointer = fopen(path, "r");

    char character = fgetc(filepointer);

    while (character != EOF){
        printf ("%c", character);
        character = fgetc(filepointer);
    }
    printf("\n");
}

void insert(char * path, char * to_insert, int line_num, int start_pos){
    undo_saver(path);

    char * copy = (char *)calloc(100000, sizeof(char));

    FILE * filepointer1 = fopen(path, "r");

    char character;

    for(int i = 0; ; i++){
        character = fgetc(filepointer1);
        copy[i] = character;
        if(character == EOF)
            break;
    }

    fclose(filepointer1);

    FILE * filepointer2 = fopen(path, "w");

    int n = -1, line = 1;

    for(int i = 0; line != line_num; i++){
        n = i;
        if (copy[i] == 10)
            line++;
    }

    for(int i = 0; i <= n+start_pos; i++){
        fputc(copy[i], filepointer2);
    }

    for(int i = 0; i < strlen(to_insert); i++){

        if(to_insert[i] == 'n' && to_insert[i-1] == 92){
            if(to_insert[i-2] != 92)
                continue;
        }
        else if(to_insert[i] == 92 && to_insert[i-1] == 92)
            continue;

        else if (to_insert[i] == 34 && to_insert[i-1] == 92)
            continue;

        if(to_insert[i] == 92){
            if(to_insert[i+1] == 92)
                fprintf(filepointer2, "\\");

            else if(to_insert[i+1] == 'n')
                fprintf(filepointer2, "\n");

            else if(to_insert[i+1] == 34)
                fprintf(filepointer2, "\"");

            else
                fputc(to_insert[i], filepointer2);
        }

        else
            fputc(to_insert[i], filepointer2);
    }

    for(int i = 0; i < strlen(copy); i++){
        if (copy[i+n+start_pos+1] == EOF)
            break;
        fputc(copy[i+n+start_pos+1], filepointer2);
    }
    fclose(filepointer2);
}

void insert_args() {
    dashfile();
    char * path = get_path();
    dashstr();
    char * to_insert = get_string();
    char dashpos[10];
    scanf("%s", dashpos);
    int line_num, start_pos;
    scanf("%d:%d", &line_num, &start_pos);
    if(does_exist(path) == 0){
        printf("file does not exist\n");
        return;
    }
    insert(path, to_insert, line_num, start_pos);
    printf("done\n");
}

void removestr(char * path, int line_num, int start_pos, int size, char * direction){
    undo_saver(path);

    char * copy = (char *)malloc(100000 * sizeof(char));

    FILE * filepointer1 = fopen(path, "r");

    char character;

    for(int i = 0; ; i++){
        character = fgetc(filepointer1);
        copy[i] = character;
        if(character == EOF)
            break;
    }

    fclose(filepointer1);

    FILE * filepointer2 = fopen(path, "w");

    if(strcmp(direction, "-f") == 0){
        int n = -1, line = 1;
        for(int i = 0; ; i++){
            if(line == line_num){
                n = i;
                break;
            }

            if(copy[i] == '\n')
                line++;
    }

        for(int i = 0; i < n+start_pos; i++)
            fputc(copy[i], filepointer2);

        for(int i = 0; ; i++){
            if (copy[i+n+start_pos+size] == EOF)
                break;
            fputc(copy[i+n+start_pos+size], filepointer2);
        }
    }

    else{
        int line = 1, n = 0;

        for(int i = 0; ; i++){
            if(line == line_num){
                n = i;
                break;
            }
            if(copy[i] == 10)
                line++;
        }

        for(int i = 0; i < n+start_pos-size; i++)
            fputc(copy[i], filepointer2);

        for(int i = 0; ; i++){
            if(copy[i+n+start_pos] == EOF)
                break;
            fputc(copy[i+n+start_pos], filepointer2);
        }
    }

    fclose(filepointer2);
}

void remove_args() {
    dashfile();
    char * path = get_path();

    char dashpos[10];
    scanf("%s", dashpos);

    int line_num, start_pos;
    scanf("%d:%d", &line_num, &start_pos);

    char dashsize[10];
    scanf("%s", dashsize);

    int size;
    scanf("%d", &size);

    char dashdir[5];
    scanf("%s", dashdir);

    if(does_exist(path) == 0){
        printf("file does not exist\n");
        return;
    }
    removestr(path, line_num, start_pos, size, dashdir);
    printf("done\n");
}

void copystr(char * path, int line_num, int start_pos, int size, char * direction){
    create_copy();
    FILE * filepointer1 = fopen(path, "r");
    FILE * filepointer2 = fopen("copy/copy.txt", "w");

    int line = 1;
    while(line != line_num){
        int c = fgetc(filepointer1);
        if (c == 10)
            line++;
    }

    if (strcmp(direction, "-f") == 0){

        for(int i = 0; i<start_pos; i++){
            int pos_count = fgetc(filepointer1);
        }

        for (int i = 0; i<size; i++)
            fputc(fgetc(filepointer1), filepointer2);
    }

    else{
        for(int i = 0; i<start_pos; i++){
            int pos_count = fgetc(filepointer1);
        }

        int position = ftell(filepointer1) - (line -1);
        rewind(filepointer1);

        for(int i = 0; i < position - size; i++){
            int pos_count = fgetc(filepointer1);
        }

        for(int i = 0; i < size; i++)
            fputc(fgetc(filepointer1), filepointer2);
    }
    fclose(filepointer1);
    fclose(filepointer2);
}

void copy_args() {
    dashfile();
    char * path = get_path();

    char dashpos[10];
    scanf("%s", dashpos);

    int line_num, start_pos;
    scanf("%d:%d", &line_num, &start_pos);

    char dashsize[10];
    scanf("%s", dashsize);

    int size;
    scanf("%d", &size);

    char dashdir[5];
    scanf("%s", dashdir);

    if(does_exist(path) == 0){
        printf("file does not exist\n");
        return;
    }
    copystr(path, line_num, start_pos, size, dashdir);
    printf("copied\n");
}

void pastestr(char * path, int line_num, int start_pos){
    FILE * filepointer = fopen("copy/copy.txt", "r");

    char * string = (char *)calloc(1000000, sizeof(char));
    char get;

    for(int i = 0; ; i++){
        get = fgetc(filepointer);
        if(get == EOF)
            break;
        string[i] = get;
    }
    fclose(filepointer);

    insert(path, string, line_num, start_pos);
}

void paste_args() {
    dashfile();
    char * path = get_path();

    char dashpos[10];
    scanf("%s", dashpos);

    int line_num, start_pos;
    scanf("%d:%d", &line_num, &start_pos);

    if(does_exist(path) == 0){
        printf("file does not exist\n");
        return;
    }
    pastestr(path, line_num, start_pos);
    printf("done\n");
}

void cutstr(char * path, int line_num, int start_pos, int size, char * direction){
    copystr(path, line_num, start_pos, size, direction);
    removestr(path, line_num, start_pos, size, direction);
}

void cut_args() {
    dashfile();
    char * path = get_path();

    char dashpos[10];
    scanf("%s", dashpos);

    int line_num, start_pos;
    scanf("%d:%d", &line_num, &start_pos);

    char dashsize[10];
    scanf("%s", dashsize);

    int size;
    scanf("%d", &size);

    char dashdir[5];
    scanf("%s", dashdir);

    if(does_exist(path) == 0){
        printf("file does not exist\n");
        return;
    }
    cutstr(path, line_num, start_pos, size, dashdir);
    printf("done\n");
}

char * handle_star(char * given_str) {
    int length = strlen(given_str);

    for(int i = 1; i<length; i++){
        if (given_str[i-1] == 92 && given_str[i] == 42){
            given_str[i-1] = '*';
            for(int j = i; j<length-1; j++){
                given_str[j] = given_str[j+1];
            }
            given_str[length-1] = NULL;
        }
    }
    return given_str;
}

int find_first(char * path, char * given_str) {
    FILE * filepointer = fopen(path, "r");
    char * save = (char *)calloc(100000, sizeof(char));
    int found = 0;

    do {
        fscanf(filepointer, "%s", save);

        if(strcmp(save, given_str) == 0){
            return (ftell(filepointer) - strlen(save));
            found = 1;
            break;
        }
    } while(fgetc(filepointer) != EOF);

    if(found == 0)
        return -1;
    fclose(filepointer);
    return 0;
}

int find_byword(char * path, char * given_str) {
    FILE * filepointer = fopen(path, "r");
    char * save = (char *)calloc(100000, sizeof(char));
    int * word_num = (int *)calloc(10000, sizeof(int));
    int word_count = 0;
    int found = 0;

    do {
        fscanf(filepointer, "%s", save);
        word_count++;

        if(strcmp(save, given_str) == 0){
            found = 1;
            return word_count;
            break;
        }
    } while(fgetc(filepointer) != EOF);

    if (found == 0)
        return -1;

    fclose(filepointer);
    return 0;
}

int * find_all(char * path, char * given_str) {
    FILE * filepointer = fopen(path, "r");
    char * save = (char *)calloc(1000, sizeof(char));
    int * pointer = (int *)calloc(1000, sizeof(int));
    int found = 0;

    do {
        fscanf(filepointer, "%s", save);

        if(strcmp(save, given_str) == 0){
            pointer[found] = ftell(filepointer) - strlen(save);
            found++;
        }
    } while(fgetc(filepointer) != EOF);

    if(found == 0)
        pointer[found] = -1;

    fclose(filepointer);
    return pointer;
}

int find_count(char * path, char * given_str) {
    FILE * filepointer = fopen(path, "r");
    char * save = (char *)calloc(1000, sizeof(char));
    int found = 0;

    do {
        fscanf(filepointer, "%s", save);
        if(strcmp(save, given_str) == 0)
            found++;
    } while(fgetc(filepointer) != EOF);

    fclose(filepointer);
    return found;
}

int find_at(char * path, char * given_str, int at_num) {
    FILE * filepointer = fopen(path, "r");
    char * save = (char *)calloc(1000, sizeof(char));
    int found = 0;

    do {
        fscanf(filepointer, "%s", save);
        if(strcmp(save, given_str) == 0){
            found++;
            if(found == at_num)
                return ftell(filepointer) - strlen(save);
        }
    } while(fgetc(filepointer) != EOF);

    if(found < at_num){
        fclose(filepointer);
        return -1;
    }
}

int * find_all_byword(char * path, char * given_str) {
    FILE * filepointer = fopen(path, "r");
    char * save = (char *)calloc(100000, sizeof(char));
    int * word_num = (int *)calloc(10000, sizeof(int));
    int word_count = 0;
    int found = 0;

    do {
        fscanf(filepointer, "%s", save);
        word_count++;

        if(strcmp(save, given_str) == 0){
            word_num[found] = word_count;
            found++;
        }
    } while(fgetc(filepointer) != EOF);

    if(found == 0)
        word_num[found] = -1;

    fclose(filepointer);
    return word_num;
}

int find_byword_at (char * path, char * given_str, int at_num) {
    FILE * filepointer = fopen(path, "r");
    char * save = (char *)calloc(1000, sizeof(char));
    int found = 0;
    int word_count = 0;

    do {
        fscanf(filepointer, "%s", save);
        word_count++;

        if(strcmp(save, given_str) == 0){
            found++;
            if(found == at_num){
                fclose(filepointer);
                return word_count;
            }
        }
    } while(fgetc(filepointer) != EOF);

    if(found < at_num) {
        fclose(filepointer);
        return -1;
    }
}

int find_first_wildcard(char * path, char * string) {
    FILE * filepointer = fopen(path, "r");
    char * save = (char *)calloc(1000, sizeof(char));
    int found = 0;

    do {
        fscanf(filepointer, "%s", save);

        if(match(string, save) == 1){
            int result = ftell(filepointer) - strlen(save);
            fclose(filepointer);
            found = 1;
            return result;
        }
    } while(fgetc(filepointer) != EOF);

    if(found == 0){
        fclose(filepointer);
        return -1;
    }
}

int find_byword_wildcard(char * path, char * string) {
    FILE * filepointer = fopen(path, "r");
    char * save = (char *)calloc(100000, sizeof(char));
    int * word_num = (int *)calloc(10000, sizeof(int));
    int word_count = 0;
    int found = 0;

    do {
        fscanf(filepointer, "%s", save);
        word_count++;

        if(match(string, save) == 1){
            found = 1;
            fclose(filepointer);
            return word_count;
        }
    } while(fgetc(filepointer) != EOF);

    if(found == 0){
        fclose(filepointer);
        return -1;
    }
}

int * find_all_wildcard(char * path, char * string) {
    FILE * filepointer = fopen(path, "r");
    char * save = (char *)calloc(1000, sizeof(char));
    int * pointer = (int *)calloc(1000, sizeof(int));
    int found = 0;

     do {
        fscanf(filepointer, "%s", save);

        if(match(string, save) == 1){
            pointer[found] = ftell(filepointer) - strlen(save);
            found++;
        }
    } while(fgetc(filepointer) != EOF);

    if(found == 0)
        pointer[found] = -1;

    fclose(filepointer);
    return pointer;
}

int find_count_wildcard(char * path, char * string) {
    FILE * filepointer = fopen(path, "r");
    char * save = (char *)calloc(1000, sizeof(char));
    int found = 0;

    do {
        fscanf(filepointer, "%s", save);

        if(match(string, save) == 1)
            found++;
    } while(fgetc(filepointer) != EOF);

    fclose(filepointer);
    return found;
}

int find_at_wildcard(char * path, char * string, int at_num) {
    FILE * filepointer = fopen(path, "r");
    char * save = (char *)calloc(1000, sizeof(char));
    int found = 0;

    do {
        fscanf(filepointer, "%s", save);

        if(match(string, save) == 1)
            found++;

        if(found == at_num){
            int to_be_returned = ftell(filepointer) - strlen(save);
            fclose(filepointer);
            return to_be_returned;
        }
    } while(fgetc(filepointer) != EOF);

    if(found < at_num) {
        fclose(filepointer);
        return -1;
    }
}

int * find_all_byword_wildcard(char * path, char * string) {
    FILE * filepointer = fopen(path, "r");
    char * save = (char *)calloc(100000, sizeof(char));
    int * word_num = (int *)calloc(10000, sizeof(int));
    int word_count = 0;
    int found = 0;

    do {
        fscanf(filepointer, "%s", save);
        word_count++;

        if(match(string, save) == 1){
            word_num[found] = word_count;
            found++;
        }
    } while(fgetc(filepointer) != EOF);

    if(found == 0)
        word_num[found] = -1;

    fclose(filepointer);
    return word_num;
}

int find_byword_at_wildcard (char * path, char * string, int at_num) {
    FILE * filepointer = fopen(path, "r");
    char * save = (char *)calloc(1000, sizeof(char));
    int found = 0;
    int word_count = 0;

    do {
        fscanf(filepointer, "%s", save);
        word_count++;
        if(match(string, save) == 1)
            found++;

        if(found == at_num){
            fclose(filepointer);
            return word_count;
        }
    } while(fgetc(filepointer) != EOF);

    if(found < at_num){
        fclose(filepointer);
        return -1;
    }
}

int find_wildcard(char string[], char path[], int num){

    if(num == 0)
        printf("%d\n", find_first_wildcard(path, string));

    else if(num == 1)
        printf("%d\n", find_byword_wildcard(path, string));

    else if(num == 2){
        int * array;
        array = find_all_wildcard(path, string);

        if(array[0] == -1)
            printf("-1\n");
        else{
            int size = find_count_wildcard(path, string);
            for(int i = 0; i < size; i++){
                printf("%d ", array[i]);
            }
            printf("\n");
        }
    }

    else if(num == 3)
        printf("%d\n", find_count_wildcard(path, string));

    else if(num == 4){
        int at_num;
        scanf("%d", &at_num);
        printf("%d\n", find_at_wildcard(path, string, at_num));
    }

    else if(num == 5){
        int * array;
        array = find_all_byword_wildcard(path, string);

        if(array[0] == -1)
            printf("-1\n");
        else{
            int size = find_count_wildcard(path, string);
            for(int i = 0; i < size; i++){
                printf("%d ", array[i]);
            }
            printf("\n");
        }
    }

    else if(num == 6){
        int at_num;
        scanf("%d", &at_num);
        printf("%d\n", find_byword_at_wildcard(path, string, at_num));
    }
}

void find(char given_str[], char * path, int num){

    for(int i = 1; i<strlen(given_str); i++){
        if((given_str[i] == 42 && given_str[i-1] != 92) || given_str[0] == 42) {
            find_wildcard(given_str, path, num);
            return;
        }
    }

    if(strstr(given_str, "\*"))
        given_str = handle_star(given_str);

    if (num == 0)
        printf("%d\n", find_first(path, given_str));

    else if(num == 1)
        printf("%d\n", find_byword(path, given_str));

    else if (num == 2){
        int* array;
        array = find_all(path, given_str);
        if(array[0] == -1)
            printf("-1\n");
        else{
            int size = find_count(path, given_str);
            for(int i = 0; i < size; i++){
                printf("%d ", array[i]);
            }
            printf("\n");
        }
    }

    else if (num == 3)
        printf("%d\n", find_count(path, given_str));

    else if (num == 4){
        int at_num;
        scanf("%d", &at_num);
        printf("%d\n", find_at(path, given_str, at_num));
    }

    else if(num == 5){
        int * array;
        array = find_all_byword(path, given_str);

        if(array[0] == -1)
            printf("-1\n");
        else{
            int size = find_count(path, given_str);
            for(int i = 0; i < size; i++){
                printf("%d ", array[i]);
            }
            printf("\n");
        }
    }

    else if (num == 6){
        int at_num;
        scanf("%d", &at_num);
        printf("%d\n", find_byword_at(path, given_str, at_num));
    }
}

void find_args() {
    char * dash_str = dashstr();
    char * given_str = get_string();
    char * dash_file = dashfile();
    char * path = get_path();
    if(does_exist(path) == 0) {
        printf("file does not exist\n");
        return;
    }

    char function[15];
    scanf("%s", function);
    int num;

    if(strcmp(function, "-f") == 0) num = 0;
    else if(strcmp(function, "-byword") == 0) num = 1;
    else if(strcmp(function, "-all") == 0) num = 2;
    else if(strcmp(function, "-count") == 0) num = 3;
    else if(strcmp(function, "-at") == 0) num = 4;
    else if(strcmp(function, "-all-byword") == 0 || strcmp(function, "-byword-all") == 0) num = 5;
    else if(strcmp(function, "-byword-at") == 0) num = 6;
    else{
        printf("invalid argument\n");
        return;
    }
    find(given_str, path, num);
}

int replace_first(char * path, char * string1, char * string2, char * copy) {
    int position = find_first(path, string1);

    if(position == -1)
        return 0;

    FILE * filepointer = fopen(path, "w");

    int i = 0;
    while (ftell(filepointer) != position) {
        fputc(copy[i], filepointer);
        i++;
    }

    fprintf(filepointer, "%s", string2);

    for(int j = i + strlen(string1); ;j++){
        if(copy[j] == EOF)
            break;
        fputc(copy[j], filepointer);
    }

    fclose(filepointer);
    return 1;
}

void replace_at(char * path, char * string1, char * string2, char * copy, int at_num) {
    int position = find_at(path, string1, at_num);

    if(position == -1){
        printf("str1 not found\n");
        return;
    }

    FILE * filepointer = fopen(path, "w");

    int i = 0;
    while (ftell(filepointer) != position) {
        fputc(copy[i], filepointer);
        i++;
    }

    fprintf(filepointer, "%s", string2);

    for(int j = i + strlen(string1); ;j++){
        if(copy[j] == EOF)
            break;
        fputc(copy[j], filepointer);
    }

    fclose(filepointer);
    printf("done\n");
}

int replace_first_wildcard (char * path, char * string1, char * string2, char * copy) {
    FILE * filepointer = fopen(path, "r");
    char * save = (char *)calloc(1000, sizeof(char));
    int found = 0;
    int position, size;

    do {
        fscanf(filepointer, "%s", save);

        if(match(string1, save) == 1){
            position = (ftell(filepointer) - strlen(save));
            size = strlen(save);
            found = 1;
            break;
        }
    } while(fgetc(filepointer) != EOF);
    fclose(filepointer);

    if(found == 0)
        return 0;

    FILE * filepointer2 = fopen(path, "w");

    int i = 0;
    while (ftell(filepointer2) != position) {
        fputc(copy[i], filepointer2);
        i++;
    }

    fprintf(filepointer2, "%s", string2);

    for(int j = i + size; ;j++){
        if(copy[j] == EOF)
            break;
        fputc(copy[j], filepointer2);
    }
    fclose(filepointer2);
    return 1;
}

void replace_at_wildcard (char * path, char * string1, char * string2, char * copy, int at_num) {
    FILE * filepointer = fopen(path, "r");
    char * save = (char *)calloc(1000, sizeof(char));
    int found = 0;
    int position, size;

    do {
        fscanf(filepointer, "%s", save);

        if(match(string1, save) == 1)
            found++;

        if(found == at_num){
            position = ftell(filepointer) - strlen(save);
            size = strlen(save);
            break;
        }
    } while(fgetc(filepointer) != EOF);
    fclose(filepointer);

    if(found < at_num){
        printf("str1 not found\n");
        return;
    }

    FILE * filepointer2 = fopen(path, "w");

    int i = 0;
    while (ftell(filepointer2) != position) {
        fputc(copy[i], filepointer2);
        i++;
    }

    fprintf(filepointer2, "%s", string2);

    for(int j = i + size; ;j++){
        if(copy[j] == EOF)
            break;
        fputc(copy[j], filepointer2);
    }
    fclose(filepointer2);
    printf("done\n");
}

char * get_copy(char * path) {
    FILE * filepointer = fopen(path, "r");
    char * copy = (char *)calloc(100000, sizeof(char));
    for(int i = 0; ; i++){
        copy[i] = fgetc(filepointer);
        if(copy[i] == EOF)
            break;
    }
    fclose(filepointer);
    return copy;
}

void replace_wildcard(char string1[], char string2[], char path[], int num){
    char * copy = (char *)calloc(100000, sizeof(char));
    copy = get_copy(path);

    if(num == 0){
        int result = replace_first_wildcard(path, string1, string2, copy);
        if(result == 1)
            printf("done\n");
        else
            printf("str1 not found\n");
    }

    else if(num == 1){
        int at_num;
        scanf("%d", &at_num);
        replace_at_wildcard(path, string1, string2, copy, at_num);
    }

    else if (num == 2) {
        int count = find_count_wildcard(path, string1);
        for(int i = 1; i <= count; i++) {
            replace_first_wildcard(path, string1, string2, copy);
            copy = get_copy(path);
        }
        if(count > 0)
            printf("done\n");
    }
}

void replace(char string1[], char string2[], char * path, int num){
    undo_saver(path);

    for(int i = 1; i<strlen(string1); i++){
        if((string1[i] == 42 && string1[i-1] != 92) || string1[0] == 42){
            replace_wildcard(string1, string2, path, num);
            return;
        }
    }

    char * copy = (char *) calloc(100000, sizeof(char));
    copy = get_copy(path);

    if(num == 0) {
        int result = replace_first(path, string1, string2, copy);
        if(result == 1)
            printf("done\n");
        else
            printf("str1 not found\n");
    }

    else if(num == 1){
        int at_num;
        scanf("%d", &at_num);
        replace_at(path, string1, string2, copy, at_num);
    }

    else if(num == 2){
        int count = find_count(path, string1);
        for(int i = 1; i <= count; i++){
            replace_first(path, string1, string2, copy);
            copy = get_copy(path);
        }
        if(count > 0)
            printf("done\n");
    }
}

void replace_args() {
    char * dash_str1 = dashstr();
    char * str1 = get_string();
    char * dash_str2 = dashstr();
    char * str2 = get_string();
    char * dash_file = dashfile();
    char * path = get_path();
    if(does_exist(path) == 0){
        printf("file does not exist\n");
        return;
    }
    char function[15];
    scanf("%s", function);
    int num;
    if(strcmp(function, "-r") == 0) num = 0;
    else if(strcmp(function, "-at") == 0) num = 1;
    else if(strcmp(function, "-all") == 0) num = 2;
    else {
        printf("invalid argument!\n");
        return;
    }
    replace(str1, str2, path, num);
}

void print_tree(char * basepath, int round, int max){

    if (round == max)
        return;

    char path[1000];

    DIR * directory;
    struct dirent *dir_entry;
    struct stat statbuf;

    directory = opendir(basepath);

    while ((dir_entry = readdir(directory)) != NULL){
        if (strcmp(dir_entry->d_name, ".") == 0 || strcmp(dir_entry->d_name, "..") ==  0)
            continue;
        else{
            if (round > 0){
                for (int i = 0; i<(round-1)*6+2; i++)
                    printf(" ");
                printf("|");
                for (int i = 0; i< 4; i++)
                    printf("-");
            }
            printf("%s\n", dir_entry->d_name);
            int mark = isDir(dir_entry->d_name);
            if (!mark){
                strcpy(path, basepath);
                strcat(path, "/");
                strcat(path, dir_entry->d_name);
                print_tree(path, round+1, max);
            }
        }
    }

    closedir(directory);
}

void tree_args() {
    int num, depth;
    scanf("%d", &num);
    if (num == -1)
        depth = 30;
    else if(num < -1){
        printf("invalid depth\n");
        return;
    }
    else
        depth = num;
    print_tree("root/", 0, depth);
}

void print_grep(char * path, char grep_text[]){
    FILE * filepointer = fopen(path, "r");
    char line[1000] = {""};

    while(1){
        fscanf(filepointer, "%[^\n]s", line);
        char character = fgetc(filepointer);

        if (strstr(line, grep_text) != 0)
            printf("%s: %s\n", path, line);

        if (character == EOF)
            break;
    }
    fclose(filepointer);
}

void print_grep_c(char * path, char grep_text[], int * is_found){
    FILE * filepointer = fopen(path, "r");
    char line[1000] = {""};

    while(1){
        fscanf(filepointer, "%[^\n]s", line);
        char character = fgetc(filepointer);

        if (strstr(line, grep_text) != 0)
            (*is_found)++;

        if (character == EOF)
            break;
    }
    fclose(filepointer);
}

void print_grep_l(char * path, char grep_text[]){
    FILE * filepointer = fopen(path, "r");
    char line[1000] = {""};

    while(1){
        fscanf(filepointer, "%[^\n]s", line);
        char character = fgetc(filepointer);

        if (strstr(line, grep_text) != 0){
            printf("%s\n", path);
            fclose(filepointer);
            break;
        }
        if (character == EOF)
            break;
    }
    fclose(filepointer);
}

char * process_grep_text(char * grep_text){
    for (int i = 0; i<1000; i++){
        if (grep_text[i+1] != 34)
            grep_text[i] = grep_text[i+1];
        else{
            grep_text[i] = NULL;
            grep_text[i+1] = NULL;
            break;
        }
    }
    return grep_text;
}

void grep_c() {
    char dashstr[10];
    scanf("%s", dashstr);

    char * grep_text = (char *) calloc(1000, sizeof(char));
    scanf("%s", grep_text);

    grep_text = process_grep_text(grep_text);

    char dashfile[10];
    scanf("%s", dashfile);

    char address[1000];
    scanf("%[^\n]s", address);

    int is_found = 0;

    char* token = strtok(address, " ");

    while (token != NULL) {
        print_grep_c(token, grep_text, &is_found);
        token = strtok(NULL, " ");
    }

    printf("%d\n", is_found);
}

void grep_l() {
    char dashstr[10];
    scanf("%s", dashstr);

    char * grep_text = (char *) calloc(1000, sizeof(char));
    scanf("%s", grep_text);

    grep_text = process_grep_text(grep_text);

    char dashfile[10];
    scanf("%s", dashfile);

    char address[1000];
    scanf("%[^\n]s", address);

    char* token = strtok(address, " ");

    while (token != NULL) {
        print_grep_l(token, grep_text);
        token = strtok(NULL, " ");
    }
}

void grep(){
    char unknown[7];
    scanf("%s", unknown);

    if (strcmp(unknown, "--str") == 0){

        char * grep_text = (char *) calloc(1000, sizeof(char));
        scanf("%s", grep_text);

        grep_text = process_grep_text(grep_text);

        char dashfile[10];
        scanf("%s ", dashfile);

        char address[1000];
        scanf("%[^\n]s", address);

        char* token = strtok(address, " ");

        while (token != NULL) {
            print_grep(token, grep_text);
            token = strtok(NULL, " ");
        }
    }

    else if(strcmp(unknown, "-c") == 0)
        grep_c();

    else if(strcmp(unknown, "-l") == 0)
        grep_l();
}

void compare(char * path1, char * path2){
    FILE * filepointer1 = fopen(path1, "r");
    FILE * filepointer2 = fopen(path2, "r");

    char * line1 = (char *)calloc(100000, sizeof(char));
    char * line2 = (char *)calloc(100000, sizeof(char));

    int line = 1;

    while(1){

        fscanf(filepointer1, "%[^\n]s", line1);
        fscanf(filepointer2, "%[^\n]s", line2);

        char character1 = fgetc(filepointer1);
        char character2 = fgetc(filepointer2);

        if(character1 == EOF && character2 == EOF){

            if (strcmp(line1, line2) != 0){
                printf("============ #%d ============\n", line);
                printf("%s\n", line1);
                printf("%s\n", line2);
        }
            break;
        }

        if(character1 == EOF && character2 != EOF){

            if (strcmp(line1, line2) != 0){
                printf("============ #%d ============\n", line);
                printf("%s\n", line1);
                printf("%s\n", line2);
            }

            int start_line = line+1;
            FILE * filepointer3 = fopen(path2, "r");
            char c;
            int end_line = 0;
            while(c != EOF){
                c = fgetc(filepointer3);
                if (c == '\n')
                    end_line++;
            }
            fclose(filepointer3);

            printf(">>>>>>>>>>>> #%d - #%d >>>>>>>>>>>>\n", start_line, end_line+1);

            while (character2 != EOF){
                character2 = fgetc(filepointer2);
                printf ("%c", character2);
            }

            printf("\n");
            break;
        }

        if(character1 != EOF && character2 == EOF){

            if (strcmp(line1, line2) != 0){
                printf("============ #%d ============\n", line);
                printf("%s\n", line1);
                printf("%s\n", line2);
            }

            int start_line = line+1;
            FILE * filepointer3 = fopen(path1, "r");
            char c;
            int end_line = 0;
            while(c != EOF){
                c = fgetc(filepointer3);
                if (c == '\n')
                    end_line++;
            }
            fclose(filepointer3);

            printf("<<<<<<<<<<<< #%d - #%d <<<<<<<<<<<<\n", start_line, end_line+1);

            while (character1 != EOF){
                character1 = fgetc(filepointer1);
                printf ("%c", character1);
            }

            printf("\n");
            break;
        }

        if (strcmp(line1, line2) == 0){
            line++;
            continue;
        }

        else if (strcmp(line1, line2) != 0){
            printf("============ #%d ============\n", line);
            printf("%s\n", line1);
            printf("%s\n", line2);
        }

        line++;
    }

    fclose(filepointer1);
    fclose(filepointer2);
}

void compare_args() {
    char * path1 = get_path();
    char * path2 = get_path();

    if(does_exist(path1) == 0){
        printf("file1 does not exist\n");
        return;
    }

    if(does_exist(path2) == 0){
        printf("file2 does not exist\n");
        return;
    }
    compare(path1, path2);
}

void indent(char * path){
    undo_saver(path);

    char * copy = (char *)calloc(100000, sizeof(char));
    FILE * filepointer = fopen(path, "r");
    copy[0] = fgetc(filepointer);
    while(copy[0] == 32){
        copy[0] = fgetc(filepointer);
    }

    int i = 1;

    while(1){
        copy[i] = fgetc(filepointer);

        while(copy[i] == 32 && (copy[i-1] == 32 || copy[i-1] == 10)){
            copy[i] = fgetc(filepointer);
        }

        if(copy[i] == EOF)
            break;
        else
            i++;
    }

    fclose(filepointer);

    FILE * filepointer2 = fopen(path, "w");

    int round = 0;

    for(int i = 0; i<strlen(copy) ;i++){

        if(copy[i] == 123){
            fprintf(filepointer2, "%c\n", copy[i]);
            round++;
            for(int i = 0; i<round; i++)
                fprintf(filepointer2, "    ");
        }

        else if((copy[i] != 32 && copy[i] != 10) && copy[i+1] == 123)
            fprintf(filepointer2, "%c ", copy[i]);

        else if(copy[i] == 32 && copy[i-1] == 123)
            continue;

        else if(copy[i] == 125){
            fprintf(filepointer2, "\n");
            round--;
            for(int i = 0; i<round; i++)
                fprintf(filepointer2, "    ");
            fprintf(filepointer2, "%c\n", copy[i]);
        }

        else if(copy[i] != 32 && copy[i+1] == 125)
            fprintf(filepointer2, "%c", copy[i]);

        else if(copy[i] == 32 && copy[i+1] == 125)
            continue;

        else if(copy[i] == EOF)
            break;

        else
            fprintf(filepointer2, "%c", copy[i]);
    }

    fclose(filepointer2);
}

void indent_args() {
    char * path = get_path();
    if(does_exist(path) == 0){
        printf("file does not exist\n");
        return;
    }
    indent(path);
    printf("done\n");
}

void undo(char * path){
    char * path2 = (char *)calloc(100000, sizeof(char));

    strcpy(path2, "undo/");
    strcat(path2, path);

    FILE * open_undo = fopen(path2, "r");
    if (open_undo == NULL)
        printf("null 1");

    FILE * open_source = fopen(path, "w");

    char copier = fgetc(open_undo);

    while(copier != EOF){
        fputc(copier, open_source);
        copier = fgetc(open_undo);
    }

    fclose(open_source);
    fclose(open_undo);
    printf("done\n");
}

int main(){

    char operation[30];
    char space;

    while(1){

        scanf("%s", operation);

        if (strcmp(operation, "createfile") == 0){
            char * dash_file = dashfile();
            char * path = get_path();
            int result = createfile(path);
            if(result == 1)
                printf("file created successfully\n");
        }

        else if (strcmp(operation, "cat") == 0)
            cat();

        else if (strcmp(operation, "insertstr") == 0)
            insert_args();

        else if (strcmp(operation, "removestr") == 0)
            remove_args();

        else if (strcmp(operation, "copystr") == 0)
            copy_args();

        else if (strcmp(operation, "pastestr") == 0)
            paste_args();

        else if (strcmp(operation, "cutstr") == 0)
            cut_args();

        else if (strcmp(operation, "tree") == 0)
            tree_args();

        else if(strcmp (operation, "find") == 0)
            find_args();

        else if(strcmp (operation, "replace") == 0)
            replace_args();

        else if(strcmp (operation, "grep") == 0){
            grep();
        }

        else if(strcmp (operation, "compare") == 0){
            scanf("%c", &space);
            compare_args();
        }

        else if(strcmp (operation, "auto-indent") == 0){
            scanf("%c", &space);
            indent_args();
        }

        else if(strcmp (operation, "undo") == 0){
            char * dash_file = dashfile();
            char * path = get_path();
            undo(path);
        }

        else{
            printf("invalid command\n");
        }
    }

}

