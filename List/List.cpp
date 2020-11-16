#include "Header.hpp"

#define SUPERPROTECT

#ifdef SUPERPROTECT

#define get_name(var) #var

#define check_up(list)                      \
    list->status = is_list_corrupted (list);\
    if (list->status)                       \
    {                                       \
        list_dump (list);                   \
        assert (0);                         \
    }

#else
#define get_name(var)
#define check_up(list)
#endif

//-----------------------------------------------------------------------------


main ()
{
    List_t list;

    test_list (&list);
}


//-----------------------------------------------------------------------------




void Construct (List_t* list, int start_capacity)
{
    if (can_list_be_constructed (list, start_capacity))
    {
        list->capacity = start_capacity;

        #ifdef SUPERPROTECT

        list->name_of_list = get_name(list);

        #endif

        list->free = 1;

        list->data = (list_node*) calloc (start_capacity + 1, sizeof (list_node));

        list->data[0].val = POISON;

        spill_poison (list, list->size+1);
        check_up (list);
    }

    return;
}

#undef get_name

bool can_list_be_constructed (List_t* list, int start_capacity)
{
    if (list == nullptr)
    {
        print_warning ("Your pointer equals nullptr");
        return 0;
    }
    if (!can_ptr_be_used(list))
    {
        print_warning ("Your pointer can not be read");
        return 0;
    }
    if (start_capacity > MAX_CAPACITY)
    {
        print_warning ("You're trying to construct list with too big start capacity");
        return 0;
    }
    if (start_capacity < MIN_CAPACITY)
    {
        print_warning ("You're trying to construct list with too small capacity");
        return 0;
    }

    #ifdef SUPERPROTECT

    if ((list->size                    != 0)       || (list->capacity != 0)       ||
        (list->data                    != nullptr) || (list->mod      != 0)       ||
        (list->name_of_list            != nullptr) || (list->status   != NoError) ||
        (list->index_of_incorrect_node != 0)       || (list->head     != 0)       ||
        (list->tail                    != 0)       || (list->free     != 0))

    #else

    if ((list->size != 0)       || (list->capacity != 0) ||
        (list->data != nullptr) || (list->mod      != 0) ||
        (list->head != 0)       || (list->tail     != 0) ||
        (list->free != 0))

    #endif


    {
        print_warning ("You're trying to construct list, but there are some data you didn't delete");
        return 0;
    }

    return 1;
}

void print_warning (char* warning)
{
    printf ("%s\n", warning);

    NUM_OF_PRINT_IN_LOG_FILE++;

    if (NUM_OF_PRINT_IN_LOG_FILE == 1)
    {
        LOG_FILE = fopen ("LogFile.txt", "w");
    }

    fprintf (LOG_FILE, "%s\n", warning);
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
    assert (list != nullptr);
    assert (start_position != 0);

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
    check_up (list);

    free (list->data);

    memset (list, 0, sizeof (List_t));
}

size_t insertion (List_t* list, size_t left, size_t right, Elem_t val)
{
    assert (list != nullptr);

    size_t new_node_index = list->free;

    list->free = list->data[list->free].next;

    list->data[list->free].prev = -1;
    list->data[new_node_index].val = val;
    list->data[new_node_index].prev = left;
    list->data[new_node_index].next = right;

    if   (right > 0) list->data[right].prev = new_node_index;
    else list->tail = new_node_index;

    if   (left > 0) list->data[left].next = new_node_index;
    else list->head = new_node_index;

    list->size++;

    return new_node_index;
}

size_t InsertBack (List_t* list, Elem_t val)
{
    check_up (list)

    return insertion (list, list->tail, 0, val);
}

size_t InsertFront (List_t* list, Elem_t val)
{
    check_up (list)

    if ((list->head != 0) && (list->mod == 0))
    {
        list->mod = 1;
    }

    check_up (list);

    return insertion (list, 0, list->head, val);
}

Elem_t Erase (List_t* list, size_t index)
{
    check_up (list);

    if (is_index_correct (list, index))
    {
        if ((list->tail != index) && (list->mod == 0))
        {
            list->mod = 1;
        }

        check_up (list);
        return erasing (list, index);
    }
    else
    {
        print_warning ("Invalid index is given to Erase");
    }

    check_up (list);
    return POISON;
}

