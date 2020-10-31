#include "Header.hpp"

void process_files_asm (int argC, char* argV[], text* text_file, keys* hamlet_keys);

FILE* preparation_to_compilation (int argC, char* argV[], text* text_file, keys* keys);

FILE* open_file_asm (char* argV[], text* text_file, keys* keys);

FILE* add_postfix_asm (char* argV[], keys* keys);

void creat_buffer (text* text_file, FILE* input);

void split_lines (text* text_file);

void compilation (text* text_file, FILE* output, char* code);

size_t compilation_run (text* text_file, char* code, label* labels);

size_t compilation_cycle (size_t ofs, char* temp_copy, char* ptr, char* reg_name, size_t* num_of_label, text* text_file, char* code, label* labels);

void process_labels (char* ptr, label* labels, size_t* num_of_label, size_t ofs);

void destroy_labels (label* labels);

void undefined_command_print (size_t line, char* ptr, size_t* ofs);


main (int argC, char* argV[])
{
    assert (argC != 0);
    assert (argV != nullptr);


    text* asm_file = construct_text ();

    assert (asm_file != nullptr);

    keys* asm_keys = (keys*) calloc (1, sizeof(keys));
    assert (asm_keys != nullptr);

    process_files_asm (argC, argV, asm_file, asm_keys);

    destroy_text (asm_file);
    destroy_keys (asm_keys);
}

void process_files_asm (int argC, char* argV[], text* text_file, keys* keys)
{
    FILE* output = preparation_to_compilation (argC, argV, text_file, keys);

    char* code = (char*) calloc (text_file->lines*(sizeof (char) + sizeof (double)) + size_of_header, 1);

    compilation (text_file, output, code);

    fclose (output);

    free (code);
}

FILE* preparation_to_compilation (int argC, char* argV[], text* text_file, keys* keys)
{
    keys->in = get_arg_val (argC, argV, "in");

    FILE* input = (keys->in != 0) ? (fopen (argV[keys->in], "r")) : (fopen ("Program files\\Commands.asm", "r"));

    keys->out = get_arg_val (argC, argV, "out");

    FILE* output = open_file_asm (argV, text_file, keys);

    assert (input  != nullptr);
    assert (output != nullptr);

    text_file->file_length = find_length_of_file (input);
    assert (text_file->file_length != 0);

    creat_buffer (text_file, input);

    fclose (input);

    text_file->struct_array = (str*) calloc (text_file->file_length/2, sizeof(str));
    assert (text_file->struct_array != nullptr);

    split_lines (text_file);

    return output;
}

void creat_buffer (text* text_file, FILE* input)
{
    text_file->pointer_on_buffer = (char*) calloc (text_file->file_length + 1, sizeof(char));
    assert (text_file->pointer_on_buffer != nullptr);


    read_file (input, text_file->pointer_on_buffer, &(text_file->file_length));
}

FILE* open_file_asm (char* argV[], text* text_file, keys* keys)
{
    FILE* output = nullptr;

    if (keys->out > 0)
    {
        output = fopen (argV[keys->out], "wb");
    }
    else
    {
        if (keys->in > 0)
        {
            output = add_postfix_asm (argV, keys);
        }
        else
        {
            char standart_name_out[] = "Program files\\ByteCode.code";

            output = fopen (standart_name_out, "wb");
        }
    }
    return output;
}

FILE* add_postfix_asm (char* argV[], keys* keys)
{
    FILE* output = nullptr;
    size_t len = strlen(argV[keys->in]);
    char standart_name_out[len + 7];
    for (size_t i = 0; i < len - 3; i++)
    {
        if (*(argV[keys->in] + i) == '.')
        {
            if (strcmp (argV[keys->in] + i, ".asm") == 0)
            {
                strcpy (standart_name_out + i, "Byte.code");
                break;
            }
        }

        standart_name_out[i] = *(argV[keys->in] + i);
    }

    output = fopen (standart_name_out, "wb");
    return output;
}

void split_lines (text* text_file)
{
    char* pointer = strtok (text_file->pointer_on_buffer, "\n");

    while (pointer != 0)
    {
        text_file->struct_array[text_file->lines].beginning = pointer;
        text_file->lines++;
        pointer = strtok (nullptr, "\n");
    }
}

void compilation (text* text_file, FILE* output, char* code)
{
    strcpy (code, "PLv1.51");

    label* labels = (label*) calloc (max_num_of_labels, sizeof (label));

    for (size_t i = 0; i < max_num_of_labels; i++)
    {
        memset (&labels[i].addr, -1, 1);
    }

    size_t ofs = 0;

    for (size_t i = 0; i < 2; i++)
    {
        ofs = compilation_run(text_file, code, labels);
        if (ofs == 0)
        {
            printf ("Error on compilation run number %u", i+1);
            return;
        }
    }

    fwrite (code, sizeof(char), ofs, output);

    destroy_labels (labels);
}


size_t compilation_run (text* text_file, char* code, label* labels)
{
    size_t ofs = size_of_header;

    char* temp_copy = (char*) calloc (100, sizeof(char));

    char* ptr = nullptr;

    char reg_name[4];

    size_t num_of_label = 0;

    ofs = compilation_cycle (ofs, temp_copy, ptr, reg_name, &num_of_label, text_file, code, labels);

    free (temp_copy);

    return ofs;
}

size_t compilation_cycle (size_t ofs, char* temp_copy, char* ptr, char* reg_name, size_t* num_of_label, text* text_file, char* code, label* labels)
{
    for (size_t line = 0; line < text_file->lines; line++)
    {
        strcpy (temp_copy, text_file->struct_array[line].beginning);

        ptr = strtok (temp_copy, " ");

        #define DEF_CMD(name, num, asm_cmd, disasm_cmd, cpu_cmd) \
                                                                 \
        if (strcmp (#name, ptr) == 0)                            \
        {                                                        \
            asm_cmd                                              \
            continue;                                            \
        }

        #include "Commands.hpp"

        #undef DEF_CMD

        if (ptr[strlen(ptr) - 1] == ':')
        {
            process_labels (ptr, labels, num_of_label, ofs);
            continue;
        }

        undefined_command_print (line, ptr, &ofs);
        break;
    }
    return ofs;
}

void process_labels (char* ptr, label* labels, size_t* num_of_label, size_t ofs)
{
    ptr[strlen(ptr) - 1] = '\0';

    labels[*num_of_label].addr = ofs - size_of_header;
    labels[*num_of_label].name = (char*) calloc (max_length_of_label, sizeof(char));

    strcpy (labels[*num_of_label].name, ptr);

    (*num_of_label)++;
}

void destroy_labels (label* labels)
{
    for (size_t i = 0; i < max_num_of_labels; i++)
    {
        free (labels[i].name);
    }

    free (labels);
}

void undefined_command_print (size_t line, char* ptr, size_t* ofs)
{
    printf ("Undefined command on line number %u\n", line+1);
    printf ("%s\n", ptr);
    *ofs = 0;
}

