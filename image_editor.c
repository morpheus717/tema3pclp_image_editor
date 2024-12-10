#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define NSIZ 101
#define CSIZ 21
#define LINESIZ 100001
typedef struct greyscale 
{
    int **value;
    int line, column;
    int scale;
    int selected_x1, selected_x2, selected_y1, selected_y2;
}greyscale;
typedef struct rgbpixel {
    int r, g, b;
}rgbpixel;
typedef struct rgbpicture {
    rgbpixel **elements;
    int line, column;
    int scale;
    int selected_x1, selected_x2, selected_y1, selected_y2;
}rgbpicture;
void swap(int *a, int *b)
{
    int aux = *a;
    *a = *b;
    *b = aux;
}
void clamp(int *a)
{
    if(*a < 0)
        *a = 0;
    else if(*a > 255)
        *a = 255;
    return;
}
void clamp_double(double *a)
{
    if(*a < 0)
        *a = 0;
    else if(*a > 255)
        *a = 255;
    return;
}
int roundd(double x)
{
    if(x - (int)x < 0.5)
        return (int)x;
    return (int)x + 1;
}
int abs(int a)
{
    if(a < 0)
    return -a;
    return a;
}
void alloc_greyscale(greyscale *a)
{
    if(a->line == 0 || a->column == 0)
    {
        return;
    }
    a->value = malloc(a->line * sizeof(int*));
    if(a->value == NULL)
    {
        printf("Error allocating greyscale. Exiting...\n");
        return;
    }
    for(int i = 0; i < a->line; i++)
        {
            a->value[i] = calloc(a->column, sizeof(int));
            if(a->value[i] == NULL)
            {
                printf("Error allocating greyscale. Exiting...\n");
                return;
            }
        }
}
void free_greyscale(greyscale *a)
{
    if(a == NULL || a->value == NULL)
    {
        //printf("No image loaded\n");
        return;
    }
    for(int i = 0; i < a->line; i++)
        free(a->value[i]);
    free(a->value);
}
void swap_matrixes(int ***a, int ***b)
{
    int **aux;
    aux = *a;
    *a = *b;
    *b = aux;
}
void transpose_matrix_grey(greyscale *mat)
{
    swap(&mat->selected_x1, &mat->selected_y1);
    swap(&mat->selected_x2, &mat->selected_y2);
    greyscale aux;
    aux.line = mat->column;
    aux.column = mat->line;
    alloc_greyscale(&aux);
    for(int i = 0; i < aux.line; i++)
        for(int j = 0; j < aux.column; j++)
            aux.value[i][j] = mat->value[j][i];
    swap_matrixes(&aux.value, &mat->value);
    mat->line = aux.line;
    mat->column = aux.column;
    swap(&aux.line, &aux.column);
    free_greyscale(&aux);
}
void reverse_rows_greyscale(greyscale *mat)
{
    for(int i = 0; i < mat->line; i++)
    {
        int start = 0, end = mat->column - 1;
        while(start < end)
        {
            swap(&mat->value[i][start], &mat->value[i][end]);
            start++, end--;
        }
    }
}
void alloc_rgb(rgbpicture *a)
{
    if(a->line == 0 || a->column == 0)
    {
        return;
    }
    a->elements = malloc(a->line * sizeof(rgbpixel*));
    for(int i = 0; i < a->line; i++)
        a->elements[i] = calloc(a->column, sizeof(rgbpixel));
}
void free_rgb(rgbpicture *a)
{
    if(a == NULL || a->elements == NULL)
    {
        //printf("No image loaded\n");
        return;
    }
    for(int i = 0; i < a->line; i++)
        free(a->elements[i]);
    free(a->elements);
}
void swap_rgb(rgbpixel *a, rgbpixel *b)
{
    rgbpixel aux = *a;
    *a = *b;
    *b = aux;
}
void swap_matrixes_rgb(rgbpixel ***a, rgbpixel ***b)
{
    rgbpixel **aux;
    aux = *a;
    *a = *b;
    *b = aux;
}
void transpose_matrix_color(rgbpicture *mat)
{
    rgbpicture aux;
    aux.line = mat->column;
    aux.column = mat->line;
    alloc_rgb(&aux);
    for(int i = 0; i < aux.line; i++)
        for(int j = 0; j < aux.column; j++)
            aux.elements[i][j] = mat->elements[j][i];
    swap_matrixes_rgb(&aux.elements, &mat->elements);
    mat->line = aux.line;
    mat->column = aux.column;
    swap(&aux.line, &aux.column);
    free_rgb(&aux);
}
void reverse_rows_color(rgbpicture *mat)
{
    for(int i = 0; i < mat->line; i++)
    {
        int start = 0, end = mat->column - 1;
        while(start < end)
        {
            swap_rgb(&mat->elements[i][start], &mat->elements[i][end]);
            start++, end--;
        }
    }
}
void handle_p2(FILE *in, greyscale *mat)
{
    int i = 0, j = 0;
    fscanf(in, "%d %d", &mat->column, &mat->line);
    alloc_greyscale(mat);
    fscanf(in, "%d", &mat->scale);
    while(i < mat->line && j < mat->column)
    {
        fscanf(in, "%d", &mat->value[i][j]);
        j++;
        if(j == mat->column)
        i++, j = 0;
    }
}
void handle_p3(FILE *in, rgbpicture *color)
{
    int i = 0, j = 0;
    fscanf(in, "%d %d", &color->column, &color->line);
    alloc_rgb(color);
    fscanf(in, "%d", &color->scale);
    while(i < color->line && j < color->column)
    {
        fscanf(in, "%d %d %d", &color->elements[i][j].r, &color->elements[i][j].g, &color->elements[i][j].b);
        j++;
        if(j == color->column)
        i++, j = 0;
    }
}
void handle_p5(FILE *in, greyscale *mat)
{
    char c;
    fscanf(in, "%d %d", &mat->column, &mat->line);
    fscanf(in, "%d", &mat->scale);
    fscanf(in, "%c", &c);
    alloc_greyscale(mat);
    for(int i = 0; i < mat->line; i++)
        for(int j = 0; j < mat->column; j++)
            fread(&mat->value[i][j], sizeof(unsigned char), 1, in);
}
void handle_p6(FILE *in, rgbpicture *color)
{
    char c;
    fscanf(in, "%d %d", &color->column, &color->line);
    fscanf(in, "%d", &color->scale);
    fscanf(in, "%c", &c);
    alloc_rgb(color);
    for(int i = 0; i < color->line; i++)
        for(int j = 0; j < color->column; j++)
            {
                fread(&color->elements[i][j].r, sizeof(unsigned char), 1, in);
                fread(&color->elements[i][j].g, sizeof(unsigned char), 1, in);
                fread(&color->elements[i][j].b, sizeof(unsigned char), 1, in);
            }
}
void load(char *input_filename, greyscale *mat, rgbpicture *color, int *used_format)
{
    FILE *in = fopen(input_filename, "rt");
    if(in == NULL)
        {
            printf("Failed to load %s\n", input_filename);
            *used_format = -1;
            return;
        }
    char p, type;
    fscanf(in, "%c%c", &p, &type);
    if(type == '2')
        {
            handle_p2(in, mat);
            *used_format = 0;
        }
    else if(type == '3')
        {
            handle_p3(in, color);
            *used_format = 1;
        }
    else if(type == '5')
        {
            handle_p5(in, mat);
            *used_format = 0;
        }
    else if(type == '6')
        {
            handle_p6(in, color);
            *used_format = 1;
        }
    fclose(in);
    printf("Loaded %s\n", input_filename);
    mat->selected_x1 = 0, mat->selected_y1 = 0;
    mat->selected_x2 = mat->column, mat->selected_y2 = mat->line;
    color->selected_x1 = 0, color->selected_y1 = 0;
    color->selected_x2 = color->column, color->selected_y2 = color->line;
}
void select_smecher(greyscale *mat, rgbpicture *color, int used_format, int x1, int y1, int x2, int y2)
{
    if(x1 > x2)
        swap(&x1,&x2);
    if(y1 > y2)
        swap(&y1, &y2);
    if(used_format == -1)
    {
        printf("No image loaded\n");
        return;
    }
    else if(used_format == 1)
        {
            if(x1 < 0 || x2 > color->column || y1 < 0 || y2 > color->line || y2 <=0 || x2 <= 0)
            {
                printf("Invalid set of coordinates\n");
                return;
            }
            color->selected_x1 = x1;
            color->selected_y1 = y1;
            color->selected_x2 = x2;
            color->selected_y2 = y2;
        }
    else if(used_format == 0)
    {
        if(x1 < 0 || x2 > mat->column || y1 < 0 || y2 > mat->line || y2 <= 0 || x2 <= 0)
            {
                printf("Invalid set of coordinates\n");
                return;
            }
        mat->selected_x1 = x1;
        mat->selected_y1 = y1;
        mat->selected_x2 = x2;
        mat->selected_y2 = y2;
    }
    printf("Selected %d %d %d %d\n", x1, y1, x2, y2);
}
void select_all(greyscale *mat, rgbpicture *color, int used_format)
{
    if(used_format == -1)
    {
        printf("No image loaded\n");
        return;
    }
    if(used_format == 0)
        {
            mat->selected_x1 = 0, mat->selected_y1 = 0;
            mat->selected_x2 = mat->column, mat->selected_y2 = mat->line;
        }
    if(used_format == 1)
        {
            color->selected_x1 = 0, color->selected_y1 = 0;
            color->selected_x2 = color->column, color->selected_y2 = color->line;
        }
    printf("Selected ALL\n");
}
void histogram(greyscale *mat, int used_format, int maxstars, int bins)
{
    if(used_format == -1)
    {
        printf("No image loaded\n");
        return;
    }
    else if(used_format == 1)
    {
        printf("Black and white image needed\n");
        return;
    }
    int fv[256] = {0}, max_sum_frecventa = -1;
    for(int i = 0; i < mat->line; i++)
        for(int j = 0; j < mat->column; j++)
            fv[mat->value[i][j]]++;
    int pixels_in_bin = (mat->scale + 1) / bins;
    for(int i = 0; i < bins; i++)
    {
        int sum = 0;
        for(int j = i * pixels_in_bin; j < (i + 1) * pixels_in_bin; j++)
            sum += fv[j];
        if(max_sum_frecventa < sum)
            max_sum_frecventa = sum;
    }
    for(int i = 0; i < bins; i++)
    {
        int sum = 0;
        for(int j = i * pixels_in_bin; j < (i + 1) * pixels_in_bin; j++)
            sum += fv[j];
        int nr_stars = maxstars * sum / max_sum_frecventa;
        printf("%d\t|\t", nr_stars);
        for(int j = 0; j < nr_stars; j++)
        printf("*");
        printf("\n");
    }
}
void crop(greyscale *mat, rgbpicture *color, int used_format)
{
    if(used_format == -1)
    {
        printf("No image loaded\n");
        return;
    }
    if(used_format == 0)
    {
        greyscale aux;
        aux.line = mat->selected_y2 - mat->selected_y1;
        aux.column = mat->selected_x2 - mat->selected_x1;
        alloc_greyscale(&aux);
        for(int i = 0; i < aux.line; i++)
            for(int j = 0; j < aux.column; j++)
                aux.value[i][j] = mat->value[mat->selected_y1 + i][mat->selected_x1 + j];
        mat->line = aux.line;
        mat->column = aux.column;
        swap_matrixes(&mat->value, &aux.value);
        mat->selected_x1 = 0, mat->selected_y1 = 0;
        mat->selected_x2 = mat->column, mat->selected_y2 = mat->line;
        free_greyscale(&aux);
    }
    else if(used_format == 1)
    {
        rgbpicture aux;
        aux.line = color->selected_y2 - color->selected_y1;
        aux.column = color->selected_x2 - color->selected_x1;
        alloc_rgb(&aux);
        for(int i = 0; i < aux.line; i++)
            for(int j = 0; j < aux.column; j++)
                aux.elements[i][j] = color->elements[color->selected_y1 + i][color->selected_x1 + j];
        color->line = aux.line;
        color->column = aux.column;
        swap_matrixes_rgb(&color->elements, &aux.elements);
        color->selected_x1 = 0, color->selected_y1 = 0;
        color->selected_x2 = color->column, color->selected_y2 = color->line;
        free_rgb(&aux);
    }
    printf("Image cropped\n");
}
void save(greyscale *mat, rgbpicture *color, int used_format)
{
    if(used_format == -1)
    {
        printf("No image loaded\n");
        return;
    }
    char output_filename[NSIZ];
    int binar = 1;
    getc(stdin);
    fgets(output_filename, NSIZ, stdin);
    for(int i = 0; output_filename[i]; i++)
        if(output_filename[i] == ' ')
        {
            binar = 0;
            output_filename[i] = '\0';
            break;
        }
    if(binar)
        output_filename[strlen(output_filename) - 1] = '\0';
    FILE *out = fopen(output_filename, "w");
    if(used_format == 0)
    {
        if(binar == 0)//P2
        {
            fprintf(out, "P2\n");
            fprintf(out, "%d %d\n", mat->column, mat->line);
            fprintf(out, "%d\n", mat->scale);
            for(int i = 0; i < mat->line; i++)
                {
                    for(int j = 0; j < mat->column; j++)
                        fprintf(out, "%d ", mat->value[i][j]);
                    fprintf(out, "\n");
                }
        }
        else //P5
        {
            fprintf(out, "P5\n");
            fprintf(out, "%d %d\n", mat->column, mat->line);
            fprintf(out, "%d\n", mat->scale);
            for(int i = 0; i < mat->line; i++)
                for(int j = 0; j < mat->column; j++)
                    fwrite(&mat->value[i][j], sizeof(unsigned char), 1, out);
            //fwrite(&mat->value, sizeof(mat->value[0][0]), mat->line * mat->column, out);
        }
    }
    else if(used_format == 1)
    {
        if(binar == 0)//P3
        {
            fprintf(out, "P3\n");
            fprintf(out, "%d %d\n", color->column, color->line);
            fprintf(out, "%d\n", color->scale);
            for(int i = 0; i < color->line; i++)
            {
                for(int j = 0; j < color->column; j++)
                    fprintf(out, "%d %d %d ", color->elements[i][j].r, color->elements[i][j].g, color->elements[i][j].b);
                fprintf(out, "\n");
            }
        }
        else//P6
        {
            fprintf(out, "P6\n");
            fprintf(out, "%d %d\n", color->column, color->line);
            fprintf(out, "%d\n", color->scale);
            for(int i = 0; i < color->line; i++)
            {
                for(int j = 0; j < color->column; j++)
                {
                    fwrite(&color->elements[i][j].r, sizeof(unsigned char), 1, out);
                    fwrite(&color->elements[i][j].g, sizeof(unsigned char), 1, out);
                    fwrite(&color->elements[i][j].b, sizeof(unsigned char), 1, out);
                }
            }
        }
    }
    fclose(out);
    printf("Saved %s\n", output_filename);
}

