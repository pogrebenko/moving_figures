//
// $Id: compare.h,v 1.0.0.0 2010-12-19 12:03:59 unt Exp $
//
#ifndef COMPARE_H
#define COMPARE_H

#include <strings.h>
#include <string>
//#include "search/Types.h"

inline int Cmp( const int &o1, const int &o2 )
{
  return ( ( o1 == o2 ) ? 0 : ( ( o1 > o2 ) ? 1 : -1 ) );
}

inline int Cmp( const unsigned int &o1, const unsigned int &o2 )
{
  return ( ( o1 == o2 ) ? 0 : ( ( o1 > o2 ) ? 1 : -1 ) );
}

inline int Cmp( const long &o1, const long &o2 )
{
  return ( ( o1 == o2 ) ? 0 : ( ( o1 > o2 ) ? 1 : -1 ) );
}

inline int Cmp( const unsigned long &o1, const unsigned long &o2 )
{
  return ( ( o1 == o2 ) ? 0 : ( ( o1 > o2 ) ? 1 : -1 ) );
}

inline int Cmp( const double &o1, const double &o2 )
{
  return ( ( o1 == o2 ) ? 0 : ( ( o1 > o2 ) ? 1 : -1 ) );
}

inline int Cmp( const char *o1, const char *o2 )
{
	return strcasecmp( o1, o2 );
}

inline int Cmp( std::string &o1, std::string &o2 )
{
	return strcasecmp( o1.c_str(), o2.c_str() );
}

// inline int Cmp( const tm64 &o1, const tm64 &o2 )
// {
//   int      ret = Cmp( o1.tm_year, o2.tm_year );
//   if(!ret) ret = Cmp( o1.tm_mon , o2.tm_mon  );
//   if(!ret) ret = Cmp( o1.tm_mday, o2.tm_mday );
//   if(!ret) ret = Cmp( o1.tm_hour, o2.tm_hour );
//   if(!ret) ret = Cmp( o1.tm_min , o2.tm_min  );
//   if(!ret) ret = Cmp( o1.tm_sec , o2.tm_sec  );
//   if(!ret) ret = Cmp( o1.tm_usec, o2.tm_usec );
//   return ret;
// }

template <class T>
class CComparePtr
{
public :

	CComparePtr( int aDir = 1 ) 
	: iDir( aDir ) 
	{}

	CComparePtr( const CComparePtr &o ) 
	{
		*this = o;
	}

	virtual ~CComparePtr(){}

	virtual int Compare( T *o1, T *o2 ) = 0;

	bool operator () ( T *o1, T *o2 )
	{
		int res = Compare( o1, o2 ); 

		if( iDir == 1 )  return res < 0; 
		else             return res > 0;  
	}

	inline CComparePtr& operator = ( const CComparePtr &c )
	{
		iDir = c.iDir;
		return *this;
	}

	int  Direction() { return iDir; }

	void Direction( int aDir ) { iDir = aDir; }

	int iDir;
};

#endif
