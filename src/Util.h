/**
 * @file Util.h
 * @author Hayden McAfee (hayden@outlook.com)
 * @date 2021-06-10
 * @copyright Copyright (c) 2021 Hayden McAfee
 */

#pragma once

namespace RTMPoop
{
    class Util
    {
    public:
        /**
         * @brief Given an errno error code, return the string representation.
         */
        static std::string ErrnoToString(int error)
        {
            char errnoStrBuf[256];
            char* errMsg = strerror_r(error, errnoStrBuf, sizeof(errnoStrBuf));
            return std::string(errMsg);
        }
    };
}

