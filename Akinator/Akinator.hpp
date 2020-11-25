#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "Stack.hpp"

const size_t MAX_LENGTH_OF_KEY = 100;

const size_t START_CAPACITY = 10;

const size_t INCREASE_PARAMETR = 10;

const size_t MAX_LENGTH_OF_REPLY = 100;

const char* STANDARD_DATA_BASE = "DataBase.txt";

const char* STANDART_GRAPHVIZ_FILE = "GraphvizFile.txt";

struct tree_node
{
    char*      key         = nullptr;
    tree_node* left_child  = nullptr;
    tree_node* right_child = nullptr;
    tree_node* parent      = nullptr;
};

struct tree
{
    tree_node* root             = nullptr;
    char*      new_nodes_buffer = nullptr;
    size_t     num_of_new_nodes = 0;
    size_t     buffer_capacity  = 0;
};

struct text
{
    size_t file_length      = 0;
    char* pointer_on_buffer = nullptr;
};

struct keys
{
    size_t in  = 0;
};

tree* create_tree_with_base (int argC, char* argV[], text* akinator_base);

size_t get_arg_val (int argC, char* argV[], char* key);

size_t find_length_of_file (FILE* input);

void copy_file_in_buffer (FILE* input, char* pointer_on_buffer, size_t* file_length);

void fill_tree (text* akinator_base, tree* acinator_tree);

char* case_add_key (char* pointer, tree_node* current_node);

tree_node* case_move_to_child (tree_node* current_node);

void construct_tree (tree* akinator_tree);

void print_greeting ();

void help_print ();

void StartProgramm (tree* akinator_tree);

void DrawTree (tree* akinator_tree);

void print_nodes_description (FILE* graphviz_file, tree_node* current_node);

void GameMod (tree* akinator_tree);

void print_answer (tree_node* current_node, tree* akinator_tree);

void add_new_key (tree_node* current_node, tree* akinator_tree);

void extend_buffer_of_new_nodes (tree* akinator_tree);

void ask_for_difference (tree_node* current_node, char* ptr_on_answer, char* ptr_on_sign_of_differentiation);

void add_new_nodes (tree_node* current_node, char* ptr_on_answer, char* ptr_on_sign_of_differentiation, tree* akinator_tree);

void CreatNewBase (tree* akinator_tree);

void print_descendant (FILE* data_base_file, tree_node* current_node);

void PrintDefinition (tree* akinator_tree);

void printing_description (tree_node* answer, Stack_t* path_stack, tree* akinator_tree);

tree_node* print_sign (bool prev_answer, tree_node* current_node, char* to_be);

void print_separator (bool prev_answer, Stack_t* path_stack);

void print_standart_description (tree_node* answer, Stack_t* path_stack, tree_node* current_node);

void CompareKeys (tree* akinator_tree);

void ask_for_words (char* key1, char* key2);

tree_node* print_similarity (tree_node* current_node, tree_node* answer1, tree_node* answer2, Stack_t* PathStack1, Stack_t* PathStack2);

tree_node* find_node_by_key (char* key, tree_node* current_node, Stack_t* path_stack);

void fill_path_stack (tree_node* current_node, Stack_t* path_stack);

void DestroyTree (tree* akinator_tree);

void destroy_nodes (tree_node* current_node);
