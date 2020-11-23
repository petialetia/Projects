#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <windows.h>
//#include "C:\Users\petialetia\Desktop\TX\txlib.h"

#include "Stack.cpp"

const size_t MAX_LENGTH_OF_KEY = 100;

const size_t START_CAPACITY = 10;

const size_t INCREASE_PARAMETR = 10;

const size_t MAX_LENGTH_OF_REPLY = 100;

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

void printing_description (tree_node* answer, Stack_t* PathStack, tree* akinator_tree);

tree_node* print_sign (bool prev_answer, tree_node* current_node, char* to_be);

void print_separator (bool prev_answer, Stack_t* PathStack);

void print_standart_description (tree_node* answer, Stack_t* PathStack, tree_node* current_node);

void CompareKeys (tree* akinator_tree);

void ask_for_words (char* key1, char* key2);

tree_node* print_similarity (tree_node* current_node, tree_node* answer1, tree_node* answer2, Stack_t* PathStack1, Stack_t* PathStack2);

tree_node* find_node_by_key (char* key, tree_node* current_node, Stack_t* path_stack);

void fill_path_stack (tree_node* current_node, Stack_t* path_stack);

void DestroyTree (tree* akinator_tree);

void destroy_nodes (tree_node* current_node);

main (int argC, char* argV[])
{
    text akinator_base = {};

    tree* akinator_tree = create_tree_with_base (argC, argV, &akinator_base);

    print_greeting ();

    help_print ();

    StartProgramm (akinator_tree);

    free (akinator_base.pointer_on_buffer);

    DestroyTree (akinator_tree);

    free (akinator_tree);
}

tree* create_tree_with_base (int argC, char* argV[], text* akinator_base)
{
    keys akinator_keys = {};

    akinator_keys.in = get_arg_val (argC, argV, "in");

    FILE* input = (akinator_keys.in != 0) ? (fopen (argV[akinator_keys.in], "r")) : (fopen ("DataBase.txt", "r"));

    akinator_base->file_length = find_length_of_file (input);

    akinator_base->pointer_on_buffer = (char*) calloc (akinator_base->file_length + 1, sizeof(char));

    copy_file_in_buffer (input, akinator_base->pointer_on_buffer, &(akinator_base->file_length));

    fclose (input);

    tree* akinator_tree = (tree*) calloc (1, sizeof (tree));

    construct_tree (akinator_tree);

    akinator_tree->root = (tree_node*) calloc (sizeof (tree_node), 1);

    fill_tree (akinator_base, akinator_tree);

    return akinator_tree;
}

size_t get_arg_val (int argC, char* argV[], char* key)
{
    assert (argC != 0);
    assert (key != nullptr);


    for (size_t cnt = 1; cnt < argC; cnt++)
    {
        if (strcmp (argV[cnt], key) == 0)
        {
            return cnt + 1;
        }
    }

    return 0;
}

size_t find_length_of_file (FILE* input)
{
    assert (input != nullptr);

    fseek (input, 0, SEEK_END);
    int size = ftell (input);
    assert (size != 0);

    fseek (input, 0, SEEK_SET);
    return size;
}

void copy_file_in_buffer (FILE* input, char* pointer_on_buffer, size_t* file_length)
{
    assert (input != nullptr);
    assert (pointer_on_buffer != nullptr);
    assert (file_length != 0);

    size_t size = fread (pointer_on_buffer, sizeof(char), *file_length, input);
    assert (size != 0);

    *file_length = size+1;
}

/*#define create_child(direction)                                                     \
{                                                                                   \
    current_node->##direction##_child = (tree_node*) calloc (1, sizeof (tree_node));\
    current_node->##direction##_child->parent = current_node;                       \
    current_node = current_node->##direction##_child;                               \
}*/

#define process_cases                                                                          \
    switch (*pointer)                                                                          \
    {                                                                                          \
        case '"' : pointer++;                                                                  \
                   current_node->key = pointer;                                                \
                   pointer = strchr (pointer, '"');                                            \
                   *pointer = '\0';                                                            \
                   break;                                                                      \
                                                                                               \
        case '{' : if (current_node->right_child == nullptr)                                   \
                   {                                                                           \
                       current_node->right_child = (tree_node*) calloc (1, sizeof (tree_node));\
                       current_node->right_child->parent = current_node;                       \
                       current_node = current_node->right_child;                               \
                       /*create_child (right)*/                                                \
                   }                                                                           \
                   else                                                                        \
                   {                                                                           \
                       current_node->left_child = (tree_node*) calloc (1, sizeof (tree_node)); \
                       current_node->left_child->parent = current_node;                        \
                       current_node = current_node->left_child;                                \
                       /*create_child (left)*/                                                 \
                   }                                                                           \
                   break;                                                                      \
                                                                                               \
        case '}' : current_node = current_node->parent;                                        \
                   break;                                                                      \
                                                                                               \
        case '\0': return;                                                                     \
                                                                                               \
        default  : break;                                                                      \
    }

