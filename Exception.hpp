#ifndef ExceptionHPP
#define ExceptionHPP

#include <execinfo.h>
#include <string>
#include <cstring> //  For memcpy.

namespace io {

  class Exception {
  public:
    Exception(const std::string& message) {
      this->message = message;
#ifdef __APPLE__
      frameCount = backtrace(frameList, Exception::MAX_FRAMES);
#else
      frameCount = backtrace(frameList, Exception::MAX_FRAMES);
#endif
    }

    Exception(const Exception& rhs) {
      message = rhs.message;
      frameCount = rhs.frameCount;
      ::memcpy(frameList, rhs.frameList, sizeof(void*) * frameCount);
    }
    
    Exception& operator=(const Exception& rhs) {
      if (this != &rhs) {
        message = rhs.message;
        frameCount = rhs.frameCount;
        ::memcpy(frameList, rhs.frameList, sizeof(void*) * frameCount);
      }
      
      return *this;
    }
    
    virtual ~Exception() { }

    virtual std::string getMessage() const {
      return message;
    }

    void printStackTrace() const;
  private:
    const static unsigned int MAX_FRAMES = 256;
    
    uint32_t frameCount;
    void* frameList[Exception::MAX_FRAMES];
    std::string message;
  };
}

#endif
