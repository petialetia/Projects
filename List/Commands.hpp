DEF_CMD (ListPtrEqualsNullptr, list == nullptr, "Pointer on list equals nullptr")

DEF_CMD (NoAccessPtr, !can_ptr_be_used(list), "No access to variables on pointer")

DEF_CMD (DataEqualsNullptr, list->data == nullptr, "Pointer on array equals nullptr")

DEF_CMD (ZeroNodeCorrupted, !isnan (list->data[0].val) || (list->data[0].next != 0) || (list->data[0].prev != 0), "Zero node is corrupted")

//DEF_CMD (IncorrectHead, (list->data[list->head].prev != 0) || (list->head > list->capacity) || ((list->head != 0) && (isnan(list->data[list->head].val)) , "Head note is corrupted")

//DEF_CMD (IncorrectTail, (list->data[list->tail].next != 0) || (list->tail > list->capacity) || ((list->tail != 0) && (isnan(list->data[list->tail].val)) , "Tail note is corrupted")
