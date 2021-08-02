// -- heap-algo.h 堆算法
#ifndef HEAP_ALGO_H_
#define HEAP_ALGO_H_  

#include "iterator.h" 

namespace mystl 
{

// push_heap 插入堆末端并进行上滤操作
template <typename RandomIter, typename Distance, typename T>  
void push_heap_aux(RandomIter first, Distance holeIndex, Distance topIndex, T value) 
{
    auto parent = (holeIndex - 1) / 2; 
    while(holeIndex > topIndex && *(first + parent) < value)
    {
        // 大顶堆 
        *(first + holeIndex) = *(first + parent); 
        holeIndex = parent; 
        parent = (holeIndex - 1) / 2; 
    }
    *(first + holeIndex) = value; 
}

template<typename RandomIter, typename Distance>  
void push_heap_d(RandomIter first, RandomIter last, Distance* )
{
    // 新元素插入位置为哨兵位置 last 
    mystl::push_heap_aux(first, (last - first -1), 
                            static_cast<Distance>(0), *(last - 1)); 
}

template <typename RandomIter> 
void push_heap(RandomIter first, RandomIter last)
{
    mystl::push_heap_d(first, last, distance_type(first)); 
}

// overload: function object comp 
template <typename RandomIter, typename Distance, typename T, typename Compar> 
void push_heap_aux(RandomIter first, Distance holeIndex, Distance topIndex, 
                    T value, Compar comp) 
{
    auto parent = (holeIndex - 1) / 2; 
    while(holeIndex > topIndex && comp(*(first + parent), value))
    {
        *(first + holeIndex) = *(first + parent); 
        holeIndex = parent; 
        parent = (holeIndex - 1)/ 2; 
    }
    *(first + holeIndex) = value; 
}

template <typename RandomIter, typename Compar, typename Distance> 
void push_heap_d(RandomIter first, RandomIter last, Distance*, Compar comp)
{
    mystl::push_heap_aux(first, (last - first) - 1, static_cast<Distance>(0), 
                            *(last - 1), comp); 
}

template <typename RandomIter, typename Compar > 
void push_heap(RandomIter first, RandomIter last, Compar comp )
{
    mystl::push_heap_d(first, last, distance_type(first), comp); 
}

// pop_heap: delete top element and percolateUp 
template <typename RandomIter, typename T, typename Distance>  
void adjust_heap(RandomIter first, Distance holeIndex, Distance len, 
                        T value)
{
    // perlocate down 
    auto topIndex = holeIndex; 
    auto rchild = 2 * holeIndex +2; 

    while(rchild < len )
    {
        if(*(first + rchild) < *(first + rchild - 1))
        {
            --rchild; 
        }
        *(first + holeIndex) = *(first + rchild); 
        holeIndex = rchild; 
        rchild = 2*(rchild + 1); 
    }

    // 没有rc 
    if(rchild == len ) 
    {
        *(first + holeIndex) = *(first + (rchild - 1)); 
        holeIndex = rchild - 1; 
    }
    // percolateUp 
    mystl::push_heap_aux(first, holeIndex, topIndex, value); 
}

template <typename RandomIter, typename T, typename Distance>  
void pop_heap_aux(RandomIter first, RandomIter last, RandomIter result, 
                        T value, Distance*) 
{
    // 交换堆顶和堆底元素， 调整[frist, last -1): 下滤操作 
    *result = *first; 
    mystl::adjust_heap(first, static_cast<Distance>(0), last - first, 
                        value); 
}

template <typename RandomIter> 
void pop_heap(RandomIter first, RandomIter last) 
{
    mystl::pop_heap_aux(first, last - 1, *(last -1), distance_type(first)); 
}

// overload: functional object 
template <typename RandomIter, typename T, typename Distance, typename Compar>  
void adjust_heap(RandomIter first, Distance holeIndex, Distance len, T value,  
                            Compar comp) 
{
    // percolate down 
    auto topIndex = holeIndex; 
    auto rchild = 2*holeIndex + 2; 
    while(rchild < len) 
    {
        if(comp(*(first + rchild), *(first + rchild - 1))) 
        {
            --rchild; 
        }
        *(first + holeIndex) = *(first + rchild); 
        holeIndex = rchild; 
        rchild = 2 * (rchild + 1); 
    }
    if(rchild == len) 
    {
        *(first + holeIndex) = *(first + (rchild -1)); 
        holeIndex = rchild - 1; 
    }
    // percolate up 
    mystl::push_heap_aux(first, holeIndex, topIndex, value, comp); 
}                           

template <typename RandomIter, typename T, typename Distance, typename Compar>  
void pop_heap_aux(RandomIter first, RandomIter last, RandomIter result,  
                        T value, Distance*, Compar comp)  
{
    *result = *first; 
    mystl::adjust_heap(first, static_cast<Distance>(0), last - first, value, 
                    comp); 
}

template <typename RandomIter, typename Compar>  
void pop_heap(RandomIter first, RandomIter last, Compar comp) 
{
    mystl::pop_heap_aux(first, last -1, last -1, *(last - 1), 
                        distance_type(first), comp); 
}

// sort_heap 
template <typename RandomIter> 
void sort_heap(RandomIter first, RandomIter last) 
{
    for(; last - first > 1; --last) 
    {
        mystl::pop_heap(first, last); 
    }
}

// overload functioal object 
template <typename RandomIter, typename Compar>  
void sort_heap(RandomIter first, RandomIter last, Compar comp)
{
    for(; last - first > 1; --last)
    {
        mystl::pop_heap(first, last, comp); 
    }
}


// make_heap: 建堆算法 
template <typename RandomIter, typename Distance>  
void make_heap_aux(RandomIter first, RandomIter last, Distance*) 
{
    if(last - first < 2) 
    {
        return ; 
    }
    auto len = last - first; 
    auto holeIndex = (len - 2) / 2; 
    while(true) 
    {
        mystl::adjust_heap(first, holeIndex, len, *(first + holeIndex)); 
        if(holeIndex == 0) return; 
        --holeIndex; 
    }
}

template <typename RandomIter> 
void make_heap(RandomIter first, RandomIter last) 
{
    mystl::make_heap_aux(first, last, distance_type(first)); 
}

// overload functional object 
template <typename RandomIter, typename Distance, typename Compar > 
void make_heap_aux(RandomIter first, RandomIter last, Distance*, Compar comp)
{
    if(last - first < 2) 
    {
        return; 
    }

    auto len = last - first; 
    auto holeIndex = (len - 2) / 2; 
    while(true) 
    {
        // 
        mystl::adjust_heap(first, holeIndex, len, *(first + holeIndex), comp); 
        if(0 == holeIndex)
        {
            return; 
        }
        --holeIndex; 
    }
}

template <typename RandomIter, typename Compar> 
void make_heap(RandomIter first, RandomIter last, Compar comp)
{
    mystl::make_heap_aux(first, last, distance_type(first), comp); 
}
} // end of mystl 
#endif // !HEAP_ALGO_H_