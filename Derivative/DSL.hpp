#define dL CalculateDerivative (original_node->left_child)

#define dR CalculateDerivative (original_node->right_child)

#define cR CopySubtree (original_node->right_child)

#define cL CopySubtree (original_node->left_child)

#define ADD(left_child, right_child) CreateTreeNode (FUNC, OP_ADD, left_child, right_child)

#define SUB(left_child, right_child) CreateTreeNode (FUNC, OP_SUB, left_child, right_child)

#define MUL(left_child, right_child) CreateTreeNode (FUNC, OP_MUL, left_child, right_child)

#define DIV(left_child, right_child) CreateTreeNode (FUNC, OP_DIV, left_child, right_child)

#define POW(left_child, right_child) CreateTreeNode (FUNC, OP_POW, left_child, right_child)

#define SIN(left_child, right_child) CreateTreeNode (FUNC, OP_SIN, left_child, right_child)

#define COS(left_child, right_child) CreateTreeNode (FUNC, OP_COS, left_child, right_child)

#define TAN(left_child, right_child) CreateTreeNode (FUNC, OP_TAN, left_child, right_child)

#define CTG(left_child, right_child) CreateTreeNode (FUNC, OP_CTG, left_child, right_child)

#define LN(left_child, right_child) CreateTreeNode (FUNC, OP_LN, left_child, right_child)

#define EXP(left_child, right_child) CreateTreeNode (FUNC, OP_EXP, left_child, right_child)

#define SH(left_child, right_child) CreateTreeNode (FUNC, OP_SH, left_child, right_child)

#define CH(left_child, right_child) CreateTreeNode (FUNC, OP_CH, left_child, right_child)

#define TH(left_child, right_child) CreateTreeNode (FUNC, OP_TH, left_child, right_child)

#define CTH(left_child, right_child) CreateTreeNode (FUNC, OP_CTH, left_child, right_child)

#define CONST_CHILD(val) CreateTreeNode (CONST, val)