void fill_tree (text* akinator_base, tree* akinator_tree)
{
    tree_node* current_node = akinator_tree->root;

    char* pointer = akinator_base->pointer_on_buffer;

    while ((current_node != akinator_tree->root) || (akinator_tree->root->left_child == nullptr))
    {
        process_cases

        pointer++;
    }
}

#undef process_cases

#undef create_child

void construct_tree (tree* akinator_tree)
{
    akinator_tree->new_nodes_buffer = (char*) calloc (START_CAPACITY, (MAX_LENGTH_OF_KEY + 1) * sizeof (char));

    akinator_tree->buffer_capacity = START_CAPACITY;
}

void print_greeting ()
{
    printf ("\n\n");
    printf ("Hello, mortal.\nYou're here today for me to show you my infinite abilities!\n\n");
}

void help_print ()
{
    printf ("\nYou can choose 5 options:\n\n");
    printf ("\t1) Play the game \n\t\"think up anything and I\'ll guess right what it was\"\n\n");
    printf ("\t2) Print definition \n\t\"print anything and I\'ll write what it is\"\n\n");
    printf ("\t3) Print the difference \n\t\"print 2 words and I\'ll show you how are they alike and how do they differ\"\n\n");
    printf ("\t4) Draw tree \n\t\"I'll show you my secret data base, but don't show it to anyone\"\n\n");
    printf ("\t5) Exit \n\t\"you can turn me off, but next time me meet I'll become smarter\"\n\n");
}

void StartProgramm (tree* akinator_tree)
{
    printf ("\n(1/2/3/4/5)\n");
    printf ("(or print \"h\" for help)\n\n");

    char reply[MAX_LENGTH_OF_REPLY] = {};

    scanf ("%s", reply);

    switch (reply[0])
    {
        case '1': GameMod (akinator_tree);
                  StartProgramm (akinator_tree);
                  break;

        case '2': PrintDefinition (akinator_tree);
                  StartProgramm (akinator_tree);
                  break;

        case '3': CompareKeys (akinator_tree);
                  StartProgramm (akinator_tree);
                  break;

        case '4': DrawTree (akinator_tree);
                  StartProgramm (akinator_tree);
                  break;

        case '5': CreatNewBase (akinator_tree);
                  break;

        case 'h': help_print ();
                  StartProgramm (akinator_tree);
                  break;

        default : printf ("\nUnknown command \"%c\", please, try again\n", reply[0]);
                  StartProgramm (akinator_tree);
                  break;
    }
}

void DrawTree (tree* akinator_tree)
{
    FILE* graphviz_file = fopen ("GraphvizFile.txt", "w");

    fprintf (graphviz_file, "digraph PL\n");
    fprintf (graphviz_file, "{\nrankdir=HR;\n");

    print_nodes_description (graphviz_file, akinator_tree->root);

    fprintf (graphviz_file, "}\n");

    fclose (graphviz_file);

    system ("dot -Tpng GraphvizFile.txt -o Graph.png");

    system ("start Graph.png");
}

void print_nodes_description (FILE* graphviz_file, tree_node* current_node)
{
    if (current_node->right_child != nullptr)
    {
        fprintf (graphviz_file, "\"%p\" [style=\"filled\", fillcolor = \"yellow\", shape=\"rectangle\", label=\"%s\"];\n", current_node, current_node->key);

        print_nodes_description (graphviz_file, current_node->left_child);
        print_nodes_description (graphviz_file, current_node->right_child);

        fprintf (graphviz_file, "\"%p\" -> \"%p\"[color=\"red\", label=\"no\"];\n", current_node, current_node->left_child);
        fprintf (graphviz_file, "\"%p\" -> \"%p\"[color=\"green\", label=\"yes\"];\n", current_node, current_node->right_child);
    }
    else
    {
        fprintf (graphviz_file, "\"%p\" [style=\"filled\", fillcolor = \"#98FB98\", label=\"%s\"];\n", current_node, current_node->key);
    }
}

