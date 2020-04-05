#include "WiFiTermBuffer.h"

WiFiTermBuffer::WiFiTermBuffer(size_t size)
{
  _size = size;
  _buffer = (char *) malloc(_size);
  reset();
}

WiFiTermBuffer::~WiFiTermBuffer()
{
  free(_buffer);
}

void WiFiTermBuffer::reset()
{
  _ix_read = 0;
  _ix_write = 0;
  _available = 0;
  _previous = 0;
}

size_t WiFiTermBuffer::write(uint8_t c)
{
  _buffer[_ix_write++] = c;
  if (_ix_write == _size) _ix_write = 0;
  if (_available < _size) _available++;
  if (_available + _previous > _size) _previous = _size - _available;
  _ix_read = (_ix_write >= _available) ? 0 : _size;
  _ix_read += _ix_write;
  _ix_read -= _available;
  return 1;
}

int WiFiTermBuffer::read()
{
  if (_available == 0) return -1;
  char c = _buffer[_ix_read++];
  if (_ix_read == _size) _ix_read = 0;
  _available--;
  _previous++;
  return c;
}

void WiFiTermBuffer::flush()
{
  _ix_read = _ix_write;
  _previous += _available;
  _available = 0;
}

int WiFiTermBuffer::peek() const
{
  if (_available == 0) return -1;
  char c = _buffer[_ix_read];
  return c;
}

void WiFiTermBuffer::peekPrevious(char *to) const
{
  size_t len = _previous;
  size_t ix = (_ix_read >= _previous) ? 0 : _size;
  ix += _ix_read;
  ix -= _previous;
  while (len--)
  {
    *to++ = _buffer[ix++];
    if (ix == _size) ix = 0;
  }
}
