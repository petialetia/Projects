DEF_CMD (ListPtrEqualsNullptr, list == nullptr, "Pointer on list equals nullptr")

DEF_CMD (NoAccessPtr, !can_ptr_be_used(list), "No access to variables on pointer")

DEF_CMD (DataEqualsNullptr, list->data == nullptr, "Pointer on array equals nullptr")

DEF_CMD (IncorrectCapacity, list->capacity < MIN_CAPACITY || list->capacity > MAX_CAPACITY, "Incorrect capacity in list")

DEF_CMD (SizeBiggerThanCapacity, list->size > list->capacity, "Size is bigger than capacity")

DEF_CMD (ZeroNodeCorrupted, !isnan (list->data[0].val) || (list->data[0].next != 0) || (list->data[0].prev != 0), "Zero node is corrupted")

DEF_CMD (IncorrectHeadPtr, (list->head > list->capacity), "Head pointer is incorrect")

DEF_CMD (IncorrectHeadNote, (list->head != 0) && ((list->data[list->head].prev != 0) || (isnan(list->data[list->head].val))), "Head node is incorrect")

DEF_CMD (IncorrectTailPtr, (list->tail > list->capacity), "Tail pointer is incorrect")

DEF_CMD (IncorrectTailNote, (list->tail != 0) && ((list->data[list->tail].next != 0) || (isnan(list->data[list->tail].val))), "Tail node is incorrect")

DEF_CMD (ConflictingHeadAndTail, (list->size == 1) && ((list->head == 0) || (list->tail != list->head)) ||
                                 (list->size > 1)  && (list->head == list->tail), "Conflicting head and tail")

DEF_CMD (IncorrectFreePtr, (list->size < list->capacity)  && (list->free == 0) ||
                           (list->size == list->capacity) && (list->free != 0), "Free pointer is incorrect")

DEF_CMD (IncorrectFreeNode, (list->size < list->capacity) && ((list->data[list->free].prev != 0) && !isnan(list->data[list->free].val)), "Free node is incorrect")

DEF_CMD (IncorrectZeroSizedList, (list->size == 0) && ((list->head != 0) || (list->tail != 0) || (list->free == 0)), "Zero sized list is incorrect")

DEF_CMD (ListLooped, is_list_looped (list), "List is looped")

DEF_CMD (EmptyNodesLooped, are_empty_nodes_looped (list), "Empty nodes are looped")
