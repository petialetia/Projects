#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <windows.h>

#define get_name(var) #var

#define check_out(list)                     \
    list->status = is_list_corrupted (list);\
    if (list->status)                       \
    {                                       \
        list_dump (list);                   \
        assert (0);                         \
    }

#define POISON NAN

typedef double Elem_t;

/*#define POISON 0

typedef int Elem_t;*/

const size_t MIN_CAPACITY = 10;

const size_t MAX_CAPACITY = 10000000;

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
    char*       name_of_list            = nullptr;

    list_status status                  = NoError;
    size_t      index_of_incorrect_node = 0;
    bool        mod                     = 0;

    size_t      head                    = 0;
    size_t      tail                    = 0;
    size_t      free                    = 0;
};

void Construct (List_t* list, int start_capacity = MIN_CAPACITY);

bool can_list_be_constructed (List_t* list, int start_capacity);

bool can_ptr_be_used (const void* ptr);

void spill_poison (List_t* list, size_t start_position);

void Destroy (List_t* list);

void insertion (List_t* list, size_t left, size_t right, Elem_t val);

void InsertBack (List_t* list, Elem_t val);

void InsertFront (List_t* list, Elem_t val);

void Erase (List_t* list, size_t index);

void erasing (List_t* list, size_t index);

void EraseFront (List_t* list);

void EraseBack (List_t* list);

void InsertBefore (List_t* list, size_t index, Elem_t val);

void InsertAfter (List_t* list, size_t index, Elem_t val);

bool is_index_correct (List_t* list, size_t index);

size_t FindIndex (List_t* list, size_t num);

size_t finding_index (List_t* list, size_t num);

Elem_t GetVal (List_t* list, size_t num);

void ChangeMod (List_t* list);

void sort_list (List_t* list);

void move_list_nodes (List_t* list, Elem_t* vals_buffer);

void move_empty_nodes (List_t* list);

bool is_list_looped (List_t* list);

bool are_empty_nodes_looped (List_t* list);

size_t find_incorrect_list_node (List_t* list);

size_t find_incorrect_node_in_unsorted_list (List_t* list);

size_t find_incorrect_empty_node (List_t* list);

list_status is_list_corrupted (List_t* list);

void list_dump (List_t* list);

void error_descriptor (List_t* list, FILE* log_file);

void print_info_about_list (List_t* list, FILE* log_file);

void print_params_of_list (List_t* list, FILE* log_file);

void print_zero_node (List_t* list, FILE* log_file);

void print_list_nodes (List_t* list, FILE* log_file);

void create_txt_file_for_graphviz (List_t* list);

void print_nodes (List_t* list, FILE* graphviz_file);

void print_links (List_t* list, FILE* graphviz_file);

void print_ptrs (List_t* list, FILE* graphviz_file);


//-----------------------------------------------------------------------------


main ()
{
    List_t list;

    Construct (&list);

    InsertBack (&list, 29);

    InsertBefore (&list, 1, 210);

    InsertAfter (&list, 1, 20);

    InsertAfter (&list, 1, 120);

    InsertBefore (&list, 1, 310);

    InsertFront (&list, 2222);

    Erase (&list, 1);

    Erase (&list, 2);

    InsertBefore (&list, 3, 230);

    InsertFront (&list, 200);

    list_dump (&list);

    ChangeMod (&list);

    Destroy (&list);

    Construct (&list);

    Destroy (&list);
}


//-----------------------------------------------------------------------------


void Construct (List_t* list, int start_capacity)
{
    if (can_list_be_constructed (list, start_capacity))
    {
        list->capacity = start_capacity;

        list->name_of_list = get_name(list);

        list->free = 1;

        list->data = (list_node*) calloc (start_capacity + 1, sizeof (list_node));

        list->data[0].val = POISON;

        spill_poison (list, list->size+1);
        check_out (list);
    }

    #undef get_name

    return;
}

