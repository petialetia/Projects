#include "TextFunctions.hpp"

#include "TreeFunctions.hpp"

const char* const  STANDART_INPUT_FILE = "Function.txt";

const double PSEUDO_NULL = 1E-6;

void ProcessFunction (tree* tree);

char NumForX ();

tree_node* CalculateDerivative (tree_node* original_node);

tree_node* CalculateDerivativeOfFunctions (tree_node* original_node);

tree_node* CalculateDerivativePow (tree_node* original_node);

bool IsThereX (tree_node* current_node);

void Simplify (tree_node** node);

void SimplifyFunctions (tree_node** node);

void SimplifyAdd (tree_node** node);

void SimplifySub (tree_node** node);

void SimplifyMul (tree_node** node);

void SimplifyDiv (tree_node** node);

void SimplifyPow (tree_node** node);

double ctg (double val);

double ln (double val);

double sh (double val);

double ch (double val);

double th (double val);

double cth (double val);

void CreateLatexFile (tree_node* root);

void ConvertTreeNode (FILE* tex_file, tree_node* current_node);

void ConvertFunc (FILE* tex_file, tree_node* current_node);

void ConvertAdd (FILE* tex_file, tree_node* current_node);

void ConvertSub (FILE* tex_file, tree_node* current_node);

void ConvertMul (FILE* tex_file, tree_node* current_node);

void ConvertDiv (FILE* tex_file, tree_node* current_node);

void ConvertPow (FILE* tex_file, tree_node* current_node);
