#include "FrontEnd.hpp"


int main (int argC, char* argV[])
{
    text program_tree = {};

    ProcessInput (argC, argV, &program_tree, STANDART_INPUT_FILE);

    //printf ("%s\n", program_tree.pointer_on_buffer);

    //printf ("%I64u %I64u\n", sizeof (double), sizeof (char*));

    tree* tree = GetGeneral (program_tree.pointer_on_buffer);

    assert (tree != nullptr);

    assert (tree->root != nullptr);

    //printf ("Neuzheli\n");

    VarChecking (tree->root);

    DrawTree (tree->root);

    SaveTreeInTxt (tree);
}

tree* GetGeneral (char* text)
{
    assert (text != nullptr);

    printf ("GetGeneral reached\n");

    dependencies dep = {};

    dep.text = text;

    SkipSpaces (&dep);

    tree* tree = NewTree ();

    tree->root = GetMain (&dep);

    //printf ("Tuta\n");

    if (tree->root == nullptr) return nullptr;

    SkipSpaces (&dep);

    tree_node** current_node = &tree->root;

    while (dep.text[dep.global_position] == 'I')
    {
        current_node = &((*current_node)->right_child);
        *current_node = GetUFunc (&dep);

        //printf ("%s\n", dep.text + dep.global_position);

        if (*current_node == nullptr)
        {
            DestroyTree (tree);
            SyntaxError (&dep, __LINE__);
            return nullptr;
        }
        //printf ("%c\n", dep.text[dep.global_position]);
        //printf ("SSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSS\n");

        SkipSpaces (&dep);
    }

    //DrawTree (tree->root->right_child);

    //SkipSpaces (&dep);

    //printf ("ALO\n");

    if (Require (&dep, '\0'))
    {
        SyntaxError (&dep, __LINE__);
        DestroyTree (tree);
        return nullptr;
    }

    //printf ("Nu\n");

    return tree;
}

tree_node* GetMain (dependencies* dep)
{
    assert (dep != nullptr);

    printf ("GetMain reached\n");

    //tree_node* current_node = nullptr;

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

    //printf ("Darova\n");

    if (current_node->left_child == nullptr)
    {
        DestroySubtree (current_node);
        return nullptr;
    }

    /*if (Require_with_skip (dep, "End of instruction."))
    {
        DestroySubtree (current_node);
        return nullptr;
    }*/

    return current_node;
}

tree_node* GetUFunc (dependencies* dep)
{
    assert (dep != nullptr);

    printf ("GetUFunc reached\n");

    if (Require_with_skip (dep, "Instruction of "))
    {
        SyntaxError (dep, __LINE__);
        return nullptr;
    }

    SkipSpaces (dep);

    char* func_name = GetFuncName (dep);

    if (func_name == nullptr) return nullptr;                      //Copypast выше

    tree_node* current_node = CreateTreeNode (USER_FUNC, func_name);

    //printf ("%s\n", dep->text + dep->global_position);

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

    //printf ("Darova\n");

    if (current_node->left_child == nullptr)
    {
        DestroySubtree (current_node);
        return nullptr;
    }

    /*if (Require_with_skip (dep, "End of instruction."))
    {
        DestroySubtree (current_node);
        return nullptr;
    }*/

    //printf ("%s\n", dep->text + dep->global_position);

    return current_node;
}

