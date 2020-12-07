#include "TreeFunctions.hpp"

tree* NewTree ()
{
    tree* ptr_on_tree = (tree*) calloc (1, sizeof (tree));

    return ptr_on_tree;
}

tree_node* CreateTreeNode (char node_type, double key, tree_node* ptr_on_left_child, tree_node* ptr_on_right_child, tree_node* ptr_on_parent)
{
    tree_node* ptr_on_tree_node = (tree_node*) calloc (1, sizeof (tree_node));

    ptr_on_tree_node->node_type = node_type;

    ptr_on_tree_node->key.val = key;

    ptr_on_tree_node->left_child = ptr_on_left_child;

    ptr_on_tree_node->right_child = ptr_on_right_child;

    ptr_on_tree_node->parent = ptr_on_parent;

    return ptr_on_tree_node;
}

tree_node* CreateTreeNode (char node_type, char key, tree_node* ptr_on_left_child, tree_node* ptr_on_right_child, tree_node* ptr_on_parent)
{
    tree_node* ptr_on_tree_node = (tree_node*) calloc (1, sizeof (tree_node));

    ptr_on_tree_node->node_type = node_type;

    ptr_on_tree_node->key.num = key;

    ptr_on_tree_node->left_child = ptr_on_left_child;

    ptr_on_tree_node->right_child = ptr_on_right_child;

    ptr_on_tree_node->parent = ptr_on_parent;

    return ptr_on_tree_node;
}

tree_node* CopySubtree (tree_node* node)
{
    if (node != nullptr) return CreateTreeNode (node->node_type, node->key.val, CopySubtree (node->left_child), CopySubtree (node->right_child), node->parent);

    return nullptr;
}

void DrawTree (tree_node* root, size_t num)
{
    assert (root != nullptr);

    if (num > MAX_NUM_OF_FUNC)
    {
        printf ("Too big num of functions\n");
        return;
    }

    char name_of_file[MAX_LENGTH_OF_NUM_OF_FUNC + SIZE_OF_BEGINNING_OF_GRAPH_FILE + 1] = {};

    sprintf (name_of_file, "%s%I64u%s", STANDART_NAME_OF_GRAPH_FILE, num, STANDDART_GRAPG_FILE_EXTENSION);

    FILE* graphviz_file = fopen (name_of_file, "w");

    fprintf (graphviz_file, "digraph PL\n");
    fprintf (graphviz_file, "{\nrankdir=HR;\n");

    PrintNodesDescription (graphviz_file, root);

    fprintf (graphviz_file, "}\n");

    fclose (graphviz_file);

    char system_create_graph [MAX_LENGTH_OF_SYSTEM_CMD + 1] = {};

    sprintf (system_create_graph, "dot -Tpng Graph\\GraphvizFile%I64u.txt -o Graph\\Graph%I64u.png", num, num);

    system (system_create_graph);
}

void PrintNodesDescription (FILE* graphviz_file, tree_node* current_node)
{
    assert (graphviz_file != nullptr);
    assert (current_node  != nullptr);

    fprintf (graphviz_file, "\"%p\" [style=\"filled\", ", current_node);

    if (current_node->left_child != nullptr)
    {
        fprintf (graphviz_file, "fillcolor=\"yellow\", shape=\"parallelogram\", label=\"%s\"];\n", FinNameOfFunc (current_node->key.num));

        PrintNodesDescription (graphviz_file, current_node->left_child);
        fprintf (graphviz_file, "\"%p\" -> \"%p\"\n", current_node, current_node->left_child);

        if (current_node->right_child != nullptr)
        {
            PrintNodesDescription (graphviz_file, current_node->right_child);
            fprintf (graphviz_file, "\"%p\" -> \"%p\"\n", current_node, current_node->right_child);
        }
    }
    else
    {
        if (current_node->node_type == CONST)
        {
            fprintf (graphviz_file, "fillcolor=\"#98FB98\", label=\"%lg\"];\n", current_node->key.val);
        }
        else
        {
            fprintf (graphviz_file, "fillcolor=\"#FFD6D4\", shape=\"rectangle\", label=\"%c\"];\n", (int) current_node->key.num + 'a');
        }
    }
}

