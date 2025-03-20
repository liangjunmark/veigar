/*
 * Copyright (c) winsoft666.
 * All rights reserved.
 *
 * This source code is licensed under the license found in the
 * LICENSE file in the root directory of this source tree.
 */
#include "veigar/veigar.h"
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <mutex>
#include <cstdlib>  //std::system
#include <inttypes.h>
#include "thread_group.h"

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#define IS_WINDOWS 1
#endif

#if IS_WINDOWS
bool IsWow64(HANDLE process, bool& result) {
    BOOL bIsWow64 = FALSE;

    typedef BOOL(WINAPI * LPFN_ISWOW64PROCESS)(HANDLE, PBOOL);
    LPFN_ISWOW64PROCESS fnIsWow64Process =
        (LPFN_ISWOW64PROCESS)GetProcAddress(GetModuleHandle(TEXT("kernel32")), "IsWow64Process");

    if (NULL == fnIsWow64Process) {
        return false;
    }

    if (!fnIsWow64Process(process, &bIsWow64)) {
        return false;
    }

    result = !!bIsWow64;
    return true;
}

bool IsWin64(HANDLE process) {
#if (defined _WIN64) || (defined WIN64)
    return true;
#else
    bool result = false;
    IsWow64(process, result);
    return result;
#endif
}

bool Is32BitProcess() {
    HANDLE process = GetCurrentProcess();
    if (!process)
        return false;

    bool wow64 = false;
    IsWow64(process, wow64);
    if (wow64)
        return true;

    BOOL win64 = IsWin64(process);
    return !win64;
}

#endif

std::string genRandomString(const uint32_t len) {
    srand((uint32_t)time(nullptr) + len);
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
    std::string ret;
    ret.reserve(len);

    for (uint32_t i = 0; i < len; ++i) {
        ret += alphanum[rand() % (sizeof(alphanum) - 1)];
    }

    return ret;
}

std::string TimeToHuman(int64_t microseconds) {
    int64_t hour = microseconds / 3600000000;
    int64_t min = (microseconds % 3600000000) / 60000000;
    int64_t sec = (microseconds % 60000000) / 1000000;
    int64_t mill = (microseconds % 1000000) / 1000;
    int64_t micro = microseconds % 1000;

    std::string str;
    if (hour > 0)
        str += std::to_string(hour) + "h";
    if (min > 0)
        str += std::to_string(min) + "m";
    if (sec > 0)
        str += std::to_string(sec) + "s";
    if (mill > 0)
        str += std::to_string(mill) + "ms";
    if (micro > 0)
        str += std::to_string(micro) + "μs";
    return str;
}

std::vector<std::string> StringSplit(const std::string& src, const std::string& delimiter, bool includeEmptyStr) {
    std::vector<std::string> fields;
    typename std::string::size_type offset = 0;
    typename std::string::size_type pos = src.find(delimiter, 0);

    while (pos != std::string::npos) {
        std::string t = src.substr(offset, pos - offset);
        if ((t.length() > 0) || (t.length() == 0 && includeEmptyStr))
            fields.push_back(t);
        offset = pos + delimiter.length();
        pos = src.find(delimiter, offset);
    }

    const std::string t = src.substr(offset);
    if ((t.length() > 0) || (t.length() == 0 && includeEmptyStr))
        fields.push_back(t);
    return fields;
}

veigar::Veigar vg;
std::string channelName;
int outputRecv = 0;
int callMethod = 0;
int threadNum = 0;
int callTimesEachThread = 0;
int callTimeout = 0;

