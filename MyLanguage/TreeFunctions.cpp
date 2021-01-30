#include "TreeFunctions.hpp"

//int main ()
//{
    //printf ("HEY\n");

    /*tree* my_tree = NewTree ();

    my_tree->root = CreateTreeNode (BLANK, 0.0);

    tree_node* current_node = my_tree->root;

    current_node->left_child = CreateTreeNode (MATH_OP, MUL);

    current_node->right_child = CopySubtree (current_node->left_child);

    //printf ("%d\n", current_node->left_child->key.math_op);

    //printf ("%d\n", current_node->right_child->key.math_op);

    current_node->left_child->left_child = CreateTreeNode (SYS_FUNC, WRITE);

    current_node->left_child->right_child = CreateTreeNode (COMPARING, EQUAL);

    current_node->right_child->left_child = CreateTreeNode (CONDITION, IF);

    current_node->right_child->right_child = CreateTreeNode (NUM, 79.5);

    current_node->right_child->right_child->right_child = CreateTreeNode (VAR, "rainbow");

    current_node->right_child->right_child->left_child = CreateTreeNode (COMMAND, 0.0);

    current_node->left_child->left_child->right_child = CreateTreeNode (LOOP, 0.0);

    current_node->right_child->left_child->right_child = CreateTreeNode (USER_FUNC, "la-la");

    current_node->left_child->left_child->right_child->right_child = CreateTreeNode (SYS_FUNC, ASSIGNMENT);

    DrawTree (current_node);

    //SaveTreeInTxt (my_tree);

    tree* read_tree = ReadTreeFromTxt ();

    SaveTreeInTxt (read_tree);

    read_tree = ReadTreeFromTxt ();

    //DrawTree (read_tree->root);

    DestroyTree (my_tree);*/

    /*tree* test_tree = NewTree ();

    test_tree->root = CreateTreeNode (BLANK, 0.0);

    test_tree->root->right_child = CreateTreeNode (LOOP, 0.0);

    test_tree->root->left_child = CreateTreeNode (COMMAND, 0.0);

    test_tree->root->left_child->left_child = CreateTreeNode (USER_FUNC, "Testim vrode ok");

    test_tree->root->left_child->right_child = CreateTreeNode (SYS_FUNC, ASSIGNMENT);

    test_tree->root->right_child->left_child = CreateTreeNode (MATH_OP, POW);

    test_tree->root->right_child->right_child = CreateTreeNode (VAR, "rax");

    test_tree->root->right_child->right_child->left_child = CreateTreeNode (NUM, 85.26);

    test_tree->root->right_child->right_child->right_child = CreateTreeNode (CONDITION, IF_ELSE);

    test_tree->root->left_child->left_child->left_child = CreateTreeNode (COMPARING, ABOVE);

    //DrawTree (test_tree->root);

    SaveTreeInTxt (test_tree);

    tree* read_tree = ReadTreeFromTxt ();

    //read_tree->root->right_child->node_type = BLANK;

    DrawTree (read_tree->root);

    DestroyTree (read_tree);

    DestroyTree (test_tree);
}*/

tree* NewTree ()
{
    return (tree*) calloc (1, sizeof (tree));
}

#define TREE_KEY(var_type, name)                                                                                 \
                                                                                                                 \
tree_node* CreateTreeNode (enum TreeNodeType tree_node_type, var_type key,                                       \
                           tree_node* ptr_on_left_child, tree_node* ptr_on_right_child, tree_node* ptr_on_parent)\
{                                                                                                                \
    tree_node* ptr_on_tree_node = (tree_node*) calloc (1, sizeof (tree_node));                                   \
                                                                                                                 \
    ptr_on_tree_node->node_type = tree_node_type;                                                                \
                                                                                                                 \
    ptr_on_tree_node->key.name = key;                                                                            \
                                                                                                                 \
    ptr_on_tree_node->left_child = ptr_on_left_child;                                                            \
                                                                                                                 \
    ptr_on_tree_node->right_child = ptr_on_right_child;                                                          \
                                                                                                                 \
    ptr_on_tree_node->parent = ptr_on_parent;                                                                    \
                                                                                                                 \
    return ptr_on_tree_node;                                                                                     \
}

#include "CodeGeneration\TreeKeyUnion.txt"

#undef TREE_KEY

tree_node* CopySubtree (tree_node* node)
{
    if (node != nullptr) return CreateTreeNode (node->node_type, node->key.val, CopySubtree (node->left_child), CopySubtree (node->right_child), node->parent);

    return nullptr;
}

tree_node* CopyTreeNode (tree_node* node)
{
    assert (node != nullptr);

    return CreateTreeNode (node->node_type, node->key.val, node->left_child, node->right_child, node->parent);
}

