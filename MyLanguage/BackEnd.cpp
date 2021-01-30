#include "BackEnd.hpp"

int main ()
{
    tree* program_tree = ReadTreeFromTxt ();

    //DrawTree (program_tree->root);

    TranslateToAsm (program_tree);

    return 0;
}

#define FUNC_REACHED printf ("%s reached\n", __FUNCTION__);

void TranslateToAsm (tree* program_tree, char* name_of_output_file)
{
    assert (program_tree       != nullptr);
    assert (program_tree->root != nullptr);

    FUNC_REACHED

    FILE* output_file = (name_of_output_file == nullptr) ? (fopen (STANDART_OUTPUT_ASM_FILE, "w")) : (fopen (name_of_output_file, "w"));

    struct translate_info translate_info = {};

    translate_info.variables_array = (char**) calloc (NUM_OF_REGISTER, sizeof (char*));

    translate_info.num_for_labels = 1;

    TranslateMainNode (program_tree->root, output_file, &translate_info);

    free (translate_info.variables_array);
}


#define CHECK_FOR_INCORRECTNESS(check)                           \
                                                                 \
    if (check)                                                   \
    {                                                            \
        printf ("Incorrect node is given to %s\n", __FUNCTION__);\
        return;                                                  \
    }

void TranslateMainNode (tree_node* current_node, FILE* output_file, translate_info* translate_info)
{
    assert (current_node   != nullptr);
    assert (output_file    != nullptr);
    assert (translate_info != nullptr);

    FUNC_REACHED

    printf ("--------------------------------------\n");

    CHECK_FOR_INCORRECTNESS (current_node->left_child == nullptr) //oshibka

    CHECK_FOR_INCORRECTNESS (current_node->left_child->node_type != COMMAND) //oshibka

    //fprintf (output_file, "%s:\n", current_node->key.name_ptr);

    TranslateCommandNode (current_node->left_child, output_file, translate_info);

    if (current_node->right_child != nullptr)
    {
        CHECK_FOR_INCORRECTNESS (current_node->right_child->node_type != USER_FUNC) //oshibka

        TranslateUserFuncNode (current_node->right_child, output_file, translate_info);
    }
}

void TranslateNode (tree_node* current_node, FILE* output_file, translate_info* translate_info)
{
    assert (current_node   != nullptr);
    assert (output_file    != nullptr);
    assert (translate_info != nullptr);

    FUNC_REACHED

    switch (current_node->node_type)
    {
        #define TREE_NODE_TYPE(enum_type, hex_code, name_type, func_lettering)                      \
                                                                                                    \
        case enum_type: Translate##func_lettering##Node (current_node, output_file, translate_info);\
                        break;

        #include "CodeGeneration\TreeNodeType.txt"

        #undef TREE_NODE_TYPE

        default: printf ("Unknown node type in TranslateNode\n");
                 break;
    }
}

void TranslateSysFuncNode (tree_node* current_node, FILE* output_file, translate_info* translate_info)
{
    assert (current_node   != nullptr);
    assert (output_file    != nullptr);
    assert (translate_info != nullptr);

    FUNC_REACHED

    switch (current_node->key.sys_func)
    {
        #define SYS_FUNC(enum_type, lettering, func_type)                                      \
                                                                                               \
        case enum_type: Translate##func_type##Node (current_node, output_file, translate_info);\
                        break;

        #include "CodeGeneration\SysFunc.txt"

        #undef SYS_FUNC

        default: printf ("Unknown type of system function in TranslateSysFuncNode\n");
                 break;
    }
}

void TranslateAssignmentNode (tree_node* current_node, FILE* output_file, translate_info* translate_info)
{
    assert (current_node   != nullptr);
    assert (output_file    != nullptr);
    assert (translate_info != nullptr);

    FUNC_REACHED

    /*if ((current_node->left_child == nullptr) || (current_node->right_child == nullptr))
    {
        printf ("Assignment node without 2 children in tree\n");
        return;
    }
    else
    {
        if ((current_node->left_child->node_type  != VAR) || !IsExpression (current_node->right_child))
        {
            printf ("Assignment node with incorrect children in tree);
            return;
        }
    }*/

    CHECK_FOR_INCORRECTNESS ((current_node->left_child == nullptr) || (current_node->right_child == nullptr))

    CHECK_FOR_INCORRECTNESS ((current_node->left_child->node_type  != VAR) || !IsExpression (current_node->right_child))

    TranslateExpression (current_node->right_child, output_file, translate_info);

    fprintf (output_file, "pop ");

    TranslateVarNode (current_node->left_child, output_file, translate_info);
}

bool IsExpression (tree_node* current_node)
{
    assert (current_node   != nullptr);

    FUNC_REACHED

    return ((current_node->node_type == VAR) || (current_node->node_type == NUM) || (current_node->node_type == MATH_OP));
}

