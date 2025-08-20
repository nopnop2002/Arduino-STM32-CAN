//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------

#include <stdint.h>

//------------------------------------------------------------------------------
//    Dynamic Array
//------------------------------------------------------------------------------

template <typename T> class ACANFD_STM32_DynamicArray {
//--- Default constructor
  public: ACANFD_STM32_DynamicArray (void) { }

//--- Destructor
  public: ~ ACANFD_STM32_DynamicArray (void) { delete [] mArray ; }

//--- Append
  public: void append (const T & inObject) {
    if (mCapacity == mCount) {
      mCapacity += 64 ;
      T * newArray = new T [mCapacity] ;
      for (uint32_t i=0 ; i<mCount ; i++) {
        newArray [i] = mArray [i] ;
      }
      delete [] mArray ;
      mArray = newArray ;
    }
    mArray [mCount] = inObject ;
    mCount += 1 ;
  }

//--- Methods
  public: void release (void) {
    delete [] mArray ;
    mArray = nullptr ;
    mCount = 0 ;
    mCapacity = 0 ;
  }

  public: void setCapacity (const uint32_t inNewCapacity) {
    if (mCapacity < inNewCapacity) {
      mCapacity = inNewCapacity ;
      T * newArray = new T [mCapacity] ;
      for (uint32_t i=0 ; i<mCount ; i++) {
        newArray [i] = mArray [i] ;
      }
      delete [] mArray ;
      mArray = newArray ;
    }
  }

//--- Access
  public: uint32_t count () const { return mCount ; }
  public: T operator [] (const uint32_t inIndex) const { return mArray [inIndex] ; }

//--- Private properties
  private: uint8_t mCapacity = 0 ;
  private: uint8_t mCount = 0 ;
  private: T * mArray = nullptr ;

//--- No copy
  private : ACANFD_STM32_DynamicArray (const ACANFD_STM32_DynamicArray &) = delete ;
  private : ACANFD_STM32_DynamicArray & operator = (const ACANFD_STM32_DynamicArray &) = delete ;
} ;

//------------------------------------------------------------------------------
