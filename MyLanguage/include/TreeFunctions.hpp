#include <stdio.h>
#include <inttypes.h>
#include <assert.h>
#include <math.h>
#include <string.h>
#include <ctype.h>

#include "TextFunctions.hpp"

const size_t NUM_OF_NODE_TYPES = 10;

const char* const STANDART_GRAPHVIZ_FILE = "Graph//GraphvizFile.txt";

const char* const STANDART_OUTPUT_TREE_FILE = "Data//Tree.txt";

const char* const STANDART_INPUT_TREE_FILE = "Data//Tree.txt";

enum TreeNodeType
{
    #define TREE_NODE_TYPE(TYPE, HEX_CODE, NODE_NAME, FUNC_NAME)\
                                                                \
    TYPE,

    #include "../CodeGeneration/TreeNodeType.txt"

    #undef TREE_NODE_TYPE
};

enum NodeName
{
    NAMELESS,

    SYS_NAME,

    USER_NAME,

    USER_NUM
};

enum SysFunc
{
    #define SYS_FUNC(type, lettering, func_lettering) type,

    #include "../CodeGeneration/SysFunc.txt"

    #undef SYS_FUNC
};

enum MathOp
{
    #define MATH_OP(type, lettering) type,

    #include "../CodeGeneration/MathOp.txt"

    #undef MATH_OP
};

enum Comparing
{
    #define COMPARING(type, lettering) type,

    #include "../CodeGeneration/Comparing.txt"

    #undef COMPARING
};

enum Condition
{
    #define CONDITION(type, lettering) type,

    #include "../CodeGeneration/Condition.txt"

    #undef CONDITION
};

union tree_key
{
    #define TREE_KEY(var_type, name) var_type name;

    #include "../CodeGeneration/TreeKeyUnion.txt"

    #undef TREE_KEY
};

struct tree_node
{
    tree_key          key         = {};
    enum TreeNodeType node_type   = BLANK;
    tree_node*        left_child  = nullptr;
    tree_node*        right_child = nullptr;
    tree_node*        parent      = nullptr;
};

struct tree
{
    tree_node* root = nullptr;
};

struct node_type_info
{
    const char*   name_of_node_type = nullptr;
    const char*   hex_code          = nullptr;
    enum NodeName name_type         = NAMELESS;
};

tree* NewTree ();

#define TREE_KEY(type, name)                                                                                                                    \
                                                                                                                                                \
tree_node* CreateTreeNode (enum TreeNodeType tree_node_type, type key,                                                                          \
                           tree_node* ptr_on_left_child = nullptr, tree_node* ptr_on_right_child = nullptr, tree_node* ptr_on_parent = nullptr);

#include "../CodeGeneration/TreeKeyUnion.txt"

#undef TREE_KEY

tree_node* CopySubtree (tree_node* node);

tree_node* CopyTreeNode (tree_node* node);

void DrawTree (tree_node* root, char* name_of_graphviz_file = nullptr);

node_type_info* FillNodeTypeInfo (node_type_info* node_type_info);

void PrintNodesDescription (FILE* graphviz_file, tree_node* current_node, node_type_info* node_type_info);

void WriteLettering (FILE* graphviz_file, tree_node* current_node, node_type_info* node_type_info);

void WriteSysName (FILE* graphviz_file, tree_node* current_node);

#define SYS_NAME_NODES(node_type, union_type, enum_type) void Write##enum_type##Name (FILE* graphviz_file, enum enum_type num_of_operation);

#include "../CodeGeneration/SysNameNodes.txt"

#undef SYS_NAME_NODES

void SaveTreeInTxt (tree* tree, char* name_of_output_file = nullptr);

void SaveSubtreeInTxt (tree_node* current_node, FILE* output_file, node_type_info* node_type_info);

void SaveSysCode (FILE* output_file, tree_node* current_node);

tree* ReadTreeFromTxt (char* input_file = nullptr);

tree* FillTree (tree* tree, text* input, node_type_info* node_type_info);

tree_node* CaseNewChild (tree_node* current_node);

void DestroyTree (tree* tree);

void DestroySubtree (tree_node* current_node);
