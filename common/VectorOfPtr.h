#if !defined(_VECTOROFPTR_H__)
#define _VECTOROFPTR_H__

#pragma once

#include <vector>
//#include <functional>
//#include <algorithm>

template <class T>
class CVectorOfPtr : public std::vector<T*>
{
public:
   CVectorOfPtr() : std::vector<T*>(), m_Owner( true )
   {
   }

   virtual ~CVectorOfPtr()
   {
      Flush();
   }

   bool IsOwner (          ) { return m_Owner;       }
   void SetOwner( bool own ) {        m_Owner = own; }

   void Delete( unsigned long i )
   {
        if( m_Owner )
        {
            delete this->at( i );
        }
        this->erase( this->begin() + i );
   }   

   void Flush()
   {
        if( m_Owner )
        {
            for( auto p : *this )
            {
                delete p;
            }
        }
        this->clear();
   }

private :
    bool m_Owner;
};

#endif