const char* FinNameOfFunc (char key)
{
    #define DEF_FUNC(name, symbol, num)\
                                       \
    if (key == num)                    \
    {                                  \
        return #symbol;                \
    }

    #include "Functions.hpp"

    #undef DEF_FUNC

    return nullptr;
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


//-----------------------------------------------------------------------------

//Recursive descent parser

//-----------------------------------------------------------------------------


tree* GetGeneral (const char* string)
{
    assert (string != nullptr);

    dependencies dep = {};

    dep.string = string;

    SkipSpaces (&dep);

    tree* tree = NewTree ();

    tree->root = GetExpression (&dep);

    if (tree->root == nullptr) return nullptr;

    SkipSpaces (&dep);

    if (Require (&dep, '\0'))
    {
        DestroyTree (tree);
        return nullptr;
    }

    return tree;
}

tree_node* GetExpression (dependencies* dep)
{
    assert (dep != nullptr);

    tree_node* ptr = GetTerm (dep);

    if (ptr == nullptr) return nullptr;

    SkipSpaces (dep);

    while ((dep->string[dep->position] == '+') || (dep->string[dep->position] == '-'))
    {
        char op = dep->string[dep->position];
        dep->position++;

        SkipSpaces (dep);
        tree_node* ptr2 = GetTerm (dep);
        if (ptr2 == nullptr) return nullptr;

        if (op == '+') ptr = CreateTreeNode (FUNC, OP_ADD, ptr, ptr2);
        else           ptr = CreateTreeNode (FUNC, OP_SUB, ptr, ptr2);
    }

    return ptr;
}

tree_node* GetTerm (dependencies* dep)
{
    assert (dep != nullptr);

    tree_node* ptr = GetPower (dep);

    if (ptr == nullptr) return nullptr;

    SkipSpaces (dep);

    while ((dep->string[dep->position] == '*') || (dep->string[dep->position] == '/'))
    {
        char op = dep->string[dep->position];
        dep->position++;

        SkipSpaces (dep);
        tree_node* ptr2 = GetPower (dep);
        if (ptr2 == nullptr) return nullptr;

        if (op == '/') ptr = CreateTreeNode (FUNC, OP_DIV, ptr, ptr2);
        else           ptr = CreateTreeNode (FUNC, OP_MUL, ptr, ptr2);
    }

    return ptr;
}

tree_node* GetPower (dependencies* dep)
{
    assert (dep != nullptr);

    tree_node* ptr = GetPrimary (dep);

    if (ptr == nullptr) return nullptr;

    SkipSpaces (dep);

    while (dep->string[dep->position] == '^')
    {
        dep->position++;

        SkipSpaces (dep);
        tree_node* ptr2 = GetPrimary (dep);
        if (ptr2 == nullptr) return nullptr;

        ptr = CreateTreeNode (FUNC, OP_POW, ptr, ptr2);
    }

    return ptr;
}

tree_node* GetPrimary (dependencies* dep)
{
    assert (dep != nullptr);

    tree_node* ptr = nullptr;

    if (islower (dep->string[dep->position]))
    {
        ptr = GetFunction(dep);
        if (ptr == nullptr) return nullptr;

        SkipSpaces (dep);

        if ((dep->string[dep->position] == '(') && (ptr->node_type == VAR))
        {
            SyntaxError (dep, __LINE__);
            free (ptr);
            return nullptr;
        }

        if (ptr->node_type == VAR)
        {
            return ptr;
        }
        else
        {
            ptr->left_child = GetBrackets (dep);
            if (ptr->left_child == nullptr) return nullptr;
        }

        SkipSpaces (dep);
        return ptr;
    }

    if (dep->string[dep->position] == '(')
    {
        ptr = GetBrackets (dep);
        if (ptr == nullptr) return nullptr;

        SkipSpaces (dep);
    }
    else
    {
        ptr = GetNumber (dep);
        if (ptr == nullptr) return nullptr;

        SkipSpaces (dep);
    }

    return ptr;
}

tree_node* GetFunction (dependencies* dep)
{
    assert (dep != nullptr);

    if (!isalpha (dep->string[dep->position]))
    {
        SyntaxError (dep, __LINE__);
        return nullptr;
    }

    size_t length = 0;

    #define DEF_FUNC(name, symbol, num)                             \
                                                                    \
    length = strlen (#symbol);                                      \
                                                                    \
                                                                    \
    if (strncmp (#symbol, dep->string + dep->position, length) == 0)\
    {                                                               \
        dep->position += length;                                    \
        return CreateTreeNode (FUNC, OP_##name);                    \
    }

    #include "Functions.hpp"

    #undef DEF_FUNCTION

    //Only for single-letter variables

    tree_node* ptr = CreateTreeNode (VAR,(char) (dep->string[dep->position] - 'a'));

    dep->position++;

    return ptr;
}

tree_node* GetBrackets (dependencies* dep)
{
    assert (dep != nullptr);

    if (Require (dep, '(')) return nullptr;
    dep->position++;

    SkipSpaces (dep);

    tree_node* ptr = GetExpression (dep);
    if (ptr == nullptr) return nullptr;

    SkipSpaces (dep);

    if (Require (dep, ')'))
    {
        DestroySubtree (ptr);
        return nullptr;
    }

    dep->position++;

    return ptr;
}

tree_node* GetNumber (dependencies* dep)
{
    assert (dep != nullptr);

    double val = NAN;

    char* end_ptr = nullptr;

    val = strtod (dep->string + dep->position, &end_ptr);

    if (end_ptr == dep->string + dep->position)
    {
        SyntaxError (dep, __LINE__);
        return nullptr;
    }

    dep->position += end_ptr - (dep->string + dep->position);

    return CreateTreeNode (CONST, val);
}

void SkipSpaces (dependencies* dep)
{
    assert (dep != nullptr);

    if (dep->string[dep->position] == ' ')
    {
        for (size_t i = 1;; i++)
        {
            if (dep->string[dep->position + i] != ' ')
            {
                dep->position += i;
                break;
            }
        }
    }
}

bool Require (dependencies* dep, char symbol)
{
    assert (dep != nullptr);

    if (dep->string[dep->position] != symbol)
    {
        SyntaxError (dep, __LINE__);
        return 1;
    }

    return 0;
}

void SyntaxError (dependencies* dep, int line)
{
    assert (dep != nullptr);

    printf ("Error is found, string number %d\n\n", line);

    printf ("%s\n", dep->string);
    for (size_t i = 0; i < dep->position; i ++)
    {
        printf ("-");
    }
    printf ("^\n");
}

bool IsZero (double val)
{
    return (fabs (val) <= PSEUDO_ZERO);
}

void DivByZero (dependencies* dep)
{
    assert (dep != nullptr);

    SyntaxError (dep, __LINE__);
    printf ("\nDIVISION BY 0\n");
}

void InvalidArgument (dependencies* dep)
{
    assert (dep != nullptr);

    SyntaxError (dep, __LINE__);
    printf ("\nInvalid argument\n");
}
