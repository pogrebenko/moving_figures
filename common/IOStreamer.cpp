#include "IOStreamer.h"

CIOStreamer::CIOStreamer( long Version )
: m_ProjectVersion( Version )
{
  m_Buffer = nullptr;
  m_Buffer_Size = m_Read_Pos = 0;
}

CIOStreamer::CIOStreamer( long Size, long Version )
: m_Buffer_Size( Size )
, m_ProjectVersion( Version )
{
  m_Read_Pos = 0;
  m_Buffer = new char[ Size ];
  ::memset( m_Buffer, 0, Size );
}

CIOStreamer::~CIOStreamer()
{
  if( m_Buffer ) delete [] m_Buffer;
}

void 
CIOStreamer::Write( void *add_ptr, long add_size )
{
    char *del_ptr = m_Buffer;

  m_Buffer  = new char[ m_Buffer_Size + add_size ];
  ::memset( m_Buffer, 0, m_Buffer_Size + add_size ); 

  if( del_ptr ) {
     ::memcpy( m_Buffer, del_ptr, m_Buffer_Size ); 
     delete [] del_ptr;
  }

  char *cur_ptr = &m_Buffer[ m_Buffer_Size ];
  ::memcpy( cur_ptr, add_ptr, add_size ); 

  m_Buffer_Size += add_size;
}

void
CIOStreamer::Read( void *get_ptr, long get_size, long /*version*/ )
{
  if( m_Buffer  == NULL ) return;
  if( m_Read_Pos + get_size > m_Buffer_Size ) throw "Exception in read from stream";

  char *cur_ptr = &m_Buffer[ m_Read_Pos ];

  ::memcpy( get_ptr, cur_ptr, get_size );

  m_Read_Pos += get_size;
}
