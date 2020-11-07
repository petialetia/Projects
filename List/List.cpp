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

typedef double Elem_t;

const size_t MIN_CAPACITY = 100;

const size_t MAX_CAPACITY = 10000000;

enum list_status
{
    NoError,

    #define DEF_CMD(name, condition, error_description)\
    name,

    #include "Commands.hpp"

    #undef DEF_CMD

    IncorrectListNode,

    IncorrectEmptyNode
};

struct list_node
{
    Elem_t val  = NAN;
    size_t next = 0;
    size_t prev = 0;
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

bool can_ptr_be_used (const void* ptr);

void spill_poison (List_t* list, size_t start_position);

void Destroy (List_t* list);

void InsertBack (List_t* list, Elem_t val);

void InsertFront (List_t* list, Elem_t val);

void insertion_front (List_t* list, Elem_t val);

void Erase (List_t* list, size_t index);

void erasing (List_t* list, size_t index);

void InsertBefore (List_t* list, size_t index, Elem_t val);

void insertion_before (List_t* list, size_t index, Elem_t val);

void InsertAfter (List_t* list, size_t index, Elem_t val);

void insertion_after (List_t* list, size_t index, Elem_t val);

bool is_index_correct (List_t* list, size_t index);

size_t FindIndex (List_t* list, size_t num);

Elem_t GetVal (List_t* list, size_t num);

void ChangeMod (List_t* list);

void sort_list (List_t* list);

void move_head (List_t* list);

void move_mid_node (List_t* list, size_t current_node_index, size_t current_node_num);

void move_tail (List_t* list);

void list_swaper (List_t* list, size_t current_node_index, size_t current_node_num);

void swaper (void* left, void* right, size_t size);

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


//-----------------------------------------------------------------------------


main ()
{
    List_t list;

    Construct (&list);

    InsertFront (&list, 120);

    InsertBefore (&list, 1, 210);

    InsertBefore (&list, 1, 220);

    InsertBefore (&list, 1, 230);

    InsertBefore (&list, 1, 240);

    InsertBefore (&list, 1, 520);

    InsertBefore (&list, 1, 320);

    InsertBefore (&list, 1, 260);

    InsertBefore (&list, 1, 203);

    InsertBefore (&list, 1, 205);

    Destroy (&list);

    Construct (&list);

    Destroy (&list);
}


//-----------------------------------------------------------------------------


void Construct (List_t* list, int start_capacity)
{
    if (list == nullptr)
    {
        printf ("Your pointer equals nullptr\n");
    }
    else
    {
        if (!can_ptr_be_used(list))
        {
            printf ("Your pointer can not be read\n");
        }
        else
        {
            if (start_capacity > MAX_CAPACITY)
            {
                printf ("You're trying to construct list with too big start capacity\n");
            }
            else
            {
                if (start_capacity < MIN_CAPACITY)
                {
                    printf ("You're trying to construct list with too small capacity\n");
                }
                else
                {
                    if ((list->size                    != 0)       || (list->capacity     != 0)       ||
                        (list->data                    != nullptr) || (list->name_of_list != nullptr) ||
                        (list->status                  != NoError) || (list->mod          != 0)       ||
                        (list->index_of_incorrect_node != 0)       || (list->head         != 0)       ||
                        (list->tail                    != 0)       || (list->free         != 0))
                    {
                        printf ("You're trying to construct list, but there are some data you didn't delete\n");
                    }
                    else
                    {
                        list->capacity = start_capacity;

                        list->name_of_list = get_name(list);

                        list->free = 1;

                        list->data = (list_node*) calloc (start_capacity + 1, sizeof (list_node));

                        list->data[0].val = NAN;

                        spill_poison (list, list->size+1);
                        check_out (list);

                        #undef get_name
                    }
                }
            }
        }
    }
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
    list->data[start_position].next = start_position + 1;

    list->data[start_position].val = NAN;

    start_position++;

    for (; start_position < list->capacity; start_position++)
    {
        list->data[start_position].next = start_position + 1;

        list->data[start_position].prev = start_position - 1;

        list->data[start_position].val = NAN;
    }

    list->data[start_position].prev = start_position - 1;

    list->data[start_position].val = NAN;
}

void Destroy (List_t* list)
{
    check_out (list);

    free (list->data);

    memset (list, 0, sizeof (List_t));
}

void InsertBack (List_t* list, Elem_t val)
{
    check_out (list)

    list->data[list->free].val = val;

    list->data[list->free].prev = list->tail;

    if (list->tail != 0)
    {
        list->data[list->tail].next = list->free;
    }

    list->tail = list->free;

    list->free = list->data[list->free].next;

    list->data[list->free].prev = 0;

    list->data[list->tail].next = 0;

    if (list->head == 0)
    {
        list->head = list->tail;
    }

    list->size++;

    check_out (list);
}

void InsertFront (List_t* list, Elem_t val)
{
    check_out (list)

    if ((list->head != 0) && (list->mod == 0))
    {
        list->mod = 1;
    }

    insertion_front (list, val);

    check_out (list);
}

void insertion_front (List_t* list, Elem_t val)
{
    list->data[list->free].val = val;

    if (list->head != 0)
    {
        list->data[list->head].prev = list->free;
    }

    size_t head = list->head;

    list->head = list->free;

    list->free = list->data[list->free].next;

    list->data[list->free].prev = 0;

    list->data[list->head].next = head;

    if (list->tail == 0)
    {
        list->tail = list->head;
    }

    list->size++;
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

    if (list->free != 0)
    {
        list->data[list->free].prev = index;
    }

    list->free = index;
    list->data[index].prev = 0;
    list->data[index].val = NAN;

    list->size--;
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

        insertion_before (list, index, val);
    }
    else
    {
        printf ("Invalid index is given to InsertBefore\n");
    }

