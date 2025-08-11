#if !defined(_VECTOROFPTR_H__)
#define _VECTOROFPTR_H__

#pragma once

#include <vector>

//#include <functional>
#include <algorithm>
#include <execution>

#include "Compare.h"

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

   inline bool IsOwner (          ) { return m_Owner;       }
   inline void SetOwner( bool own ) {        m_Owner = own; }

   inline void Delete( unsigned long i )
   {
        if( m_Owner )
        {
            delete this->at( i );
        }
        this->erase( this->begin() + i );
   }   

   inline void Flush()
   {
        if( m_Owner )
        {
            for_each( this->begin(), this->end(), []( auto p ) { delete p; } );
            //for( auto p : *this ) { delete p; }
        }
        this->clear();
   }

   inline long bindexof( const T &aRef, CComparePtr<T> *aCmp, long *pInsertIndex = nullptr )
   {
        long nInsertIndex = -1;

        auto pItem = std::lower_bound( std::execution::par, this->begin(), this->end(), aRef, *aCmp );
        nInsertIndex = std::distance( this->begin(), pItem );

        if( pInsertIndex != nullptr )
            *pInsertIndex = nInsertIndex;

        if( pItem != this->end() )
        {
            if( !(*aCmp)( aRef, *pItem ) == 0 )
                return nInsertIndex;
        }

        return -1;
   }

   inline void sort( CComparePtr<T> &aCmp )
   {
        std::sort( std::execution::par, this->begin(), this->end(), aCmp );
   }

private :

    bool m_Owner;
};

#endif