char* GetFuncName (dependencies* dep)
{
    assert (dep != nullptr);

    printf ("GetFuncName reached\n");

    //printf ("%s\n", dep->text + dep->global_position);

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

    printf ("GetFuncBody reached\n");

    //printf ("%s\n", dep->text + dep->global_position);

    //tree_node* current_node =  CreateTreeNode (COMMAND, 0.0);

    //if (strncmp (5, "While", dep->text + dep->global_position) == 0)

    size_t old_global_position = dep->global_position;

    tree_node* first_node_of_func = GetCommand (dep);

    //printf ("Neghoroshi\n");

    //if (current_node == nullptr) return nullptr;

    if (old_global_position == dep->global_position)
    {
        DestroySubtree (first_node_of_func);
        SyntaxError (dep, __LINE__);                                        /////!!!!!Поправь ошибку
        return nullptr;
    }

    tree_node** current_node = &first_node_of_func;

    SkipSpaces (dep);

    //printf ("Priva\n");

    //printf ("%c\n", dep->text[dep->global_position]);

    while (strncmp ("End of instruction.", dep->text + dep->global_position, strlen ("End of instruction.")) != 0)
    {
        current_node = &((*current_node)->right_child);
        *current_node = GetCommand (dep);
        SkipSpaces (dep);
    }

    dep->global_position += strlen ("End of instruction.");


    if (is_main) (*current_node)->right_child = CreateTreeNode (SYS_FUNC, HLT);
    else         (*current_node)->right_child = CreateTreeNode (SYS_FUNC, RETURN);

    //printf ("Priva\n");

    //printf ("%s\n", dep->text + dep->global_position);

    return first_node_of_func;
}

tree_node* GetCommand (dependencies* dep)
{
    assert (dep != nullptr);

    printf ("GetCommand reached\n");

    //printf ("%s\n", dep->text + dep->global_position);
    //printf ("%c\n", dep->text[dep->global_position]);

    tree_node* command_node = nullptr; /*CreateTreeNode (COMMAND, 0.0);*/
    tree_node* begin_of_command = nullptr;

    //printf ("GetCommand reached\n");

    //printf ("%s\n", dep->text + dep->global_position);

    //printf ("%s!!!!!!!!!!!\n", dep->text + dep->global_position);

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
                //printf ("Rabotay\n");
            }
            else
            {
                //printf ("assig\n");
                //printf ("%c\n", dep->text[dep->global_position]);
                command_node = GetAssignment (dep);
                if (command_node == nullptr) return nullptr;
            }

        }
    }

    begin_of_command = CreateTreeNode (COMMAND, 0.0);
    begin_of_command->left_child = command_node;

    //printf ("OK\n");

    return begin_of_command;
}

