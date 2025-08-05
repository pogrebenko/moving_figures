#ifndef LOGGER_H
#define LOGGER_H

#include <stdarg.h>
#include <assert.h>
#include <string>

typedef enum {
    LOG_LEVEL_NONE  = 0,
    LOG_LEVEL_ERROR = 1,
    LOG_LEVEL_WARN  = 2,
    LOG_LEVEL_INFO  = 3,
    LOG_LEVEL_DEBUG = 4,
    LOG_LEVEL_TRACE = 5,
} LogLevel_t;

// remove asserts in release
#ifdef QT_NO_DEBUG
    #define APP_LOG_LEVEL LogLevel_t::LOG_LEVEL_ERROR
    #define NDEBUG
#else
    #define APP_LOG_LEVEL LogLevel_t::LOG_LEVEL_DEBUG
#endif

// определим реализацию логирования для данного проекта
class CLogStream
{
public :

    inline          CLogStream(){}
    inline virtual ~CLogStream(){}

    void trace( LogLevel_t nLL, const char *sFormat, ... )
    {
        if( nLL >= LogLevel_t::LOG_LEVEL_TRACE )
        {
            va_list args;
            va_start( args, sFormat );
            out( sFormat, args );
            va_end( args );
        }
    }

    void debug( LogLevel_t nLL, const char *sFormat, ... )
    {
        if( nLL >= LogLevel_t::LOG_LEVEL_DEBUG )
        {
            va_list args;
            va_start( args, sFormat );
            out( sFormat, args );
            va_end( args );
        }
    }

    void info( LogLevel_t nLL, const char *sFormat, ... )
    {
        if( nLL >= LogLevel_t::LOG_LEVEL_INFO )
        {
            va_list args;
            va_start( args, sFormat );
            out( sFormat, args );
            va_end( args );
        }
    }

    void warn( LogLevel_t nLL, const char *sFormat, ... )
    {
        if( nLL >= LogLevel_t::LOG_LEVEL_WARN )
        {
            va_list args;
            va_start( args, sFormat );
            out( sFormat, args );
            va_end( args );
        }
    }

    void error( LogLevel_t nLL, const char *sFormat, ... )
    {
        if( nLL >= LogLevel_t::LOG_LEVEL_ERROR )
        {
            va_list args;
            va_start( args, sFormat );
            out( sFormat, args );
            va_end( args );
        }
    }

private:

    void out( const char *sFormat, va_list &args )
    {
        ::vprintf( sFormat, args );
        ::printf( "\n" );
        ::fflush( stdout ); // stderr
    }

};

// определим логирование
typedef CLogStream Logger_t;

// определим обертку логирования
class CLoggerHolder
{
public:

    explicit CLoggerHolder()
    : _pLogger( nullptr )
    {
        assert( _pLogger != nullptr );
    }

    inline CLoggerHolder( const CLoggerHolder &c )
    : _pLogger( c._pLogger )
    {
        assert( _pLogger != nullptr );
    }

    inline CLoggerHolder( Logger_t * const pLogger )
    : _pLogger( pLogger )
    {
        assert( _pLogger != nullptr );
    }

    virtual ~CLoggerHolder()
    {
    }

    virtual Logger_t * getLogger()
    {
        assert( _pLogger != nullptr );
        return _pLogger;
    }

protected:
private:

    Logger_t * _pLogger;
};

// трассировка функций
class CFuncTrace : public CLoggerHolder
{
public:

    inline CFuncTrace( Logger_t * const pLogger, const char * pPrettyFuncName )
    : CLoggerHolder( pLogger )
    , _sPrettyFuncName( pPrettyFuncName )
    {
        getLogger()->trace( APP_LOG_LEVEL, "trace: %s -->", _sPrettyFuncName.c_str() );
    }

    inline ~CFuncTrace()
    {
        getLogger()->trace( APP_LOG_LEVEL, "trace: %s <--", _sPrettyFuncName.c_str() );
    }

protected:
private:

    std::string _sPrettyFuncName;
};

#define FUNC_TRACE(logger, name) CFuncTrace __AppTraceInstance__((logger), (name))

#endif // LOGGER_H