#define check_reply(yes_command, no_command, unknown_answer_command)\
                                                                    \
    if (reply[0] == 'y')                                            \
    yes_command                                                     \
    else                                                            \
    {                                                               \
        if (reply[0] == 'n')                                        \
        no_command                                                  \
        else                                                        \
        unknown_answer_command                                      \
    }

void GameMod (tree* akinator_tree)
{
    tree_node* current_node = akinator_tree->root;

    char reply[MAX_LENGTH_OF_REPLY] = {};

    while (current_node->right_child != nullptr)
    {
        printf ("\nIs it %s?\n(y/n)\n\n", current_node->key);
        scanf ("%s", reply);

        check_reply (
        {
            current_node = current_node->right_child;
        },
        {
            current_node = current_node->left_child;
        },
        {
            printf ("\nUnknown command \"%c\", please, try again\n", reply[0]);
        })
    }

    print_answer (current_node, akinator_tree);
}

void print_answer (tree_node* current_node, tree* akinator_tree)
{
    char reply[MAX_LENGTH_OF_REPLY] = {};

    printf ("\nI think I know what it is. It's \"%s\".\nIsn't it?\n(y/n)\n\n", current_node->key);
    scanf ("%s", reply);
    check_reply (
    {
        printf ("\nYeah, I knew it, I'm smartest thing in the world, kneel before me, leather bag!!!\n");
    },
    {
        add_new_key (current_node, akinator_tree);
    },
    {
        printf ("\nUnknown command \"%c\", please, try again\n", reply[0]);
        print_answer (current_node, akinator_tree);
    })
}

#undef check_reply

void add_new_key (tree_node* current_node, tree* akinator_tree)
{
    if (akinator_tree->num_of_new_nodes == akinator_tree->buffer_capacity)
    {
        extend_buffer_of_new_nodes (akinator_tree);
    }

    char* ptr_on_answer = akinator_tree->new_nodes_buffer + akinator_tree->num_of_new_nodes * (MAX_LENGTH_OF_KEY + 1);

    char* ptr_on_sign_of_differentiation = ptr_on_answer + (MAX_LENGTH_OF_KEY + 1);

    printf ("\nThen what is it?\n(write what you think of)\n\n");

    scanf ("%s", ptr_on_answer);

    ask_for_difference (current_node, ptr_on_answer, ptr_on_sign_of_differentiation);

    add_new_nodes (current_node, ptr_on_answer, ptr_on_sign_of_differentiation, akinator_tree);

    akinator_tree->num_of_new_nodes = akinator_tree->num_of_new_nodes + 2;

    printf ("\nYeah, I know even more now, you're helping me to become smarter, you're doomed, Skynet is near\n");
}

void extend_buffer_of_new_nodes (tree* akinator_tree)
{
    akinator_tree->buffer_capacity = akinator_tree->buffer_capacity + INCREASE_PARAMETR;

    akinator_tree->new_nodes_buffer = (char*) realloc (akinator_tree->new_nodes_buffer, akinator_tree->buffer_capacity * (MAX_LENGTH_OF_KEY + 1) * sizeof(char));
}


void ask_for_difference (tree_node* current_node, char* ptr_on_answer, char* ptr_on_sign_of_differentiation)
{
    printf ("\nWhat the difference between \"%s\" and \"%s\"\n\"%s\" is ...\n(write sign of differentiation)\n\n", current_node->key, ptr_on_answer, ptr_on_answer);

    scanf ("%s", ptr_on_sign_of_differentiation);

    if (strncmp ("not", ptr_on_sign_of_differentiation, 3) == 0)
    {
        printf ("Don't use \"not\" in differentiation, think up something else and try again\n");
        ask_for_difference (current_node, ptr_on_answer, ptr_on_sign_of_differentiation);
    }
}

