#include <cstdio>
#include <cxxabi.h>
#include "Exception.hpp"
#include "StringTokenizer.hpp"

namespace io {

  void Exception::printStackTrace() const {
    fprintf(stderr, "Stack Trace:  \n");
    if (frameCount > 0) {
#ifdef __APPLE__
      char** frameStrs = backtrace_symbols(frameList, frameCount);
      /*
       * We start at 2 because the stack trace information is obtained at
       * the instantiation of the exception.  This results in extra
       * entries in the stack trace that are of no relevance.
       */
      for (uint32_t i = 2; i < frameCount; i++) {
        StringTokenizer st;
        st.tokenize(frameStrs[i], ' ');

        char buffer[1024];
        memset(buffer, 0, 1024);
        int result = 0;
        size_t size = 1024;
        abi::__cxa_demangle(st.getToken(3).c_str(), buffer, &size, &result);
        switch (result) {
        case -2:
          fprintf(stderr, "%-30s %s %s()\n",
                  st.getToken(1).c_str(), st.getToken(2).c_str(),
                  st.getToken(3).c_str());
          break;
        case 0:
          fprintf(stderr, "%-30s %s %s\n", st.getToken(1).c_str(),
                  st.getToken(2).c_str(), buffer);
          break;
        default:
          break;
        }
      }
#elif !__APPLE__

#else

#endif
    }
  }
}