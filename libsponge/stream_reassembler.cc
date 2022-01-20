
#include <cassert>
#include "stream_reassembler.hh"

// Dummy implementation of a stream reassembler.

// For Lab 1, please replace with a real implementation that passes the
// automated checks run by `make check_lab1`.

// You will need to add private members to the class declaration in `stream_reassembler.hh`

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

StreamReassembler::StreamReassembler(const size_t capacity)
    : _unassembled_strs()
    , _unassembled_bytes(0)
    , _next_unassembled_index(0)
    , _eof_index(-1)
    , _output(capacity)
    , _capacity(capacity) {}


//! \details This function accepts a substring (aka a segment) of bytes,
//! possibly out-of-order, from the logical stream, and assembles any newly
//! contiguous substrings and writes them into the output stream in order.
void StreamReassembler::push_substring(const string &data, const size_t index, const bool eof) {
    DUMMY_CODE(data, index, eof);

    auto pos_iter = _unassembled_strs.upper_bound(index);

    if(pos_iter!=_unassembled_strs.begin())
        pos_iter--;

    size_t new_index = index;
    if(pos_iter != _unassembled_strs.end() && pos_iter->first <= index){
        const size_t up_index = pos_iter->first;

        if(index < up_index + pos_iter->second.size())
            new_index = up_index + pos_iter->second.size();
    }
    else if(index < _next_unassembled_index)
        new_index = _next_unassembled_index;

    const size_t data_start_pos = new_index - index;

    ssize_t data_size = data.size() - data_start_pos;

    pos_iter = _unassembled_strs.lower_bound(new_index);

    while(pos_iter != _unassembled_strs.end() && new_index <= pos_iter->first){
        const size_t data_end_pos = new_index + data_size;

        if(pos_iter->first < data_end_pos){

            if(data_end_pos < pos_iter->first + pos_iter->second.size()){
                data_size = pos_iter->first - new_index;
                break;
            }
            else{
                _unassembled_bytes -= pos_iter->second.size();
                pos_iter = _unassembled_strs.erase(pos_iter);
                continue;
            }
        }

        else
            break;

        size_t first_unacceptable_index = _next_unassembled_index + _capacity - _output.buffer_size();
        if(first_unacceptable_index <= new_index)
            return;

        if(data_size>0){
            const string new_data = data.substr(data_start_pos,data_size);

            if(new_index == _next_unassembled_index){
                const size_t write_byte = _output.write(new_data);
                _next_unassembled_index += write_byte;

                if(write_byte < new_data.size()){
                    const string data_to_store = new_data.substr(write_byte,new_data.size() - write_byte);
                    _unassembled_bytes += data_to_store.size();
                    _unassembled_strs.insert(make_pair(_next_unassembled_index,std::move(data_to_store)));
                }
            }

            for(auto iter = _unassembled_strs.begin(); iter != _unassembled_strs.end();){
                assert(_next_unassembled_index <= iter->first);

                if(iter->first == _next_unassembled_index){
                    const size_t write_num = _output.write(iter->second);
                    _next_unassembled_index += write_num;

                    if(write_num < iter->second.size()){
                        _unassembled_bytes += iter->second.size() - write_num;
                        _unassembled_strs.insert(make_pair(_next_unassembled_index,iter->second.substr(write_num)));

                        _unassembled_bytes -= iter->second.size();
                        _unassembled_strs.erase(iter);
                        break;
                    }

                    _unassembled_bytes -= iter->second.size();
                    iter = _unassembled_strs.erase(iter);
                }

                else
                    break;
            }

            if(eof)
                _eof_index = index + data.size();
            if(_eof_index <= _next_unassembled_index)
                _output.end_input();
        }
    }
}

size_t StreamReassembler::unassembled_bytes() const { return _unassembled_bytes; }

bool StreamReassembler::empty() const { return _unassembled_bytes == 0; }
