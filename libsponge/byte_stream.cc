#include "byte_stream.hh"
// Dummy implementation of a flow-controlled in-memory byte stream.

// For Lab 0, please replace with a real implementation that passes the
// automated checks run by `make check_lab0`.

// You will need to add private members to the class declaration in `byte_stream.hh`

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

ByteStream::ByteStream(const size_t capacity) { DUMMY_CODE(capacity); }

size_t ByteStream::write(const string &data) {
    DUMMY_CODE(data);

    //如果写入数据字节数大于容量减去已经写入或者说是使用的字节数，令其等于容量减去写入队列的字节数
    size_t _byte_length = data.size();
    if(_byte_length>_byte_capacity - _que.size()) _byte_length = _byte_capacity - _que.size();

    //循环放入队列中
    for(size_t i = 0;i<_byte_length;i++){
        _que.push_back(data[i]);
    }

    //计算写入的字节数
    _byte_write+=_byte_length;

    return _byte_length;
}

//! \param[in] len bytes will be copied from the output side of the buffer
string ByteStream::peek_output(const size_t len) const {
    DUMMY_CODE(len);

    size_t _lenght = len;

    //如果将要读取的字节数大于缓冲池中已经存在的字节数，令其等于缓冲池中已经存在的字节数
    if(_lenght>_que.size()) _lenght = _que.size();

    return std::string(_que.begin(),_que.begin()+_lenght);
}

//! \param[in] len bytes will be removed from the output side of the buffer
void ByteStream::pop_output(const size_t len) {
    DUMMY_CODE(len);

    size_t _length = len;

    if(_length>_que.size()) _length = _que.size();

    for(size_t i = 0;i<len;i++) _que.pop_front();

    _byte_read += _length;

}

//! Read (i.e., copy and then pop) the next "len" bytes of the stream
//! \param[in] len bytes will be popped and returned
//! \returns a string
std::string ByteStream::read(const size_t len) {
    DUMMY_CODE(len);

    size_t _length = len;
    if(_length>_que.size()) _length = _que.size();

    std::string result(_que.begin(),_que.begin()+_length);

    pop_output(_length);

    return result;
}

void ByteStream::end_input() {
    _end_input = true;
}

bool ByteStream::input_ended() const {
    return _end_input;
}

size_t ByteStream::buffer_size() const {
    return _que.size();
}

bool ByteStream::buffer_empty() const {
    return _que.empty();
}

bool ByteStream::eof() const {

    return _que.empty() && _end_input;
}

size_t ByteStream::bytes_written() const {
    return _byte_write;
}

size_t ByteStream::bytes_read() const {
    return _byte_read;
}

size_t ByteStream::remaining_capacity() const {
    return _byte_capacity - _que.size();
}
