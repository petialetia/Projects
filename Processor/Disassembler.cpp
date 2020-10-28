#include "Header.hpp"

void process_files_rev_asm (int argC, char* argV[], text* text_file, keys* hamlet_keys);

FILE* open_file_disasm (char* argV[], text* text_file, keys* keys);

FILE* add_postfix_disasm (char* argV[], keys* keys);

void creat_buffer (text* text_file, FILE* input);

void reversed_compilation (text* text_file, FILE* output);


main (int argC, char* argV[])
{
    assert (argC != 0);
    assert (argV != nullptr);


    text* asm_file = construct_text ();

    assert (asm_file != nullptr);

    keys* asm_keys = (keys*) calloc (1, sizeof(keys));
    assert (asm_keys != nullptr);

    process_files_rev_asm (argC, argV, asm_file, asm_keys);

    destroy_text (asm_file);
    destroy_keys (asm_keys);
}

void process_files_rev_asm (int argC, char* argV[], text* text_file, keys* keys)
{
    keys->in = get_arg_val (argC, argV, "in");

    FILE* input = (keys->in != 0) ? (fopen (argV[keys->in], "r")) : (fopen ("Program files\\ByteCode.code", "r"));

    keys->out = get_arg_val (argC, argV, "out");

    FILE* output = open_file_disasm (argV, text_file, keys);

    assert (input  != nullptr);
    assert (output != nullptr);

    text_file->file_length = find_length_of_file (input);
    assert (text_file->file_length != 0);

    creat_buffer (text_file, input);

    fclose (input);

    reversed_compilation (text_file, output);

    fclose (output);

}

FILE* open_file_disasm (char* argV[], text* text_file, keys* keys)
{
    FILE* output = nullptr;

    if (keys->out > 0)
    {
        output = fopen (argV[keys->out], "w");
    }
    else
    {


        if (keys->in > 0)
        {
            output = add_postfix_disasm (argV, keys);
        }
        else
        {
            char standart_name_out[] = "Program files\\ByteCodeReversed.asm";

            output = fopen (standart_name_out, "w");

        }

    }
    return output;
}

FILE* add_postfix_disasm (char* argV[], keys* keys)
{
    FILE* output = nullptr;
    size_t len = strlen(argV[keys->in]);
    char standart_name_out[len + 8];
    for (size_t i = 0; i < len - 4; i++)
    {
        if (*(argV[keys->in] + i) == '.')
        {
            if (strcmp (argV[keys->in] + i, ".code") == 0)
            {
                strcpy (standart_name_out + i, "Reversed.asm");
                break;
            }
        }
        standart_name_out[i] = *(argV[keys->in] + i);
    }
    output = fopen (standart_name_out, "w");
    return output;
}

void creat_buffer (text* text_file, FILE* input)
{
    text_file->pointer_on_buffer = (char*) calloc (text_file->file_length + 1, sizeof(char));
    assert (text_file->pointer_on_buffer != nullptr);

    read_file (input, text_file->pointer_on_buffer, &(text_file->file_length));
}

void reversed_compilation (text* text_file, FILE* output)
{
    if (strncmp (text_file->pointer_on_buffer + 3, "1.51", 4) != 0)
    {
        printf ("Assembler and disassembler are incompatible\n");
    }
    else
    {
        size_t ofs = size_of_header;
        for (size_t current_command = 0; current_command < text_file->file_length; current_command++)
        {
            switch ((unsigned char) text_file->pointer_on_buffer[ofs])
            {
                #define DEF_CMD(name, num, asm_cmd, disasm_cmd, cpu_cmd)\
                                                                        \
                case CMD_##name: disasm_cmd                             \
                                 break;


                #include "Commands.hpp"

                #undef DEF_CMD
            }
        }
    }
}