void DrawTree (tree_node* root, char* name_of_graphviz_file)
{
    assert (root != nullptr);

    FILE* graphviz_file = (name_of_graphviz_file == nullptr) ? (fopen (STANDART_GRAPHVIZ_FILE, "w")) : (fopen (name_of_graphviz_file, "W"));

    assert (graphviz_file != nullptr);

    fprintf (graphviz_file, "digraph PL\n");
    fprintf (graphviz_file, "{\nrankdir=HR;\n");

    struct node_type_info node_type_info[NUM_OF_NODE_TYPES] = {};

    FillNodeTypeInfo (node_type_info);

    //printf ("nushe\n");

    PrintNodesDescription (graphviz_file, root, node_type_info);

    fprintf (graphviz_file, "}\n");

    fclose (graphviz_file);

    system ("dot -Tpng Graph\\GraphvizFile.txt -o Graph\\Graph.png");
}

node_type_info* FillNodeTypeInfo (node_type_info* node_type_info)
{
    #define TREE_NODE_TYPE(TYPE, HEX_CODE, NODE_NAME, FUNC_NAME)\
                                                                \
    node_type_info[TYPE].hex_code = #HEX_CODE;                  \
                                                                \
    node_type_info[TYPE].name_of_node_type = #TYPE;             \
                                                                \
    node_type_info[TYPE].name_type = NODE_NAME;

    #include "CodeGeneration\TreeNodeType.txt"

    #undef TREE_NODE_TYPE

    return node_type_info;
}

