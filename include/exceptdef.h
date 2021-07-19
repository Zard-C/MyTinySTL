// -- exceptdef.h 异常检测宏函数
#ifndef EXCEPTDEF_H_
#define EXCEPTDEF_H_

#include <stdexcept> 
#include <cassert> 

namespace mystl
{

#define MYSTL_DEBUG(expr) assert(expr) 
#define THROW_LENGTH_ERROR_IF(expr, what) if((expr)) throw std::length_error(what) 
#define THROW_OUT_OF_RANGE_IF(expr, what) if((expr)) throw std::out_of_range(what) 
#define THROW_RUNTIME_ERROR_IF(expr, what) if((expr)) throw std::runtime_error(what) 

}   // end of namespace mystl 
#endif // !EXCEPTDEF_H_