bool can_list_be_constructed (List_t* list, int start_capacity)
{
    if (list == nullptr)
    {
        printf ("Your pointer equals nullptr\n");
        return 0;
    }
    if (!can_ptr_be_used(list))
    {
        printf ("Your pointer can not be read\n");
        return 0;
    }
    if (start_capacity > MAX_CAPACITY)
    {
        printf ("You're trying to construct list with too big start capacity\n");
        return 0;
    }
    if (start_capacity < MIN_CAPACITY)
    {
        printf ("You're trying to construct list with too small capacity\n");
        return 0;
    }
    if ((list->size                    != 0)       || (list->capacity     != 0)       ||
        (list->data                    != nullptr) || (list->name_of_list != nullptr) ||
        (list->status                  != NoError) || (list->mod          != 0)       ||
        (list->index_of_incorrect_node != 0)       || (list->head         != 0)       ||
        (list->tail                    != 0)       || (list->free         != 0))
    {
        printf ("You're trying to construct list, but there are some data you didn't delete\n");
        return 0;
    }
    return 1;
}

bool can_ptr_be_used (const void* ptr)
{
    MEMORY_BASIC_INFORMATION mbi;

    if (!VirtualQuery (ptr, &mbi, sizeof (mbi)))
    {
        return false;
    }

    if (mbi.Protect & (PAGE_GUARD | PAGE_NOACCESS))
    {
        return false;
    }

    DWORD read_rights = PAGE_READONLY | PAGE_READWRITE | PAGE_WRITECOPY | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY;

    return (mbi.Protect & read_rights);
}

void spill_poison (List_t* list, size_t start_position)
{
    for (; start_position < list->capacity; start_position++)
    {
        list->data[start_position].next = start_position + 1;

        list->data[start_position].prev = -1;

        list->data[start_position].val = POISON;
    }

    list->data[start_position].prev = -1;

    list->data[start_position].val = POISON;
}

void Destroy (List_t* list)
{
    check_out (list);

    free (list->data);

    memset (list, 0, sizeof (List_t));
}

void insertion (List_t* list, size_t left, size_t right, Elem_t val)
{
    size_t new_node_index = list->free;

    list->free = list->data[list->free].next;

    list->data[list->free].prev = -1;

    list->data[new_node_index].val = val;

    list->data[new_node_index].prev = left;

    list->data[new_node_index].next = right;

    if (right > 0)
    {
        list->data[right].prev = new_node_index;
    }
    else
    {
        list->tail = new_node_index;
    }

    if (left > 0)
    {
        list->data[left].next = new_node_index;
    }
    else
    {
        list->head = new_node_index;
    }

    list->size++;
}

void InsertBack (List_t* list, Elem_t val)
{
    check_out (list)

    insertion (list, list->tail, 0, val);

    check_out (list);
}

void InsertFront (List_t* list, Elem_t val)
{
    check_out (list)

    if ((list->head != 0) && (list->mod == 0))
    {
        list->mod = 1;
    }

    insertion (list, 0, list->head, val);

    check_out (list);
}

void Erase (List_t* list, size_t index)
{
    check_out (list);

    if (is_index_correct (list, index))
    {
        if ((list->tail != index) && (list->mod == 0))
        {
            list->mod = 1;
        }

        erasing (list, index);
    }
    else
    {
        printf ("Invalid index is given to Erase\n");
    }

    check_out (list);
}

void erasing (List_t* list, size_t index)
{
    if (list->data[index].next != 0)
    {
        list->data[list->data[index].next].prev = list->data[index].prev;
    }
    else
    {
        list->tail = list->data[index].prev;
    }
    if (list->data[index].prev != 0)
    {
        list->data[list->data[index].prev].next = list->data[index].next;
    }
    else
    {
        list->head = list->data[index].next;
    }

    list->data[index].next = list->free;

    list->free = index;
    list->data[index].prev = -1;
    list->data[index].val = POISON;

    list->size--;
}

