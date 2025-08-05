#ifndef APP_H
#define APP_H

#include <QApplication>

#include "common/Logger.h"

class CApp : public QApplication
           , public CLoggerHolder
{
public :

    CApp( int& argc, char** argv, Logger_t * const pLogger );

    bool notify(QObject* receiver, QEvent* event) override;
};


#endif // APP_H
