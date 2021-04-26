#include "TreeFunctions.hpp"

const char* const STANDART_OUTPUT_ASM_FILE = "Data//Program.txt";

const int NUM_OF_REGISTER = 4;

struct translate_info
{
    char** variables_array = nullptr;
    size_t num_for_labels  = 1;
};

void TranslateToAsm (tree* program_tree, char* name_of_output_file = nullptr);

void TranslateMainNode (tree_node* current_node, FILE* output_file, translate_info* translate_info);

void TranslateNode (tree_node* current_node, FILE* output_file, translate_info* translate_info);

void TranslateSysFuncNode (tree_node* current_node, FILE* output_file, translate_info* translate_info);

void TranslateAssignmentNode (tree_node* current_node, FILE* output_file, translate_info* translate_info);

bool IsExpression (tree_node* current_node);

void TranslateExpression (tree_node* current_node, FILE* output_file, translate_info* translate_info);

void TranslateVarNode (tree_node* current_node, FILE* output_file, translate_info* translate_info);

int GetNumOfReg (tree_node* current_node, translate_info* translate_info);

void TranslateRequiredNode (tree_node* current_node, FILE* output_file, translate_info* translate_info);

void TranslateSeeNode (tree_node* current_node, FILE* output_file, translate_info* translate_info);

void TranslateWriteNode (tree_node* current_node, FILE* output_file, translate_info* translate_info);

void TranslateReturnNode (tree_node* current_node, FILE* output_file, translate_info* translate_info);

void TranslateHltNode (tree_node* current_node, FILE* output_file, translate_info* translate_info);

void TranslateMathOpNode (tree_node* current_node, FILE* output_file, translate_info* translate_info);

void TranslateComparingNode (tree_node* current_node, FILE* output_file, translate_info* translate_info, size_t num = 0);

void TranslateConditionNode (tree_node* current_node, FILE* output_file, translate_info* translate_info);

void TranslateUserFuncNode (tree_node* current_node, FILE* output_file, translate_info* translate_info);

//void TranslateVarNode (tree_node* current_node, FILE* output_file, translate_info* translate_info);

void TranslateNumNode (tree_node* current_node, FILE* output_file, translate_info* translate_info);

void TranslateCommandNode (tree_node* current_node, FILE* output_file, translate_info* translate_info);

void TranslateLoopNode (tree_node* current_node, FILE* output_file, translate_info* translate_info);

void TranslateBlankNode (tree_node* current_node, FILE* output_file, translate_info* translate_info);

char* my_strlwr (const char* string);
