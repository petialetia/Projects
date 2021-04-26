#include "../include/FrontEnd.hpp"


int main (int argC, char* argV[])
{
    text program_tree = {};

    ProcessInput (argC, argV, &program_tree, STANDART_INPUT_FILE);

    tree* tree = GetGeneral (program_tree.pointer_on_buffer);

    assert (tree != nullptr);

    assert (tree->root != nullptr);

    VarChecking (tree->root);

    DrawTree (tree->root);

    SaveTreeInTxt (tree);
}

tree* GetGeneral (char* text)
{
    assert (text != nullptr);

    //printf ("GetGeneral reached\n");

    dependencies dep = {};

    dep.text = text;

    SkipSpaces (&dep);

    tree* tree = NewTree ();

    tree->root = GetMain (&dep);

    if (tree->root == nullptr) return nullptr;

    SkipSpaces (&dep);

    tree_node** current_node = &tree->root;

    while (dep.text[dep.global_position] == 'I')
    {
        current_node = &((*current_node)->right_child);
        *current_node = GetUFunc (&dep);

        if (*current_node == nullptr)
        {
            DestroyTree (tree);
            SyntaxError (&dep, __LINE__);
            return nullptr;
        }

        SkipSpaces (&dep);
    }

    if (Require (&dep, '\0'))
    {
        SyntaxError (&dep, __LINE__);
        DestroyTree (tree);
        return nullptr;
    }

    return tree;
}

tree_node* GetMain (dependencies* dep)
{
    assert (dep != nullptr);

    //printf ("GetMain reached\n");

    if (Require_with_skip (dep, "Guide how to build "))
    {
        SyntaxError (dep, __LINE__);
        return nullptr;
    }

    SkipSpaces (dep);

    char* func_name = GetFuncName (dep);

    if (func_name == nullptr) return nullptr;

    tree_node* current_node = CreateTreeNode (USER_FUNC, func_name);

    if (Require (dep, ':'))
    {
        SyntaxError (dep, __LINE__);
        DestroySubtree (current_node);
        return nullptr;
    }
    else
    {
        dep->text[dep->global_position] = '\0';
        dep->global_position++;
    }

    SkipSpaces (dep);

    current_node->left_child = GetFuncBody (dep, MAIN);

    if (current_node->left_child == nullptr)
    {
        DestroySubtree (current_node);
        return nullptr;
    }

    return current_node;
}

tree_node* GetUFunc (dependencies* dep)
{
    assert (dep != nullptr);

    //printf ("GetUFunc reached\n");

    if (Require_with_skip (dep, "Instruction of "))
    {
        SyntaxError (dep, __LINE__);
        return nullptr;
    }

    SkipSpaces (dep);

    char* func_name = GetFuncName (dep);

    if (func_name == nullptr) return nullptr;    

    tree_node* current_node = CreateTreeNode (USER_FUNC, func_name);

    if (Require (dep, ':'))
    {
        SyntaxError (dep, __LINE__);
        DestroySubtree (current_node);
        return nullptr;
    }
    else
    {
        dep->text[dep->global_position] = '\0';
        dep->global_position++;
    }

    SkipSpaces (dep);

    current_node->left_child = GetFuncBody (dep);

    if (current_node->left_child == nullptr)
    {
        DestroySubtree (current_node);
        return nullptr;
    }

    return current_node;
}

char* GetFuncName (dependencies* dep)
{
    assert (dep != nullptr);

    //printf ("GetFuncName reached\n");

    char* name_ptr = dep->text + dep->global_position;

    while (isalpha(dep->text[dep->global_position]) || dep->text[dep->global_position] == '_')
    {
        dep->global_position++;
    }

    return name_ptr;
}

