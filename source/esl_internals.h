////////////////////////////////////////////////////////////////////////////////
/// \copyright    Embedded Systems Library - Copyright (c) 2018 Fabian Zahn
///               [Released under MIT License. See LICENSE for details]
///
/// \file         esl_internals.h
/// \brief        Header file for internal purposes such as the default config.
////////////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------------
/// Standard Includes
//------------------------------------------------------------------------------

#include <stddef.h>   // NULL macro
#include <stdint.h>   // (u)int_{x}t
#include <stdbool.h>  // boolean types

//------------------------------------------------------------------------------
/// User Configuration
//------------------------------------------------------------------------------

/// \brief Define ESL_INCLUDE_CONFIG_H globally to include the user configuration
#ifdef ESL_INCLUDE_CONFIG_H
  #include "esl_config.h"
#endif

//------------------------------------------------------------------------------
/// Default Assignments
//------------------------------------------------------------------------------

/// \brief Default assignment of the lock acquisition (e.g. enter critical section)
#ifndef ESL_CFG_ACQUIRE_LOCK
  #define ESL_CFG_ACQUIRE_LOCK()
#endif

/// \brief Default assignment of the lock release (e.g. leaving critical section)
#ifndef ESL_CFG_RELEASE_LOCK
  #define ESL_CFG_RELEASE_LOCK()
#endif