Elem_t erasing (List_t* list, size_t index)
{
    assert (list != nullptr);
    assert (index != 0);

    if   (list->data[index].next != 0) list->data[list->data[index].next].prev = list->data[index].prev;
    else list->tail = list->data[index].prev;

    if   (list->data[index].prev != 0) list->data[list->data[index].prev].next = list->data[index].next;
    else list->head = list->data[index].next;

    list->data[index].next = list->free;

    list->free = index;
    list->data[index].prev = -1;

    Elem_t erased_val = list->data[index].val;
    list->data[index].val = POISON;

    list->size--;

    return erased_val;
}

Elem_t EraseFront (List_t* list)
{
    check_up (list);

    if (list->size > 0)
    {
        if ((list->mod == 0) && (list->size > 1))
        {
            list->mod = 1;
        }

        check_up (list);
        return erasing (list, list->head);
    }
    else
    {
        print_warning ("Empty list is given to EraseFront");
    }

    check_up (list);
    return POISON;
}

Elem_t EraseBack (List_t* list)
{
    check_up (list);

    if (list->size > 0)
    {
        check_up (list);
        return erasing (list, list->tail);
    }
    else
    {
        print_warning ("Empty list is given to EraseBack");
    }

    check_up (list);
    return POISON;
}

size_t InsertBefore (List_t* list, size_t index, Elem_t val)
{
    check_up (list);

    if (is_index_correct (list, index))
    {
        if (list->mod == 0)
        {
            list->mod = 1;
        }

        check_up (list);
        return insertion (list, list->data[index].prev, index, val);
    }
    else
    {
        print_warning ("Invalid index is given to InsertBefore");
        check_up (list);
        return 0;
    }
}

size_t InsertAfter (List_t* list, size_t index, Elem_t val)
{
    check_up (list);

    if (is_index_correct (list, index))
    {
        if ((index != list->tail) && (list->mod == 0))
        {
            list->mod = 1;
        }

        check_up (list);
        return insertion (list, index, list->data[index].next, val);
    }
    else
    {
        print_warning ("Invalid index is given to InsertAfter");
        check_up (list);
        return 0;
    }
}

bool is_index_correct (List_t* list, size_t index)
{
    assert (list != nullptr);

    if ((index == 0) || (index > list->capacity))
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
    check_up (list);

    if (num == 0)
    {
        print_warning ("Number of system zero node is given to FindIndex");
        check_up (list);
        return 0;
    }

    if (num > list->size)
    {
        print_warning ("Number of nonexistent or empty node is given to FindIndex");
        check_up (list);
        return 0;
    }

    check_up (list);

    return finding_index (list, num);

}

size_t finding_index (List_t* list, size_t num)
{
    assert (list != nullptr);

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
    check_up (list);

    if (num == 0)
    {
        print_warning ("Number of system zero node is given to GetVal");
        check_up (list);
        return POISON;
    }

    if (num > list->capacity)
    {
        print_warning ("Number of nonexistent node is given to GetVal");
        check_up (list);
        return POISON;
    }

    if (list->data[num].prev == -1)
    {
        print_warning ("Number of empty node is given to GetVal");
        check_up (list);
        return POISON;
    }

    check_up (list);
    return list->data[num].val;
}

void ChangeMod (List_t* list)
{
    check_up (list);

    if (list->mod == 0)
    {
        check_up (list);
        return;
    }
    else
    {
        if (list->size == 0)
        {
            list->mod = 0;
            print_warning ("Empty list is given to ChangeMod");
            check_up (list);
            return;
        }
        else
        {
            sort_list (list);
            list->mod = 0;
        }
    }

    check_up (list);
}

