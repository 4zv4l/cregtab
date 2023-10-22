#include "persolib/regex.h"
#include "persolib/utils.h"

void
draw(string *cols, i32 width[static 1], usize length)
{
    for (usize i = 0; i < length-1; i++)
        printf("%*s | ", -width[i], cols[i]);
    printf("%*s\n", -width[length-1], cols[length-1]);
}

void
draw_header(string *cols, i32 width[static 1], usize length)
{
    string *sep = malloc(length * sizeof(*sep));
    if (!sep)
        die("couldnt allocate memory for separator");

    plforeach(s, sep, length)
        *s = "-";
    draw(cols, width, length);
    draw(sep, width, length);
    free(sep);
}

string*
getcols(string fmt, i32 **width)
{
    string *cols_width = split(fmt, ';');
    usize  length      = ptr_len(cols_width, sizeof(string));
    usize  count       = 0;
    string *columns    = calloc(length+1, sizeof(string));
    i32    *twidth     = calloc(length+1, sizeof(u32));

    if (!fmt[0])
        die("columns argument is empty");
    if (!cols_width)
        die("couldnt allocate memory for the columns");
    if (!columns)
        die("couldnt allocate memory for the columns");
    if (!twidth)
        die("couldnt allocate memory for the width");

    pforeach(col_width, cols_width)
    {
        string *col = split(*col_width, ':');
        if (!col)
            die("couldnt allocate memory for the column");

        columns[count] = col[0];
        twidth[count] = atoi(col[1] ? col[1] : "0");
        count += 1;

        free(col[1]);
        free(col);
    }

    free_ptrarrayptr(cols_width, -1);
    *width = twidth;
    return columns;
}

i32
main(i32 argc, string argv[])
{
    string usage = "usage: %s [regex] [col:width;] [filename]";
    if (argc != 4)
        die(usage, *argv);

    i32    *width  = { 0 };
    string *cols   = { getcols(argv[2], &width) };
    u32    length  = { ptr_len(cols, sizeof(string)) };
    string *groups = { calloc(length+1, sizeof(*groups)) };
    string re      = { argv[1] };
    FILE   *file   = { fopen(argv[3], "r") };
    string line    = { 0 };

    if (!file)
        die("couldnt open '%s': %s", argv[3], strerror(errno));
    if (!width)
        die("couldnt get width");
    if (!cols)
        die("couldnt allocate memory for the columns");
    if (!groups)
        die("couldnt allocate memory for the regex groups");
    if (!cols)
        die("missing columns in arguments");

    draw_header(cols, width, length);
    while(fget_line(&line, file))
    {
        chomp(line);
        if (match(re, line, groups, length+1))
        {
            draw(&groups[1], width, length);
            // length+1 because match the whole line as first group
            plforeach(group, groups, length+1)
                free(*group);
        }
    }

    free(width);
    free_ptrarrayptr(cols, length);
    free(groups);
    fclose(file);
    free(line);
}
