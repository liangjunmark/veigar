/*
 * Copyright (c) winsoft666.
 * All rights reserved.
 *
 * This source code is licensed under the license found in the
 * LICENSE file in the root directory of this source tree.
 */
#include "log.h"
#include "os_platform.h"
#include <memory>
#include "log_util.h"

#ifdef VEIGAR_OS_WINDOWS
#ifndef _INC_WINDOWS
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif  // !WIN32_LEAN_AND_MEAN
#ifndef _WINSOCKAPI_
#define _WINSOCKAPI_
#endif  // !_WINSOCKAPI_
#include <Windows.h>
#endif
#include <strsafe.h>
#else
#include <string>
#include <stdarg.h>
#endif
#include <assert.h>
#include "string_helper.h"

namespace veigar {
void log(const wchar_t* format, ...) {
    std::wstring output;
    va_list args;
    va_start(args, format);
    const bool ret = StringHelper::StringPrintfV(format, args, output);
    va_end(args);

    assert(ret);
    if (ret) {
#ifdef VEIGAR_OS_WINDOWS
        OutputDebugStringW(output.c_str());
#else
        printf("%ls", output.c_str());
#endif
    }
}

void log(const char* format, ...) {
    std::string output;
    va_list args;
    va_start(args, format);
    const bool ret = StringHelper::StringPrintfV(format, args, output);
    va_end(args);

    assert(ret);
    if (ret) {
#ifdef VEIGAR_OS_WINDOWS
        OutputDebugStringA(output.c_str());
#else
        printf("%s", output.c_str());
#endif
        auto pos = output.find("Veigar");
        if (pos != std::string::npos) {
            output.replace(pos, 6, "RPC");
        }
        if (!output.empty() && output.back() == '\n') {
            output.erase(output.size() - 1);
        }
        SPD_LOG(INFO)(output);
    }
}
}  // namespace veigar