tree_node* GetFuncBody (dependencies* dep, enum FuncType is_main)
{
    assert (dep != nullptr);

    //printf ("GetFuncBody reached\n");

    size_t old_global_position = dep->global_position;

    tree_node* first_node_of_func = GetCommand (dep);

    if (old_global_position == dep->global_position)
    {
        DestroySubtree (first_node_of_func);
        SyntaxError (dep, __LINE__);                   
        return nullptr;
    }

    tree_node** current_node = &first_node_of_func;

    SkipSpaces (dep);

    while (strncmp ("End of instruction.", dep->text + dep->global_position, strlen ("End of instruction.")) != 0)
    {
        current_node = &((*current_node)->right_child);
        *current_node = GetCommand (dep);
        SkipSpaces (dep);
    }

    dep->global_position += strlen ("End of instruction.");


    if (is_main) (*current_node)->right_child = CreateTreeNode (SYS_FUNC, HLT);
    else         (*current_node)->right_child = CreateTreeNode (SYS_FUNC, RETURN);

    return first_node_of_func;
}

tree_node* GetCommand (dependencies* dep)
{
    assert (dep != nullptr);

    //printf ("GetCommand reached\n");

    tree_node* command_node = nullptr;
    tree_node* begin_of_command = nullptr;

    if (strncmp ("While", dep->text + dep->global_position, strlen ("While")) == 0)
    {
        command_node = GetLoop (dep);
        if (command_node == nullptr) return nullptr;
    }

    else
    {
        if (strncmp ("If", dep->text + dep->global_position, strlen ("If")) == 0)
        {
            command_node = GetCondOper (dep);
            if (command_node == nullptr) return nullptr;
        }
        else
        {
            enum SysFunc num = IsSysFunc (dep);
            if (num > 0)
            {
                command_node = GetSysFunc (dep, num);
            }
            else
            {
                command_node = GetAssignment (dep);
                if (command_node == nullptr) return nullptr;
            }

        }
    }

    begin_of_command = CreateTreeNode (COMMAND, 0.0);
    begin_of_command->left_child = command_node;

    return begin_of_command;
}

tree_node* GetSysFunc (dependencies* dep, enum SysFunc num)
{
    assert (dep != nullptr);
    assert (num != ASSIGNMENT);

    //printf ("GetSysFunc reached\n");

    tree_node* sys_func_node = CreateTreeNode (SYS_FUNC, num);

    SkipSpaces (dep);

    tree_node* current_node = sys_func_node;

    if (AddArg (dep, current_node) != 0) return nullptr;

    while (dep->text[dep->global_position] != '.')
    {
        if (Require (dep, ','))
        {
            DestroySubtree (sys_func_node);
            return nullptr;
        }
        dep->text[dep->global_position] = '\0';
        dep->global_position++;

        current_node->right_child = CreateTreeNode (sys_func_node->node_type, sys_func_node->key.sys_func);

        current_node = current_node->right_child;
        SkipSpaces (dep);

        if (AddArg (dep, current_node) != 0)
        {
            DestroySubtree (sys_func_node);
            return nullptr;
        }
    }

    dep->text[dep->global_position] = '\0';
    dep->global_position++;

    return sys_func_node;
}

bool AddArg (dependencies* dep, tree_node* current_node)
{
    assert (dep          != nullptr);
    assert (current_node != nullptr);

    current_node->left_child = GetExpression (dep);

    if (current_node->left_child == nullptr)
    {
        return 1;
    }
    SkipSpaces (dep);

    return 0;
}

tree_node* GetAssignment (dependencies* dep)
{
    assert (dep != nullptr);

    //printf ("GetAssignment reached\n");

    char* var_ptr = dep->text + dep->global_position;

    while (isalpha (dep->text[dep->global_position]) || dep->text[dep->global_position] == ' ' || dep->text[dep->global_position] == '_') dep->global_position++;

    SkipSpaces (dep);

    if (Require (dep, '='))
    {
        SyntaxError (dep, __LINE__);
        return nullptr;
    }

    dep->text[dep->global_position] = '\0';

    dep->global_position++;

    tree_node* assignment_node = CreateTreeNode (SYS_FUNC, ASSIGNMENT);

    assignment_node->left_child = CreateTreeNode (VAR, var_ptr);

    SkipSpaces (dep);

    assignment_node->right_child = GetExpression (dep);

    if (Require (dep, '.'))
    {
        DestroySubtree (assignment_node);
        SyntaxError (dep, __LINE__);
        return nullptr;
    }

    dep->global_position++;

    return assignment_node;
}