tree_node* GetSysFunc (dependencies* dep, enum SysFunc num)
{
    assert (dep != nullptr);
    assert (num != ASSIGNMENT);

    printf ("GetSysFunc reached\n");

    //printf ("GetSysFunc worked\n"); //

    //printf ("%d\n", num);

    //printf ("%s\n", dep->text + dep->global_position);

    tree_node* sys_func_node = CreateTreeNode (SYS_FUNC, num);

    SkipSpaces (dep);   //IsSysFunc скипает функцию

    //tree_node** current_node = &sys_func_node->left_child;

    tree_node* current_node = sys_func_node;

    // (*current_node)->parent = sys_func_node;

    //current_node = current_node->left_child;
    /*if (isdigit (dep->text[dep->global_position]))
    {
        //printf ("Zifra\n");
        *current_node = GetNum(dep);
        //printf ("%c!!!!!\n", dep->text[dep->global_position]);
        SkipSpaces (dep);
    }
    else
    {
        if (isalpha (dep->text[dep->global_position]))
        {
            *current_node = CreateTreeNode (VAR, dep->text + dep->global_position);
            dep->global_position++;

            while (isalpha (dep->text[dep->global_position]) || (dep->text[dep->global_position] == ' ') || (dep->text[dep->global_position] == '_'))
            {
                dep->global_position++;
            }
        }
        else
        {
            DestroySubtree (sys_func_node);
            SyntaxError (dep, __LINE__);
            return nullptr;
        }
    }*/

    /*if (isalpha (dep->text[dep->global_position]))
    {
        *current_node = CreateTreeNode (VAR, dep->text + dep->global_position);
        dep->global_position++;

        while (isalpha (dep->text[dep->global_position]) || (dep->text[dep->global_position] == ' ') || (dep->text[dep->global_position] == '_'))
        {
            dep->global_position++;
        }
    }
    else
    {
        *current_node = GetExpression (dep);
        if (*current_node == nullptr)
        {
            DestroySubtree (sys_func_node);
            return nullptr;
        }
        SkipSpaces (dep);
    }*/

    if (AddArg (dep, current_node) != 0) return nullptr;

    //Require (dep, ',');

    //printf ("Hey\n");

    //sys_func_node->left_child = current_node;

    //printf ("Zdec \n");

    //printf ("%lg!!!SMARI\n", sys_func_node->left_child->key.val);

    //printf ("%cla\n", dep->text[dep->global_position]);

    //printf ("%c\n", dep->text[dep->global_position]);

    //printf ("Yeah\n");

    while (dep->text[dep->global_position] != '.')
    {
        if (Require (dep, ','))
        {
            DestroySubtree (sys_func_node);
            return nullptr;
        }
        dep->text[dep->global_position] = '\0';
        dep->global_position++;

        //printf ("Hi\n");

        //printf ("%c\n", dep->text[dep->global_position]);

        //*current_node->parent->right_child = CopyTreeNode ();

        //*current_node->parent->right_child->parent = *current_node->parent;

        current_node->right_child = CreateTreeNode (sys_func_node->node_type, sys_func_node->key.sys_func);

        current_node = current_node->right_child;

        //printf ("HEY\n");

        //current_node = &(*current_node->parent->right_child->left_child);

        //current_node = &((*current_node)->left_child);
        SkipSpaces (dep);

        if (AddArg (dep, current_node) != 0)
        {
            DestroySubtree (sys_func_node);
            return nullptr;
        }

        //printf ("%c\n", dep->text[dep->global_position]);
    }

        /*if (isdigit (dep->text[dep->global_position]))    //copypast
        {
            *current_node = GetNum(dep);
            SkipSpaces (dep);
        }
        else
        {
            if (isalpha (dep->text[dep->global_position]))
            {
                *current_node = CreateTreeNode (VAR, dep->text + dep->global_position);
                dep->global_position++;

                while (isalpha(dep->text[dep->global_position]) || (dep->text[dep->global_position] == ' ') || (dep->text[dep->global_position] == '_'))
                {
                    dep->global_position++;
                }
            }
            else
            {
                DestroySubtree (sys_func_node);
                //printf ("Something strange here in loop\n");
                SyntaxError (dep, __LINE__);
                return nullptr;
            }
        }*/

    dep->text[dep->global_position] = '\0';
    dep->global_position++;

    //printf ("%lg!!!SSSSSSSS\n", sys_func_node->left_child->left_child->key.val);

    return sys_func_node;
}

bool AddArg (dependencies* dep, tree_node* current_node)
{
    assert (dep          != nullptr);
    assert (current_node != nullptr);
    /*if (isalpha (dep->text[dep->global_position]))
    {
        *current_node = CreateTreeNode (VAR, dep->text + dep->global_position);
        current_node->left_child = CreateTreeNode (VAR, dep->text + dep->global_position);

        do
        {
            dep->global_position++;
        }
        while (isalpha (dep->text[dep->global_position]) || (dep->text[dep->global_position] == ' ') || (dep->text[dep->global_position] == '_'));
    }
    else
    {
        *current_node = GetExpression (dep);

        current_node->left_child = GetExpression (dep);

        if (current_node->left_child == nullptr)
        {
            //DestroySubtree (sys_func_node);
            return 1;
        }
        SkipSpaces (dep);
    }

    return 0;*/

    //printf ("La\n");

    current_node->left_child = GetExpression (dep);

    if (current_node->left_child == nullptr)
    {
        return 1;
    }
    SkipSpaces (dep);

    //printf ("WIN\n");

    return 0;
}