void EraseFront (List_t* list)
{
    check_out (list);

    if (list->size > 0)
    {
        if ((list->mod == 0) && (list->size > 1))
        {
            list->mod = 1;
        }

        erasing (list, list->head);
    }
    else
    {
        printf ("Empty list is given to EraseFront\n");
    }

    check_out (list);
}

void EraseBack (List_t* list)
{
    check_out (list);

    if (list->size > 0)
    {
        erasing (list, list->tail);
    }
    else
    {
        printf ("Empty list is given to EraseBack\n");
    }

    check_out (list);
}

void InsertBefore (List_t* list, size_t index, Elem_t val)
{
    check_out (list);

    if (is_index_correct (list, index))
    {
        if (list->mod == 0)
        {
            list->mod = 1;
        }

        insertion (list, list->data[index].prev, index, val);
    }
    else
    {
        printf ("Invalid index is given to InsertBefore\n");
    }

    check_out (list);
}

void InsertAfter (List_t* list, size_t index, Elem_t val)
{
    check_out (list);

    if (is_index_correct (list, index))
    {
        if ((index != list->tail) && (list->mod == 0))
        {
            list->mod = 1;
        }

        insertion (list, index, list->data[index].next, val);
    }
    else
    {
        printf ("Invalid index is given to InsertAfter\n");
    }

    check_out (list);
}

bool is_index_correct (List_t* list, size_t index)
{
    if ((index <= 0) || (index > list->capacity))
    {
        return false;
    }
    else
    {
        if (list->data[index].prev == -1)
        {
            return false;
        }
    }

    return true;
}

size_t FindIndex (List_t* list, size_t num)
{
    check_out (list);

    if (num == 0)
    {
        printf ("Number of system zero node is given to FindIndex\n");
        check_out (list);
        return 0;
    }

    if (num > list->size)
    {
        printf ("Number of empty or nonexistent node is given to FindIndex\n");
        check_out (list);
        return 0;
    }

    check_out (list);

    return finding_index (list, num);

}

size_t finding_index (List_t* list, size_t num)
{
    if (list->mod != 0)
    {
        size_t index = list->head;

        for (size_t i = 1; i < num; i++)
        {
            index = list->data[index].next;
        }

        return index;
    }

    return num;
}

Elem_t GetVal (List_t* list, size_t num)
{
    check_out (list);

    if (num == 0)
    {
        printf ("Number of system zero node is given to GetVal\n");
        check_out (list);
        return POISON;
    }

    if (num > list->size)
    {
        printf ("Number of empty or nonexistent node is given to GetVal\n");
        check_out (list);
        return POISON;
    }

    if (list->mod != 0)
    {
        size_t index = list->head;

        for (size_t i = 1; i < num; i++)
        {
            index = list->data[index].next;
        }
        check_out (list);
        return list->data[index].val;
    }
    check_out (list);
    return list->data[num].val;
}

void ChangeMod (List_t* list)
{
    check_out (list);

    if (list->mod == 0)
    {
        check_out (list);
        return;
    }
    else
    {
        if (list->size == 0)
        {
            list->mod = 0;
            printf ("Empty list is given to ChangeMod\n");
            check_out (list);
            return;
        }
        else
        {
            sort_list (list);
            list->mod = 0;
        }
    }

    check_out (list);
}

void sort_list (List_t* list)
{
    Elem_t* vals_buffer = (Elem_t*) calloc (list->size, sizeof (Elem_t));

    for (size_t i = 0, current_node_index = list->head; i < list->size; i++)
    {
        vals_buffer[i] = list->data[current_node_index].val;

        current_node_index = list->data[current_node_index].next;
    }

    move_list_nodes (list, vals_buffer);

    free (vals_buffer);

    move_empty_nodes (list);

    list->head = 1;

    list->tail = list->size;

    list->free = list->size + 1;
}

