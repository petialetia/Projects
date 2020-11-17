#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <windows.h>

#define SUPERPROTECT

#define WINDOWS

//system ("dot -Tpng graphviz_file_logic.txt -o graphviz_file_logic.png");

//system ("dot -Tpng graphviz_file_phys.txt -o graphviz_file_phys.png");

//system ("start graphviz_file_logic.png");

//system ("start graphviz_file_phys.png");

typedef double Elem_t;

const Elem_t POISON = NAN;

/*typedef int Elem_t;

const Elem_t POISON = 0;*/

size_t NUM_OF_PRINT_IN_LOG_FILE = 0;

FILE* LOG_FILE = nullptr;

const size_t PREFERABLE_START_CAPACITY = 60;

const size_t MAX_CAPACITY              = 10000000;

const size_t SIZE_OF_NAME_OF_PTR       = 5;

const bool   LIST_ORDERED              = 0;

const bool   LIST_NOT_ORDERED          = 1;

const bool   AFTER                     = 1;

const bool   BEFORE                    = 0;

#ifdef SUPERPROTECT

enum list_status
{
    NoError,

    #define DEF_CMD(name, condition, error_description)\
    name,

    #include "Errors.hpp"

    #undef DEF_CMD

    IncorrectListNode,

    IncorrectEmptyNode
};

#endif

struct list_node
{
    Elem_t val  = POISON;
    int next = 0;
    int prev = 0;
};

struct List_t
{
    size_t      size                    = 0;
    size_t      capacity                = 0;
    list_node*  data                    = nullptr;
    bool        mod                     = 0;

#ifdef SUPERPROTECT

    char*       name_of_list            = nullptr;
    list_status status                  = NoError;
    size_t      index_of_incorrect_node = 0;

#endif

    size_t      head                    = 0;
    size_t      tail                    = 0;
    size_t      free                    = 0;
};

void Construct (List_t* list, int start_capacity = PREFERABLE_START_CAPACITY);

#ifdef SUPERPROTECT

bool can_list_be_constructed (List_t* list, int start_capacity);

void print_warning (char* str);

#endif

#ifdef WINDOWS

bool can_ptr_be_used (const void* ptr);

#endif

void spill_poison (List_t* list, size_t start_position);

void Destroy (List_t* list);

size_t insertion (List_t* list, size_t left, size_t right, Elem_t val);

void check_will_list_stay_ordered (List_t* list, bool condition);

size_t InsertBack (List_t* list, Elem_t val);

size_t InsertFront (List_t* list, Elem_t val);

Elem_t Erase (List_t* list, size_t index);

Elem_t erasing (List_t* list, size_t index);

Elem_t EraseFront (List_t* list);

Elem_t EraseBack (List_t* list);

size_t insert_in_any_place (List_t* list, size_t index, Elem_t val, bool condition_of_disordering, bool direction);

size_t InsertBefore (List_t* list, size_t index, Elem_t val);

size_t InsertAfter (List_t* list, size_t index, Elem_t val);

#ifdef SUPERPROTECT

bool is_index_correct (List_t* list, size_t index);

#endif

size_t FindIndex (List_t* list, size_t num);

#ifdef SUPERPROTECT

bool is_num_correct (List_t* list, size_t num);

#endif

Elem_t GetVal (List_t* list, size_t num);

void ChangeMod (List_t* list);

void put_nodes_in_order (List_t* list);

void move_list_nodes (List_t* list, Elem_t* vals_buffer);

void move_empty_nodes (List_t* list);


//-----------------------------------------------------------------------------


bool is_list_looped (List_t* list);

bool are_empty_nodes_looped (List_t* list);

size_t find_incorrect_list_node (List_t* list);

size_t find_incorrect_node_in_unsorted_list (List_t* list);

size_t find_incorrect_empty_node (List_t* list);

#ifdef SUPERPROTECT

list_status is_list_corrupted (List_t* list);

#endif

void list_dump (List_t* list);

void error_descriptor (List_t* list, FILE* log_file);

void log_print_info_about_list (List_t* list, FILE* log_file);

void log_print_params_of_list (List_t* list, FILE* log_file);

void log_print_zero_node (List_t* list, FILE* log_file);

void log_print_list_nodes (List_t* list, FILE* log_file);


//-----------------------------------------------------------------------------


void create_txt_file_for_graphviz_logic (List_t* list);

void create_txt_file_for_graphviz_phys (List_t* list);

void graph_print_beginning (List_t* list, FILE* graphviz_file);

void graph_print_zero_node (List_t* list, FILE* graphviz_file);

void graph_print_nodes_logic (List_t* list, FILE* graphviz_file);

void graph_print_nodes_phys (List_t* list, FILE* graphviz_file);

void graph_print_links_phys (List_t* list, FILE* graphviz_file);

void graph_print_links_logic (List_t* list, FILE* graphviz_file);

void graph_print_ptrs_phys (List_t* list, FILE* graphviz_file);

void graph_print_head_logic (List_t* list, FILE* graphviz_file);

void graph_print_tail_logic (List_t* list, FILE* graphviz_file);


//-----------------------------------------------------------------------------


void test_list (List_t* list);

void empty_test (List_t* list);

void insert_test (List_t* list);

void erase_test (List_t* list);

void change_mod_test (List_t* list);

#undef SUPERPROTECT
