#ifndef SET_ALGO_H_ 
#define SET_ALGO_H_ 

// set_algo.h -- union, intersection, difference, symmetric_difference 
// 

#include "iterator.h"

namespace mystl 
{
// set_union 
template <typename InputIter1, typename InputIter2, typename OutputIter> 
OutputIter set_union(InputIter1 first1, InputIter1 last1, 
                     InputIter2 first2, InputIter2 last2, OutputIter result)
{
    while( first1 != last1 && first2 != last2)
    {
        if(*first1 < *first2)
        {
            *result = *first1; 
            ++first1; 
        }
        else if (*first2 < *first1)
        {
            *result = *first2; 
            ++first2; 
        }
        else  
        {
            *result = *result; 
            ++first1; 
            ++first2; 
        }
        ++result; 
    }
    return std::copy(first2, last2, std::copy(first1, last1, result )); 
}

// more generic version using comp 
template <typename InputIter1, typename InputIter2, typename OutputIter, typename Compar> 
OutputIter set_union(InputIter1 first1, InputIter1 last1, 
                     InputIter2 first2, InputIter2 last2, 
                     OutputIter result, Compar comp)
{
    while(first1 != last1 && first2 != last2)
    {
        if(comp(*first1, *first2))
        {
            *result = *first1; 
            ++first1; 
        }
        else if (comp(*first2, *first1))
        {
            *result = *first2; 
            ++first2; 
        }
        else  
        {
            *result = *first1; 
            ++first1; 
            ++first2;
        }
        ++result; 
    }
    return std::copy(first2, last2, std::copy(first1, last1, result)); 
}

// set_intersection 
template<typename InputIter1, typename InputIter2, typename OutputIter> 
OutputIter set_intersection(InputIter1 first1, InputIter1 last1, 
                            InputIter2 first2, InputIter2 last2, 
                                OutputIter result )
{
    while(first1 != last1 && first2 != last2 )
    {
        if(*first1 < *first2)
        {
            ++first1; 
        }
        else if (*first2 < *first1)
        {
            ++first2; 
        }
        else  
        {
            *result = *first1; 
            ++first1; 
            ++first2; 
            ++result; 
        }
    }
    return result; 
}

// more generic using comp 
template <typename InputIter1, typename InputIter2, typename OutputIter, typename Compar>  
OutputIter set_intersection(InputIter1 first1, InputIter1 last1, 
                            InputIter2 first2, InputIter2 last2, 
                            OutputIter result, Compar comp)
{
    while(first1 != last1 && first2 != last2)
    {
        if (comp(*first1, *first2))
        {
            ++first1; 
        }
        else if (comp(*first2, *first1))
        {
            ++first2;
        }
        else  
        {
            *result = *first1; 
            ++first1; 
            ++first2; 
            ++result;
        }
    }
    return result; 
}

// set_difference 
template <typename InputIter1, typename InputIter2, typename OutputIter>  
OutputIter set_difference(InputIter1 first1, InputIter1 last1, 
                          InputIter2 first2, InputIter2 last2, 
                          OutputIter result) 
{
    while( first1 != last1 && first2 != last2) 
    {
        if(*first1 < * first2)
        {
            *result = *first1; 
            ++first1;
            ++result; 
        }
        else if ( *first2 < * first1)
        {
            ++first2; 
        }
        else  
        {
            ++first1;
            ++first2; 
        }
    }
    return std::copy(first1, last1, result); 
}

// more generic using comp  
template <typename InputIter1, typename InputIter2, typename OutputIter, typename Compar>  
OutputIter set_difference(InputIter1 first1, InputIter1 last1, 
                          InputIter2 first2, InputIter2 last2,  
                         OutputIter result, Compar comp) 
{
    while(first1 != last1 && first2 != last2)
    {
        if(comp(*first1, *first2))
        {
            *result = *first1; 
            ++first1; 
            ++result; 
        }
        else if (comp(*first2, *first1))
        {
            ++first2; 
        }
        else  
        {
            ++first1; 
            ++first2; 
        }
    }
    return std::copy(first1, last1, result); 
}

// set_symmetric_difference 

template <typename InputIter1, typename InputIter2, typename OutputIter> 
OutputIter set_symmetric_difference(InputIter1 first1, InputIter1 last1, 
                                    InputIter2 first2, InputIter2 last2,   
                                    OutputIter result ) 
{
    while( first1 != last1 && first2 != last2)
    {
        if(*first1 < *first2) 
        {
            *result = *first1; 
            ++first1; 
            ++result; 
        }
        else  if ( *first2 < *first1)
        {
            *result = *first2; 
            ++first2; 
            ++result; 
        }
        else  
        {
            ++first1; 
            ++first2; 
        }
    }
    return std::copy(first2, last2, std::copy(first1, last1, result)); 
}

// more generic using comp 
template <typename InputIter1, typename InputIter2, typename OutputIter, typename Compar>  
OutputIter set_symmetric_difference(InputIter1 first1, InputIter1 last1, 
                                    InputIter2 first2, InputIter1 last2,    
                                    OutputIter result, Compar comp )
{
    while(first1 != last1 && first2 != last2)
    {
        if(comp(*first1, *first2))
        {
            *result = *first1; 
            ++first1; 
            ++result; 
        }
        else if (comp(*first2, *first1))
        {
            *result = *first2; 
            ++first2; 
            ++result; 
        }
        else  
        {
            ++first1; 
            ++first2; 
        }
    }
    return std::copy(first2, last2, std::copy(first1, last1, result)); 
}  
}
#endif // !SET_ALGO_H_ 