void move_list_nodes (List_t* list, Elem_t* vals_buffer)
{
    for (size_t i = 0, current_node_index = 1; current_node_index < list->size; i++, current_node_index++)
    {
        list->data[current_node_index].val = vals_buffer[i];

        list->data[current_node_index].next = i + 2;

        list->data[current_node_index].prev = i;
    }

    list->data[list->size].val = vals_buffer[list->size - 1];

    list->data[list->size].next = 0;

    list->data[list->size].prev = list->size - 1;
}

void move_empty_nodes (List_t* list)
{
    for (size_t i = list->size + 1; i < list->capacity; i++)
    {
        list->data[i].val = POISON;

        list->data[i].next = i + 1;

        list->data[i].prev = -1;
    }

    list->data[list->capacity].next = 0;

    list->data[list->capacity].val = POISON;

    list->data[list->capacity].prev = -1;
}

bool is_list_looped (List_t* list)
{
    if (list->size == 0)
    {
        return 0;
    }

    size_t ptr1 = list->head;
    size_t ptr2 = list->head;

    #define checking_for_loop(direction)                           \
                                                                   \
    for (;;)                                                       \
    {                                                              \
        if (list->data[ptr2].direction == 0)                       \
        {                                                          \
            break;                                                 \
        }                                                          \
        else                                                       \
        {                                                          \
            ptr1= list->data[ptr1].direction;                      \
            ptr2= list->data[list->data[ptr2].direction].direction;\
                                                                   \
            if (ptr2 == 0)                                         \
            {                                                      \
                break;                                             \
            }                                                      \
                                                                   \
            if (ptr1 == ptr2)                                      \
            {                                                      \
                return 1;                                          \
            }                                                      \
        }                                                          \
    }

    checking_for_loop (next);

    ptr1 = list->tail;

    ptr2 = list->tail;

    checking_for_loop (prev);

    #undef cheking_for_loop_in_list

    return 0;
}

bool are_empty_nodes_looped (List_t* list)
{
    if (list->size == list->capacity)
    {
        return 0;
    }

    size_t ptr1 = list->free;

    size_t ptr2 = list->free;

    checking_for_loop (next);

    #undef checking_for_loop

    return 0;

}

size_t find_incorrect_list_node (List_t* list)
{
    if (list->mod == 0)
    {
        for (size_t i = 1; i < list->size; i++)
        {
            if ((list->data[i].next != (i + 1)) || (list->data[i].prev != i - 1))
            {
                return i;
            }
        }

        if (list->data[list->size].prev != list->size -1)  //list->size == list->tail
        {
            return list->size;
        }
    }
    else
    {
        return find_incorrect_node_in_unsorted_list (list);
    }

    return 0;
}

size_t find_incorrect_node_in_unsorted_list (List_t* list)
{
    size_t current_node_index = list->head;

    size_t previous_next = 0;

    for (size_t i = 1; i < list->size; i++)
    {
        if ((list->data[current_node_index].next > list->capacity) ||
            (list->data[current_node_index].next == 0)             ||
            (list->data[current_node_index].prev != previous_next))
        {
            return current_node_index;
        }

        previous_next = current_node_index;
        current_node_index = list->data[current_node_index].next;

    }

    if (list->data[current_node_index].prev != previous_next) //current_node_index == list->tail
    {
        return current_node_index;
    }

    return 0;
}

size_t find_incorrect_empty_node (List_t* list)
{
    size_t current_node_index = list->free;

    for (size_t i = 1; i < list->capacity - list->size; i++)
    {
        if ((list->data[current_node_index].next > list->capacity) ||
            (list->data[current_node_index].next ==  0)            ||
            (list->data[current_node_index].prev != -1))
        {
            return current_node_index;
        }

        current_node_index = list->data[current_node_index].next;

    }

    if ((list->data[current_node_index].prev != -1) || //current_node_index == index of last free node
        (list->data[current_node_index].next !=  0))
    {
        return current_node_index;
    }

    return 0;
}