void PrintNodesDescription (FILE* graphviz_file, tree_node* current_node, node_type_info* node_type_info)
{
    assert (graphviz_file  != nullptr);
    assert (current_node   != nullptr);
    assert (node_type_info != nullptr);

    fprintf (graphviz_file, "\"%p\" [style=\"filled\", fillcolor=\"#", current_node);

    fprintf (graphviz_file, "%s", node_type_info[current_node->node_type].hex_code);

    fprintf (graphviz_file, "\", label=\"");

    fprintf (graphviz_file, "%s", node_type_info[current_node->node_type].name_of_node_type);

    WriteLettering (graphviz_file, current_node, node_type_info);

    fprintf (graphviz_file, "\"];\n");

    #define CONNECT_WITH_CHILD(direction)                                                            \
                                                                                                     \
    if (current_node->direction##_child != nullptr)                                                  \
    {                                                                                                \
        PrintNodesDescription (graphviz_file, current_node->direction##_child, node_type_info);      \
        fprintf (graphviz_file, "\"%p\" -> \"%p\"\n", current_node, current_node->direction##_child);\
    }

    CONNECT_WITH_CHILD (left)

    CONNECT_WITH_CHILD (right)

    #undef CONNECT_WITH_CHILD
}

void WriteLettering (FILE* graphviz_file, tree_node* current_node, node_type_info* node_type_info)
{
    assert (graphviz_file  != nullptr);
    assert (current_node   != nullptr);
    assert (node_type_info != nullptr);

    switch (node_type_info[current_node->node_type].name_type)
    {
        case NAMELESS:  return;

        case USER_NUM:  fprintf (graphviz_file, " | %lg", current_node->key.val);
                        return;

        case USER_NAME: fprintf (graphviz_file, " | %s", current_node->key.name_ptr);
                        return;

        case SYS_NAME:  fprintf (graphviz_file, " | ");
                        WriteSysName (graphviz_file, current_node);
                        return;

        default:        printf ("Unknown type of node is found in WriteLettering\n");
                        break;
    }
}

void WriteSysName (FILE* graphviz_file, tree_node* current_node)
{
    assert (graphviz_file  != nullptr);
    assert (current_node   != nullptr);

    switch (current_node->node_type)
    {
        #define SYS_NAME_NODES(node_type, union_type, enum_type)                             \
                                                                                             \
        case node_type: Write##enum_type##Name (graphviz_file, current_node->key.union_type);\
                        return;

        #include "CodeGeneration\SysNameNodes.txt"

        #undef SYS_NAME_NODES

        #define NOT_SYS_NAME_NODES(node_type)                                          \
                                                                                       \
        case node_type: printf ("Node type %s is given to WriteSysName\n", #node_type);\
                        return;

        #include "CodeGeneration\NotSysNameNodes.txt"

        #undef NOT_SYS_NAME_NODES

        default: printf ("Something went wrong in WriteSysName\n");
                 break;
    }

}

void WriteSysFuncName (FILE* graphviz_file, enum SysFunc num_of_operation)
{
    assert (graphviz_file  != nullptr);

    switch (num_of_operation)
    {
        #define SYS_FUNC(type, lettering, func_lettering)\
                                                         \
        case type: fprintf (graphviz_file, #lettering);  \
                   return;

        #include "CodeGeneration\SysFunc.txt"

        #undef SYS_FUNC

        default: printf ("Something went wrong in WriteSysFuncName\n");
    }
}

void WriteMathOpName (FILE* graphviz_file, enum MathOp num_of_operation)
{
    assert (graphviz_file  != nullptr);

    switch (num_of_operation)
    {
        #define MATH_OP(type, lettering)               \
                                                       \
        case type: fprintf (graphviz_file, #lettering);\
                   return;

        #include "CodeGeneration\MathOp.txt"

        #undef MATH_OP

        default: printf ("Something went wrong in WriteMathOpName\n");
    }
}

void WriteComparingName (FILE* graphviz_file, enum Comparing num_of_operation)
{
    assert (graphviz_file  != nullptr);

    switch (num_of_operation)
    {
        #define COMPARING(type, lettering)             \
                                                       \
        case type: fprintf (graphviz_file, #lettering);\
                   return;

        #include "CodeGeneration\Comparing.txt"

        #undef COMPARING

        default: printf ("Something went wrong in WriteComparingName\n");
    }
}

void WriteConditionName (FILE* graphviz_file, enum Condition num_of_operation)
{
    assert (graphviz_file  != nullptr);

    switch (num_of_operation)
    {
        #define CONDITION(type, lettering)             \
                                                       \
        case type: fprintf (graphviz_file, #lettering);\
                   return;

        #include "CodeGeneration\Condition.txt"

        #undef CONDITION

        default: printf ("Something went wrong in WriteConditionName\n");
    }
}

void SaveTreeInTxt (tree* tree, char* name_of_output_file)
{
    assert (tree != nullptr);

    FILE* output_file = (name_of_output_file == nullptr) ? (fopen (STANDART_OUTPUT_TREE_FILE, "w")) : (fopen (name_of_output_file, "W"));

    struct node_type_info node_type_info[NUM_OF_NODE_TYPES] = {};

    FillNodeTypeInfo (node_type_info);

    SaveSubtreeInTxt (tree->root, output_file, node_type_info);

    fclose (output_file);
}

void SaveSubtreeInTxt (tree_node* current_node, FILE* output_file, node_type_info* node_type_info)
{
    assert (output_file    != nullptr);
    assert (current_node   != nullptr);
    assert (node_type_info != nullptr);

    fprintf (output_file, "%d", current_node->node_type);

    switch (node_type_info[current_node->node_type].name_type)
    {
        case NAMELESS:  break;

        case USER_NUM:  fprintf (output_file, " | %lg", current_node->key.val);
                        break;

        case USER_NAME: fprintf (output_file, " | %s", current_node->key.name_ptr);
                        break;

        case SYS_NAME:  fprintf (output_file, " | ");
                        SaveSysCode (output_file, current_node);
                        break;

        default:        printf ("Unknown type of node is found in SaveSubtreeInTxt\n");
                        break;
    }

    fprintf (output_file, "\n");

    if (current_node->left_child != nullptr)
    {
        fprintf (output_file, "{\n");
        SaveSubtreeInTxt (current_node->left_child, output_file, node_type_info);
        fprintf (output_file, "}\n");

        if (current_node->right_child != nullptr)
        {
            fprintf (output_file, "{\n");
            SaveSubtreeInTxt (current_node->right_child, output_file, node_type_info);
            fprintf (output_file, "}\n");
        }
    }
}

void SaveSysCode (FILE* output_file, tree_node* current_node)
{
    assert (output_file  != nullptr);
    assert (current_node != nullptr);

    switch (current_node->node_type)
    {
        #define SYS_NAME_NODES(node_type, union_type, enum_type)                  \
                                                                                  \
        case node_type: fprintf (output_file, "%d", current_node->key.union_type);\
                        return;

        #include "CodeGeneration\SysNameNodes.txt"

        #undef SYS_NAME_NODES

        #define NOT_SYS_NAME_NODES(node_type)                                         \
                                                                                      \
        case node_type: printf ("Node type %s is given to SaveSysCode\n", #node_type);\
                        return;

        #include "CodeGeneration\NotSysNameNodes.txt"

        #undef NOT_SYS_NAME_NODES

        default: printf ("Something went wrong in SaveSysCode\n");
                 break;
    }
}

tree* ReadTreeFromTxt (char* name_of_input_file)
{
    FILE* input_file = (name_of_input_file == nullptr) ? (fopen (STANDART_INPUT_TREE_FILE, "r")) : (fopen (name_of_input_file, "r"));

    tree* tree = NewTree ();

    text input = {};

    input.file_length = FindLengthOfFile (input_file);

    input.pointer_on_buffer = (char*) calloc (input.file_length + 1, sizeof(char));

    CopyFileInBuffer (input_file, input.pointer_on_buffer, &(input.file_length));

    fclose (input_file);

    struct node_type_info node_type_info[NUM_OF_NODE_TYPES] = {};

    FillNodeTypeInfo (node_type_info);

    return FillTree (tree, &input, node_type_info);
}

tree* FillTree (tree* tree, text* input, node_type_info* node_type_info)
{
    assert (tree           != nullptr);
    assert (input          != nullptr);
    assert (node_type_info != nullptr);

    size_t i = 0;

    tree->root = CreateTreeNode (BLANK, 0.0);

    tree_node* current_node = tree->root;

    while (input->pointer_on_buffer[i] != '\0')
    {
        if (isdigit(input->pointer_on_buffer[i]))
        {
            current_node->node_type = (TreeNodeType) atoi (input->pointer_on_buffer + i);

            while (isdigit(input->pointer_on_buffer[i])) i++;

            if (node_type_info[current_node->node_type].name_type != NAMELESS)
            {
                while (input->pointer_on_buffer[i] == ' ') i++;

                if (input->pointer_on_buffer[i] != '|') printf ("%s\nError in data file with tree\n", input->pointer_on_buffer + i); //Обработай ошибку

                i++;

                while (input->pointer_on_buffer[i] == ' ') i++;

                switch (node_type_info[current_node->node_type].name_type)
                {
                    case USER_NUM:  current_node->key.val = atof (input->pointer_on_buffer + i);
                                    while ((isdigit (input->pointer_on_buffer[i])) || (input->pointer_on_buffer[i] == '.') || (input->pointer_on_buffer[i] == '-'))
                                    {
                                        i++;
                                    }
                                    break;

                    case USER_NAME: current_node->key.name_ptr = input->pointer_on_buffer + i;
                                    while (isalpha (input->pointer_on_buffer[i]) || (input->pointer_on_buffer[i] == '_')) i++; //!!!
                                    if (input->pointer_on_buffer[i] == '{') current_node = CaseNewChild (current_node);
                                    else if (input->pointer_on_buffer[i] == '}') current_node = current_node->parent;
                                    input->pointer_on_buffer[i] = '\0';
                                    break;

                    case SYS_NAME:  switch (current_node->node_type)
                                    {
                                        #define SYS_NAME_NODES(node_type, union_type, enum_type)                                       \
                                                                                                                                       \
                                        case node_type: current_node->key.union_type = (enum_type) atoi (input->pointer_on_buffer + i);\
                                                        break;

                                        #include "CodeGeneration\SysNameNodes.txt"

                                        #undef SYS_NAME_NODES

                                        #define NOT_SYS_NAME_NODES(node_type)                       \
                                                                                                    \
                                        case node_type: printf ("Something went wrong in FillTree");\
                                                        break;                                      //Обработай ошибку

                                        #include "CodeGeneration\NotSysNameNodes.txt"

                                        #undef NOT_SYS_NAME_NODES

                                        default: printf ("Unknown type of node\n"); //Обработай ошибку
                                    }
                                    break;

                    case NAMELESS:  printf ("there shouldn't be nameless type of node\n");
                                    break;

                    default:        printf ("Something strange happened\n"); //Обработай ошибку
                                    break;
                }
            }
        }

        else
        {
            switch (input->pointer_on_buffer[i])
            {
                case ' ':  break;

                case '\n': break;

                case '{':  current_node = CaseNewChild (current_node);
                           break;

                case '}':  current_node = current_node->parent;
                           break;

                default:   printf ("Something went wrong\n");
                           break;
            }
        }
        i++;
    }

    return tree;
}

tree_node* CaseNewChild (tree_node* current_node)
{
    assert (current_node != nullptr);

    if (current_node->left_child == nullptr)
    {
        current_node->left_child = CreateTreeNode (BLANK, 0.0);

        current_node->left_child->parent = current_node;

        return current_node->left_child;
    }
    else
    {
        if (current_node->right_child == nullptr)
        {
            current_node->right_child = CreateTreeNode (BLANK, 0.0);

            current_node->right_child->parent = current_node;

            return current_node->right_child;
        }
        else
        {
            printf ("Tree node can't have any more children\n");
            return nullptr;
        }
    }
}

void DestroyTree (tree* tree)
{
    assert (tree != nullptr);

    DestroySubtree (tree->root);
}

void DestroySubtree (tree_node* current_node)
{
    assert (current_node != nullptr);

    if (current_node->left_child != nullptr)
    {
        DestroySubtree (current_node->left_child);
    }

    if (current_node->right_child != nullptr)
    {
        DestroySubtree (current_node->right_child);
    }

    free (current_node);
}