void CallThreadProc(std::size_t threadId, std::string targetChannel) {
    printf("[Thread %" PRId64 ", Target %s] Calling...\n", (int64_t)threadId, targetChannel.c_str());

    for (int i = 0; i < callTimesEachThread; i++) {
        std::string strRandom = genRandomString(10 + i % 500);

        if (callMethod == 0) {
            veigar::CallResult cr = vg.syncCall(targetChannel, callTimeout, "echo", strRandom);
            if (cr.isSuccess() && cr.obj.get().as<std::string>() == strRandom) {
                //printf("[Thread %" PRId64 ", Target %s] Call %d OK\n", (int64_t)threadId, targetChannel.c_str(), i);
            }
            else {
                printf("[Thread %" PRId64 ", Target %s] Call %d Failed: %s\n", (int64_t)threadId, targetChannel.c_str(), i, cr.errorMessage.c_str());
            }
        }
        else if (callMethod == 1) {  // Async with promise
            std::shared_ptr<veigar::AsyncCallResult> acr = vg.asyncCall(targetChannel, callTimeout, "echo", strRandom);
            assert(acr);
            if (acr) {
                if (acr->second.valid()) {
                    veigar::CallResult cr;
                    auto waitResult = acr->second.wait_for(std::chrono::milliseconds(callTimeout));
                    if (waitResult == std::future_status::timeout) {
                        cr.errCode = veigar::ErrorCode::TIMEOUT;
                        cr.errorMessage = "Timeout";
                    }
                    else {
                        cr = std::move(acr->second.get());
                    }

                    if (cr.isSuccess() && cr.obj.get().as<std::string>() == strRandom) {
                        //printf("[Thread %" PRId64 ", Target %s] Call %d OK\n", (int64_t)threadId, targetChannel.c_str(), i);
                    }
                    else {
                        printf("[Thread %" PRId64 ", Target %s] Call %d Failed: %s\n", (int64_t)threadId, targetChannel.c_str(), i, cr.errorMessage.c_str());
                    }
                }
                vg.releaseCall(acr->first);
            }
        }
        else if (callMethod == 2) {  // Async with callback
            vg.asyncCall(
                [threadId, targetChannel, strRandom, i](const veigar::CallResult& cr) {
                    if (cr.isSuccess() && cr.obj.get().as<std::string>() == strRandom) {
                        //printf("[Thread %" PRId64 ", Target %s] Call %d OK\n", (int64_t)threadId, targetChannel.c_str(), i);
                    }
                    else {
                        printf("[Thread %" PRId64 ", Target %s] Call %d Failed: %s\n", (int64_t)threadId, targetChannel.c_str(), i, cr.errorMessage.c_str());
                    }
                },
                targetChannel, callTimeout, "echo", strRandom);
        }
    }
}

int main0(int argc, char** argv) {
    setlocale(LC_ALL, "");
    printf("Veigar: Cross platform RPC library using shared memory.\n");
#if IS_WINDOWS
    printf("Version: %d.%d (%s)\n", veigar::VERSION_MAJOR, veigar::VERSION_MINOR, Is32BitProcess() ? "x86" : "x64");
#else
    printf("Version: %d.%d\n", veigar::VERSION_MAJOR, veigar::VERSION_MINOR);
#endif
    printf("Input 'quit' to exit the program.\n");
    printf("\n");

    vg.setTimeoutOfRWLock(260);
    callTimeout = 260 * 2;

    while (true) {
        if (channelName.empty()) {
            std::cout << "Channel Name: ";
            std::cin >> channelName;

            if (channelName.empty()) {
                std::cout << "Channel name can not be empty.\n";
                continue;
            }

            if (channelName == "quit")
                break;

            if (!vg.init(channelName, 200, 10240)) {
                printf("Init failed.\n");
                channelName.clear();
                continue;
            }

            std::cout << "Init success.\n";

            if (!vg.bind("echo", [](const std::string& msg) {
                    if (outputRecv != 0) {
                        std::cout << "RECV:" << msg << std::endl;
                    }
                    return msg;
                })) {
                printf("Bind echo function failed.\n");
                continue;
            }

            std::cout << "Bind functions success.\n";
            std::cout << "Output received message(0/1): ";
            std::cin >> outputRecv;
            std::cout << "Armed.\n\n";
        }
        else {
            std::string targetChannels;
            std::vector<std::string> targetChannelList;

            std::cout << "Target channel names (Split by comma): ";
            std::cin >> targetChannels;

            if (targetChannels == "quit")
                break;

            targetChannelList = StringSplit(targetChannels, ",", false);
            if (targetChannelList.empty())
                continue;

            std::cout << "Call method (0 = Sync, 1 = Async with promise, 2 = Async with callback): ";
            std::cin >> callMethod;

            std::cout << "Thread Number: ";
            std::cin >> threadNum;

            std::cout << "Call times for each thread: ";
            std::cin >> callTimesEachThread;

            if (callMethod == 2) {
                if (threadNum * callTimesEachThread > vg.msgQueueCapacity()) {
                    printf("The total number of call can't greater than message queue's capacity when use callback method(%d).\n", vg.msgQueueCapacity());
                    continue;
                }
            }

            std::vector<std::shared_ptr<ThreadGroup>> threadGroups;
            for (auto targetChannel : targetChannelList) {
                auto tg = std::make_shared<ThreadGroup>();
                tg->createThreads(threadNum, targetChannel, CallThreadProc);
                threadGroups.push_back(tg);
            }

            for (auto tg : threadGroups) {
                tg->joinAll();
            }

            threadGroups.clear();
        }
    }

    if (vg.isInit()) {
        vg.uninit();
    }

    return 0;
}