list_status is_list_corrupted (List_t* list)
{
    #define DEF_CMD(name, condition, error_description)\
    if (condition)                                     \
    {                                                  \
        return name;                                   \
    }

    #include "Errors.hpp"

    #undef DEF_CMD

    list->index_of_incorrect_node = find_incorrect_list_node (list);

    if (list->index_of_incorrect_node > 0)
    {
        return IncorrectListNode;
    }

    list->index_of_incorrect_node = find_incorrect_empty_node (list);

    if (list->index_of_incorrect_node > 0)
    {
        return IncorrectEmptyNode;
    }

    return NoError;
}

void list_dump (List_t* list)
{
    static size_t num_of_call = 0;
    num_of_call++;

    static FILE* log_file = nullptr;

    if (num_of_call == 1)
    {
        log_file = fopen ("LogFile.txt", "w");
    }

    fprintf (log_file, "List_t (");

    error_descriptor (list, log_file);

    fprintf (log_file, ")");

    fprintf (log_file, " [%p]", list);

    if (list->status == ListPtrEqualsNullptr)
    {
        fprintf (log_file, "\nPointer on list equals nullptr, variables are lost\n");
    }
    else
    {
        print_info_about_list (list, log_file);
    }

    create_txt_file_for_graphviz (list);
}

void error_descriptor (List_t* list, FILE* log_file)
{
    switch (list->status)
    {
        #define DEF_CMD(name, condition, error_description)                    \
        case name: fprintf (log_file, "Error %u: %s", name, error_description);\
                   break;

        #include "Errors.hpp"

        #undef DEF_CMD

        case IncorrectListNode:  fprintf (log_file, "Error %u: List node with index %u is incorrect", IncorrectListNode, list->index_of_incorrect_node);
                                 break;

        case IncorrectEmptyNode: fprintf (log_file, "Error %u: Empty node with index %u is incorrect", IncorrectEmptyNode, list->index_of_incorrect_node);
                                 break;

        case NoError:            fprintf (log_file, "OK");
                                 break;

        default: fprintf (log_file, "Something went wrong");
    }
}

void print_info_about_list (List_t* list, FILE* log_file)
{
    if ((list->status == NoAccessPtr))
    {
        fprintf (log_file, "\nPointer on list might be invalid, variables are lost\n");
    }
    else
    {
        print_params_of_list (list, log_file);

        if ((list->status == DataEqualsNullptr) || (list->data == nullptr))
        {
            fprintf (log_file, "\tData equals nullptr, elements are lost\n\t}\n");
        }
        else
        {
            print_zero_node (list, log_file);

            print_list_nodes (list, log_file);
        }

        fprintf (log_file, "}\n");
    }
}

void print_params_of_list (List_t* list, FILE* log_file)
{
    if (list->name_of_list != nullptr)
    {
        fprintf (log_file, " \"%s\"\n{\n", list->name_of_list);
    }
    else
    {
        fprintf (log_file, " <Nameless>\n{\n", list->name_of_list);
    }

    fprintf (log_file, "mod = %u\n", list->mod);

    fprintf (log_file, "size = %u\n", list->size);

    fprintf (log_file, "capacity = %u\n", list->capacity);

    fprintf (log_file, "head = %u\n", list->head);

    fprintf (log_file, "tail = %u\n", list->tail);

    fprintf (log_file, "free = %u\n", list->free);

    fprintf (log_file, "data[%p]\n", list->data);

    fprintf (log_file, "\t{\n");
}

void print_zero_node (List_t* list, FILE* log_file)
{
    fprintf (log_file, "\t!!![0]!!!\n");

    fprintf (log_file, "\tval = ");

    fprintf (log_file, "POISON\n");

    fprintf (log_file, "\tnext = %u\n", list->data[0].next);

    fprintf (log_file, "\tprev = %u\n", list->data[0].prev);
}