void add_new_nodes (tree_node* current_node, char* ptr_on_answer, char* ptr_on_sign_of_differentiation, tree* akinator_tree)
{
    tree_node* new_node = (tree_node*) calloc (1, sizeof (tree_node));

    new_node->key = ptr_on_sign_of_differentiation;

    if (current_node->parent != nullptr)
    {
        new_node->parent = current_node->parent;

        if (current_node->parent->left_child == current_node)
        {
            current_node->parent->left_child = new_node;
        }
        else
        {
            current_node->parent->right_child = new_node;
        }
    }
    else
    {
        akinator_tree->root = new_node;
    }

    current_node->parent = new_node;

    new_node->left_child = current_node;
    new_node->right_child = (tree_node*) calloc (1, sizeof (tree_node));
    new_node->right_child->key = ptr_on_answer;
    new_node->right_child->parent = new_node;
}

void CreatNewBase (tree* akinator_tree)
{
    if (akinator_tree->num_of_new_nodes == 0)
    {
        return;
    }

    FILE* data_base_file = fopen ("DataBase.txt", "w");

    print_descendant (data_base_file, akinator_tree->root);

    fclose (data_base_file);
}

void print_descendant (FILE* data_base_file, tree_node* current_node)
{
    assert (current_node != nullptr);

    fprintf (data_base_file, "\"%s\"\n", current_node->key);

    if (current_node->right_child != nullptr)
    {
        fprintf (data_base_file, "{\n");

        print_descendant (data_base_file, current_node->right_child);

        fprintf (data_base_file, "}\n");

        fprintf (data_base_file, "{\n");

        print_descendant (data_base_file, current_node->left_child);

        fprintf (data_base_file, "}\n");
    }
}

void PrintDefinition (tree* akinator_tree)
{
    Stack_t* PathStack = (Stack_t*) calloc (1, sizeof (Stack_t));

    Construct (PathStack);

    char key[MAX_LENGTH_OF_KEY + 1] = {};

    printf ("\nWhat is the word definition of which you are interested?\n(enter word)\n\n");

    scanf ("%s", key);

    //key = strlwr (key);

    tree_node* answer = find_node_by_key (key, akinator_tree->root, PathStack);

    printing_description (answer, PathStack, akinator_tree);

    Destroy (PathStack);

    free (PathStack);
}

void printing_description (tree_node* answer, Stack_t* PathStack, tree* akinator_tree)
{
    if (answer == nullptr)
    {
        printf ("\nI don't know what it is, you can play the game and add this word\n");
        return;
    }

    if (answer == akinator_tree->root)
    {
        printf ("\n\"%s\" is \"%s\", nothing less, nothing more\n", answer->key, answer->key);
        return;
    }

    print_standart_description (answer, PathStack, akinator_tree->root);
}

void print_standart_description (tree_node* answer, Stack_t* PathStack, tree_node* current_node)
{
    printf ("\n\"%s\"", answer->key);

    bool prev_answer = 0;

    while (PathStack->size != 1)
    {
        prev_answer = Pop(PathStack);

        current_node = print_sign (prev_answer, current_node, "is");

        print_separator (prev_answer, PathStack);
    }

    prev_answer = Pop(PathStack);

    if (prev_answer == 1)
    {
        printf (" is \"%s\"\n", current_node->key);
    }
    else
    {
        printf (" isn't \"%s\"\n", current_node->key);
    }
}

tree_node* print_sign (bool prev_answer, tree_node* current_node, char* to_be)
{
    if (prev_answer == 1)
    {
        printf (" %s \"%s\"", to_be, current_node->key);
        return current_node->right_child;
    }
    else
    {
        printf (" %sn't \"%s\"", to_be, current_node->key);
        return current_node->left_child;
    }
}

void print_separator (bool prev_answer, Stack_t* PathStack)
{
    printf (",");

    if (prev_answer != Top(PathStack))
    {
        printf (" but");
    }
}

#define process_special_cases                                                                     \
{                                                                                                 \
    if (answer1 == nullptr)                                                                       \
    {                                                                                             \
        printf ("\nI don't know what \"%s\" is, you can play the game and add this word\n", key1);\
        StartProgramm (akinator_tree);                                                            \
        return;                                                                                   \
    }                                                                                             \
                                                                                                  \
    if (answer2 == nullptr)                                                                       \
    {                                                                                             \
        printf ("\nI don't know what \"%s\" is, you can play the game and add this word\n", key2);\
        StartProgramm (akinator_tree);                                                            \
        return;                                                                                   \
    }                                                                                             \
                                                                                                  \
    if ((answer1 == akinator_tree->root) || (answer2 == akinator_tree->root))                     \
    {                                                                                             \
        printing_description (answer1, PathStack1, akinator_tree);                                \
        printing_description (answer2, PathStack2, akinator_tree);                                \
        return;                                                                                   \
    }                                                                                             \
}

