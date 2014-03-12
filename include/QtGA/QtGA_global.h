#ifndef QTGA_CONFIG_H
#define QTGA_CONFIG_H

#ifdef Q_OS_WIN
    #ifdef BUILD_SHARED
        #ifdef QT_GA_BUILD
            #define QT_GA_EXPORTS Q_DECL_EXPORT
        #else
            #define QT_GA_EXPORTS Q_DECL_IMPORT
        #endif
    #else
        #define QT_GA_EXPORTS
    #endif
#else
    #define QT_GA_EXPORTS
#endif

#endif // QTGA_CONFIG_H
