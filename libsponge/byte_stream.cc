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
    //如果缓冲区容量为0，则可以写入的字节数为0
    if(_byte_capacity == 0) return 0;

    //选择较小的一个作为写入字节数的阈值
    size_t _byte_length = data.size();
    if(_byte_length>_byte_capacity) _byte_length = _byte_capacity;

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

    if(_lenght>_que.size()) _lenght = _que.size();

    return string().assign(_que.begin(),_que.begin()+_lenght);
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

    string res = "";

    for(size_t i = 0;i<len;i++){
        res += _que.front();
        _que.pop_front();
    }
    return res;
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
    if(_que.empty()) return true;
    return false;
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