void sort_list (List_t* list)
{
    assert (list != nullptr);

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
    assert (list != nullptr);
    assert (vals_buffer != nullptr);

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
    assert (list != nullptr);

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

//-----------------------------------------------------------------------------

#ifdef SUPERPROTECT

#define checking_for_loop(direction)                               \
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

bool is_list_looped (List_t* list)
{
    assert (list != nullptr);

    if (list->size == 0)
    {
        return 0;
    }

    size_t ptr1 = list->head;
    size_t ptr2 = list->head;

    checking_for_loop (next);

    ptr1 = list->tail;
    ptr2 = list->tail;

    checking_for_loop (prev);

    return 0;
}

bool are_empty_nodes_looped (List_t* list)
{
    assert (list != nullptr);

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
    assert (list != nullptr);

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
    assert (list != nullptr);

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
    assert (list != nullptr);

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
    assert (list != nullptr);

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
    assert (list != nullptr);

    NUM_OF_PRINT_IN_LOG_FILE++;

    if (NUM_OF_PRINT_IN_LOG_FILE == 1)
    {
        LOG_FILE = fopen ("LogFile.txt", "w");\
    }

    fprintf (LOG_FILE, "\n\n\n\n\n");

    fprintf (LOG_FILE, "List_t (");
    error_descriptor (list, LOG_FILE);
    fprintf (LOG_FILE, ")");

    fprintf (LOG_FILE, " [%p]", list);

    if   (list->status == ListPtrEqualsNullptr) fprintf (LOG_FILE, "\nPointer on list equals nullptr, variables are lost\n");
    else log_print_info_about_list (list, LOG_FILE);

    fclose (LOG_FILE);

    create_txt_file_for_graphviz_phys (list);
    create_txt_file_for_graphviz_logic (list);
}

void error_descriptor (List_t* list, FILE* log_file)
{
    assert (list != nullptr);
    assert (log_file != nullptr);

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

void log_print_info_about_list (List_t* list, FILE* log_file)
{
    assert (list != nullptr);
    assert (log_file != nullptr);

    if ((list->status == NoAccessPtr))
    {
        fprintf (log_file, "\nPointer on list might be invalid, variables are lost\n");
    }
    else
    {
        log_print_params_of_list (list, log_file);

        if ((list->status == DataEqualsNullptr) || (list->data == nullptr))
        {
            fprintf (log_file, "\tData equals nullptr, elements are lost\n\t}\n");
        }
        else
        {
            log_print_zero_node (list, log_file);

            log_print_list_nodes (list, log_file);
        }

        fprintf (log_file, "}\n");
    }
}

void log_print_params_of_list (List_t* list, FILE* log_file)
{
    assert (list != nullptr);
    assert (log_file != nullptr);

    if   (list->name_of_list != nullptr) fprintf (log_file, " \"%s\"\n{\n", list->name_of_list);
    else fprintf (log_file, " <Nameless>\n{\n", list->name_of_list);

    fprintf (log_file, "mod = %u\n", list->mod);
    fprintf (log_file, "size = %u\n", list->size);
    fprintf (log_file, "capacity = %u\n", list->capacity);
    fprintf (log_file, "head = %u\n", list->head);
    fprintf (log_file, "tail = %u\n", list->tail);
    fprintf (log_file, "free = %u\n", list->free);
    fprintf (log_file, "data[%p]\n", list->data);
    fprintf (log_file, "\t{\n");
}

void log_print_zero_node (List_t* list, FILE* log_file)
{
    assert (list != nullptr);
    assert (log_file != nullptr);

    fprintf (log_file, "\t!!![0]!!!\n");
    fprintf (log_file, "\tval = ");
    fprintf (log_file, "POISON\n");
    fprintf (log_file, "\tnext = %u\n", list->data[0].next);
    fprintf (log_file, "\tprev = %u\n", list->data[0].prev);
}

void log_print_list_nodes (List_t* list, FILE* log_file)
{
    assert (list != nullptr);
    assert (log_file != nullptr);

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


//-----------------------------------------------------------------------------

#define open_file(param, name)     \
                                   \
    static size_t num_of_call = 0; \
    num_of_call++;                 \
                                   \
    if (num_of_call == 1)          \
    {                              \
        param = fopen (#name, "w");\
    }

void create_txt_file_for_graphviz_phys (List_t* list)
{
    assert (list != nullptr);

    if (list->capacity <= 100)
    {
        static FILE* graphviz_file = nullptr;

        open_file (graphviz_file, graphviz_file_phys.txt)

        graph_print_beginning (list, graphviz_file);

        graph_print_zero_node (list, graphviz_file);

        graph_print_nodes_phys (list, graphviz_file);

        graph_print_links_phys (list, graphviz_file);

        graph_print_ptrs_phys (list, graphviz_file);

        fprintf (graphviz_file, "}");

        fclose (graphviz_file);
    }
}

void create_txt_file_for_graphviz_logic (List_t* list)
{
    assert (list != nullptr);

    if (list->capacity <= 100)
    {
        static FILE* graphviz_file = nullptr;

        open_file (graphviz_file, graphviz_file_logic.txt)

        graph_print_beginning (list, graphviz_file);

        if (list->size > 0)
        {
            graph_print_nodes_logic (list, graphviz_file);

            graph_print_links_logic (list, graphviz_file);

            graph_print_head_logic (list, graphviz_file);

            graph_print_tail_logic (list, graphviz_file);
        }
        else
        {
            fprintf (graphviz_file, "\"List is empty\" [style=\"filled\", fillcolor = \"red\",shape=\"rectangle\"];\n");
        }

        fprintf (graphviz_file, "}");

        fclose (graphviz_file);
    }
}

#undef open_file

void graph_print_beginning (List_t* list, FILE* graphviz_file)
{
    assert (list != nullptr);
    assert (graphviz_file != nullptr);

    fprintf (graphviz_file, "digraph PL\n");
    fprintf (graphviz_file, "{\nrankdir=HR;\n");
}

void graph_print_zero_node (List_t* list, FILE* graphviz_file)
{
    assert (list != nullptr);
    assert (graphviz_file != nullptr);

    fprintf (graphviz_file, "0 [style=\"filled\", fillcolor = \"red\", shape=record,label=\" <next> 0 | { POISON | 0} | <prev> 0\" ];\n");
}

#define print_filled_node_description                                                                                                 \
                                                                                                                                      \
    "%u [style=\"filled\", fillcolor = \"green\", shape=record,label=\" <next> next: %d | { %lg | index: %u} | <prev> prev: %d\" ];\n"\

#define print_empty_node_description                                                                                                      \
                                                                                                                                          \
    "%u [style=\"filled\", fillcolor = \"yellow\", shape=record,label=\" <next> next: %d | { POISON | index: %u} | <prev> prev: %d\" ];\n"\

void graph_print_nodes_phys (List_t* list, FILE* graphviz_file)
{
    assert (list != nullptr);
    assert (graphviz_file != nullptr);

    for (size_t i = 1; i <= list->capacity; i++)
    {
        if   (list->data[i].prev == -1) fprintf (graphviz_file, print_empty_node_description, i, list->data[i].next, i, list->data[i].prev);
        else fprintf (graphviz_file, print_filled_node_description, i, list->data[i].next, list->data[i].val, i, list->data[i].prev);
    }
}

#undef print_empty_node_description

void graph_print_nodes_logic (List_t* list, FILE* graphviz_file)
{
    assert (list != nullptr);
    assert (graphviz_file != nullptr);

    for (size_t i = 1, num = list->head; i <= list->size; i++)
    {
        fprintf (graphviz_file, print_filled_node_description, i, list->data[num].next, list->data[num].val, num, list->data[num].prev);
        num = list->data[num].next;
    }
}

#undef print_filled_node_description

void graph_print_links_phys (List_t* list, FILE* graphviz_file)
{
    assert (list != nullptr);
    assert (graphviz_file != nullptr);

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

void graph_print_links_logic (List_t* list, FILE* graphviz_file)
{
    assert (list != nullptr);
    assert (graphviz_file != nullptr);

    if (list->size > 1)
    {
        fprintf (graphviz_file, "{\nedge[color=white]\n");

        for (size_t i = 1; i < list->size; i++)
        {
            fprintf (graphviz_file, "%u->", i);
        }

        fprintf (graphviz_file, "%u;\n", list->size);
        fprintf (graphviz_file, "}\n");
        fprintf (graphviz_file, "1:<next> -> 2:<next>[color=\"blue\",constraint=false];\n");

        for (size_t i = 2, size_t; i < list->size; i++)
        {
            fprintf (graphviz_file, "%u:<next> -> %u:<next>[color=\"blue\",constraint=false];\n", i, i + 1);
            fprintf (graphviz_file, "%u:<prev> -> %u:<prev>[color=\"red\",constraint=false];\n", i, i - 1);
        }

        fprintf (graphviz_file, "%u:<prev> -> %u:<prev>[color=\"red\",constraint=false];\n", list->size, list->size - 1);
    }
}

void graph_print_ptrs_phys (List_t* list, FILE* graphviz_file)
{
    assert (list != nullptr);
    assert (graphviz_file != nullptr);

    char name_of_ptr[SIZE_OF_NAME_OF_PTR];

    #define print_ptr(param)                                                                   \
                                                                                               \
    if (list->param > 0)                                                                       \
    {                                                                                          \
        strcpy (name_of_ptr, #param);                                                          \
        strcpy (name_of_ptr, strupr(name_of_ptr));                                             \
        fprintf (graphviz_file, "%s [style=\"filled\", fillcolor = \"purple\"];", name_of_ptr);\
        fprintf (graphviz_file, "%s -> %u\n", name_of_ptr, list->param);                       \
    }

    print_ptr (head)

    print_ptr (tail)

    print_ptr (free)

    #undef print_ptr
}

void graph_print_head_logic (List_t* list, FILE* graphviz_file)
{
    assert (list != nullptr);
    assert (graphviz_file != nullptr);

    fprintf (graphviz_file, "HEAD [style=\"filled\", fillcolor = \"purple\"];");
    fprintf (graphviz_file, "HEAD -> 1\n");
}

void graph_print_tail_logic (List_t* list, FILE* graphviz_file)
{
    assert (list != nullptr);
    assert (graphviz_file != nullptr);

    fprintf (graphviz_file, "TAIL [style=\"filled\", fillcolor = \"purple\"];");
    fprintf (graphviz_file, "TAIL -> %u\n", list->size);
}

#endif

//-----------------------------------------------------------------------------


void test_list (List_t* list)
{
    empty_test (list);
    insert_test (list);
    erase_test (list);
    change_mod_test (list);
}

void empty_test (List_t* list)
{
    if   (list->data == nullptr) printf ("empty_test ok\n");
    else printf ("empty_test failed\n");
}

#define ckeck_vals_test(num_of_vals, num_of_test)                 \
                                                                  \
    for (size_t i = 1; i <= num_of_vals; i++)                     \
    {                                                             \
        if ( GetVal (list, FindIndex (list, i)) != right_val[i-1])\
        {                                                         \
            printf ("%s failed\n", #num_of_test);                 \
            break;                                                \
        }                                                         \
    }                                                             \
    printf ("%s ok\n", #num_of_test);

void insert_test (List_t* list)
{
    Construct (list);

    for (size_t i = 0; i < 5; i++)
    {
        InsertFront (list, 20);
        InsertBack (list, 30);
    }

    size_t index_for_test = FindIndex (list, 6);

    for (size_t i = 0; i < 5; i++)
    {
        InsertAfter (list, index_for_test, 1);
        InsertBefore (list, index_for_test, 2);
    }

    Elem_t right_val[20] {20, 20, 20, 20, 20, 2, 2, 2, 2, 2, 30, 1, 1, 1, 1, 1, 30, 30, 30, 30};

    ckeck_vals_test (20, insert_test)

    Destroy (list);
}

void erase_test (List_t* list)
{
    Construct (list);

    for (size_t i = 0; i < 10; i++)
    {
        InsertFront (list, i);
        InsertBack (list, i);
    }

    for (size_t i = 0; i < 2; i++)
    {
        EraseBack (list);
        EraseFront (list);
    }

    size_t index_for_test = FindIndex (list, 10);

    Erase (list, index_for_test);

    Elem_t right_val[15] {7, 6, 5, 4, 3, 2, 1, 0, 0, 2, 3, 4, 5, 6, 7};

    ckeck_vals_test (15, erase_test)

    Destroy (list);
}

void change_mod_test (List_t* list)
{
    Construct (list);

    for (size_t i = 0; i < 10; i++)
    {
        InsertBack (list, i);
        InsertFront (list, i);
    }

    ChangeMod (list);

    Elem_t right_val[20] {9, 8, 7, 6, 5, 4, 3, 2, 1, 0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

    ckeck_vals_test (20, change_mod_test)

    Destroy (list);
}

#undef ckeck_vals_test

#undef check_up