tree_node* GetAssignment (dependencies* dep)
{
    assert (dep != nullptr);

    printf ("GetAssignment reached\n");

    //printf ("%s\n", dep->text + dep->global_position);

    char* var_ptr = dep->text + dep->global_position;

    while (isalpha (dep->text[dep->global_position]) || dep->text[dep->global_position] == ' ' || dep->text[dep->global_position] == '_') dep->global_position++;

    SkipSpaces (dep);

    //printf ("%s\n", dep->text + dep->global_position);

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

    printf ("GetCondOper reached\n");

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

    //dep->global_position++;

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
        //printf ("HEY\n");
        dep->global_position += else_size;
        SkipSpaces (dep);
        //printf ("%c\n", dep->text[dep->global_position - 2]);
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

    printf ("GetCondition reached\n");

    tree_node* left_child = GetExpression (dep);
    if (left_child == nullptr) return nullptr;

    SkipSpaces (dep);

    size_t size_of_cond_operator = 0;

    tree_node* cond_node = nullptr;

    //printf ("%c!!!!!!!!!!!!!!!\n", dep->text[dep->global_position]);

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

    #include "CodeGeneration\Comparing.txt"

    #undef COMPARING

    DestroySubtree (cond_node);
    DestroySubtree (left_child);
    return nullptr;
}