void print_list_nodes (List_t* list, FILE* log_file)
{
    for (size_t i = 1; i < list->capacity + 1; i++)
    {
        fprintf (log_file, "\n");

        if (list->data[i].prev == -1)
        {
            fprintf (log_file, "\t [%u]\n", i);

            fprintf (log_file, "\tval = ");

            fprintf (log_file, "POISON\n");
        }
        else
        {
            fprintf (log_file, "\t*[%u]\n", i);

            fprintf (log_file, "\tval = ");

            fprintf (log_file, "%lg\n", list->data[i].val);
        }

        fprintf (log_file, "\tnext = %d\n", list->data[i].next);

        fprintf (log_file, "\tprev = %d\n", list->data[i].prev);
    }

    fprintf (log_file, "\t}\n");
}

void create_txt_file_for_graphviz (List_t* list)
{
    static size_t num_of_call = 0;
    num_of_call++;

    static FILE* graphviz_file = nullptr;

    if (num_of_call == 1)
    {
        graphviz_file = fopen ("graphviz_file.txt", "w");
    }

    fprintf (graphviz_file, "digraph PL\n");

    fprintf (graphviz_file, "{\nrankdir=HR;\n");

    fprintf (graphviz_file, "0 [style=\"filled\", fillcolor = \"red\", shape=record,label=\" <next> 0 | { POISON | 0} | <prev> 0\" ];\n");

    print_nodes (list, graphviz_file);

    print_links (list, graphviz_file);

    print_ptrs (list, graphviz_file);

    fprintf (graphviz_file, "}");
}

void print_nodes (List_t* list, FILE* graphviz_file)
{
    for (size_t i = 1; i <= list->capacity; i++)
    {
        if (list->data[i].prev == -1)
        {
            fprintf (graphviz_file, "%u [style=\"filled\", fillcolor = \"yellow\", shape=record,label=\" <next> %d | { POISON | %u} | <prev> %d\" ];\n", i, list->data[i].next, /*list->data[i].val, */i, list->data[i].prev);
        }
        else
        {
            fprintf (graphviz_file, "%u [style=\"filled\", fillcolor = \"green\", shape=record,label=\" <next> %d | { %lg | %u} | <prev> %d\" ];\n", i, list->data[i].next, list->data[i].val, i, list->data[i].prev);
        }
    }
}

void print_links (List_t* list, FILE* graphviz_file)
{
    fprintf (graphviz_file, "{\nedge[color=white]\n");

    for (size_t i = 0; i < list->capacity; i++)
    {
        fprintf (graphviz_file, "%u->", i);
    }

    fprintf (graphviz_file, "%u;\n", list->capacity);

    fprintf (graphviz_file, "}\n");

    for (size_t i = 1; i <= list->capacity; i++)
    {
        if (list->data[i].next > 0)
        {
            fprintf (graphviz_file, "%u:<next> -> %u:<next>[color=\"blue\",constraint=false];\n", i, list->data[i].next);
        }

        if (list->data[i].prev > 0)
        {
            fprintf (graphviz_file, "%u:<prev> -> %u:<prev>[color=\"red\",constraint=false];\n", i, list->data[i].prev);
        }
    }
}

void print_ptrs (List_t* list, FILE* graphviz_file)
{
    char s[5];

    #define print_ptr(param)                                                         \
                                                                                     \
    if (list->param > 0)                                                             \
    {                                                                                \
        strcpy (s, #param);                                                          \
        strcpy (s, strupr(s));                                                       \
        fprintf (graphviz_file, "%s [style=\"filled\", fillcolor = \"purple\"];", s);\
        fprintf (graphviz_file, "%s -> %u\n", /*strupr(s)*/s, list->param);          \
    }

    print_ptr (head)

    print_ptr (tail)

    print_ptr (free)

    #undef print_ptr
}

#undef check_out

#undef POISON
