
typedef struct line_ranges line_ranges_t;

// structure to store the line ranges in a safe array plus a flag 
struct line_ranges {
    int flag;
    int *ranges;
    int num_ranges;
};

// bool like in is returned show whether the line is any of the line ranges
int in_range(line_ranges_t *line_ranges, int line);

// prints an array
void print_arr(int *a, int len);

// function to implement -s/-show flag. Prints line with # replacing trailing white spaces
void show_tws(FILE *f, int char_count, int white_spaces);