tree_node* GetCondOper (dependencies* dep)
{
    assert (dep != nullptr);

    //printf ("GetCondOper reached\n");

    if (Require_with_skip (dep, "If"))
    {
        SyntaxError (dep, __LINE__);
        return nullptr;
    }

    SkipSpaces (dep);

    if (Require (dep, '('))
    {
        SyntaxError (dep, __LINE__);
        return nullptr;
    }

    dep->global_position++;

    SkipSpaces (dep);

    tree_node* cond_node = CreateTreeNode (CONDITION, IF);

    cond_node->left_child = GetCondition (dep);

    SkipSpaces (dep);

    if (Require (dep, ')'))
    {
        DestroySubtree (cond_node);
        SyntaxError (dep, __LINE__);
        return nullptr;
    }

    dep->global_position++;

    SkipSpaces (dep);

    cond_node->right_child = GetCondCommand (dep);

    SkipSpaces (dep);

    size_t else_size = strlen ("Else");

    if (strncmp ("Else", dep->text + dep->global_position, else_size) == 0)
    {
        dep->global_position += else_size;
        SkipSpaces (dep);
        tree_node* else_command = GetCondCommand (dep);
        tree_node* if_command = cond_node->right_child;
        cond_node->right_child = CreateTreeNode (CONDITION, IF_ELSE);
        cond_node->right_child->left_child  = if_command;
        cond_node->right_child->right_child = else_command;
    }

    return cond_node;
}