tree_node* GetCondCommand (dependencies* dep)
{
    assert (dep != nullptr);

    printf ("GetCondCommand reached\n");

    //printf ("%s\n", dep->text + dep->global_position);

    size_t begin_len = strlen ("List of commands:");

    size_t end_len   = strlen ("End of commands.");

    tree_node* begin_node   = nullptr;

    tree_node* current_node = nullptr;

    if (strncmp ("List of commands:", dep->text + dep->global_position, begin_len) == 0)
    {
        dep->global_position += begin_len;
        SkipSpaces (dep);
        //printf ("HEY\n");
        //begin_node = CreateTreeNode (COMMAND, 0.0);
        //printf ("%s\n", dep->text + dep->global_position);
        begin_node/*->left_child*/ = GetCommand (dep);
        if (begin_node/*->left_child*/ == nullptr)
        {
            DestroySubtree (begin_node);
            return nullptr;
        }
        current_node = begin_node;
        SkipSpaces (dep);
        while (strncmp ("End of commands.", dep->text + dep->global_position, end_len) != 0)
        {
            //current_node->right_child = CreateTreeNode (COMMAND, 0.0);
            //current_node = current_node->right_child;
            //printf ("%s\n", dep->text + dep->global_position);
            current_node->/*left_child*/right_child = GetCommand (dep);
            if (current_node->/*left_child*/right_child == nullptr)
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

    //printf ("%s\n", dep->text + dep->global_position);

    return GetCommand (dep);
    //return begin_node;
}

tree_node* GetLoop (dependencies* dep)
{
    assert (dep != nullptr);

    printf ("GetLoop reached\n");

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

    //printf ("%s\n", dep->text + dep->global_position);

    dep->global_position++;

    //printf ("%s\n", dep->text + dep->global_position);

    SkipSpaces (dep);

    loop_node->right_child = GetCondCommand (dep);

    return loop_node;
}

tree_node* GetExpression (dependencies* dep)
{
    assert (dep != nullptr);

    printf ("GetExpression reached\n");

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

    printf ("GetTerm reached\n");

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

    printf ("GetPower reached\n");

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

    printf ("GetPrimary reached\n");

    tree_node* ptr = nullptr;

    if (isalpha (dep->text[dep->global_position]))
    {
        /*ptr = GetVar(dep);
        if (ptr == nullptr) return nullptr;

        SkipSpaces (dep);

        if ((dep->text[dep->global_position] == '(') && (ptr->node_type == VAR))
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
        return ptr;*/

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

/*tree_node* GetFunction (dependencies* dep)
{
    assert (dep != nullptr);

    if (!isalpha (dep->text[dep->global_position]))
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
    if (strncmp (#symbol, dep->text + dep->global_position, length) == 0)\
    {                                                               \
        dep->global_position += length;                                    \
        return CreateTreeNode (FUNC, OP_##name);                    \
    }

    #include "Functions.hpp"

    #undef DEF_FUNCTION

    //Only for single-letter variables

    tree_node* ptr = CreateTreeNode (VAR,(char) (dep->text[dep->global_position] - 'a'));

    dep->global_position++;

    return ptr;
}*/

tree_node* GetVar (dependencies* dep)
{
    assert (dep != nullptr);

    //printf ("%c\n", dep->text[dep->global_position]);

    printf ("GetVar reached\n");

    if (!isalpha (dep->text[dep->global_position])) return nullptr;

    tree_node* ptr = CreateTreeNode (VAR, dep->text + dep->global_position);

    //printf ("Nachalo\n%s\n", dep->text + dep->global_position);

    do
    {
        dep->global_position++;
    }
    while (isalpha (dep->text[dep->global_position]) || (dep->text[dep->global_position] == '_'));

    /*dep->text[dep->global_position] = '\0';
    dep->global_position++;*/

    //printf ("Conec\n%s\n", dep->text + dep->global_position);

    return ptr;
}

tree_node* GetBrackets (dependencies* dep)
{
    assert (dep != nullptr);

    printf ("GetBrackets reached\n");

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

    //printf ("%c\n", dep->text[dep->global_position]);

    printf ("GetNum reached\n");

    double val = NAN;

    char* end_ptr = nullptr;

    //printf ("%c\n", dep->text[dep->global_position]);

    val = strtod (dep->text + dep->global_position, &end_ptr);

    //printf ("%lg\n", val);

    if (end_ptr == dep->text + dep->global_position)
    {
        SyntaxError (dep, __LINE__);
        return nullptr;
    }

    if (*(end_ptr - 1)  == '.')
    {
        end_ptr--;
    }



    //printf ("%d\n", end_ptr - (dep->text + dep->global_position));
    //printf ("%c\n", *end_ptr);

    dep->global_position += end_ptr - (dep->text + dep->global_position)/* - 1*/;

    //printf ("%c\n", dep->text[dep->global_position]);

    //printf ("%lg\n", val);
    //printf ("%c!!!!!!!!!!!!!!!!!!!!!!!!!\n", dep->text + dep->global_position);

    //printf ("%lg\n", val);

    return CreateTreeNode (NUM, val);
}

enum SysFunc IsSysFunc (dependencies* dep)
{
    //printf ("IsSysFunc worked \n");

    assert (dep != nullptr);

    size_t size_of_lettering = 0;

    //printf ("HEY\n");

    #define SYS_FUNC(enum_num, lettering, func_lettering)                              \
                                                                                       \
    size_of_lettering = strlen (#lettering);                                           \
                                                                                       \
    if (strncmp (#lettering, dep->text + dep->global_position, size_of_lettering) == 0)\
    {                                                                                  \
        dep->global_position += size_of_lettering;                                     \
        return enum_num;                                                               \
    }

    #include "CodeGeneration\SysFunc.txt"

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

    printf ("Correct line %I64u\n", dep->num_of_lines + 1);

    /*printf ("%s\n", dep->text);
    for (size_t i = 0; i < dep->global_position; i ++)
    {
        printf ("-");
    }
    printf ("^\n");*/
}

void VarChecking (tree_node* current_node)
{
    assert (current_node != nullptr);

    if (current_node->right_child != nullptr) VarChecking (current_node->right_child);

    if (current_node->left_child != nullptr) VarChecking (current_node->left_child);

    if (current_node->node_type == VAR)
    {
        char* ptr = current_node->key.name_ptr;

        //size_t i = 0;

        while (isalpha (*ptr)|| (*ptr == '_'))
        {
            ptr++;
        }

        *ptr = '\0';
    }
}
