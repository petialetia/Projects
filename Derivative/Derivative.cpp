#include "Derivative.hpp"

int main (int argC, char* argV[])
{
    text functions = {};

    ProcessInput (argC, argV, &functions, STANDART_INPUT_FILE);

    tree* tree = GetGeneral (functions.pointer_on_buffer);

    DrawTree (tree->root, 1);

    tree_node* derivative = CalculateDerivative (tree->root);

    DrawTree (derivative, 2);

    Simplify (&derivative);

    DrawTree (derivative, 3);

    CreateLatexFile (derivative);

    free (functions.pointer_on_buffer);

    DestroyTree (tree);

    DestroySubtree (derivative);

    system ("start tex.pdf");

    return 0;
}

#include "DSL.hpp"

tree_node* CalculateDerivative (tree_node* original_node)
{
    assert (original_node != nullptr);

    switch (original_node->node_type)
    {
        case CONST: return CreateTreeNode (CONST, 0.0);

        case VAR:   if (original_node->key.num == 'x' - 'a') return CreateTreeNode (CONST, 1.0);
                    return CreateTreeNode (CONST, 0.0);

        case FUNC:  return CalculateDerivativeOfFunctions (original_node);

        default:    printf ("Undefined type of node\n");
                    break;
    }

    return nullptr;
}

tree_node* CalculateDerivativeOfFunctions (tree_node* original_node)
{
    assert (original_node != nullptr);

    switch (original_node->key.num)
    {
        case OP_ADD: return ADD (dL, dR);

        case OP_SUB: return SUB (dL, dR);

        case OP_MUL: return ADD (MUL (dL, cR), MUL (cL, dR));

        case OP_DIV: return DIV (SUB (MUL (dL, cR), MUL (cL, dR)), POW (cR, CONST_CHILD (2.0)));

        case OP_POW: return MUL (MUL (POW (cL, SUB (cR, CONST_CHILD (1.0))), cR), dL);

        case OP_SIN: return MUL (COS (cL, nullptr), dL);

        case OP_COS: return MUL (MUL (CONST_CHILD (-1.0), SIN (cL, nullptr)), dL);

        case OP_TAN: return MUL (DIV (CONST_CHILD (1.0), POW (COS (cL, nullptr), CONST_CHILD (2.0))), dL);

        case OP_CTG: return MUL (MUL (CONST_CHILD (-1.0), DIV (CONST_CHILD (1.0), POW (SIN (dL, nullptr), CONST_CHILD (2.0)))), dL);

        case OP_LN:  return MUL (DIV (CONST_CHILD (1.0), cL), dL);

        default:     printf ("Undefined function is written in node\n");
        break;
    }
    return nullptr;
}

#include "UnDSL.hpp"

#define check_for_const(operator)                                                           \
                                                                                            \
if (((*node)->right_child->node_type == CONST) && ((*node)->left_child->node_type == CONST))\
{                                                                                           \
    (*node)->node_type = CONST;                                                             \
    (*node)->key.val = (*node)->left_child->key.val operator (*node)->right_child->key.val; \
    free ((*node)->right_child);                                                            \
    free ((*node)->left_child);                                                             \
    (*node)->left_child = nullptr;                                                          \
    (*node)->right_child = nullptr;                                                         \
    return;                                                                                 \
}

#define check_for_const_func(func)                                                          \
                                                                                            \
if (((*node)->right_child->node_type == CONST) && ((*node)->left_child->node_type == CONST))\
{                                                                                           \
    (*node)->node_type = CONST;                                                             \
    (*node)->key.val = func ((*node)->left_child->key.val, (*node)->right_child->key.val);  \
    free ((*node)->right_child);                                                            \
    free ((*node)->left_child);                                                             \
    (*node)->left_child = nullptr;                                                          \
    (*node)->right_child = nullptr;                                                         \
    return;                                                                                 \
}

#define check_for_const_func_unary(func)                   \
                                                           \
if ((*node)->left_child->node_type == CONST)               \
{                                                          \
    (*node)->node_type = CONST;                            \
    (*node)->key.val = func ((*node)->left_child->key.val);\
    free ((*node)->left_child);                            \
    (*node)->left_child = nullptr;                         \
    return;                                                \
}

#define ckeck_for_neutral_elem_commutative(neutral_elem)\
                                                        \
ckeck_for_neutral_elem (neutral_elem, left,  right)     \
ckeck_for_neutral_elem (neutral_elem, right, left)

#define ckeck_for_neutral_elem(neutral_elem, direction, opposite_direction)                                           \
                                                                                                                      \