#include "file_util.h"
#include "swig_util.h"

static std::vector<std::string> split(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::istringstream stream(str);
    std::string token;
    while (std::getline(stream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

int main(int argc, char** argv) {
    std::string channel = "68F4F2B04D144957A27F2FBA81EAD48E";//channel
    veigar::Veigar vg;
    vg.init("test", 2 << 6, 2 << 20);
    vg.setTimeoutOfRWLock(260);
    int callTimeout = 260 * 2;
    //std::string strRandom = "hello";

    //veigar::CallResult cr = vg.syncCall(channel, callTimeout, "help", "hello");
    //if (cr.isSuccess()) {
    //    printf("[Thread %" PRId64 ", Target %s] Call %d OK\n", 1, "test", 1);
    //}
    //else {
    //    printf("[Thread %" PRId64 ", Target %s] Call %d Failed: %s\n", 1, "test", 1, cr.errorMessage.c_str());
    //}

    auto data = util::file::read_file("C:\\Users\\liangjunmark\\Desktop\\xhh\\params_20250213.json");
    auto any_obj = json_to_map(data);
    map_read_script(any_obj, "D:\\WorkSpace\\Cpp\\aps3.5\\gui\\script.py");

    {
        veigar_msgpack::sbuffer buffer;
        veigar_msgpack::pack(buffer, any_obj);
        std::cout << data.size() << " " << buffer.size() << std::endl;
    }

    auto ef = [&](int i) {
        std::string exp = "#locals";
        if (i == 1) {
            exp = "#locals;xbuf_input";
        }
        else if (i == 2) {
            exp = "#locals;dat_obj";
        }
        else if (i == 3) {
            exp = "#locals;dat_obj;ResAS";
        }
        else if (i == 4) {
            exp = "#locals;dat_obj;ResAS;1";
        }
        else if (i == 5) {
            exp = "#locals@dat_obj;ResAS;1";
        }
        else if (i == 6) {
            exp = "#locals@dat_obj[\"ResAS\"];1";
        }
        else if (i == 7) {
            exp = "#locals@dat_obj[\"ResAS\"][1]";
        }
        else if (i == 8) {
            exp = "#locals@dat_obj[\"ResAS\"][1][\"qty\"]";
        }
        auto cr = vg.syncCall(channel, callTimeout, "dbg_watch_variable", 0, exp);
        if (cr.isSuccess()) {
            auto frame_stack = cr.obj.get().as<std::list<std::tuple<std::string, std::string, std::string>>>();
            for (auto& frame : frame_stack) {
                std::cout << std::get<0>(frame) << " " << std::get<1>(frame) << " " << std::get<2>(frame) << std::endl;
            }
        }
        else {
            std::cout << "error" << std::endl;
        }
    };
    

    while (true) {
        std::cout << "input cmd ... " << std::endl;
        std::string str;
        std::cin >> str;
        std::vector<std::string> result = split(str, ';');
        result.emplace_back("");
        auto& num = result.at(0);
        if (num == "h")
            auto acr = vg.asyncCall(channel, callTimeout, "help", result.at(1).c_str());
        else if (num == "r")
            auto acr = vg.asyncCall(channel, callTimeout, "cmd_run_python_script", any_obj, true);
        else if (num == "b")
            auto acr = vg.asyncCall(channel, callTimeout, "dbg_add_breakpoint", "<string>", std::atoi(result.at(1).c_str()), "1 == 1");
        else if (num == "bn")
            auto acr = vg.asyncCall(channel, callTimeout, "dbg_add_breakpoint", "<string>", std::atoi(result.at(1).c_str()), "1 <= 0");
        else if (num == "d")
            auto acr = vg.asyncCall(channel, callTimeout, "dbg_del_breakpoint", "<string>", std::atoi(result.at(1).c_str()));
        else if (num == "sta") {
            if (auto acr = vg.asyncCall(channel, callTimeout, "dbg_get_state")) {
                veigar::CallResult cr;
                auto waitResult = acr->second.wait_for(std::chrono::milliseconds(callTimeout));
                if (waitResult == std::future_status::timeout) {
                    cr.errCode = veigar::ErrorCode::TIMEOUT;
                    cr.errorMessage = "Timeout";
                }
                else {
                    cr = std::move(acr->second.get());
                }
                if (cr.isSuccess()) {
                    std::cout << cr.obj.get().as<int>() << std::endl;
                }
                else {
                    std::cout << "error" << std::endl;
                }
                vg.releaseCall(acr->first);
            }
        }
        else if (num == "s")
            auto acr = vg.asyncCall(channel, callTimeout, "dbg_step");
        else if (num == "i")
            auto acr = vg.asyncCall(channel, callTimeout, "dbg_into");
        else if (num == "o")
            auto acr = vg.asyncCall(channel, callTimeout, "dbg_out");
        else if (num == "p")
            auto acr = vg.asyncCall(channel, callTimeout, "dbg_pause");
        else if (num == "c")
            auto acr = vg.asyncCall(channel, callTimeout, "dbg_continue");
        else if (num == "q")
            auto acr = vg.asyncCall(channel, callTimeout, "dbg_quit");
        else if (num == "fs") {
            auto cr = vg.syncCall(channel, callTimeout, "dbg_get_framestack");
            if (cr.isSuccess()) {
                auto frame_stack = cr.obj.get().as<std::vector<std::tuple<std::string, std::string, int>>>();
                for (auto& frame : frame_stack) {
                    std::cout << std::get<0>(frame) << " " << std::get<1>(frame) << " " << std::get<2>(frame) << std::endl;
                }
            }
            else {
                std::cout << "error" << std::endl;
            }
        }
        else if (num == "ef")
            ef(0);
        else if (num == "ef1") 
            ef(1);
        else if (num == "ef2")
            ef(2);
        else if (num == "ef3")
            ef(3);
        else if (num == "ef4")
            ef(4);
        else if (num == "ef5")
            ef(5);
        else if (num == "ef6")
            ef(6);
        else if (num == "ef7")
            ef(7);
        else if (num == "ef8")
            ef(8);
        else if (num == "e") {
            auto acr = vg.asyncCall(channel, callTimeout, "exit");
            break;
        }
        else
            std::cout << "error cmd" << std::endl;
    }

    //std::shared_ptr<veigar::AsyncCallResult> acr1 = vg.asyncCall(channel, callTimeout, "dbg_add_breakpoint", "<string>", 3);
    //std::shared_ptr<veigar::AsyncCallResult> acr2 = vg.asyncCall(channel, callTimeout, "cmd_run_python_script", any_obj, true);
    //std::shared_ptr<veigar::AsyncCallResult> acr3 = vg.asyncCall(channel, callTimeout, "dbg_step");
    //std::shared_ptr<veigar::AsyncCallResult> acr4 = vg.asyncCall(channel, callTimeout, "dbg_add_breakpoint", "<string>", 14);
    //std::shared_ptr<veigar::AsyncCallResult> acr5 = vg.asyncCall(channel, callTimeout, "dbg_continue");

    //if (std::shared_ptr<veigar::AsyncCallResult> acr = vg.asyncCall(channel, callTimeout, "exit")) {
    //    if (acr->second.valid()) {
    //        veigar::CallResult cr;
    //        acr->second.wait_for(std::chrono::milliseconds(callTimeout));
    //    }
    //    vg.releaseCall(acr->first);
    //}

    if (vg.isInit()) {
        vg.uninit();
    }
    return 0;
}