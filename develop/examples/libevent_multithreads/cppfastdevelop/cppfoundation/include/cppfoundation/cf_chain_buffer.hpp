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

#ifndef _HEADER_FILE_CFD_CF_CHAIN_BUFFER_HPP_
#define _HEADER_FILE_CFD_CF_CHAIN_BUFFER_HPP_

#include "cppfoundation/cf_root.hpp"
#include "cppfoundation/cf_exception.hpp"

namespace cf
{

namespace chainbufferdefs
{
enum
{   
    CB_SIZE_INIT =0,
    CB_SIZE_NORMAL =(1024*2+128) ,
    CB_SIZE_LARGE =(1024*32+128),
    CB_SIZE_MAX =(1024*512+128),
    CB_SIZE_PREAPPENDABLE =16
};
} // namespace socketdefs


class ChainBuffer : public NonCopyable
{
public:

    ChainBuffer(cf_int32 initSize =chainbufferdefs::CB_SIZE_INIT,
                cf_int32 normalSize =chainbufferdefs::CB_SIZE_NORMAL,
                cf_int32 largeSize =chainbufferdefs::CB_SIZE_LARGE,
                cf_int32 maxSize =chainbufferdefs::CB_SIZE_MAX ):
        _initSize(initSize),_normalSize(normalSize),
        _largeSize(largeSize),_maxSize(maxSize),_currentSize(0)
    {
        if(_initSize>chainbufferdefs::CB_SIZE_MAX)
            _THROW_FMT(ValueError, "_initSize{%u}>CB_SIZE_MAX{%u} !",_initSize,chainbufferdefs::CB_SIZE_MAX);
        if(_initSize>0)
        {
            _data.assign(_initSize,'\0');
			_currentSize =_initSize;
        }
    }
    ~ChainBuffer()
    {
    }

    inline cf_int32 UsedSize() const
    {
        return _writePos-_readPos;
    }
    inline cf_int32 LeftSize() const
    {
        return _currentSize-_writePos+_readPos;
    }
    
    //read
    cf_void Read(std::string & output, cf_int32 & in_out_bufLen)
    {
		if(in_out_bufLen>0)
		{
			output.assign((cf_char*)&_data[0]+_readPos,in_out_bufLen);
		}
        return ;
    }
    cf_void Remove(const cf_int32 dataLen)
    {
		_readPos +=dataLen;
    }
    
    // write
    cf_void Write(const cf_byte * buf, const cf_int32 dataLen)
    {
		if(dataLen<LeftSize())
		{
			if(_currentSize-_writePos+1>=dataLen) // write directly !
			{
				_writePos+=dataLen;
			}
			else// move , and then write.
			{
				memmove(&_data[0],(cf_char*)&_data[0]+_readPos,_writePos-_readPos);
				_writePos =_writePos-_readPos;
				_readPos =0;				
			}
		}
		else if(_currentSize==_initSize)
		{
		}
		else if(_currentSize==_normalSize)
		{
		}
		else if(_currentSize==_largeSize)
		{
		}
		else if(_currentSize==_maxSize)
		{
		}
		else
		{
			return;
		}
    }
private:

    std::string _data;
    cf_int32 _writePos;
    cf_int32 _readPos;
    
    cf_int32 _initSize;
    cf_int32 _normalSize;
    cf_int32 _largeSize;
    cf_int32 _maxSize;
    cf_int32 _currentSize;
};


} // namespace cf

#endif // _HEADER_FILE_CFD_CF_CHAIN_BUFFER_HPP_