tree_node* GetCondition (dependencies* dep)
{
    assert (dep != nullptr);

    //printf ("GetCondition reached\n");

    tree_node* left_child = GetExpression (dep);
    if (left_child == nullptr) return nullptr;

    SkipSpaces (dep);

    size_t size_of_cond_operator = 0;

    tree_node* cond_node = nullptr;

    #define COMPARING(enum_type, lettering)                                                \
                                                                                           \
    size_of_cond_operator = strlen (#lettering);                                           \
    if (strncmp (#lettering, dep->text + dep->global_position, size_of_cond_operator) == 0)\
    {                                                                                      \
        cond_node = CreateTreeNode (COMPARING, enum_type);                                 \
        cond_node->left_child = left_child;                                                \
        dep->global_position += size_of_cond_operator;                                     \
        SkipSpaces(dep);                                                                   \
        cond_node->right_child = GetExpression (dep);                                      \
        if (cond_node->right_child == nullptr)                                             \
        {                                                                                  \
            DestroySubtree (cond_node);                                                    \
            return nullptr;                                                                \
        }                                                                                  \
        return cond_node;                                                                  \
    }

    #include "../CodeGeneration/Comparing.txt"

    #undef COMPARING

    DestroySubtree (cond_node);
    DestroySubtree (left_child);
    return nullptr;
}

tree_node* GetCondCommand (dependencies* dep)
{
    assert (dep != nullptr);

    //printf ("GetCondCommand reached\n");

    size_t begin_len = strlen ("List of commands:");

    size_t end_len   = strlen ("End of commands.");

    tree_node* begin_node   = nullptr;

    tree_node* current_node = nullptr;

    if (strncmp ("List of commands:", dep->text + dep->global_position, begin_len) == 0)
    {
        dep->global_position += begin_len;
        SkipSpaces (dep);
        begin_node = GetCommand (dep);
        if (begin_node == nullptr)
        {
            DestroySubtree (begin_node);
            return nullptr;
        }
        current_node = begin_node;
        SkipSpaces (dep);
        while (strncmp ("End of commands.", dep->text + dep->global_position, end_len) != 0)
        {
            current_node->right_child = GetCommand (dep);
            if (current_node->right_child == nullptr)
            {
                DestroySubtree (current_node);
                return nullptr;
            }
            current_node = current_node->right_child;
            SkipSpaces (dep);
        }
        dep->global_position += end_len;
        return begin_node;
    }

    return GetCommand (dep);
}

tree_node* GetLoop (dependencies* dep)
{
    assert (dep != nullptr);

    //printf ("GetLoop reached\n");

    if (Require_with_skip (dep, "While")) return nullptr;

    SkipSpaces (dep);

    if (Require (dep, '('))
    {
        SyntaxError (dep, __LINE__);
        return nullptr;
    }

    dep->global_position++;

    tree_node* loop_node = CreateTreeNode (LOOP, 0.0);

    loop_node->left_child = GetCondition (dep);

    if (loop_node->left_child == nullptr)
    {
        DestroySubtree (loop_node);
        return nullptr;
    }

    if (Require (dep, ')'))
    {
        DestroySubtree (loop_node);
        SyntaxError (dep, __LINE__);
        return nullptr;
    }

    dep->global_position++;

    SkipSpaces (dep);

    loop_node->right_child = GetCondCommand (dep);

    return loop_node;
}

tree_node* GetExpression (dependencies* dep)
{
    assert (dep != nullptr);

    //printf ("GetExpression reached\n");

    tree_node* ptr = GetTerm (dep);

    if (ptr == nullptr) return nullptr;

    SkipSpaces (dep);

    while ((dep->text[dep->global_position] == '+') || (dep->text[dep->global_position] == '-'))
    {
        char op = dep->text[dep->global_position];
        dep->global_position++;

        SkipSpaces (dep);
        tree_node* ptr2 = GetTerm (dep);
        if (ptr2 == nullptr) return nullptr;

        if (op == '+') ptr = CreateTreeNode (MATH_OP, ADD, ptr, ptr2);
        else           ptr = CreateTreeNode (MATH_OP, SUB, ptr, ptr2);
    }

    return ptr;
}

tree_node* GetTerm (dependencies* dep)
{
    assert (dep != nullptr);

    //printf ("GetTerm reached\n");

    tree_node* ptr = GetPower (dep);

    if (ptr == nullptr) return nullptr;

    SkipSpaces (dep);

    while ((dep->text[dep->global_position] == '*') || (dep->text[dep->global_position] == '/'))
    {
        char op = dep->text[dep->global_position];
        dep->global_position++;

        SkipSpaces (dep);
        tree_node* ptr2 = GetPower (dep);
        if (ptr2 == nullptr) return nullptr;

        if (op == '/') ptr = CreateTreeNode (MATH_OP, DIV, ptr, ptr2);
        else           ptr = CreateTreeNode (MATH_OP, MUL, ptr, ptr2);
    }

    return ptr;
}

tree_node* GetPower (dependencies* dep)
{
    assert (dep != nullptr);

    //printf ("GetPower reached\n");

    tree_node* ptr = GetPrimary (dep);

    if (ptr == nullptr) return nullptr;

    SkipSpaces (dep);

    while (dep->text[dep->global_position] == '^')
    {
        dep->global_position++;

        SkipSpaces (dep);
        tree_node* ptr2 = GetPrimary (dep);
        if (ptr2 == nullptr) return nullptr;

        ptr = CreateTreeNode (MATH_OP, POW, ptr, ptr2);
    }

    return ptr;
}

tree_node* GetPrimary (dependencies* dep)
{
    assert (dep != nullptr);

    //printf ("GetPrimary reached\n");

    tree_node* ptr = nullptr;

    if (isalpha (dep->text[dep->global_position]))
    {
        ptr = GetVar (dep);
        return ptr;
    }

    if (dep->text[dep->global_position] == '(')
    {
        ptr = GetBrackets (dep);
        if (ptr == nullptr) return nullptr;

        SkipSpaces (dep);
    }
    else
    {
        ptr = GetNum (dep);
        if (ptr == nullptr) return nullptr;

        SkipSpaces (dep);
    }

    return ptr;
}

tree_node* GetVar (dependencies* dep)
{
    assert (dep != nullptr);

    //printf ("GetVar reached\n");

    if (!isalpha (dep->text[dep->global_position])) return nullptr;

    tree_node* ptr = CreateTreeNode (VAR, dep->text + dep->global_position);

    do
    {
        dep->global_position++;
    }
    while (isalpha (dep->text[dep->global_position]) || (dep->text[dep->global_position] == '_'));

    return ptr;
}

tree_node* GetBrackets (dependencies* dep)
{
    assert (dep != nullptr);

    //printf ("GetBrackets reached\n");

    if (Require (dep, '(')) return nullptr;
    dep->global_position++;

    SkipSpaces (dep);

    tree_node* ptr = GetExpression (dep);
    if (ptr == nullptr) return nullptr;

    SkipSpaces (dep);

    if (Require (dep, ')'))
    {
        DestroySubtree (ptr);
        return nullptr;
    }

    dep->global_position++;

    return ptr;
}

tree_node* GetNum (dependencies* dep)
{
    assert (dep != nullptr);

    //printf ("GetNum reached\n");

    double val = NAN;

    char* end_ptr = nullptr;

    val = strtod (dep->text + dep->global_position, &end_ptr);

    if (end_ptr == dep->text + dep->global_position)
    {
        SyntaxError (dep, __LINE__);
        return nullptr;
    }

    if (*(end_ptr - 1)  == '.')
    {
        end_ptr--;
    }

    dep->global_position += end_ptr - (dep->text + dep->global_position);

    return CreateTreeNode (NUM, val);
}

enum SysFunc IsSysFunc (dependencies* dep)
{

    assert (dep != nullptr);

    size_t size_of_lettering = 0;

    #define SYS_FUNC(enum_num, lettering, func_lettering)                              \
                                                                                       \
    size_of_lettering = strlen (#lettering);                                           \
                                                                                       \
    if (strncmp (#lettering, dep->text + dep->global_position, size_of_lettering) == 0)\
    {                                                                                  \
        dep->global_position += size_of_lettering;                                     \
        return enum_num;                                                               \
    }

    #include "../CodeGeneration/SysFunc.txt"

    #undef SYS_FUNC

    return ASSIGNMENT;
}

void SkipSpaces (dependencies* dep)
{
    assert (dep != nullptr);

    while (isspace (dep->text[dep->global_position]))
    {
        if (dep->text[dep->global_position] == '\n')
        {
            dep->num_of_lines++;
        }
        dep->global_position++;
    }
}

bool Require (dependencies* dep, char char_elem)
{
    assert (dep != nullptr);

    if (dep->text[dep->global_position] != char_elem) return true;

    return false;
}

bool Require_with_skip (dependencies* dep, const char* string)
{
    assert (dep    != nullptr);
    assert (string != nullptr);

    size_t length = strlen (string);

    if (strncmp (string, dep->text + dep->global_position, length) != 0) return true;

    dep->global_position += length;

    return false;
}

void SyntaxError (dependencies* dep, int line)
{
    assert (dep != nullptr);

    printf ("Error is found, string number %d\n\n", line);

    printf ("Correct line %zu\n", dep->num_of_lines + 1);
}

void VarChecking (tree_node* current_node)
{
    assert (current_node != nullptr);

    if (current_node->right_child != nullptr) VarChecking (current_node->right_child);

    if (current_node->left_child != nullptr) VarChecking (current_node->left_child);

    if (current_node->node_type == VAR)
    {
        char* ptr = current_node->key.name_ptr;

        while (isalpha (*ptr)|| (*ptr == '_'))
        {
            ptr++;
        }

        *ptr = '\0';
    }
}
