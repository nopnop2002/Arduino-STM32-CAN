//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------

#include "ACANFD_STM32_CANFDMessage.h"
#include "ACANFD_STM32_DynamicArray.h"

//------------------------------------------------------------------------------

enum class ACANFD_STM32_FilterAction {
  FIFO0 = 0,
  FIFO1 = 1,
  REJECT = 2
} ;

//------------------------------------------------------------------------------
//    Standard filters
//------------------------------------------------------------------------------

class ACANFD_STM32_StandardFilters {
//--- Default constructor
  public: ACANFD_STM32_StandardFilters (void) { }

//--- Append filter
  public: bool addSingle (const uint16_t inIdentifier,
                          const ACANFD_STM32_FilterAction inAction,
                          const ACANFDCallBackRoutine inCallBack = nullptr) ;

  public: bool addDual (const uint16_t inIdentifier1,
                        const uint16_t inIdentifier2,
                        const ACANFD_STM32_FilterAction inAction,
                        const ACANFDCallBackRoutine inCallBack = nullptr) ;

  public: bool addRange (const uint16_t inIdentifier1,
                         const uint16_t inIdentifier2,
                         const ACANFD_STM32_FilterAction inAction,
                         const ACANFDCallBackRoutine inCallBack = nullptr) ;

  public: bool addClassic (const uint16_t inIdentifier,
                           const uint16_t inMask,
                           const ACANFD_STM32_FilterAction inAction,
                           const ACANFDCallBackRoutine inCallBack = nullptr) ;

//--- Access
  public: uint32_t count () const {
    return mFilterArray.count () ;
  }

  public: uint32_t filterAtIndex (const uint32_t inIndex) const {
    return mFilterArray [inIndex] ;
  }

  public: ACANFDCallBackRoutine callBackAtIndex (const uint32_t inIndex) const {
    return mCallBackArray [inIndex] ;
  }

//--- Private properties
  private: ACANFD_STM32_DynamicArray <uint32_t> mFilterArray ;
  private: ACANFD_STM32_DynamicArray < ACANFDCallBackRoutine > mCallBackArray ;

//--- No copy
  private : ACANFD_STM32_StandardFilters (const ACANFD_STM32_StandardFilters &) = delete ;
  private : ACANFD_STM32_StandardFilters & operator = (const ACANFD_STM32_StandardFilters &) = delete ;
} ;

//------------------------------------------------------------------------------
//    Extended filters
//------------------------------------------------------------------------------

class ACANFD_STM32_ExtendedFilters {
//--- Default constructor
  public: ACANFD_STM32_ExtendedFilters (void) { }

//--- Append filter
  public: bool addSingle (const uint32_t inExtendedIdentifier,
                          const ACANFD_STM32_FilterAction inAction,
                          const ACANFDCallBackRoutine inCallBack = nullptr) ;

  public: bool addDual (const uint32_t inExtendedIdentifier1,
                        const uint32_t inExtendedIdentifier2,
                        const ACANFD_STM32_FilterAction inAction,
                        const ACANFDCallBackRoutine inCallBack = nullptr) ;

  public: bool addRange (const uint32_t inExtendedIdentifier1,
                         const uint32_t inExtendedIdentifier2,
                         const ACANFD_STM32_FilterAction inAction,
                         const ACANFDCallBackRoutine inCallBack = nullptr) ;

  public: bool addClassic (const uint32_t inExtendedIdentifier,
                           const uint32_t inExtendedMask,
                           const ACANFD_STM32_FilterAction inAction,
                           const ACANFDCallBackRoutine inCallBack = nullptr) ;

//--- Access
  public: uint32_t count () const { return mCallBackArray.count () ; }
  public: uint32_t firstWordAtIndex (const uint32_t inIndex) const { return mFilterArray [inIndex * 2] ; }
  public: uint32_t secondWordAtIndex (const uint32_t inIndex) const { return mFilterArray [inIndex * 2 + 1] ; }
  public: ACANFDCallBackRoutine callBackAtIndex (const uint32_t inIndex) const {
    return mCallBackArray [inIndex] ;
  }

//--- Private properties
  private: ACANFD_STM32_DynamicArray <uint32_t> mFilterArray ;
  private: ACANFD_STM32_DynamicArray < void (*) (const CANFDMessage & inMessage) > mCallBackArray ;

//--- No copy
  private : ACANFD_STM32_ExtendedFilters (const ACANFD_STM32_ExtendedFilters &) = delete ;
  private : ACANFD_STM32_ExtendedFilters & operator = (const ACANFD_STM32_ExtendedFilters &) = delete ;
} ;


//------------------------------------------------------------------------------
