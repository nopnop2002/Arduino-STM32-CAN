//------------------------------------------------------------------------------

#include "ACANFD_STM32_Filters.h"

//------------------------------------------------------------------------------
//    Standard filters
//------------------------------------------------------------------------------

bool ACANFD_STM32_StandardFilters::addSingle (const uint16_t inIdentifier,
                                               const ACANFD_STM32_FilterAction inAction,
                                               const ACANFDCallBackRoutine inCallBack) {
  return addDual (inIdentifier, inIdentifier, inAction, inCallBack) ;
}

//------------------------------------------------------------------------------

bool ACANFD_STM32_StandardFilters::addDual (const uint16_t inIdentifier1,
                                             const uint16_t inIdentifier2,
                                             const ACANFD_STM32_FilterAction inAction,
                                             const ACANFDCallBackRoutine inCallBack) {
  const bool ok = (inIdentifier1 <= 0x7FF) && (inIdentifier2 <= 0x7FF) ;
  if (ok) {
    uint32_t filter = inIdentifier2 ;
    filter |= uint32_t (inIdentifier1) << 16 ;
    filter |= (1U << 30) ; // Dual filter
    filter |= ((uint32_t (inAction) + 1) << 27) ; // Filter action
    mFilterArray.append (filter) ;
    mCallBackArray.append (inCallBack) ;
  }
  return ok ;
}

//------------------------------------------------------------------------------

bool ACANFD_STM32_StandardFilters::addRange (const uint16_t inIdentifier1,
                                              const uint16_t inIdentifier2,
                                              const ACANFD_STM32_FilterAction inAction,
                                              const ACANFDCallBackRoutine inCallBack) {
  const bool ok = (inIdentifier1 <= inIdentifier2) && (inIdentifier2 <= 0x7FF) ;
  if (ok) {
    uint32_t filter = inIdentifier2 ;
    filter |= uint32_t (inIdentifier1) << 16 ;
    filter |= ((uint32_t (inAction) + 1) << 27) ; // Filter action
    mFilterArray.append (filter) ;  // Filter type is 0 (RANGE)
    mCallBackArray.append (inCallBack) ;
  }
  return ok ;
}

//------------------------------------------------------------------------------

bool ACANFD_STM32_StandardFilters::addClassic (const uint16_t inIdentifier,
                                                const uint16_t inMask,
                                                const ACANFD_STM32_FilterAction inAction,
                                                const ACANFDCallBackRoutine inCallBack) {
  const bool ok = (inIdentifier <= 0x7FF)
               && (inMask <= 0x7FF)
               && ((inIdentifier & inMask) == inIdentifier) ;
  if (ok) {
    uint32_t filter = inMask ;
    filter |= uint32_t (inIdentifier) << 16 ;
    filter |= (2U << 30) ; // Classic filter
    filter |= ((uint32_t (inAction) + 1) << 27) ; // Filter action
    mFilterArray.append (filter) ;
    mCallBackArray.append (inCallBack) ;
  }
  return ok ;
}

//------------------------------------------------------------------------------
//    Extended filters
//------------------------------------------------------------------------------

static const uint32_t MAX_EXTENDED_IDENTIFIER = 0x1FFFFFFF ;

//------------------------------------------------------------------------------

bool ACANFD_STM32_ExtendedFilters::addSingle (const uint32_t inIdentifier,
                                               const ACANFD_STM32_FilterAction inAction,
                                               const ACANFDCallBackRoutine inCallBack) {
  return addDual (inIdentifier, inIdentifier, inAction, inCallBack) ;
}

//------------------------------------------------------------------------------

bool ACANFD_STM32_ExtendedFilters::addDual (const uint32_t inIdentifier1,
                                             const uint32_t inIdentifier2,
                                             const ACANFD_STM32_FilterAction inAction,
                                             const ACANFDCallBackRoutine inCallBack) {
  const bool ok = (inIdentifier1 <= MAX_EXTENDED_IDENTIFIER)
               && (inIdentifier2 <= MAX_EXTENDED_IDENTIFIER) ;
  if (ok) {
    uint32_t filter = inIdentifier1 ;
    filter |= ((uint32_t (inAction) + 1) << 29) ; // Filter action
    mFilterArray.append (filter) ;
    filter = inIdentifier2 ;
    filter |= (1U << 30) ; // Dual filter
    mFilterArray.append (filter) ;
    mCallBackArray.append (inCallBack) ;
  }
  return ok ;
}

//------------------------------------------------------------------------------

bool ACANFD_STM32_ExtendedFilters::addRange (const uint32_t inIdentifier1,
                                              const uint32_t inIdentifier2,
                                              const ACANFD_STM32_FilterAction inAction,
                                              const ACANFDCallBackRoutine inCallBack) {
  const bool ok = (inIdentifier1 <= inIdentifier2)
               && (inIdentifier2 <= MAX_EXTENDED_IDENTIFIER) ;
  if (ok) {
    uint32_t filter = inIdentifier1 ; // Filter type is 0 (RANGE)
    filter |= ((uint32_t (inAction) + 1) << 29) ; // Filter action
    mFilterArray.append (filter) ;
    filter = inIdentifier2 ;
    mFilterArray.append (filter) ;  // Filter type is 0 (RANGE)
    mCallBackArray.append (inCallBack) ;
  }
  return ok ;
}

//------------------------------------------------------------------------------

bool ACANFD_STM32_ExtendedFilters::addClassic (const uint32_t inIdentifier,
                                                const uint32_t inMask,
                                                const ACANFD_STM32_FilterAction inAction,
                                                const ACANFDCallBackRoutine inCallBack) {
  const bool ok = (inIdentifier <= MAX_EXTENDED_IDENTIFIER)
               && (inMask <= MAX_EXTENDED_IDENTIFIER)
               && ((inIdentifier & inMask) == inIdentifier) ;
  if (ok) {
    uint32_t filter = inIdentifier ;
    filter |= ((uint32_t (inAction) + 1) << 29) ; // Filter action
    mFilterArray.append (filter) ;
    filter = inMask ;
    filter |= (2U << 30) ; // Classic filter
    mFilterArray.append (filter) ;
    mCallBackArray.append (inCallBack) ;
  }
  return ok ;
}

//------------------------------------------------------------------------------
