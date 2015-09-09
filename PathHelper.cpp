//
//  PathHelper.cpp
//
//  Created by Sitael S.p.A. on 19/02/15.
//

#include "PathHelper.h"

/**
* Get current path of executable
* @return string Current path
*/
string PathHelper::getCurrentPath()
{
   
    string currentPath = filesystem::current_path().string();
    return currentPath;
}
