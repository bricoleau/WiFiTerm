#ifndef WiFiTermBuffer_h
#define WiFiTermBuffer_h

//circular buffer for WiFiTerm

#include <Arduino.h>

class WiFiTermBuffer
{
  private :
    char *_buffer;
    size_t _size, _ix_read, _ix_write, _available, _previous;

  public :
    WiFiTermBuffer(size_t);
    ~WiFiTermBuffer();

    void reset();

    size_t write(uint8_t);
    int read();
    int peek() const;
    void flush();
    inline int available() const {return _available;}

    inline size_t size()       const {return _size;}
    inline bool   isEmpty()    const {return _available == 0;}
    inline bool   isNotEmpty() const {return _available != 0;}
    inline bool   isFull()     const {return _available == _size;}
    inline bool   isNotFull()  const {return _available != _size;}

    inline size_t previousAvailable() const {return _previous;}
    void peekPrevious(char *to) const;//peek only previously read char
};

#endif