if (((*node)->direction##_child->node_type == CONST) && (IsZero ((*node)->direction##_child->key.val - neutral_elem)))\
{                                                                                                                     \
    if ((*node)->parent == nullptr)                                                                                   \
    {                                                                                                                 \
        free ((*node)->direction##_child);                                                                            \
                                                                                                                      \
        tree_node* old_node = *node;                                                                                  \
                                                                                                                      \
        *node = (*node)->opposite_direction##_child;                                                                  \
                                                                                                                      \
        free (old_node);                                                                                              \
        return;                                                                                                       \
    }                                                                                                                 \
}

#define ckeck_for_definite_elem_commutative(definite_elem, result)\
                                                                  \
ckeck_for_definite_elem (definite_elem, result, left,  right)     \
ckeck_for_definite_elem (definite_elem, result, right, left)


#define ckeck_for_definite_elem(definite_elem, result, direction, opposite_direction)                                  \
                                                                                                                       \
if (((*node)->direction##_child->node_type == CONST) && (IsZero ((*node)->direction##_child->key.val) - definite_elem))\
{                                                                                                                      \
    (*node)->node_type = CONST;                                                                                        \
    (*node)->key.val   = result;                                                                                       \
    free ((*node)->direction##_child);                                                                                 \
    DestroySubtree ((*node)->opposite_direction##_child);                                                              \
    (*node)->direction##_child = nullptr;                                                                              \
    (*node)->opposite_direction##_child = nullptr;                                                                     \
    return;                                                                                                            \
}                                                                                                                      \

void Simplify (tree_node** node)
{
    assert (node != nullptr);

    if ((*node)->left_child != nullptr)  Simplify (&((*node)->left_child));

    if ((*node)->right_child != nullptr) Simplify (&((*node)->right_child));

    if (((*node)->node_type == CONST) || ((*node)->node_type == VAR)) return;

    SimplifyFunctions (node);
}

void SimplifyFunctions (tree_node** node)
{
    assert (node != nullptr);

    switch ((*node)->key.num)
    {
        case OP_ADD: SimplifyAdd (node);
                     break;

        case OP_SUB: SimplifySub (node);
                     break;

        case OP_MUL: SimplifyMul (node);
                     break;

        case OP_DIV: SimplifyDiv (node);
                     break;

        case OP_POW: SimplifyPow (node);
                     break;

        #define DEF_FUNCTION(name, symbol, num)            \
                                                           \
        case OP_##name: check_for_const_func_unary (symbol)\
                        break;

        #include "BracketFunctions.hpp"

        #undef DEF_FUNCTION

        default:     printf ("Unknown function found\n");
                     break;
    }
}

void SimplifyAdd (tree_node** node)
{
    assert (node != nullptr);

    check_for_const (+)
    ckeck_for_neutral_elem_commutative (0)
}

void SimplifySub (tree_node** node)
{
    assert (node != nullptr);

    check_for_const (-)
    ckeck_for_neutral_elem (0, right, left)
    if (((*node)->left_child->node_type == CONST) && (IsZero ((*node)->left_child->key.val)))
    {
        (*node)->key.num = OP_MUL;
        (*node)->left_child->key.val = -1;
        return;
    }
}

void SimplifyMul (tree_node** node)
{
    assert (node != nullptr);

    check_for_const (*)
    ckeck_for_neutral_elem_commutative  (1)
    ckeck_for_definite_elem_commutative (0, 0)
}

void SimplifyDiv (tree_node** node)
{
    assert (node != nullptr);

    check_for_const (/)
    ckeck_for_neutral_elem  (1, right, left)
    ckeck_for_definite_elem (0, 0, left, right)
}

void SimplifyPow (tree_node** node)
{
    assert (node != nullptr);

    check_for_const_func (pow)
    ckeck_for_neutral_elem  (1, right, left)
    ckeck_for_definite_elem (1, 1, left, right)
    ckeck_for_definite_elem (0, 0, left,  right)
    ckeck_for_definite_elem (0, 1, right, left)
}

double ctg (double val)
{
    return cos (val)/sin (val);
}

double ln (double val)
{
    return log (val);
}

#undef check_for_const

#undef check_for_const_func

#undef check_for_const_func_unary

#undef ckeck_for_neutral_elem_commutative

#undef ckeck_for_neutral_elem

#undef ckeck_for_definite_elem_commutative

#undef ckeck_for_definite_elem

void CreateLatexFile (tree_node* root)
{
    assert (root != nullptr);

    FILE* tex_file = fopen ("tex.tex", "w");

    assert (tex_file != nullptr);

    fprintf (tex_file, "\\documentclass[32pt]{article}\n");
    fprintf (tex_file, "\\begin{document}\n$");

    ConvertTreeNode (tex_file, root);

    fprintf (tex_file, "$\n\\end{document}");

    fclose (tex_file);

    system ("pdflatex tex.tex");
}

void ConvertTreeNode (FILE* tex_file, tree_node* current_node)
{
    assert (tex_file != nullptr);
    assert (current_node != nullptr);

    switch (current_node->node_type)
    {
        case VAR:   fprintf (tex_file, "%c", current_node->key.num + 'a');
                    return;

        case CONST: fprintf (tex_file, "%lg", current_node->key.val);
                    return;

        case FUNC:  ConvertFunc (tex_file, current_node);
                    break;

        default:    printf ("Undefined type of node found\n");
                    break;
    }
}

#define mul_convert_check_for_priority(direction)                                                                              \
                                                                                                                               \
if ((current_node->direction##_child->node_type == FUNC) && (current_node->direction##_child->key.num < current_node->key.num))\
{                                                                                                                              \
    fprintf (tex_file, "(");                                                                                                   \
    ConvertTreeNode (tex_file, current_node->direction##_child);                                                               \
    fprintf (tex_file, ")");                                                                                                   \
}                                                                                                                              \
else ConvertTreeNode (tex_file, current_node->direction##_child);


void ConvertFunc (FILE* tex_file, tree_node* current_node)
{
    assert (tex_file != nullptr);
    assert (current_node != nullptr);

    switch (current_node->key.num)
    {
        case OP_ADD: ConvertAdd (tex_file, current_node);
                     return;

        case OP_SUB: ConvertSub (tex_file, current_node);
                     return;

        case OP_MUL: ConvertMul (tex_file, current_node);
                     return;

        case OP_DIV: ConvertDiv (tex_file, current_node);
                     return;

        case OP_POW: ConvertPow (tex_file, current_node);
                     return;

        #define DEF_FUNCTION(name, symbol, num)                              \
                                                                             \
        case OP_##name: fprintf (tex_file,  "%s", #symbol);                  \
                        fprintf (tex_file, "(");                             \
                        ConvertTreeNode (tex_file, current_node->left_child);\
                        fprintf (tex_file, ")");                             \
                        return;

        #include "BracketFunctions.hpp"

        #undef DEF_FUNCTION

        default:     printf ("Undefined function found\n");
                     break;
        }
}

void ConvertAdd (FILE* tex_file, tree_node* current_node)
{
    assert (tex_file != nullptr);
    assert (current_node != nullptr);

    ConvertTreeNode (tex_file, current_node->left_child);
    fprintf (tex_file, "+");
    ConvertTreeNode (tex_file, current_node->right_child);
}

void ConvertSub (FILE* tex_file, tree_node* current_node)
{
    assert (tex_file != nullptr);
    assert (current_node != nullptr);

    ConvertTreeNode (tex_file, current_node->left_child);
    fprintf (tex_file, "-");
    ConvertTreeNode (tex_file, current_node->right_child);
}

void ConvertMul (FILE* tex_file, tree_node* current_node)
{
    assert (tex_file != nullptr);
    assert (current_node != nullptr);

    if ((current_node->left_child->node_type == CONST) && (current_node->right_child->node_type == CONST))
    {
        ConvertTreeNode (tex_file, current_node->left_child);
        fprintf (tex_file, "*");
        ConvertTreeNode (tex_file, current_node->right_child);
        return;
    }

    if ((current_node->left_child->node_type == VAR) && (current_node->right_child->node_type == VAR))
    {
        ConvertTreeNode (tex_file, current_node->left_child);
        fprintf (tex_file, "*");
        ConvertTreeNode (tex_file, current_node->right_child);
        return;
    }

    if ((current_node->right_child->node_type == CONST) && (current_node->left_child->node_type == VAR))
    {
        ConvertTreeNode (tex_file, current_node->right_child);
        ConvertTreeNode (tex_file, current_node->left_child);
        return;
    }

    mul_convert_check_for_priority (left)
    mul_convert_check_for_priority (right)
}

void ConvertDiv (FILE* tex_file, tree_node* current_node)
{
    assert (tex_file != nullptr);
    assert (current_node != nullptr);

    fprintf (tex_file, "\\frac{");
    ConvertTreeNode (tex_file, current_node->left_child);
    fprintf (tex_file, "}{");
    ConvertTreeNode (tex_file, current_node->right_child);
    fprintf (tex_file, "}");
}

void ConvertPow (FILE* tex_file, tree_node* current_node)
{
    assert (tex_file != nullptr);
    assert (current_node != nullptr);

    if (current_node->left_child->node_type == FUNC)
    {
        fprintf (tex_file, "(");
        ConvertTreeNode (tex_file, current_node->left_child);
        fprintf (tex_file, ")");
    }
    else ConvertTreeNode (tex_file, current_node->left_child);

    fprintf (tex_file, "^");

    fprintf (tex_file, "{");

    if (current_node->right_child->node_type == FUNC)
    {
        fprintf (tex_file, "(");
        ConvertTreeNode (tex_file, current_node->right_child);
        fprintf (tex_file, ")");
    }
    else ConvertTreeNode (tex_file, current_node->right_child);

    fprintf (tex_file, "}");
}

#undef mul_convert_check_for_priority
