#include <stdio.h>
#include <inttypes.h>
#include <assert.h>
#include <math.h>
#include <string.h>
#include <ctype.h>

const char CONST = 1;

const char FUNC  = 2;

const char VAR   = 3;

const char EMPTY = 0;

#define DEF_FUNC(name, symbol, num)\
                                   \
const char OP_##name = num;

#include "Functions.hpp"

#undef DEF_FUNC

const double PSEUDO_ZERO = 1E-6;

const char* const STANDART_NAME_OF_GRAPH_FILE = "Graph\\GraphvizFile";

const char* const STANDDART_GRAPG_FILE_EXTENSION = ".txt";

const size_t SIZE_OF_BEGINNING_OF_GRAPH_FILE = 16;

const size_t MAX_NUM_OF_FUNC = 9999;

const size_t MAX_LENGTH_OF_NUM_OF_FUNC = 4;

const size_t MAX_LENGTH_OF_SYSTEM_CMD = 48;

union tree_key
{
    double val;
    char   num;
};

struct tree_node
{
    tree_key   key         = {};
    char       node_type   = EMPTY;
    tree_node* left_child  = nullptr;
    tree_node* right_child = nullptr;
    tree_node* parent      = nullptr;
};

struct tree
{
    tree_node* root = nullptr;
};

tree* NewTree ();

tree_node* CreateTreeNode (char node_type, double key,
                           tree_node* ptr_on_left_child = nullptr, tree_node* ptr_on_right_child = nullptr, tree_node* ptr_on_parent = nullptr);

tree_node* CreateTreeNode (char node_type, char key,
                           tree_node* ptr_on_left_child = nullptr, tree_node* ptr_on_right_child = nullptr, tree_node* ptr_on_parent = nullptr);

tree_node* CopySubtree (tree_node* node);

void DrawTree (tree_node* root, size_t num = 0);

void PrintNodesDescription (FILE* graphviz_file, tree_node* current_node);

const char* FinNameOfFunc (char key);

void DestroyTree (tree* tree);

void DestroySubtree (tree_node* current_node);


//-----------------------------------------------------------------------------

//Recursive descent parser

//-----------------------------------------------------------------------------


struct dependencies
{
    const char*       string    = nullptr;
    size_t            position  = 0;
};

tree* GetGeneral (const char* string);

tree_node* GetExpression (dependencies* dep);

tree_node* GetTerm (dependencies* dep);

tree_node* GetPower (dependencies* dep);

tree_node* GetPrimary (dependencies* dep);

tree_node* GetFunction (dependencies* dep);

tree_node* GetBrackets (dependencies* dep);

tree_node* GetNumber (dependencies* dep);

void SkipSpaces (dependencies* dep);

bool Require (dependencies* dep, char symbol);

void SyntaxError (dependencies* dep, int line);

bool IsZero (double val);

void DivByZero (dependencies* dep);

void InvalidArgument (dependencies* dep);
