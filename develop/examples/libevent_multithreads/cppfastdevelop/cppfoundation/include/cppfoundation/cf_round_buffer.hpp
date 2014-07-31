/*
 * Copyright 2013, Jeffery Qiu. All rights reserved.
 *
 * Licensed under the GNU LESSER GENERAL PUBLIC LICENSE(the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.gnu.org/licenses/lgpl.html
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

//// Author: Jeffery Qiu (dungeonsnd at gmail dot com)
////

#ifndef _HEADER_FILE_CFD_CF_ROUND_BUFFER_HPP_
#define _HEADER_FILE_CFD_CF_ROUND_BUFFER_HPP_

#include "cppfoundation/cf_root.hpp"
#include "cppfoundation/cf_exception.hpp"

namespace cf
{

namespace roundbufferdefs
{
enum
{
    DEFAULT_WATERMARK_HIGH =1025*1024
};
} // namespace socketdefs


class StreamRoundBuffer : public NonCopyable
{
public:

    StreamRoundBuffer(cf_uint32 watermarkHigh
                      =roundbufferdefs::DEFAULT_WATERMARK_HIGH):
        _watermarkHigh(watermarkHigh)
    {
    }
    ~StreamRoundBuffer()
    {
    }

    cf_void Push(cf_cpstr data,cf_uint32 dataLen);
    cf_void Top(std::string & data);
    cf_void Pop(cf_uint32 dataLen);
private:
    cf_uint32 _idxPush;
    cf_uint32 _idxPop;
    cf_uint32 _watermarkHigh;
};


} // namespace cf

#endif // _HEADER_FILE_CFD_CF_ROUND_BUFFER_HPP_
