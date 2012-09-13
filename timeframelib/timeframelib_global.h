#include <QtCore/QtGlobal>

#if defined(TIMEFRAMELIB_LIBRARY)
#  define TIMEFRAMELIB_EXPORT Q_DECL_EXPORT
#else
#  define TIMEFRAMELIB_EXPORT Q_DECL_IMPORT
#endif
