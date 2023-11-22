/*******************************************************************************
*    Veigar: Cross platform RPC library using shared memory.
*    ---------------------------------------------------------------------------
*    Copyright (C) 2023 winsoft666 <winsoft666@outlook.com>.
*
*    This program is free software: you can redistribute it and/or modify
*    it under the terms of the GNU General Public License as published by
*    the Free Software Foundation, either version 3 of the License, or
*    (at your option) any later version.
*
*    This program is distributed in the hope that it will be useful,
*    but WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*    GNU General Public License for more details.
*
*    You should have received a copy of the GNU General Public License
*    along with this program.  If not, see <http://www.gnu.org/licenses/>.
******************************************************************************/
#ifndef VEIGAR_H_
#define VEIGAR_H_
#pragma once

#include <string>
#include <vector>
#include <future>
#include "veigar/config.h"
#include "veigar/call_result.h"
#include "veigar/dispatcher.h"
#include "veigar/detail/meter.h"

namespace veigar {
class Veigar {
   public:
    Veigar() noexcept;

    Veigar(Veigar&& other) noexcept;

    virtual ~Veigar();

    Veigar& operator=(Veigar&& other) noexcept;

    bool init(const std::string& channelName, unsigned int defaultBufferSize = 1048576) noexcept;

    bool isInit() const noexcept;

    void uninit() noexcept;

    std::string channelName() const noexcept;

    template <typename F>
    bool bind(const std::string& funcName, F func) noexcept;

    void unbind(const std::string& funcName) noexcept;

    std::vector<std::string> bindNames() const noexcept;

    template <typename... Args>
    std::shared_ptr<AsyncCallResult> asyncCall(
        const std::string& targetChannel,
        const std::string& funcName,
        Args... args) noexcept;

    // After obtaining the results, must call releaseCall to release resources
    void releaseCall(const std::string& callId);

    template <typename... Args>
    CallResult syncCall(
        const std::string& targetChannel,
        unsigned int timeoutMS,
        const std::string& funcName,
        Args... args) noexcept;

    void waitAllResponse() noexcept;

    bool sendMessage(
        const std::string& targetChannel,
        const uint8_t* buf,
        size_t bufSize,
        unsigned int timeoutMS,
        std::string& errMsg) noexcept;

   private:
    std::string getNextCallId(const std::string& funcName) const noexcept;

    // std::promise will not set_exception forever
    template <typename... Args>
    std::shared_ptr<AsyncCallResult> doAsyncCall(
        const std::string& targetChannel,
        const std::string& funcName,
        Args... args) noexcept;

    bool sendCall(
        const std::string& channelName,
        std::shared_ptr<veigar_msgpack::sbuffer> buffer,
        const std::string& callId,
        const std::string& funcName,
        std::shared_ptr<std::promise<CallResult>> p,
        std::string& errMsg) noexcept;

   private:
    class Impl;
    Impl* impl_ = nullptr;
    std::shared_ptr<detail::Dispatcher> disp_;
};
}  // namespace veigar

#include "veigar.inl"

#endif  // !VEIGAR_H_