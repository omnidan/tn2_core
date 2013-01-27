/**
 * Some functions for the kernel.
 * 
 * This file is part of LOSK, the LayOS Kernel.
 * 
 * @author Daniel Bugl <Daniel@touchlay.com>
 * @copyright Copyright (c) 2012 by TouchLay (http://touchlay.com)
 */

#ifndef ETC_H
#define	ETC_H

#include <iostream>
#include <algorithm>
#include <cctype>
#include <functional>

std::string stoupper( std::string s ) {
    std::transform(
        s.begin(),
        s.end(),
        s.begin(),
        std::ptr_fun <int, int> ( std::toupper )
    );
    return s;
}

#endif /* ETC_H */
