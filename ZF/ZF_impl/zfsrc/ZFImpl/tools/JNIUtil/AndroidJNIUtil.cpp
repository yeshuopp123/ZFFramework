#include "AndroidJNIUtil.h"
#if (defined(ANDROID) || defined(__ANDROID__))

namespace AndroidJNIUtil {

std::string &_AndroidLogTagDefault(void)
{
    static std::string _s = "JNI log";
    return _s;
}
void AndroidLogTagDefaultSet(const char *s)
{
    _AndroidLogTagDefault() = ((s == NULL) ? "" : s);
}

}

#endif
