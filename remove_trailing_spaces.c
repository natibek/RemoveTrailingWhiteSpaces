#include <stdlib.h>  
#include <stdio.h>
#include <string.h>
#include "remove_trailing_spaces.h"

void print_arr(int *a, int len)
{
    fprintf(stderr, "[ ");
    for (int i = 0; i < len; i++)
        fprintf(stderr, "%d ", a[i]);
    fprintf(stderr, "]\n");
}

int in_range(line_ranges_t *line_ranges, int line)  
{
    // print_arr(line_ranges->ranges, line_ranges->num_ranges);
    for (int i = 0; i < line_ranges->num_ranges; i++){
        if (line <= line_ranges->ranges[i+1] && line >= line_ranges->ranges[i])
            return 1;
    }
    return 0;
}

int main(int argc, char *argv[])
{
    if (argc == 1){
        fprintf(stderr, "No file chosen\n");
        exit(1);
    }

    line_ranges_t line_ranges;
    line_ranges.flag = 0;

    int verbose = 0;

    if (argc > 2){
        for (int i = 2; i < argc; i++){
            // fprintf(stderr,"Current arg = %s\n", argv[i]);

            if (!strcmp(argv[i],"-lines") || !strcmp(argv[i],"-l")){
                if (line_ranges.flag){
                    fprintf(stderr, "Can't have the -lines tag multiple times\n");
                    exit(1);
                }
                line_ranges.flag = 1;

                int start_ind = i + 1;
                int num_ranges = 0;
                
                for (int j = start_ind; j < argc && argv[j][0] != '-'; j++, i++, num_ranges++){
                    if (!atoi(argv[j])){
                        fprintf(stderr, "Invalid line number %s\n", argv[j]);
                        exit(1);
                    }
                }

                line_ranges.ranges = malloc(sizeof(int)*num_ranges);
                for (int j = 0; j < num_ranges; j++){
                    line_ranges.ranges[j] = atoi(argv[j + start_ind]);
                }
                line_ranges.num_ranges = num_ranges;

                if (num_ranges % 2){
                    fprintf(stderr, "There should be an even number of ranges: found %d\n", num_ranges);
                    print_arr(line_ranges.ranges, line_ranges.num_ranges);
                    exit(1);
                }
            } else if (!strcmp(argv[i],"-verbose") || !strcmp(argv[i],"-v")) verbose = 1;
        }
    }

    FILE *ft;
    const char *file_name = argv[1];

    // fprintf(stderr, "File Name: %s\n", file_name);
    ft = fopen(file_name, "r");

    if (ft == NULL){
        fprintf(stderr, "Cannot open file: %s\n", file_name);
        exit(1);
    }

    int file_name_len = strlen(file_name);
    char new_file[file_name_len + 5];
    char *ext = "_ntws";

    //fprintf(stderr, "%d\n", file_name_len);

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

    FILE *new_ft = fopen(new_file, "w");

    int ch, line_num = 1, white_spaces = 0, total = 0;

    while ((ch = fgetc(ft)) != EOF){
        if (ch == 32) {
            white_spaces++;
            //fprintf(stderr, "&");
        } else if (ch == '\n') {
            //fprintf(stderr, "%c", ch);


            if (line_ranges.flag){
                if (in_range(&line_ranges, line_num)){
                    fputc(ch, new_ft);
                    total += !(!white_spaces);

                    if (white_spaces && verbose) fprintf(stdout, "\tLine Number: %5d | Number of Trailing White Spaces: %2d\n", line_num, white_spaces);
                } else {
                    for (int i=0; i < white_spaces; i++)
                        fputc(32, new_ft);
                }
            } else {
                fputc(ch, new_ft);
                total += !(!white_spaces);
                if (white_spaces && verbose) fprintf(stdout, "\tLine Number: %5d | Number of Trailing White Spaces: %2d\n", line_num, white_spaces);
            }

            white_spaces = 0;
            line_num ++;
        } else {
            //fprintf(stderr, "%c", ch);
            for (int i=0; i < white_spaces; i++)
                fputc(32, new_ft);
            fputc(ch, new_ft);
            white_spaces = 0;
        }
    }
    if (total == 0) remove(new_file);
    fprintf(stdout, "!! Total Number of Lines with trailing white spaces: %d !!\n", total);
    fclose(ft);
    fclose(new_ft);
    return 0;
}