void TranslateExpression (tree_node* current_node, FILE* output_file, translate_info* translate_info)
{
    assert (current_node   != nullptr);
    assert (output_file    != nullptr);
    assert (translate_info != nullptr);

    /*if (!IsExpressionNode (current_node)
    {
        printf ("Incorrect node given to TranslateExpression\n");
        return;
    }*/

    FUNC_REACHED

    CHECK_FOR_INCORRECTNESS (!IsExpression (current_node))

    if (current_node->left_child != nullptr)
    {
        TranslateExpression (current_node->left_child, output_file, translate_info);

        if (current_node->right_child != nullptr) TranslateExpression (current_node->right_child, output_file, translate_info);
    }

    switch (current_node->node_type)
    {
        case MATH_OP: switch (current_node->key.math_op)
                      {
                          #define MATH_OP(operation, lettering)                                 \
                                                                                                \
                          case operation: fprintf (output_file, "%s\n", my_strlwr (#operation));\
                                          break;

                          #include "CodeGeneration\MathOp.txt"

                          #undef MATH_OP

                          default: printf ("Unknown type of operation in node given to TranslateExpression\n");
                                   break;
                      }
                      break;

        case NUM:     fprintf (output_file, "push %lg\n", current_node->key.val);
                      break;

        case VAR:     fprintf (output_file, "push ");
                      TranslateVarNode (current_node, output_file, translate_info);
                      break;

        default:      printf ("Incorrect type of node given to TranslateExpression\n");
                      break;
    }
}

void TranslateVarNode (tree_node* current_node, FILE* output_file, translate_info* translate_info)
{
    assert (current_node   != nullptr);
    assert (output_file    != nullptr);
    assert (translate_info != nullptr);

    FUNC_REACHED

    /*if (current_node->node_type != VAR)
    {
        printf ("Incorrect node is given to TranslateVarNode\n");
        return;
    }*/

    CHECK_FOR_INCORRECTNESS (current_node->node_type != VAR)

    int num_of_reg = GetNumOfReg (current_node, translate_info);

    fprintf (output_file, "r%cx\n", 'a' + num_of_reg);
}

int GetNumOfReg (tree_node* current_node, translate_info* translate_info)
{
    assert (current_node   != nullptr);
    assert (translate_info != nullptr);

    FUNC_REACHED

    int num_of_reg = 0;

    for (int i = 0; i < NUM_OF_REGISTER; i++)
    {
        if (translate_info->variables_array[i] != nullptr)
        {
            if (strcmp (translate_info->variables_array[i], current_node->key.name_ptr) == 0)
            {
                num_of_reg = i;
                break;
            }
        }
        else
        {
            translate_info->variables_array[i] = current_node->key.name_ptr;
            num_of_reg = i;
            break;
        }
    }

    return num_of_reg;
}

void TranslateRequiredNode (tree_node* current_node, FILE* output_file, translate_info* translate_info)
{
    assert (current_node   != nullptr);
    assert (output_file    != nullptr);
    assert (translate_info != nullptr);

    FUNC_REACHED

    /*if (current_node->left_child == nullptr)
    {
        printf ("Incorrect node given to TranslateRequiredNode\n");
        return;
    }

    if (current_node->left_child->node_type != VAR)
    {
        printf ("Incorrect node given to TranslateRequiredNode\n");
        return;
    }*/

    CHECK_FOR_INCORRECTNESS (current_node->left_child == nullptr)

    CHECK_FOR_INCORRECTNESS (current_node->left_child->node_type != VAR)

    fprintf (output_file, "in\npop ");

    TranslateVarNode (current_node->left_child, output_file, translate_info);

    if (current_node->right_child != nullptr)
    {
        CHECK_FOR_INCORRECTNESS ((current_node->right_child->node_type != SYS_FUNC) || (current_node->right_child->key.sys_func != REQUIRED))

        TranslateRequiredNode (current_node->right_child, output_file, translate_info);
    }
}

void TranslateSeeNode (tree_node* current_node, FILE* output_file, translate_info* translate_info)
{
    assert (current_node   != nullptr);
    assert (output_file    != nullptr);
    assert (translate_info != nullptr);

    FUNC_REACHED

    //if ((current_node->right_child != nullptr) || (current_node->left_child == nullptr)) //oshibka

    CHECK_FOR_INCORRECTNESS ((current_node->right_child != nullptr) || (current_node->left_child == nullptr))

    //if (current_node->left_child->node_type != VAR) //oshibka

    CHECK_FOR_INCORRECTNESS (current_node->left_child->node_type != VAR)

    fprintf (output_file, "call %s\n", current_node->left_child->key.name_ptr);
}

void TranslateWriteNode (tree_node* current_node, FILE* output_file, translate_info* translate_info)
{
    assert (current_node   != nullptr);
    assert (output_file    != nullptr);
    assert (translate_info != nullptr);

    FUNC_REACHED

    CHECK_FOR_INCORRECTNESS (current_node->left_child == nullptr)

    CHECK_FOR_INCORRECTNESS (!IsExpression (current_node->left_child))

    TranslateExpression (current_node->left_child, output_file, translate_info);

    fprintf (output_file, "out\n");

    if (current_node->right_child != nullptr)
    {
        CHECK_FOR_INCORRECTNESS ((current_node->right_child->node_type != SYS_FUNC) || (current_node->right_child->key.sys_func != WRITE))

        TranslateWriteNode (current_node->right_child, output_file, translate_info);
    }
}

void TranslateReturnNode (tree_node* current_node, FILE* output_file, translate_info* translate_info)
{
    assert (current_node   != nullptr);
    assert (output_file    != nullptr);
    assert (translate_info != nullptr);

    FUNC_REACHED

    fprintf (output_file, "ret\n");
}

void TranslateHltNode (tree_node* current_node, FILE* output_file, translate_info* translate_info)
{
    assert (current_node   != nullptr);
    assert (output_file    != nullptr);
    assert (translate_info != nullptr);

    FUNC_REACHED

    fprintf (output_file, "hlt\n");
}

void TranslateMathOpNode (tree_node* current_node, FILE* output_file, translate_info* translate_info)
{
    assert (current_node   != nullptr);
    assert (output_file    != nullptr);
    assert (translate_info != nullptr);

    FUNC_REACHED

    TranslateExpression (current_node, output_file, translate_info);
}

void TranslateComparingNode (tree_node* current_node, FILE* output_file, translate_info* translate_info, size_t num)
{
    assert (current_node   != nullptr);
    assert (output_file    != nullptr);
    assert (translate_info != nullptr);

    FUNC_REACHED

    CHECK_FOR_INCORRECTNESS (num == 0)

    CHECK_FOR_INCORRECTNESS ((current_node->left_child == nullptr) || (current_node->right_child == nullptr))

    CHECK_FOR_INCORRECTNESS ((!IsExpression (current_node->left_child)) || (!IsExpression (current_node->left_child)))

    TranslateExpression (current_node->left_child, output_file, translate_info);

    TranslateExpression (current_node->right_child, output_file, translate_info);

    switch (current_node->key.comparing)
    {
        case ABOVE_OR_EQUAL: fprintf (output_file, "jb :%I64u\n", num);    //
                             break;

        case BELOW_OR_EQUAL: fprintf (output_file, "ja :%I64u\n", num);
                             break;

        case ABOVE:          fprintf (output_file, "jbe :%I64u\n", num);
                             break;

        case BELOW:          fprintf (output_file, "jae :%I64u\n", num);
                             break;

        case EQUAL:          fprintf (output_file, "jne :%I64u\n", num);
                             break;

        case NOT_EQUAL:      fprintf (output_file, "je :%I64u\n", num);
                             break;

        default:             printf ("Unknown type of comparing in node given to TranslateComparingNode\n");
                             break;
    }
}

void TranslateConditionNode (tree_node* current_node, FILE* output_file, translate_info* translate_info)
{
    assert (current_node   != nullptr);
    assert (output_file    != nullptr);
    assert (translate_info != nullptr);

    FUNC_REACHED

    CHECK_FOR_INCORRECTNESS ((current_node->left_child == nullptr) || (current_node->right_child == nullptr)) // oshibka

    CHECK_FOR_INCORRECTNESS (current_node->left_child->node_type != COMPARING) //oshibka

    size_t num = translate_info->num_for_labels;

    TranslateComparingNode (current_node->left_child, output_file, translate_info, num);

    if (current_node->right_child->node_type == COMMAND)
    {
        translate_info->num_for_labels++;

        TranslateCommandNode (current_node->right_child, output_file, translate_info);

        fprintf (output_file, "%I64u:\n", num); //
    }
    else
    {
        if (current_node->right_child->node_type == CONDITION)
        {
            CHECK_FOR_INCORRECTNESS ((current_node->right_child->left_child == nullptr) || (current_node->right_child->right_child == nullptr)) //oshibka

            CHECK_FOR_INCORRECTNESS ((current_node->right_child->left_child->node_type  != COMMAND)  ||
                                     (current_node->right_child->right_child->node_type != COMMAND)) //oshibka

            translate_info->num_for_labels = translate_info->num_for_labels + 2;

            TranslateCommandNode (current_node->right_child->left_child, output_file, translate_info);

            fprintf (output_file, "jmp :%I64u\n%I64u:\n", num + 1, num); //

            TranslateCommandNode (current_node->right_child->right_child, output_file, translate_info);

            fprintf (output_file, "%I64u:\n", num + 1);
        }
        else
        {
            printf ("Incorrect right child in condition node given to TranslateConditionNode\n");
            return;
        }
    }
}

void TranslateUserFuncNode (tree_node* current_node, FILE* output_file, translate_info* translate_info)
{
    assert (current_node   != nullptr);
    assert (output_file    != nullptr);
    assert (translate_info != nullptr);

    FUNC_REACHED

    printf ("--------------------------------------\n");

    CHECK_FOR_INCORRECTNESS (current_node->left_child == nullptr) //oshibka

    CHECK_FOR_INCORRECTNESS (current_node->left_child->node_type != COMMAND) //oshibka

    fprintf (output_file, "%s:\n", current_node->key.name_ptr);

    TranslateCommandNode (current_node->left_child, output_file, translate_info);

    if (current_node->right_child != nullptr)
    {
        CHECK_FOR_INCORRECTNESS (current_node->right_child->node_type != USER_FUNC) //oshibka

        TranslateUserFuncNode (current_node->right_child, output_file, translate_info);
    }
}

/*void TranslateVarNode (tree_node* current_node, FILE* output_file, translate_info* translate_info)
{
    assert (current_node   != nullptr);
    assert (output_file    != nullptr);
    assert (translate_info != nullptr);

    TranslateExpression (current_node, output_file, translate_info);
}*/

void TranslateNumNode (tree_node* current_node, FILE* output_file, translate_info* translate_info)
{
    assert (current_node   != nullptr);
    assert (output_file    != nullptr);
    assert (translate_info != nullptr);

    FUNC_REACHED

    TranslateExpression (current_node, output_file, translate_info);
}

void TranslateCommandNode (tree_node* current_node, FILE* output_file, translate_info* translate_info)
{
    assert (current_node   != nullptr);
    assert (output_file    != nullptr);
    assert (translate_info != nullptr);

    FUNC_REACHED

    CHECK_FOR_INCORRECTNESS (current_node->left_child == nullptr) //oshibka

    TranslateNode (current_node->left_child, output_file, translate_info);

    if (current_node->right_child != nullptr) TranslateNode (current_node->right_child, output_file, translate_info);
}

void TranslateLoopNode (tree_node* current_node, FILE* output_file, translate_info* translate_info)
{
    assert (current_node   != nullptr);
    assert (output_file    != nullptr);
    assert (translate_info != nullptr);

    FUNC_REACHED

    CHECK_FOR_INCORRECTNESS ((current_node->left_child == nullptr) || (current_node->right_child == nullptr)) //oshibka

    CHECK_FOR_INCORRECTNESS ((current_node->left_child->node_type != COMPARING) || (current_node->right_child->node_type != COMMAND)) //oshibka

    size_t num = translate_info->num_for_labels;

    translate_info->num_for_labels = translate_info->num_for_labels + 2;

    fprintf (output_file, "%I64u:\n", num + 1);

    TranslateComparingNode (current_node->left_child, output_file, translate_info, num);

    TranslateCommandNode (current_node->right_child, output_file, translate_info);

    fprintf (output_file, "jmp :%I64u\n%I64u:\n", num + 1, num);
}

void TranslateBlankNode (tree_node* current_node, FILE* output_file, translate_info* translate_info)
{
    assert (current_node   != nullptr);
    assert (output_file    != nullptr);
    assert (translate_info != nullptr);

    FUNC_REACHED

    printf ("Blank node shouldn't exist in program tree\n");

    return;
}

#undef CHECK_FOR_INCORRECTNESS

/*char* FromUpperCaseToPascalCase (char* string)
{
    assert (string != nullptr);

    if (string[0] == '\0') return nullptr;

    char* pascal_case = (char*) calloc (strlen (string), sizeof (char));

    pascal_case[0] = string[0];

    size_t pascal_case_index = 1;
    size_t string_index      = 1;

    while (string[string_index] != '\0')
    {
        if (string[string_index] == '_')
        {
            string_index++;
            if (string[string_index] == '\0') return nullptr;
            pascal_case[pascal_case_index] = string[string_index];
        }
        else
        {
            pascal_case[pascal_case_index] = string[string_index] + ('a' - 'A');
        }

        string_index++;
        pascal_case_index++;
    }
    return pascal_case;
}*/

char* my_strlwr (char* string)
{
    char* lower_string = (char*) calloc (strlen (string), sizeof (char));

    size_t i = 0;

    while (string[i] != '\0')
    {
        if ((string[i] >= 'A') && (string[i] <= 'Z')) lower_string[i] = string[i] + ('a' - 'A');
        else                                          lower_string[i] = string[i];

        i++;
    }

    return lower_string;
}

#undef FUNC_REACHED
