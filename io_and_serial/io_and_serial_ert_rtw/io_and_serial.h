//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// File: io_and_serial.h
//
// Code generated for Simulink model 'io_and_serial'.
//
// Model version                  : 1.11
// Simulink Coder version         : 9.5 (R2021a) 14-Nov-2020
// C/C++ source code generated on : Thu May 20 10:19:09 2021
//
// Target selection: ert.tlc
// Embedded hardware selection: ARM Compatible->ARM Cortex
// Code generation objectives: Unspecified
// Validation result: Not run
//
#ifndef RTW_HEADER_io_and_serial_h_
#define RTW_HEADER_io_and_serial_h_
#include <math.h>
#include <string.h>
#include <stddef.h>
#include <poll.h>
#include <uORB/uORB.h>
#include "rtwtypes.h"
#include "MW_PX4_PWM.h"
#include "MW_SCI.h"
#include "MW_uORB_Read.h"
#include "io_and_serial_types.h"
#include "rt_defines.h"
#include "rt_nonfinite.h"
#include "rtGetInf.h"

// Macros for accessing real-time model data structure
#ifndef rtmGetErrorStatus
#define rtmGetErrorStatus(rtm)         ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
#define rtmSetErrorStatus(rtm, val)    ((rtm)->errorStatus = (val))
#endif

// Block signals (default storage)
struct B_io_and_serial_T {
  px4_Bus_vehicle_odometry In1;        // '<S13>/In1'
  px4_Bus_vehicle_odometry b_varargout_2;
  uint16_T pwmValue[8];
  char_T b_SCIModuleLoc[13];
};

// Block states (default storage) for system '<Root>'
struct DW_io_and_serial_T {
  px4_internal_block_PWM_io_and_T obj; // '<Root>/PX4 PWM Output'
  px4_internal_block_PX4SCIWrit_T obj_b;// '<S2>/Serial Transmit1'
  px4_internal_block_Subscriber_T obj_f;// '<S12>/SourceBlock'
};

// Parameters (default storage)
struct P_io_and_serial_T_ {
  px4_Bus_vehicle_odometry Out1_Y0;    // Computed Parameter: Out1_Y0
                                          //  Referenced by: '<S13>/Out1'

  px4_Bus_vehicle_odometry Constant_Value;// Computed Parameter: Constant_Value
                                             //  Referenced by: '<S12>/Constant'

  real_T Constant_Value_e;             // Expression: 1
                                          //  Referenced by: '<S7>/Constant'

  real_T Constant_Value_m;             // Expression: 1
                                          //  Referenced by: '<S8>/Constant'

  real_T Constant3_Value;              // Expression: 1000
                                          //  Referenced by: '<Root>/Constant3'

  real_T Constant_Value_f;             // Expression: 0.5*pi
                                          //  Referenced by: '<Root>/Constant'

  real_T Gain_Gain;                    // Expression: 500
                                          //  Referenced by: '<Root>/Gain'

  uint16_T Saturation_UpperSat;       // Computed Parameter: Saturation_UpperSat
                                         //  Referenced by: '<Root>/Saturation'

  uint16_T Saturation_LowerSat;       // Computed Parameter: Saturation_LowerSat
                                         //  Referenced by: '<Root>/Saturation'

  boolean_T Constant1_Value;           // Computed Parameter: Constant1_Value
                                          //  Referenced by: '<Root>/Constant1'

  boolean_T Constant2_Value;           // Computed Parameter: Constant2_Value
                                          //  Referenced by: '<Root>/Constant2'

};

// Real-time Model Data Structure
struct tag_RTM_io_and_serial_T {
  const char_T *errorStatus;
};

// Block parameters (default storage)
#ifdef __cplusplus

extern "C" {

#endif

  extern P_io_and_serial_T io_and_serial_P;

#ifdef __cplusplus

}
#endif

// Block signals (default storage)
#ifdef __cplusplus

extern "C" {

#endif

  extern struct B_io_and_serial_T io_and_serial_B;

#ifdef __cplusplus

}
#endif

// Block states (default storage)
extern struct DW_io_and_serial_T io_and_serial_DW;

#ifdef __cplusplus

extern "C" {

#endif

  // Model entry point functions
  extern void io_and_serial_initialize(void);
  extern void io_and_serial_step(void);
  extern void io_and_serial_terminate(void);

#ifdef __cplusplus

}
#endif

// Real-time Model object
#ifdef __cplusplus

extern "C" {

#endif

  extern RT_MODEL_io_and_serial_T *const io_and_serial_M;

#ifdef __cplusplus

}
#endif

//-
//  These blocks were eliminated from the model due to optimizations:
//
//  Block '<S12>/NOT' : Unused code path elimination


//-
//  The generated code includes comments that allow you to trace directly
//  back to the appropriate location in the model.  The basic format
//  is <system>/block_name, where system is the system number (uniquely
//  assigned by Simulink) and block_name is the name of the block.
//
//  Use the MATLAB hilite_system command to trace the generated code back
//  to the model.  For example,
//
//  hilite_system('<S3>')    - opens system 3
//  hilite_system('<S3>/Kp') - opens and selects block Kp which resides in S3
//
//  Here is the system hierarchy for this model
//
//  '<Root>' : 'io_and_serial'
//  '<S1>'   : 'io_and_serial/Quaternions to Rotation Angles'
//  '<S2>'   : 'io_and_serial/Subsystem'
//  '<S3>'   : 'io_and_serial/Vehicle Attitude'
//  '<S4>'   : 'io_and_serial/Quaternions to Rotation Angles/Angle Calculation'
//  '<S5>'   : 'io_and_serial/Quaternions to Rotation Angles/Quaternion Normalize'
//  '<S6>'   : 'io_and_serial/Quaternions to Rotation Angles/Angle Calculation/Protect asincos input'
//  '<S7>'   : 'io_and_serial/Quaternions to Rotation Angles/Angle Calculation/Protect asincos input/If Action Subsystem'
//  '<S8>'   : 'io_and_serial/Quaternions to Rotation Angles/Angle Calculation/Protect asincos input/If Action Subsystem1'
//  '<S9>'   : 'io_and_serial/Quaternions to Rotation Angles/Angle Calculation/Protect asincos input/If Action Subsystem2'
//  '<S10>'  : 'io_and_serial/Quaternions to Rotation Angles/Quaternion Normalize/Quaternion Modulus'
//  '<S11>'  : 'io_and_serial/Quaternions to Rotation Angles/Quaternion Normalize/Quaternion Modulus/Quaternion Norm'
//  '<S12>'  : 'io_and_serial/Vehicle Attitude/PX4 uORB Read'
//  '<S13>'  : 'io_and_serial/Vehicle Attitude/PX4 uORB Read/Enabled Subsystem'

#endif                                 // RTW_HEADER_io_and_serial_h_

//
// File trailer for generated code.
//
// [EOF]
//
