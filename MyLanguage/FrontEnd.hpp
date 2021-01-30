//#include "TextFunctions.cpp"

#include "TreeFunctions.cpp"

const char* const  STANDART_INPUT_FILE = "Code.txt";

struct dependencies
{
    char*  text            = nullptr;
    size_t num_of_lines    = 0;
    size_t global_position = 0;
};

enum FuncType
{
    ADDITIONAL_FUNC,
    MAIN
};

tree* GetGeneral (char* text);

tree_node* GetMain (dependencies* dep);

tree_node* GetUFunc (dependencies* dep);

char* GetFuncName (dependencies* dep);

tree_node* GetFuncBody (dependencies* dep, enum FuncType is_main = ADDITIONAL_FUNC);

tree_node* GetCommand (dependencies* dep);

tree_node* GetSysFunc (dependencies* dep, enum SysFunc num);

bool AddArg (dependencies* dep, tree_node* current_node);

tree_node* GetAssignment (dependencies* dep);

tree_node* GetCondOper (dependencies* dep);

tree_node* GetCondition (dependencies* dep);

tree_node* GetCondCommand (dependencies* dep);

tree_node* GetLoop (dependencies* dep);

tree_node* GetExpression (dependencies* dep);

tree_node* GetTerm (dependencies* dep);

tree_node* GetPower (dependencies* dep);

tree_node* GetPrimary (dependencies* dep);

tree_node* GetVar (dependencies* dep);

tree_node* GetBrackets (dependencies* dep);

tree_node* GetNum (dependencies* dep);

enum SysFunc IsSysFunc (dependencies* dep);

void SkipSpaces (dependencies* dep);

bool Require (dependencies* dep, char char_elem);

bool Require_with_skip (dependencies* dep, const char* string);

void SyntaxError (dependencies* dep, int line);

void VarChecking (tree_node* current_node);
