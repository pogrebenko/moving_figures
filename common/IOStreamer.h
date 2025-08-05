#ifndef IOSTREAMER_H
#define IOSTREAMER_H

#include <cstring>

class CIOStreamer
{
public:
    CIOStreamer( long version );
    CIOStreamer( long size, long version );
    virtual ~CIOStreamer();

    void Write( void *add_ptr, long add_size );
    void Read ( void *get_ptr, long get_size, long version = 0 );

    char *m_Buffer;
    long  m_Buffer_Size, m_Read_Pos, m_ProjectVersion;
};

template<class T>
inline
    void __Write( CIOStreamer &stream, T &obj )
{
    stream.Write( &obj  , sizeof(T) );
}

template<class T>
inline
    T& __Read( CIOStreamer &stream, T &obj )
{
    stream.Read( &obj  , sizeof(T) );
    return obj;
}

#define MARK_LEFT  '<'
#define MARK_RIGHT '>'

template<class T>
inline
    void Write( CIOStreamer &stream, T &obj )
{
    char Left = MARK_LEFT, Right = MARK_RIGHT;
    stream.Write( &Left , sizeof( char ) );
    __Write<T>( stream, obj );
    stream.Write( &Right, sizeof( char ) );
}

template<class T>
inline
    T& Read( CIOStreamer &stream, T &obj )
{
    char Left = 0, Right = 0;
    stream.Read( &Left , sizeof( char ) ); if( Left  != MARK_LEFT  ) throw "Exception in read from stream";
    __Read<T>( stream, obj );
    stream.Read( &Right, sizeof( char ) ); if( Right != MARK_RIGHT ) throw "Exception in read from stream";
    return obj;
}

template<class T>
inline void WriteInt( CIOStreamer &ios, T &obj )
{
    int type = (int)obj;
    ::Write( ios, type );
}

template<class T>
inline T& ReadInt( CIOStreamer &ios, T &obj )
{
    int type = 0; ::Read( ios, type );
    obj = (T)type;
    return obj;
}

#endif
