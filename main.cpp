#include <QLocale>
#include <QTranslator>

#include "common/Options.h"

#include "app.h"
#include "mainwindow.h"

#ifdef __SMART__
    CLogStream __Logger;
//    std::unique_ptr<CLogStream> __Logger( new CLogStream() );
    std::unique_ptr<CAppOption> __AppOption__( new CAppOption() );
    #define __AppOption     (*__AppOption__.get())
#else
    CLogStream __Logger;
    CAppOption __AppOption;
#endif


int main(int argc, char *argv[])
{
    FUNC_TRACE( &__Logger, __PRETTY_FUNCTION__ );
    int done = -1;
    try
    {
        CApp app( argc, argv, &__Logger );

        MainWindow w;
                   w.show();

        done = app.exec();
    }
    catch (const std::exception& ex)
    {
        __Logger.error( APP_LOG_LEVEL, "error: %s", ex.what() );
    }
    catch (const std::string& ex)
    {
        __Logger.error( APP_LOG_LEVEL, "error: %s", ex.c_str() );
    }
    catch (...)
    {
        __Logger.error( APP_LOG_LEVEL, "error: %s", "Unknown exception" );
    }
    return done;
}
