#include "Derivative.hpp"

int main (int argC, char* argV[])
{
    text functions = {};

    ProcessInput (argC, argV, &functions, STANDART_INPUT_FILE);

    tree* tree = GetGeneral (functions.pointer_on_buffer);

    if (tree == nullptr)
    {
        free (functions.pointer_on_buffer);

        return 0;
    }

    ProcessFunction (tree);

    free (functions.pointer_on_buffer);

    return 0;
}

void ProcessFunction (tree* tree)
{
    DrawTree (tree->root, 1);

    tree_node* derivative = CalculateDerivative (tree->root);

    DrawTree (derivative, 2);

    Simplify (&derivative);

    DrawTree (derivative, 3);

    CreateLatexFile (derivative);

    DestroyTree (tree);

    DestroySubtree (derivative);

    system ("start tex.pdf");
}

char NumForX ()
{
    return 'x' - 'a';
}

#include "DSL.hpp"

tree_node* CalculateDerivative (tree_node* original_node)
{
    assert (original_node != nullptr);

    switch (original_node->node_type)
    {
        case CONST: return CreateTreeNode (CONST, 0.0);

        case VAR:   if (original_node->key.num == NumForX()) return CreateTreeNode (CONST, 1.0);
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

        case OP_POW: return CalculateDerivativePow (original_node);

        case OP_SIN: return MUL (COS (cL, nullptr), dL);

        case OP_COS: return MUL (MUL (CONST_CHILD (-1.0), SIN (cL, nullptr)), dL);

        case OP_TAN: return MUL (DIV (CONST_CHILD (1.0), POW (COS (cL, nullptr), CONST_CHILD (2.0))), dL);

        case OP_CTG: return MUL (MUL (CONST_CHILD (-1.0), DIV (CONST_CHILD (1.0), POW (SIN (dL, nullptr), CONST_CHILD (2.0)))), dL);

        case OP_LN:  return MUL (DIV (CONST_CHILD (1.0), cL), dL);

        case OP_EXP: return MUL (EXP (cL, nullptr), dL);

        case OP_SH:  return MUL (CH (cL, nullptr), dL);

        case OP_CH:  return MUL (SH (cL, nullptr), dL);

        case OP_TH:  return MUL (DIV (CONST_CHILD (1.0), POW (CH (cL, nullptr), CONST_CHILD (2.0))), dL);

        case OP_CTH: return MUL (MUL (CONST_CHILD (-1.0), DIV (CONST_CHILD (1.0), POW (SH (dL, nullptr), CONST_CHILD (2.0)))), dL);

        default:     printf ("Undefined function is written in node\n");
        break;
    }
    return nullptr;
}

tree_node* CalculateDerivativePow (tree_node* original_node)
{
    if (IsThereX (original_node->right_child))
    {
        if (IsThereX (original_node->left_child)) return MUL (POW (cL, cR), ADD (MUL (LN (cL, nullptr), dR), DIV (MUL (cR, dL), cL)));
        else                                      return MUL (POW (cL, cR), MUL (dR, LN (cL, nullptr)));
    }
    else
    {
        if (IsThereX (original_node->left_child)) return MUL (MUL (POW (cL, SUB (cR, CONST_CHILD (1.0))), cR), dL);
        else                                      return CONST_CHILD (0.0);
    }
}

bool IsThereX (tree_node* current_node)
{
    if (current_node == nullptr) return 0;

    if ((current_node->node_type == VAR) && (current_node->key.num == NumForX ())) return 1;
    if (IsThereX (current_node->left_child))  return 1;
    if (IsThereX (current_node->right_child)) return 1;

    return 0;
}

#include "UnDSL.hpp"

#define CHECK_FOR_CONST(operator)                                                           \
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

#define CHECK_FOR_CONST_FUNC(func)                                                          \
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

#define CHECK_FOR_CONST_FUNC_UNARY(func)                   \
                                                           \
if ((*node)->left_child->node_type == CONST)               \
{                                                          \
    (*node)->node_type = CONST;                            \
    (*node)->key.val = func ((*node)->left_child->key.val);\
    free ((*node)->left_child);                            \
    (*node)->left_child = nullptr;                         \
    return;                                                \
}

#define CHECK_FOR_NEUTRAL_ELEM_COMMUTATIVE(neutral_elem)\
                                                        \
CHECK_FOR_NEYTRAL_ELEM (neutral_elem, left,  right)     \
CHECK_FOR_NEYTRAL_ELEM (neutral_elem, right, left)

#define CHECK_FOR_NEYTRAL_ELEM(neutral_elem, direction, opposite_direction)                                           \
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

#define CHECK_FOR_DEFINITIVE_ELEM_COMMUTATIVE(definite_elem, result)\
                                                                    \
CHECK_FOR_DEFINITIVE_ELEM (definite_elem, result, left,  right)     \
CHECK_FOR_DEFINITIVE_ELEM (definite_elem, result, right, left)


#define CHECK_FOR_DEFINITIVE_ELEM(definite_elem, result, direction, opposite_direction)                                \
                                                                                                                       \