void freq(greyscale *mat, int fv[], int x1, int y1, int x2, int y2)
{
    for(int i = y1; i < y2; i++)
        for(int j = x1; j < x2; j++)
            fv[mat->value[i][j]]++;
    for(int i = 1; i <= 255; i++)
        fv[i] += fv[i - 1];
}
void equalize(greyscale *mat, int used_format)
{
    if(used_format == -1)
    {
        printf("No image loaded\n");
        return;
    }
    if(used_format == 1)
    {
        printf("Black and white image needed\n");
        return;
    }
    int fv[256] = {0};
    freq(mat, fv, 0, 0, mat->column, mat->line);
    for(int i = 0; i < mat->line; i++)
        for(int j = 0; j < mat->column; j++)
            {
                mat->value[i][j] = roundd(255.0 / (mat->line * mat->column) * fv[mat->value[i][j]]);
                clamp(&mat->value[i][j]);
            }
    printf("Equalize done\n");
}
void rotate_selection_grey(greyscale *mat)
{
    for(int i = 0; i < mat->selected_y2 - mat->selected_y1; i++)
        for(int j = 0; j < i; j++)
            swap(&mat->value[mat->selected_y1 + i][mat->selected_x1 + j], &mat->value[mat->selected_y1 + j][mat->selected_x1 + i]);
    for(int i = mat->selected_y1; i < mat->selected_y2; i++)
    {
        int start = mat->selected_x1, end = mat->selected_x2 - 1;
        while(start < end)
        {
            swap(&mat->value[i][start], &mat->value[i][end]);
            start++, end--;
        }
    }
}
void rotate_selection_color(rgbpicture *mat)
{
    for(int i = 0; i < mat->selected_y2 - mat->selected_y1; i++)
        for(int j = 0; j < i; j++)
            swap_rgb(&mat->elements[mat->selected_y1 + i][mat->selected_x1 + j], &mat->elements[mat->selected_y1 + j][mat->selected_x1 + i]);
    for(int i = mat->selected_y1; i < mat->selected_y2; i++)
    {
        int start = mat->selected_x1, end = mat->selected_x2 - 1;
        while(start < end)
        {
            swap_rgb(&mat->elements[i][start], &mat->elements[i][end]);
            start++, end--;
        }
    }
}
void rotate_image_greyscale(greyscale *mat)
{
    transpose_matrix_grey(mat);
    reverse_rows_greyscale(mat);
}
void rotate_image_color(rgbpicture *color)
{
    transpose_matrix_color(color);
    reverse_rows_color(color);
}
void rotate(greyscale *mat, rgbpicture *color, int used_format)
{
    int angle, angle2;
    scanf("%d", &angle);
    angle2 = angle;
    if(!(abs(angle) == 90 || abs(angle) == 180 || abs(angle) == 270 || abs(angle) == 360 || angle == 0))
        {
        printf("Unsupported rotation angle\n");
        return;
        }
    if(used_format == -1)
        {
            printf("No image loaded\n");
            return;
        }
    if(angle < 0)
            angle += 360;
    if(used_format == 0)//greyscale
    {
        if((mat->selected_x1 == 0) && (mat->selected_x2 == mat->column) && (mat->selected_y1 == 0) && (mat->selected_y2 == mat->line))//sunt pe imagine
            {
                for(int i = 1; i <= angle / 90; i++)
                rotate_image_greyscale(mat);
            }
        else
        {
            if(mat->selected_x2 - mat->selected_x1 != mat->selected_y2 - mat->selected_y1)
            {
                printf("The selection must be square\n");
                return;
            }
            for(int i = 1; i <= angle / 90; i++)
                rotate_selection_grey(mat);
        }
    }
    else if(used_format == 1)//color
    {
        if(color->selected_x1 == 0 && color->selected_x2 == color->column && color->selected_y1 == 0 && color->selected_y2 == color->line)//sunt pe imagine
            for(int i = 1; i <= angle / 90; i++)
                rotate_image_color(color);
        else {
            if(color->selected_x2 - color->selected_x1 != color->selected_y2 - color->selected_y1)
            {
                printf("The selection must be square\n");
                return;
            }
            for(int i = 1; i <= angle / 90; i++)
                rotate_selection_color(color);
        }
    }
    printf("Rotated %d\n", angle2);
}
void apply_edge(rgbpicture *color)
{
    double kernel[3][3] = {{-1, -1, -1}, {-1, 8, -1}, {-1, -1, -1}};
    rgbpicture aux;
    aux.line = color->selected_y2 - color->selected_y1;
    aux.column = color->selected_x2 - color->selected_x1;
    alloc_rgb(&aux);
    for(int i = 0; i < aux.line; i++)
        for(int j = 0; j < aux.column; j++)
        {
            if(color->selected_y1 + i == 0 || color->selected_x1 + j == 0 || color->selected_y1 + i == color->line - 1 || color->selected_x1 + j == color->column - 1)
            {
                aux.elements[i][j].r = color->elements[color->selected_y1 + i][color->selected_x1 + j].r;
                aux.elements[i][j].g = color->elements[color->selected_y1 + i][color->selected_x1 + j].g;
                aux.elements[i][j].b = color->elements[color->selected_y1 + i][color->selected_x1 + j].b;
                continue;
            }
            for(int t = -1; t <= 1; t++)
                for(int p = -1; p <= 1; p++)
                    {
                        aux.elements[i][j].r += color->elements[color->selected_y1 + i + t][color->selected_x1 + j + p].r * kernel[1 + t][1 + p];
                        aux.elements[i][j].g += color->elements[color->selected_y1 + i + t][color->selected_x1 + j + p].g * kernel[1 + t][1 + p];
                        aux.elements[i][j].b += color->elements[color->selected_y1 + i + t][color->selected_x1 + j + p].b * kernel[1 + t][1 + p];
                    }
            clamp(&aux.elements[i][j].r);
            clamp(&aux.elements[i][j].g);
            clamp(&aux.elements[i][j].b);
        }
    for(int i = 0; i < aux.line; i++)
        for(int j = 0; j < aux.column; j++)
            {
                color->elements[color->selected_y1 + i][color->selected_x1 + j].r = aux.elements[i][j].r;
                color->elements[color->selected_y1 + i][color->selected_x1 + j].g = aux.elements[i][j].g;
                color->elements[color->selected_y1 + i][color->selected_x1 + j].b = aux.elements[i][j].b;
            }
    free_rgb(&aux);
}
void apply_sharpen(rgbpicture *color)
{
    double kernel[3][3] = {{0, -1, 0}, {-1, 5, -1}, {0, -1, 0}};
    rgbpicture aux;
    aux.line = color->selected_y2 - color->selected_y1;
    aux.column = color->selected_x2 - color->selected_x1;
    alloc_rgb(&aux);
    for(int i = 0; i < aux.line; i++)
        for(int j = 0; j < aux.column; j++)
        {
            if(color->selected_y1 + i == 0 || color->selected_x1 + j == 0 || color->selected_y1 + i == color->line - 1 || color->selected_x1 + j == color->column - 1)
            {
                aux.elements[i][j].r = color->elements[color->selected_y1 + i][color->selected_x1 + j].r;
                aux.elements[i][j].g = color->elements[color->selected_y1 + i][color->selected_x1 + j].g;
                aux.elements[i][j].b = color->elements[color->selected_y1 + i][color->selected_x1 + j].b;
                continue;
            }
            for(int t = -1; t <= 1; t++)
                for(int p = -1; p <= 1; p++)
                    {
                        aux.elements[i][j].r += color->elements[color->selected_y1 + i + t][color->selected_x1 + j + p].r * kernel[1 + t][1 + p];
                        aux.elements[i][j].g += color->elements[color->selected_y1 + i + t][color->selected_x1 + j + p].g * kernel[1 + t][1 + p];
                        aux.elements[i][j].b += color->elements[color->selected_y1 + i + t][color->selected_x1 + j + p].b * kernel[1 + t][1 + p];
                    }
            clamp(&aux.elements[i][j].r);
            clamp(&aux.elements[i][j].g);
            clamp(&aux.elements[i][j].b);
        }
    for(int i = 0; i < aux.line; i++)
        for(int j = 0; j < aux.column; j++)
            {
                color->elements[color->selected_y1 + i][color->selected_x1 + j].r = aux.elements[i][j].r;
                color->elements[color->selected_y1 + i][color->selected_x1 + j].g = aux.elements[i][j].g;
                color->elements[color->selected_y1 + i][color->selected_x1 + j].b = aux.elements[i][j].b;
            }
    free_rgb(&aux);
}
void apply_blur(rgbpicture *color)
{
    double kernel[3][3] = {{1.0/9, 1.0/9, 1.0/9}, {1.0/9, 1.0/9, 1.0/9}, {1.0/9, 1.0/9, 1.0/9}};
    rgbpicture aux;
    aux.line = color->selected_y2 - color->selected_y1;
    aux.column = color->selected_x2 - color->selected_x1;
    alloc_rgb(&aux);
    for(int i = 0; i < aux.line; i++)
        for(int j = 0; j < aux.column; j++)
        {
            if(color->selected_y1 + i == 0 || color->selected_x1 + j == 0 || color->selected_y1 + i == color->line - 1 || color->selected_x1 + j == color->column - 1)
            {
                aux.elements[i][j].r = color->elements[color->selected_y1 + i][color->selected_x1 + j].r;
                aux.elements[i][j].g = color->elements[color->selected_y1 + i][color->selected_x1 + j].g;
                aux.elements[i][j].b = color->elements[color->selected_y1 + i][color->selected_x1 + j].b;
                continue;
            }
            double r = 0, g = 0, b = 0;
            for(int t = -1; t <= 1; t++)
                for(int p = -1; p <= 1; p++)
                    {
                        r += color->elements[color->selected_y1 + i + t][color->selected_x1 + j + p].r * kernel[1 + t][1 + p];
                        g += color->elements[color->selected_y1 + i + t][color->selected_x1 + j + p].g * kernel[1 + t][1 + p];
                        b += color->elements[color->selected_y1 + i + t][color->selected_x1 + j + p].b * kernel[1 + t][1 + p];
                    }
            r = roundd(r);
            clamp_double(&r);
            aux.elements[i][j].r = r;
            g = roundd(g);
            clamp_double(&g);
            aux.elements[i][j].g = g;
            b = roundd(b);
            clamp_double(&b);
            aux.elements[i][j].b = b;
        }
    for(int i = 0; i < aux.line; i++)
        for(int j = 0; j < aux.column; j++)
            {
                color->elements[color->selected_y1 + i][color->selected_x1 + j].r = aux.elements[i][j].r;
                color->elements[color->selected_y1 + i][color->selected_x1 + j].g = aux.elements[i][j].g;
                color->elements[color->selected_y1 + i][color->selected_x1 + j].b = aux.elements[i][j].b;
            }
    free_rgb(&aux);
}
void apply_gauss_blur(rgbpicture *color)
{
    double kernel[3][3] = {{1.0/16, 1.0/8, 1.0/16}, {1.0/8, 1.0/4, 1.0/8}, {1.0/16, 1.0/8, 1.0/16}};
    rgbpicture aux;
    aux.line = color->selected_y2 - color->selected_y1;
    aux.column = color->selected_x2 - color->selected_x1;
    alloc_rgb(&aux);
    for(int i = 0; i < aux.line; i++)
        for(int j = 0; j < aux.column; j++)
        {
            if(color->selected_y1 + i == 0 || color->selected_x1 + j == 0 || color->selected_y1 + i == color->line - 1 || color->selected_x1 + j == color->column - 1)
            {
                aux.elements[i][j].r = color->elements[color->selected_y1 + i][color->selected_x1 + j].r;
                aux.elements[i][j].g = color->elements[color->selected_y1 + i][color->selected_x1 + j].g;
                aux.elements[i][j].b = color->elements[color->selected_y1 + i][color->selected_x1 + j].b;
                continue;
            }
            double r = 0, g = 0, b = 0;
            for(int t = -1; t <= 1; t++)
                for(int p = -1; p <= 1; p++)
                    {
                        r += color->elements[color->selected_y1 + i + t][color->selected_x1 + j + p].r * kernel[1 + t][1 + p];
                        g += color->elements[color->selected_y1 + i + t][color->selected_x1 + j + p].g * kernel[1 + t][1 + p];
                        b += color->elements[color->selected_y1 + i + t][color->selected_x1 + j + p].b * kernel[1 + t][1 + p];
                    }
            r = roundd(r);
            clamp_double(&r);
            aux.elements[i][j].r = r;
            g = roundd(g);
            clamp_double(&g);
            aux.elements[i][j].g = g;
            b = roundd(b);
            clamp_double(&b);
            aux.elements[i][j].b = b;
        }
    for(int i = 0; i < aux.line; i++)
        for(int j = 0; j < aux.column; j++)
            {
                color->elements[color->selected_y1 + i][color->selected_x1 + j].r = aux.elements[i][j].r;
                color->elements[color->selected_y1 + i][color->selected_x1 + j].g = aux.elements[i][j].g;
                color->elements[color->selected_y1 + i][color->selected_x1 + j].b = aux.elements[i][j].b;
            }
    free_rgb(&aux);
}
void apply(rgbpicture *color, int used_format)
{
    char parameter[21];
    scanf("%s", parameter);
    if(strcmp(parameter, "EXIT") == 0)
        exit(0);
    if(used_format == 0)
    {
        printf("Easy, Charlie Chaplin\n");
        return;
    }
    if(used_format == -1)
    {
        printf("No image loaded\n");
        return;
    }
    if(strcmp(parameter, "EDGE") == 0)
        apply_edge(color);
    else if(strcmp(parameter, "SHARPEN") == 0)
        apply_sharpen(color);
    else if(strcmp(parameter, "BLUR") == 0)
        apply_blur(color);
    else if(strcmp(parameter, "GAUSSIAN_BLUR") == 0)
        apply_gauss_blur(color);
    else
        {
            printf("APPLY parameter invalid\n");
            return;
        }
    printf("APPLY %s done\n", parameter);
}
int main()
{
    int x1, y1, x2, y2;
    int cnt = 0;
    char *command, *input_filename;
    greyscale mat;
    rgbpicture color;
    int used_format = -1; //1=color 0=grayscale
    color.line = 0, color.column = 0, color.elements = NULL;
    mat.line = 0, mat.column = 0, mat.value = NULL;
    command = malloc(CSIZ * sizeof(char));
    input_filename = malloc(NSIZ * sizeof(char));
    while(scanf("%s", command))
    {
        if(strcmp(command, "LOAD") == 0)
        {
            scanf("%s", input_filename);
            load(input_filename, &mat, &color, &used_format);
        }
        else if(strcmp(command, "SELECT") == 0)
        {
            char all[10];
            scanf("%s", all);
            if(strcmp(all, "ALL") == 0)
                select_all(&mat, &color, used_format);
            else
            {
                x1 = atoi(all);
                scanf("%d %d %d", &y1, &x2, &y2);
                select_smecher(&mat, &color, used_format, x1, y1, x2, y2);
            }
        }
        else if(strcmp(command, "HISTOGRAM") == 0)
        {
            int x, y;
            scanf("%d %d", &x, &y);
            histogram(&mat, used_format, x, y);
        }
        else if(strcmp(command, "CROP") == 0)
            crop(&mat, &color, used_format);
        else if(strcmp(command, "SAVE") == 0)
        {
            save(&mat, &color, used_format);
        }
        else if(strncmp(command, "EXIT", 2) == 0)
        {
            if(used_format == -1)
                {
                    printf("No image loaded\n");
                    free_greyscale(&mat);
                free_rgb(&color);
                free(command);
                free(input_filename);
                exit(0);
                }
            else
            {
                free_greyscale(&mat);
                free_rgb(&color);
                free(command);
                free(input_filename);
                exit(0);
            }
        }
        else if(strcmp(command, "EQUALIZE") == 0)
            {
                //printf("equalizein\n");
                equalize(&mat, used_format);
            }
        else if(strcmp(command, "ROTATE") == 0)
        {
            rotate(&mat, &color, used_format);
        }
        else if(strcmp(command, "APPLY") == 0)
            apply(&color, used_format);
        else
            {
                cnt++;
                printf("Invalid command\n");
            }
        if(cnt > 100)
            return 0;
    }
}