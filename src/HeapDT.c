// 
// file: HeapDT.c
// Implementation file of a generic heap data structure
// @author Daniel Tregea
// @author RIT CS (Documentation from HeapDT.h)

#ifndef HEAPDT_C
#define HEAPDT_C

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include "HeapDT.h"

/// Definition of heap_s
struct heap_s {
    void ** contents;
    size_t size;
    size_t data_size;
    size_t capacity;
    int (*cmp_fun)(const void * lhs, const void * rhs );
    void (*print_item)( const void * item, FILE * outfp );
};

/// hdt_create constructs an empty Heap with a given, initial capacity.
/// A Heap's capacity must be able to grow 
/// to accommodate an arbitrarily large number of items.
/// When the number of items reaches the capacity,
/// the heap must increase its capacity to enable further growth.
///
/// @param capacity : the maximum number of items the Heap can currently hold
///
/// @param cmp_fun : function determines the ordering operation of the Heap
///        and is applied to items in the Heap payload.
///        A function that implements '<' or '<=' produces a 'min-heap',
///        and one that implements '>' or '>=' produces a 'max-heap'.
///
/// @param print_item : function iterates through the heap and prints
///        each payload item in the Heap to the provided output stream.
///        The print function is largely for debugging.
///
/// @return a Heap instance, a pointer to the Heap created
///
/// @pre cmp_fun and print_item must not be NULL.
///
/// @post the Heap is not NULL (i.e. there was enough storage).

Heap hdt_create( size_t capacity
               , int (*cmp_fun)( const void * lhs, const void * rhs )
               , void (*print_item)( const void * item, FILE * outfp ) ){
    Heap h = NULL;
    h = malloc(sizeof(struct heap_s));
    h->size = 0;
    h->capacity = capacity;
    h->contents = NULL; // allocate later based on contents size
    h->cmp_fun = cmp_fun;
    h->print_item = print_item;
    return h;
}


/// hdt_destroy deletes all dynamic Heap storage.
///
/// @param a_heap : the subject Heap
///
/// @pre a_heap is a valid Heap data type reference.
/// @pre client has previously removed and deleted all items that
///      use dynamic memory allocation; otherwise memory will leak.
///
/// @post the a_heap reference is no longer valid.

void hdt_destroy( Heap a_heap ){
    free(a_heap->contents);
    free(a_heap);
}

/// Get the number of elements in the heap
/// @param a_heap : the subject Heap
/// @return the current number of active items in the Heap.
///
/// @pre a_heap is a valid Heap data type reference.

size_t hdt_size( Heap a_heap ){
    return a_heap->size;
}

/// hdt_top peeks at the top and
/// returns an immutable pointer to the topmost item in the Heap.
///
/// @param a_heap : the subject Heap
/// @return an immutable pointer to the topmost item.
///
/// @pre a_heap is a non-empty, valid Heap data type instance.
///
/// @post the internal state of a_heap has not changed.

const void * hdt_top( const Heap a_heap ){
    return a_heap->contents[0];
}

/// Between a parent and (0-2) children, determine which to swap with based on the heap comparison function
/// @param a_heap The heap to determine the first of three
/// @param current_index the parent
/// @return the index of the first of the three
static size_t first_of_three( Heap a_heap, size_t current_index ){
    size_t left_index = (2 * current_index) + 1, right_index = (2 * current_index) + 2;
    void * left_val = NULL, * right_val = NULL, * cur_val = a_heap->contents[current_index];
    if(right_index < a_heap->size){
         left_val = a_heap->contents[left_index];
         right_val = a_heap->contents[right_index];
         if(a_heap->cmp_fun(left_val, cur_val) || a_heap->cmp_fun(right_val, cur_val)){
             if(a_heap->cmp_fun(left_val, right_val))
                 return left_index;
             else
                 return right_index;
         } else
             return current_index;
    } else if(left_index < a_heap->size){
        left_val = a_heap->contents[left_index];
        if(a_heap->cmp_fun(left_val, cur_val))
            return left_index;
        else
            return current_index;
    } else
          return current_index;
}

/// hdt_remove_top removes the topmost item from the Heap.
///
/// @param a_heap : the subject Heap
/// @return a pointer to the removed item.
///
/// @pre a_heap is a non-empty, valid Heap data type instance.
///
/// @post client assumes ownership of the item removed.

void * hdt_remove_top( Heap a_heap ){
    void * top = a_heap->contents[0]; // record the removed value and swap it with the last element in the heap
    a_heap->contents[0] = a_heap->contents[a_heap->size - 1];
    a_heap->contents[a_heap->size - 1] = top;
    
    a_heap->size--;
    size_t swap_index = first_of_three(a_heap, 0), cur_index = 0; // determine which side to sift down
    while (swap_index != cur_index){
        void * temp = a_heap->contents[cur_index]; // swap down
        a_heap->contents[cur_index] = a_heap->contents[swap_index];
        a_heap->contents[swap_index] = temp;
        cur_index = swap_index;
        swap_index = first_of_three(a_heap, cur_index);
    }
    return top;
}


/// hdt_insert_item inserts an item into the Heap.
///
/// @param a_heap : the subject Heap
/// @param item : the item to insert into the Heap
///
/// @pre a_heap is a valid Heap data type instance.
///
/// @post the Heap assumes (temporary) ownership/custody of the item, and
/// the client must not delete the memory while the Heap owns it.

void hdt_insert_item( Heap a_heap, const void * item ){
    if(a_heap->contents == NULL){ // on first insert, allocate memory based on size of data given
        a_heap->data_size = sizeof(item);
        a_heap->contents = malloc(a_heap->capacity * a_heap->data_size);
    }
    
    a_heap->size++;

    if(a_heap->size == a_heap->capacity){ // Double capacity when capacity reached
        a_heap->capacity *= 2;
        a_heap->contents = realloc(a_heap->contents, a_heap->capacity * a_heap->data_size);
        if(a_heap->contents == NULL) // End early on realloc fail
            return;
    }
    
    size_t curr_node_index = a_heap->size - 1, parent_node_index = (curr_node_index - 1) / 2;
    a_heap->contents[curr_node_index] = (void *) item; // insert new item at end of heap
    
    // sift up
    while(curr_node_index > 0 && a_heap->cmp_fun(a_heap->contents[curr_node_index], a_heap->contents[parent_node_index]) == 1){
        void * temp = a_heap->contents[curr_node_index];
        a_heap->contents[curr_node_index] = a_heap->contents[parent_node_index];
        a_heap->contents[parent_node_index] = temp;
        curr_node_index = parent_node_index;
        parent_node_index = (curr_node_index - 1) / 2;
    }
}

/// hdt_print prints the Heap items onto the specified output file stream
/// using the Heap::print_item function
///
/// @param a_heap : the subject Heap
/// @param outfp : the output file stream
///
/// @pre a_heap is a valid Heap data type instance.
///
/// @post the internal state of a_heap has not changed.
/// @see Heap::print_item

void hdt_print( Heap a_heap, FILE * outfp ){
    for(size_t i = 0; i < a_heap->size; i++){
        a_heap->print_item(a_heap->contents[i], outfp);
    }
}

#endif // HEAPDT_C

