#define standart_asm_cmd(name)\
{                             \
    code[ofs] = CMD_##name;   \
    ofs += sizeof (char);     \
}

#define standart_disasm_cmd(name)   \
{                                   \
    fprintf (output, "%s\n", #name);\
    ofs += sizeof (char);           \
}

#define standart_binary_operator_cpu(oper) \
{                                          \
    val2 = Pop (&cpu->stack);              \
    val1 = Pop (&cpu->stack);              \
    Push (&cpu->stack, val1 oper val2);    \
    cpu->ofs += sizeof (char);             \
}

#define standart_jump_cmd_asm(title)                                       \
{                                                                          \
                                                                           \
    ptr = strtok (nullptr, " ");                                           \
                                                                           \
    if (ptr == nullptr)                                                    \
    {                                                                      \
        printf ("No arguments for %s on line number %u\n", #title, line+1);\
        ofs = 0;                                                           \
        break;                                                             \
    }                                                                      \
                                                                           \
    else                                                                   \
                                                                           \
    {                                                                      \
        if (ptr[0] == ':')                                                 \
        {                                                                  \
            process_label_asm(title)                                       \
        }                                                                  \
        else                                                               \
        {                                                                  \
            standart_asm_cmd(title)                                        \
                                                                           \
            code[ofs] = atoi(ptr);                                         \
                                                                           \
            ofs += sizeof (char);                                          \
        }                                                                  \
    }                                                                      \
}

#define process_label_asm(title)                                  \
{                                                                 \
    standart_asm_cmd(title)                                       \
                                                                  \
    code[ofs] = -1;                                               \
                                                                  \
    for (size_t k = 0; k < max_num_of_labels; k++)                \
    {                                                             \
        if (labels[k].name == nullptr)                            \
        {                                                         \
            break;                                                \
        }                                                         \
                                                                  \
        if (strcmp (ptr+1, labels[k].name) == 0)                  \
        {                                                         \
            code[ofs] = (unsigned char) labels[k].addr;           \
            break;                                                \
        }                                                         \
                                                                  \
                                                                  \
    }                                                             \
                                                                  \
    ofs += sizeof (char);                                         \
}


#define standart_jump_cmd_disasm(title)                                                         \
{                                                                                               \
    ofs += sizeof (char);                                                                       \
    fprintf (output, "%s %d\n", #title, *((unsigned char*) text_file->pointer_on_buffer + ofs));\
    ofs += sizeof (char);                                                                       \
    current_command++;                                                                          \
}

#define conditional_jmp_cpu(cond)                                           \
{                                                                           \
    val1 = Pop (&cpu->stack);                                               \
    val2 = Pop (&cpu->stack);                                               \
    if (val2 cond val1)                                                     \
    {                                                                       \
        cpu->ofs = size_of_header + (unsigned char) cpu->code[cpu->ofs + 1];\
    }                                                                       \
    else                                                                    \
    {                                                                       \
        cpu->ofs += 2*sizeof(char);                                         \
    }                                                                       \
}


DEF_CMD (in,1,standart_asm_cmd(in),standart_disasm_cmd(in),
{
    printf ("Enter number\n");
    scanf ("%lf", &val1);
    Push (&cpu->stack, val1);
    cpu->ofs += sizeof (char);
})

DEF_CMD (out,2,standart_asm_cmd(out),standart_disasm_cmd(out),
{
    printf ("%lf\n", Pop (&cpu->stack));
    cpu->ofs += sizeof (char);
})

DEF_CMD (pop,5,
{
    ptr = strtok (nullptr, " ");

    if (ptr != nullptr)
    {
        standart_asm_cmd(pop)

        sscanf (ptr, "%s", reg_name);

        code[ofs] = reg_name[1] - 97;

        ofs += sizeof (char);
    }
    else
    {
        printf ("No arguments for pop on line number %u\n", line+1);
        ofs = 0;
        break;
    }
},
{
    ofs += sizeof (char);
    fprintf (output, "pop r%cx\n", text_file->pointer_on_buffer[ofs] + 97);
    ofs += sizeof (char);
    current_command++;
},
{
    cpu->ofs += sizeof (char);
    cpu->reg[cpu->code[cpu->ofs]] = Pop (&cpu->stack);
    cpu->ofs += sizeof (char);
})

DEF_CMD (push,3,
{
    ofs += sizeof (char);
    ptr = strtok (nullptr, " ");

    if (ptr == nullptr)
    {
        printf ("No arguments for push on line number %u\n", line+1);
        ofs = 0;
        break;
    }
    else
    {
        size_t scan = sscanf (ptr, "%lg", (double*)(code + ofs));

        if (scan > 0)
        {
            code[ofs - sizeof (char)] = CMD_push;
            ofs += sizeof (double);
        }
        else
        {
            sscanf (ptr, "%s", reg_name);

            code[ofs] = reg_name[1] - 97;
            code[ofs - sizeof (char)] = CMD_push_reg;

            ofs += sizeof (char);
        }
    }
},
{
    ofs += sizeof (char);
    fprintf (output, "push %lf\n", *(double*)(text_file->pointer_on_buffer + ofs));
    ofs += sizeof (double);
    current_command += sizeof (double);
},
{
    cpu->ofs += sizeof (char);
    Push (&cpu->stack, *(double*)(cpu->code + cpu->ofs));
    cpu->ofs += sizeof (double);
})

DEF_CMD (hlt,9, standart_asm_cmd(hlt), standart_disasm_cmd(hlt),
{
    return;
})

DEF_CMD (add,112,standart_asm_cmd(add),standart_disasm_cmd(add), standart_binary_operator_cpu (+))

DEF_CMD (sub,110,standart_asm_cmd(sub),standart_disasm_cmd(sub), standart_binary_operator_cpu (-))

DEF_CMD (mul,122,standart_asm_cmd(mul),standart_disasm_cmd(mul), standart_binary_operator_cpu (*))

DEF_CMD (div,221,standart_asm_cmd(div),standart_disasm_cmd(div),
{
    val2 = Pop (&cpu->stack);

    if (val2 == 0)
    {
        printf ("Division by 0 is prohibited\n");
        return;
    }
    val1 = Pop (&cpu->stack);
    Push (&cpu->stack, val1 / val2);
    cpu->ofs += sizeof (char);
})

DEF_CMD (pow,238,standart_asm_cmd(pow),standart_disasm_cmd(pow),
{
    val2 = Pop (&cpu->stack);
    val1 = Pop (&cpu->stack);
    Push (&cpu->stack, pow (val1, val2));
    cpu->ofs += sizeof (char);
})

DEF_CMD (sqrt,93,standart_asm_cmd(sqrt),standart_disasm_cmd(sqrt),
{
    val1 = Pop (&cpu->stack);
    Push (&cpu->stack, sqrt (val1));
    cpu->ofs += sizeof (char);
})

DEF_CMD (jmp, 10, standart_jump_cmd_asm(jmp), standart_jump_cmd_disasm(jmp),
{
    cpu->ofs = size_of_header + (unsigned char) cpu->code[cpu->ofs + 1];
})

DEF_CMD (ja, 11, standart_jump_cmd_asm(ja), standart_jump_cmd_disasm(ja), conditional_jmp_cpu(>))

DEF_CMD (jae, 12, standart_jump_cmd_asm(jae), standart_jump_cmd_disasm(jae), conditional_jmp_cpu(>=))

DEF_CMD (jb, 13, standart_jump_cmd_asm(jb), standart_jump_cmd_disasm(jb),conditional_jmp_cpu(<))

DEF_CMD (jbe, 14, standart_jump_cmd_asm(jbe), standart_jump_cmd_disasm(jbe), conditional_jmp_cpu(<=))

DEF_CMD (je, 15, standart_jump_cmd_asm(je), standart_jump_cmd_disasm(je), conditional_jmp_cpu(==))

DEF_CMD (jne, 16, standart_jump_cmd_asm(jne), standart_jump_cmd_disasm(jne), conditional_jmp_cpu(!=))

DEF_CMD (call, 17,
{
    ptr = strtok (nullptr, " ");

    if (ptr == nullptr)
    {
        printf ("No arguments for call on line number %u\n", line+1);
        ofs = 0;
        break;
    }

    else

    {
        standart_asm_cmd(call)
        int offset = 0;
        size_t scan = sscanf (ptr, "%d", &offset);

        if (scan == 0)
        {
            code[ofs] = -1;

            for (size_t i = 0; i < max_num_of_labels; i++)
            {
                if (labels[i].name == nullptr)
                {
                    break;
                }

                if (strcmp (ptr, labels[i].name) == 0)
                {
                    code[ofs] = labels[i].addr;
                    break;
                }


            }
        }

        else
        {
            code[ofs] = offset;
        }

        ofs += sizeof (char);
    }
},

standart_jump_cmd_disasm (call),

{
    Push (&cpu->stack_of_calls, cpu->ofs + 2*sizeof(char) - size_of_header);
    cpu->ofs = size_of_header + cpu->code[cpu->ofs + 1];
})

DEF_CMD (ret, 18, standart_asm_cmd(ret), standart_disasm_cmd(ret),
{
    cpu->ofs = Pop (&cpu->stack_of_calls) + size_of_header;
})

DEF_CMD (push_reg,4,
{
    printf ("It's a system command on line %u, use standard push\n", line+1);
    printf ("%s\n", ptr);
    ofs = 0;
    break;
},
{
    ofs += sizeof (char);
    fprintf (output, "push r%cx\n", text_file->pointer_on_buffer[ofs] + 97);
    ofs += sizeof (char);
    current_command++;
},
{
    cpu->ofs += sizeof (char);
    Push (&cpu->stack, cpu->reg[cpu->code[cpu->ofs]]);
    cpu->ofs += sizeof (char);
})

#undef standart_asm_cmd

#undef standart_disasm_cmd

#undef standart_binary_operator_cpu

#undef standart_jump_cmd_asm

#undef process_label_asm

#undef standart_jump_cmd_disasm

#undef conditional_jmp_cpu

