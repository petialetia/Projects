#include "Header.hpp"


void process_files_cpu (int argC, char* argV[], text* text_file, keys* hamlet_keys);

void create_buffer (text* text_file, FILE* input);

CPU_struct* construct_cpu_struct (text* text_file);

void evaluation (CPU_struct* cpu);

void destroy_cpu (CPU_struct* cpu);


main (int argC, char* argV[])
{
    assert (argC != 0);
    assert (argV != nullptr);


    text* cpu_file = construct_text ();

    assert (cpu_file != nullptr);

    keys* cpu_keys = (keys*) calloc (1, sizeof(keys));
    assert (cpu_keys != nullptr);

    process_files_cpu (argC, argV, cpu_file, cpu_keys);

    destroy_text (cpu_file);
    destroy_keys (cpu_keys);
}

void process_files_cpu (int argC, char* argV[], text* text_file, keys* keys)
{
    keys->in = get_arg_val (argC, argV, "in");

    FILE* input = (keys->in != 0) ? (fopen (argV[keys->in], "rb")) : (fopen ("Program files\\ByteCode.code", "rb"));

    assert (input  != nullptr);

    text_file->file_length = find_length_of_file (input);
    assert (text_file->file_length != 0);

    create_buffer (text_file, input);

    fclose (input);

    CPU_struct* cpu = construct_cpu_struct(text_file);

    evaluation (cpu);

    destroy_cpu (cpu);
}

void create_buffer (text* text_file, FILE* input)
{
    text_file->pointer_on_buffer = (char*) calloc (text_file->file_length + 1, sizeof(char));
    assert (text_file->pointer_on_buffer != nullptr);

    read_file (input, text_file->pointer_on_buffer, &(text_file->file_length));
}

CPU_struct* construct_cpu_struct (text* text_file)
{
    CPU_struct* cpu = (CPU_struct*) calloc (1, sizeof (CPU_struct));

    Construct (&cpu->stack);

    Construct (&cpu->stack_of_calls);

    cpu->code = text_file->pointer_on_buffer;

    return cpu;
}

void evaluation (CPU_struct* cpu)
{
    if (strncmp (cpu->code + 3, "1.51", 4) != 0)
    {
        printf ("Assembler and processor are incompatible\n");
    }
    else
    {

        cpu->ofs = size_of_header;

        for (;;)
        {
            Elem_t val1 = NAN;
            Elem_t val2 = NAN;
            switch ((unsigned char) cpu->code[cpu->ofs])
            {
                #define DEF_CMD(name, num, asm_cmd, disasm_cmd, cpu_cmd)\
                                                                        \
                case CMD_##name: cpu_cmd                                \
                                 break;


                #include "Commands.hpp"

                #undef DEF_CMD

            }
        }
    }
}

void destroy_cpu (CPU_struct* cpu)
{
    Destroy (&cpu->stack);

    Destroy (&cpu->stack_of_calls);

    free (cpu);
}


