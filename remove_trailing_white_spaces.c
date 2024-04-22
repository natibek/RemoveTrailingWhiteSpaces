#include <stdlib.h>  
#include <stdio.h>
#include <string.h>
#include "remove_trailing_white_spaces.h"  

#define MAX_CHAR_COUNT 100

void print_arr(int *a, int len)
{
    fprintf(stderr, "[ ");
    for (int i = 0; i < len; i++)
        fprintf(stderr, "%d ", a[i]);
    fprintf(stderr, "]\n");
}
//int in_range(line_ranges_t *line_ranges, int line)   

int in_range(line_ranges_t *line_ranges, int line) 
{
    // print_arr(line_ranges->ranges, line_ranges->num_ranges);
    for (int i = 0; i < line_ranges->num_ranges; i++){
        if (line <= line_ranges->ranges[i+1] && line >= line_ranges->ranges[i])
            return 1;
    }
    return 0;
}

void show_tws(FILE *f, int char_count, int white_spaces){  
    int long_line = char_count > MAX_CHAR_COUNT;
    int buffer_len = long_line ? MAX_CHAR_COUNT : char_count;  
    char *buffer = malloc(buffer_len + 1);

    fseek(f, -(buffer_len+1), SEEK_CUR);
    fread(buffer, sizeof(char), buffer_len, f);
    fseek(f, 1, SEEK_CUR);

    int ind = white_spaces >= buffer_len ? 0 : buffer_len - white_spaces;
    for (int i = ind; i < buffer_len; i++)
        buffer[i] = '@';

    if (long_line) buffer[0] = buffer[1] = buffer[2] = '.';

    buffer[buffer_len] = '\0';
    fprintf(stdout, "%s\n", buffer);
    free(buffer);
}

int main(int argc, char *argv[])
{
    if (argc == 1){
        fprintf(stderr, "No file chosen\n");
        exit(1);
    }

    line_ranges_t line_ranges;
    line_ranges.flag = 0;
    int show = 0;
    int verbose = 0;

    if (argc >= 2 && !strcmp(argv[1], "--help")){
        fprintf(stdout, "./rm_tws file_name:\tcreates new file with name file_name_ntws that has no trailing white spaces.\n");
        fprintf(stdout, "./rm_tws --help:    \tdisplays infro about the remove trailing white spaces utility.\n");
        fprintf(stdout, "\nFlags: (need a file target first)\n");
        fprintf(stdout, "\t -l/-lines:  \t followed by an ranges of line numbers to remove trailing white spaces from,\n");
        fprintf(stdout, "\t -v/-verbose:\t will print the line numbers where trainling white spaces were found and how many,\n");
        fprintf(stdout, "\t -s/-show:   \t will not create a new file. Prints out the lines of code/text with trailing white spaces shown as @.\n");   
        exit(1);
    }
    if (argc > 2){
        for (int i = 2; i < argc; i++){
            // fprintf(stderr,"Current arg = %s\n", argv[i]);

            if (!strcmp(argv[i],"--lines") || !strcmp(argv[i],"-l")){
                if (line_ranges.flag){
                    fprintf(stderr, "Can't have the -lines tag multiple times.\n");
                    exit(1);
                }
                line_ranges.flag = 1;

                int start_ind = i + 1;
                int num_ranges = 0;
                
                for (int j = start_ind; j < argc && argv[j][0] != '-'; j++, i++, num_ranges++){
                    if (!atoi(argv[j])){
                        fprintf(stderr, "Invalid line number %s.\n", argv[j]);
                        exit(1);
                    }
                }

                line_ranges.ranges = malloc(sizeof(int)*num_ranges);
                for (int j = 0; j < num_ranges; j++){
                    line_ranges.ranges[j] = atoi(argv[j + start_ind]);
                }
                line_ranges.num_ranges = num_ranges;

                if (num_ranges % 2){
                    fprintf(stderr, "There should be an even number of ranges: found %d.\n", num_ranges);
                    print_arr(line_ranges.ranges, line_ranges.num_ranges);
                    exit(1);
                }
            } 
            else if (!strcmp(argv[i],"--verbose") || !strcmp(argv[i],"-v")) verbose = 1;
            else if (!strcmp(argv[i], "--show") || !strcmp(argv[i], "-s")) show = 1;
        }
    }

    FILE *ft;
    const char *file_name = argv[1];

    // fprintf(stderr, "File Name: %s\n", file_name);
    ft = fopen(file_name, "r");

    if (ft == NULL){
        if (argv[1][0] == '-'){
            fprintf(stderr, "First argument should be file name.\nInvalid flag: %s. Try --help to see available flags.\n", file_name);
            exit(1);
        } else {
            fprintf(stderr, "Cannot open/find file: %s.\n", file_name);
            exit(1);
        }
    }

    int file_name_len = strlen(file_name);
    char new_file[file_name_len + 5];
    char *ext = "_ntws";
    FILE *new_ft;
    //fprintf(stderr, "%d\n", file_name_len);
    if (!show){
        for (int i = 0, ind = 0; i < file_name_len; i++, ind++){
            if (file_name[i] != '.'){
                new_file[ind] = file_name[i];
            } else {
                for (int j = 0; j < 5; j ++, ind++){
                    new_file[ind] = ext[j];
                }
                new_file[ind] = '.';
            }
        }
        new_file[file_name_len + 5] = '\0';
        //fprintf(stderr, "The new name is %s\n", new_file);

        new_ft = fopen(new_file, "w");
    }


    int ch, line_num = 1, white_spaces = 0, total = 0, char_count = 0;
    char *buffer;
    // increase char count until you hit \n
    // don't reset char count when white space is non trailing
    // fseek back, fread into malloc buffer, print, free the buffer, fseek backs
    while ((ch = fgetc(ft)) != EOF){
        if (ch == 32) {
            white_spaces++;
            char_count++;
            //fprintf(stderr, "&");
        } else if (ch == '\n') {
            //fprintf(stderr, "%c", ch);
            if (line_ranges.flag){

                if (in_range(&line_ranges, line_num)){
                    total += !(!white_spaces);
                    if (!show) fputc(ch, new_ft);
                    if (white_spaces && verbose) fprintf(stdout, "\tLine Number: %5d | Number of Trailing White Spaces: %2d\n", line_num, white_spaces);
                    if (white_spaces && show){
                        fprintf(stdout, "\t\t     %5d > ", line_num);
                        show_tws(ft, char_count, white_spaces);
                    }
                } else if (!show){
                    for (int i=0; i < white_spaces; i++)
                        fputc(32, new_ft);
                }
            } else {
                if (!show) fputc(ch, new_ft);
                total += !(!white_spaces);
                if (white_spaces && verbose) fprintf(stdout, "\tLine Number: %5d | Number of Trailing White Spaces: %2d\n", line_num, white_spaces);
                if (white_spaces && show) {
                    fprintf(stdout, "\t\t     %5d > ", line_num);
                    show_tws(ft, char_count, white_spaces);
                }
            }

            white_spaces = 0;
            char_count = 0;
            line_num ++;
        } else {
            //fprintf(stderr, "%c", ch);
            if (!show){
                for (int i=0; i < white_spaces; i++)
                    fputc(32, new_ft);
                fputc(ch, new_ft);
            }
            char_count++;
            white_spaces = 0;
        }
    }

    if (total == 0 && !show) remove(new_file);
    fprintf(stdout, "\n!! Total Number of Lines with Trailing White Spaces: %d !!\n", total);
    fclose(ft);
    
    if (!show) fclose(new_ft);
    return 0;
}