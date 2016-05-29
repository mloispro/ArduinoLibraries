/*
 * pnew.cpp
 *
 *  Created on: 24 Dec 2011
 *      Author: Andy Brown
 */

/**
 * Global placement operator new
 */
#ifndef __PNEW_H__
#define __PNEW_H__
void* operator new(size_t size_, void* ptr_) {
    return ptr_;
}
#endif
