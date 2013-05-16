#include <stdlib.h>
#include <stdio.h>
#define BUF_SIZE 1024

struct info_st {
    int rows;
    int columns;
    int gen;
};

typedef struct info_st inf;

void generate_grid(int ****pointer, inf info)
{
    int ***field, i, j;
    field = (int ***) calloc(info.rows + 2, sizeof(int *));
    *pointer = field;
    for (i = 0; i < info.rows + 2; i++) {
        field[i] = (int **) calloc(info.columns + 2, sizeof(int));
    }

    for (i = 1; i <= info.rows; i++)
        for (j = 1; j <= info.columns; j++)
            field[i][j] = (int *) calloc(1, sizeof(int));
    for (i = 1; i <= info.rows; i++)
        field[i][0] = field[i][info.columns];
    for (i = 1; i <= info.rows; i++)
        field[i][info.columns + 1] = field[i][1];
    for (i = 1; i <= info.columns; i++)
        field[0][i] = field[info.rows][i];
    for (i = 1; i <= info.columns; i++)
        field[info.rows + 1][i] = field[1][i];

    field[0][0] = field[info.rows][info.columns];
    field[info.rows + 1][info.columns + 1] = field[1][1];
    field[info.rows + 1][0] = field[1][info.columns];
    field[0][info.columns + 1] = field[info.rows][1];
}

void get_size_and_gen(FILE * file, inf * info)
{
    char buf[BUF_SIZE], *pEnd;
    int num, arr[3], i = 0, j = 0;
    fgets(buf, BUF_SIZE, file);
    for (pEnd = buf; *pEnd && j < 3; pEnd++, j++) {
        num = strtol(pEnd, &pEnd, 10);
        if (num == 0) {
            puts("Wrong size or generation input");
            exit(0);
        }
        arr[i++] = num;
    }
    info->rows = arr[0];
    info->columns = arr[1];
    info->gen = arr[2];
}

int born_or_die(int ****pointer, int i, int j)
{
    int neighbors, ***field = *pointer;
    neighbors =
        *field[i + 1][j] + *field[i + 1][j + 1] + *field[i][j + 1] +
        *field[i - 1][j - 1] + *field[i - 1][j] + *field[i - 1][j + 1] +
        *field[i][j - 1] + *field[i + 1][j - 1];
    if (neighbors == 3) {
        return 1;
    } else if (*field[i][j] && neighbors == 2) {
        return 1;
    }
    return 0;
}

void get_first_gen(int ****pointer, FILE * file, inf info)
{
    char buf[BUF_SIZE], *pEnd, *last_pos;
    int num, ***field = *pointer, i, j;
    for (i = 1; fgets(buf, BUF_SIZE, file) && i <= info.rows; i++) {
        for (pEnd = buf, j = 1; *pEnd != 0 && j <= info.columns; j++) {
            last_pos = pEnd;
            num = strtol(pEnd, &pEnd, 10);
            if (last_pos == pEnd) {
                puts("Wrong input of grid for game");
                exit(0);
            }
            if (num == 1)
                *field[i][j] = 1;
        }
    }
}

void get_grid_with_gen(int ****pointer, inf info)
{
    int i, j, g, ***field = *pointer,
        tmp_field[info.rows + 1][info.columns + 1];

    for (g = 0; g < info.gen; g++) {
        for (i = 1; i <= info.rows; i++)
            for (j = 1; j <= info.columns; j++)
                tmp_field[i][j] = born_or_die(&field, i, j);

        for (i = 1; i <= info.rows; i++)
            for (j = 1; j <= info.columns; j++)
                *field[i][j] = tmp_field[i][j];
    }
}

void free_grid(int ***field, inf info)
{
    int i, j;
    for (i = 1; i <= info.rows; i++)
        for (j = 1; j <= info.columns; j++)
            free(field[i][j]);

    for (i = 0; i < info.rows + 2; i++)
        free(field[i]);
    free(field);
}

void print_grid(int ***field, inf info)
{
    FILE *out;
    int i, j;
    if (!(out = fopen("life.out", "wt"))) {
        puts("Can't open output file");
        exit(0);
    }

    for (i = 1; i <= info.rows; i++) {
        for (j = 1; j <= info.columns; j++)
            fprintf(out, "%d ", *field[i][j]);
        fprintf(out, "\n");
    }
    fclose(out);
}

int main(int argc, char *argv[])
{
    FILE *in;
    int ***field;
    inf info;

    if (!(in = fopen("life.in", "rt"))) {
        puts("Can't open input file");
        return 0;
    }
    get_size_and_gen(in, &info);
    generate_grid(&field, info);
    get_first_gen(&field, in, info);
    get_grid_with_gen(&field, info);
    print_grid(field, info);
    free_grid(field, info);
    fclose(in);
    return 0;
}