#define check_for_similarity                                        \
                                                                    \
    if (strcmp (key1, key2) == 0)                                   \
    {                                                               \
        printf ("\nYou wrote 2 similar words, please, try again\n");\
        CompareKeys (akinator_tree);                                \
        return;                                                     \
    }

#define finish_description(num)                                                \
                                                                               \
    if (PathStack##num->size != 0)                                             \
    {                                                                          \
        print_standart_description (answer##num, PathStack##num, current_node);\
    }                                                                          \

void CompareKeys (tree* akinator_tree)
{
    Stack_t* PathStack1 = (Stack_t*) calloc (1, sizeof (Stack_t));
    Construct (PathStack1);

    Stack_t* PathStack2 = (Stack_t*) calloc (1, sizeof (Stack_t));
    Construct (PathStack2);

    char key1[MAX_LENGTH_OF_KEY + 1] = {};
    char key2[MAX_LENGTH_OF_KEY + 1] = {};

    ask_for_words (key1, key2);

    check_for_similarity

    tree_node* answer1 = find_node_by_key (key1, akinator_tree->root, PathStack1);
    tree_node* answer2 = find_node_by_key (key2, akinator_tree->root, PathStack2);

    process_special_cases

    tree_node* current_node = print_similarity (akinator_tree->root, answer1, answer2, PathStack1, PathStack2);

    finish_description (1)
    finish_description (2)

    Destroy (PathStack1);
    Destroy (PathStack2);
    free (PathStack1);
    free (PathStack2);
}

void ask_for_words (char* key1, char* key2)
{
    printf ("\nWhat is the first word?\n(enter word)\n\n");

    scanf ("%s", key1);

    printf ("\nWhat is the second word?\n(enter word)\n\n");

    scanf ("%s", key2);
}

tree_node* print_similarity (tree_node* current_node, tree_node* answer1, tree_node* answer2, Stack_t* PathStack1, Stack_t* PathStack2)
{
    bool prev_answer1, prev_answer2 = 0;

    if (Top (PathStack1) == Top (PathStack2))
    {
        printf ("\n\"%s\" and \"%s\" both", answer1->key, answer2->key);

        while (true)
        {
            prev_answer1 = Pop(PathStack1);
            prev_answer2 = Pop (PathStack2);

            current_node = print_sign (prev_answer1, current_node, "are");

            if ((PathStack1->size == 0) || (PathStack2->size == 0))
            {
                break;
            }

            if (Top (PathStack1) != Top (PathStack2))
            {
                break;
            }

            print_separator (prev_answer1, PathStack1);
        }
        printf ("\n\nHowever:\n");
    }
    return current_node;
}

#undef process_special_cases

#undef check_for_similarity

#undef finish_description

tree_node* find_node_by_key (char* key, tree_node* current_node, Stack_t* path_stack)
{
    if (strcmp (key, strlwr(current_node->key)) != 0)
    {
        if (current_node->right_child != nullptr)
        {
            tree_node* required_node_r = find_node_by_key (key, current_node->right_child, path_stack);

            tree_node* required_node_l = find_node_by_key (key, current_node->left_child, path_stack);

            if (required_node_r != nullptr)
            {
                return required_node_r;
            }

            return required_node_l;
        }

        return nullptr;
    }
    else
    {
        fill_path_stack (current_node, path_stack);
    }

    return current_node;
}

void fill_path_stack (tree_node* current_node, Stack_t* path_stack)
{
    assert (path_stack != nullptr);
    assert (current_node != nullptr);

    if (path_stack->size != 0)
    {
        printf ("Path stack is filled, you're doing something wrong, or may be there are two nodes with the same key\n");
    }
    else
    {
        while (current_node->parent != nullptr)
        {
            if (current_node->parent->right_child == current_node)
            {
                Push (path_stack, 1);
            }
            else
            {
                Push (path_stack, 0);
            }

            current_node = current_node->parent;
        }
    }
}

void DestroyTree (tree* akinator_tree)
{
    destroy_nodes (akinator_tree->root);

    free (akinator_tree->new_nodes_buffer);
}

void destroy_nodes (tree_node* current_node)
{
    if (current_node->right_child != nullptr)
    {
        destroy_nodes (current_node->left_child);
        destroy_nodes (current_node->right_child);
    }

    free (current_node);
}



