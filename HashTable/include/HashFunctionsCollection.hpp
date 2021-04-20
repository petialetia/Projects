#define NDEBUG

#include <assert.h>
#include <string.h>

#include "ForHashTable.hpp"

const size_t NUM_OF_HASH_FUNCTIONS = 7;

const size_t BYTE_SIZE = 8;

const size_t BIG_PRIME_NUMBER = 999983; // the biggest prime number less than 1 000 000

size_t CountUnaryHash      (hash_table_val_type elem);
size_t CountLengthHash     (hash_table_val_type elem);
size_t CountFirstAsciiHash (hash_table_val_type elem);
size_t CountSumAsciiHash   (hash_table_val_type elem);
size_t CountRorHash        (hash_table_val_type elem);
size_t CountRolHash        (hash_table_val_type elem);
size_t CountPolynomialHash (hash_table_val_type elem);