if (((*node)->direction##_child->node_type == CONST) && (IsZero ((*node)->direction##_child->key.val - definite_elem)))\
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
        case OP_##name: CHECK_FOR_CONST_FUNC_UNARY (symbol)\
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

    CHECK_FOR_CONST (+)
    CHECK_FOR_NEUTRAL_ELEM_COMMUTATIVE (0)
}

void SimplifySub (tree_node** node)
{
    assert (node != nullptr);

    CHECK_FOR_CONST (-)
    CHECK_FOR_NEYTRAL_ELEM (0, right, left)
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

    CHECK_FOR_CONST (*)
    CHECK_FOR_NEUTRAL_ELEM_COMMUTATIVE  (1)
    CHECK_FOR_DEFINITIVE_ELEM_COMMUTATIVE (0, 0)
}

void SimplifyDiv (tree_node** node)
{
    assert (node != nullptr);

    CHECK_FOR_CONST (/)
    CHECK_FOR_NEYTRAL_ELEM  (1, right, left)
    CHECK_FOR_DEFINITIVE_ELEM (0, 0, left, right)
}

void SimplifyPow (tree_node** node)
{
    assert (node != nullptr);

    CHECK_FOR_CONST_FUNC (pow)
    CHECK_FOR_NEYTRAL_ELEM  (1, right, left)
    CHECK_FOR_DEFINITIVE_ELEM (1, 1, left, right)
    CHECK_FOR_DEFINITIVE_ELEM (0, 0, left,  right)
    CHECK_FOR_DEFINITIVE_ELEM (0, 1, right, left)
}

double ctg (double val)
{
    return cos (val)/sin (val);
}

double ln (double val)
{
    return log (val);
}

double sh (double val)
{
    return sinh (val);
}

double ch (double val)
{
    return cosh (val);
}

double th (double val)
{
    return tanh (val);
}

double cth (double val)
{
    return ch (val)/sh (val);
}

#undef CHECK_FOR_CONST

#undef CHECK_FOR_CONST_FUNC

#undef CHECK_FOR_CONST_FUNC_UNARY

#undef CHECK_FOR_NEUTRAL_ELEM_COMMUTATIVE

#undef CHECK_FOR_NEYTRAL_ELEM

#undef CHECK_FOR_DEFINITIVE_ELEM_COMMUTATIVE

#undef CHECK_FOR_DEFINITIVE_ELEM

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
    assert (tex_file     != nullptr);
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

#define CONVERT_CHECK_FOR_PRIORITY(direction)                                                                                  \
                                                                                                                               \
if ((current_node->direction##_child->node_type == FUNC) && (current_node->direction##_child->key.num < current_node->key.num))\
{                                                                                                                              \
    fprintf (tex_file, "(");                                                                                                   \
    ConvertTreeNode (tex_file, current_node->direction##_child);                                                               \
    fprintf (tex_file, ")");                                                                                                   \
}                                                                                                                              \
else ConvertTreeNode (tex_file, current_node->direction##_child);

#define CONVERT_MUL_CHECK_FOR_CONST(direction, opposite_direction)           \
                                                                             \
if (current_node->direction##_child->node_type == CONST)                     \
{                                                                            \
    if (current_node->direction##_child->key.val > 0)                        \
    {                                                                        \
        ConvertTreeNode (tex_file, current_node->direction##_child);         \
        ConvertTreeNode (tex_file, current_node->opposite_direction##_child);\
    }                                                                        \
    else                                                                     \
    {                                                                        \
        fprintf (tex_file, "(");                                             \
        ConvertTreeNode (tex_file, current_node->direction##_child);         \
        fprintf (tex_file, ")");                                             \
        fprintf (tex_file, "*");                                             \
        ConvertTreeNode (tex_file, current_node->opposite_direction##_child);\
    }                                                                        \
    return;                                                                  \
}


void ConvertFunc (FILE* tex_file, tree_node* current_node)
{
    assert (tex_file     != nullptr);
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
    assert (tex_file     != nullptr);
    assert (current_node != nullptr);

    ConvertTreeNode (tex_file, current_node->left_child);
    fprintf (tex_file, "+");
    ConvertTreeNode (tex_file, current_node->right_child);
}

void ConvertSub (FILE* tex_file, tree_node* current_node)
{
    assert (tex_file     != nullptr);
    assert (current_node != nullptr);

    ConvertTreeNode (tex_file, current_node->left_child);
    fprintf (tex_file, "-");
    ConvertTreeNode (tex_file, current_node->right_child);
}

void ConvertMul (FILE* tex_file, tree_node* current_node)
{
    assert (tex_file     != nullptr);
    assert (current_node != nullptr);

    CONVERT_MUL_CHECK_FOR_CONST (left,  right)
    CONVERT_MUL_CHECK_FOR_CONST (right, left )

    CONVERT_CHECK_FOR_PRIORITY (left)
    fprintf (tex_file, "*");
    CONVERT_CHECK_FOR_PRIORITY (right)
}

void ConvertDiv (FILE* tex_file, tree_node* current_node)
{
    assert (tex_file     != nullptr);
    assert (current_node != nullptr);

    fprintf (tex_file, "\\frac{");
    ConvertTreeNode (tex_file, current_node->left_child);
    fprintf (tex_file, "}{");
    ConvertTreeNode (tex_file, current_node->right_child);
    fprintf (tex_file, "}");
}

void ConvertPow (FILE* tex_file, tree_node* current_node)
{
    assert (tex_file     != nullptr);
    assert (current_node != nullptr);

    if (current_node->left_child->node_type == FUNC)
    {
        fprintf (tex_file, "(");                                                                                                   \
        ConvertTreeNode (tex_file, current_node->left_child);                                                               \
        fprintf (tex_file, ")");
    }
    else ConvertTreeNode (tex_file, current_node->left_child);

    fprintf (tex_file, "^");

    fprintf (tex_file, "{");

    CONVERT_CHECK_FOR_PRIORITY (right)

    fprintf (tex_file, "}");
}

#undef CONVERT_CHECK_FOR_PRIORITY

#undef CONVERT_MUL_CHECK_FOR_CONST