    check_out (list);
}

void insertion_before (List_t* list, size_t index, Elem_t val)
{
    size_t new_node_index = list->free;

    list->free = list->data[list->free].next;

    list->data[list->free].prev = 0;

    list->data[new_node_index].val = val;

    list->data[new_node_index].prev = list->data[index].prev;

    list->data[new_node_index].next = index;

    list->data[index].prev = new_node_index;

    if (index != list->head)
    {
        list->data[list->data[new_node_index].prev].next = new_node_index;
    }
    else
    {
        list->head = new_node_index;
    }

    list->size++;
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

        insertion_after (list, index, val);
    }
    else
    {
        printf ("Invalid index is given to InsertAfter\n");
    }

    check_out (list);
}

void insertion_after (List_t* list, size_t index, Elem_t val)
{
    size_t new_node_index = list->free;

    list->free = list->data[list->free].next;

    list->data[list->free].prev = 0;

    list->data[new_node_index].val = val;

    list->data[new_node_index].prev = index;

    list->data[new_node_index].next = list->data[index].next;

    list->data[index].next = new_node_index;

    if (index != list->tail)
    {
        list->data[list->data[new_node_index].next].prev = new_node_index;
    }
    else
    {
        list->tail = new_node_index;
    }

    list->size++;
}

bool is_index_correct (List_t* list, size_t index)
{
    if ((index <= 0) || (index > list->capacity))
    {
        return false;
    }
    else
    {
        if (isnan(list->data[index].val))
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

    if (list->mod != 0)
    {
        size_t index = list->head;

        for (size_t i = 1; i < num; i++)
        {
            index = list->data[index].next;
        }
        check_out (list);
        return index;
    }
    check_out (list);
    return num;
}

Elem_t GetVal (List_t* list, size_t num)
{
    check_out (list);

    if (num == 0)
    {
        printf ("Number of system zero node is given to GetVal\n");
        check_out (list);
        return NAN;
    }

    if (num > list->size)
    {
        printf ("Number of empty or nonexistent node is given to GetVal\n");
        check_out (list);
        return NAN;
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
    if (list->head != 1)
    {
        move_head (list);
    }

    size_t current_node_index = list->data[list->head].next;

    for (size_t i = 2; i < list->size; i++)
    {
        if (current_node_index != i)
        {
            move_mid_node (list, current_node_index, i);
        }

        current_node_index = list->data[i].next;
    }

    if (list->tail != list->size)
    {
        move_tail (list);
    }
}

void move_head (List_t* list)
{
    list->data[list->data[list->head].next].prev = 1;

    list_swaper (list, list->head, 1);

    if (list->tail == 1)
    {
        list->tail = list->head;
    }
    else
    {
        if (list->free == 1)
        {
            list->free = list->head;
        }
    }

    list->head = 1;
}

void move_mid_node (List_t* list, size_t current_node_index, size_t current_node_num)
{
    list->data[list->data[current_node_index].prev].next = current_node_num;

    if (list->data[current_node_index].next != current_node_num)
    {
        list->data[list->data[current_node_index].next].prev = current_node_num;
    }

    list_swaper (list, current_node_index, current_node_num);

    if (list->tail == current_node_num)
    {
        list->tail = current_node_index;
    }
    else
    {
        if (list->free == current_node_num)
        {
            list->free = current_node_index;
        }
    }
}

void move_tail (List_t* list)
{
    list->data[list->data[list->tail].prev].next = list->size;

    list_swaper (list, list->tail, list->size);

    if (list->free == list->size)
    {
        list->free = list->tail;
    }

    list->tail = list->size;
}

void list_swaper (List_t* list, size_t current_node_index, size_t current_node_num)
{
    if (list->data[current_node_num].prev != 0)
    {
        list->data[list->data[current_node_num].prev].next = current_node_index;

        if (list->data[current_node_num].prev == current_node_index)
        {
            list->data[current_node_num].prev = current_node_num;
        }
    }

    if (list->data[current_node_num].next != 0)
    {
        list->data[list->data[current_node_num].next].prev = current_node_index;
    }

    swaper (&list->data[current_node_index].val,  &list->data[current_node_num].val,  sizeof(Elem_t));
    swaper (&list->data[current_node_index].next, &list->data[current_node_num].next, sizeof(size_t));
    swaper (&list->data[current_node_index].prev, &list->data[current_node_num].prev, sizeof(size_t));
}

void swaper (void* left, void* right, size_t size)
{
    void* temp = calloc (1, size);

    memcpy (temp,  left,  size);
    memcpy (left,  right, size);
    memcpy (right, temp,  size);

    free(temp);
}

bool is_list_looped (List_t* list)
{
    if (list->size == 0)
    {
        return 0;
    }

    size_t ptr1 = list->head;
    size_t ptr2 = list->head;

    #define checking_for_loop_in_list(direction)                   \
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

    checking_for_loop_in_list (next);

    ptr1 = list->tail;

    ptr2 = list->tail;

    checking_for_loop_in_list (prev);

    #undef cheking_for_loop_in_list

    return 0;
}

bool are_empty_nodes_looped (List_t* list)
{
    if (list->size == list->capacity)
    {
        return 0;
    }

    #define checking_for_loop_in_free_nodes(direction)\
                                                      \
    for (;;)                                          \
    {                                                 \
        if (list->data[ptr2].direction == 0)          \
        {                                             \
            break;                                    \
        }                                             \
        else                                          \
        {                                             \
            ptr1= list->data[ptr1].direction;         \
            ptr2= list->data[ptr2].direction;         \
                                                      \
            if (list->data[ptr2].direction == 0)      \
            {                                         \
                break;                                \
            }                                         \
                                                      \
            ptr2= list->data[ptr2].direction;         \
                                                      \
                                                      \
            if (ptr1 == ptr2)                         \
            {                                         \
                return 1;                             \
            }                                         \
        }                                             \
    }

    size_t ptr1 = list->free;
    size_t ptr2 = list->free;

    checking_for_loop_in_free_nodes (next);

    ptr1 = ptr2;

    checking_for_loop_in_free_nodes (prev);

    #undef checking_for_loop_in_free_nodes

    return 0;

}

size_t find_incorrect_list_node (List_t* list)
{
    if (list->mod == 0)
    {
        for (size_t i = 1; i < list->size; i++)
        {
            if (isnan(list->data[i].val) || (list->data[i].next != (i + 1)) || (list->data[i].prev != i -1))
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
        if (isnan(list->data[current_node_index].val)              ||
            (list->data[current_node_index].next > list->capacity) ||
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

    size_t previous_next = 0;

    for (size_t i = 1; i < list->capacity - list->size; i++)
    {
        if  (!isnan(list->data[current_node_index].val)            ||
            (list->data[current_node_index].next > list->capacity) ||
            (list->data[current_node_index].next == 0)             ||
            (list->data[current_node_index].prev != previous_next))
        {
            return current_node_index;
        }

        previous_next = current_node_index;
        current_node_index = list->data[current_node_index].next;

    }

    if ((list->data[current_node_index].prev != previous_next) || //current_node_index == index of last free node
        (list->data[current_node_index].next != 0)             ||
        (!isnan(list->data[current_node_index].val)))
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

    #include "Commands.hpp"

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
}

void error_descriptor (List_t* list, FILE* log_file)
{
    switch (list->status)
    {
        #define DEF_CMD(name, condition, error_description)                    \
        case name: fprintf (log_file, "Error %u: %s", name, error_description);\
                   break;

        #include "Commands.hpp"

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

    if (isnan(list->data[0].val))
    {
        fprintf (log_file, "NAN (POISON!)\n");
    }
    else
    {
        fprintf (log_file, "%lg\n", list->data[0].val);
    }

    fprintf (log_file, "\tnext = %u\n", list->data[0].next);

    fprintf (log_file, "\tprev = %u\n\n", list->data[0].prev);
}

void print_list_nodes (List_t* list, FILE* log_file)
{
    for (size_t i = 1; i < list->capacity + 1; i++)
    {
        if (isnan(list->data[i].val))
        {
            fprintf (log_file, "\t [%u]\n", i);

            fprintf (log_file, "\tval = ");

            fprintf (log_file, "NAN (POISON!)\n");
        }
        else
        {
            fprintf (log_file, "\t*[%u]\n", i);

            fprintf (log_file, "\tval = ");

            fprintf (log_file, "%lg\n", list->data[i].val);
        }

        fprintf (log_file, "\tnext = %u\n", list->data[i].next);

        fprintf (log_file, "\tprev = %u\n", list->data[i].prev);

        fprintf (log_file, "\n");
    }

    fprintf (log_file, "\t}\n");
}

#undef check_out
