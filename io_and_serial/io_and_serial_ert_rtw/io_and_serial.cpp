//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// File: io_and_serial.cpp
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
#include "io_and_serial.h"
#include "io_and_serial_private.h"

// Block signals (default storage)
B_io_and_serial_T io_and_serial_B;

// Block states (default storage)
DW_io_and_serial_T io_and_serial_DW;

// Real-time model
RT_MODEL_io_and_serial_T io_and_serial_M_ = RT_MODEL_io_and_serial_T();
RT_MODEL_io_and_serial_T *const io_and_serial_M = &io_and_serial_M_;

// Forward declaration for local functions
static void io_and_serial_SystemCore_setup(px4_internal_block_PWM_io_and_T *obj,
  boolean_T varargin_1, boolean_T varargin_2);
static void io_and_seria_SystemCore_setup_g(px4_internal_block_PX4SCIWrit_T *obj);
real_T rt_atan2d_snf(real_T u0, real_T u1)
{
  real_T y;
  int32_T u0_0;
  int32_T u1_0;
  if (rtIsNaN(u0) || rtIsNaN(u1)) {
    y = (rtNaN);
  } else if (rtIsInf(u0) && rtIsInf(u1)) {
    if (u0 > 0.0) {
      u0_0 = 1;
    } else {
      u0_0 = -1;
    }

    if (u1 > 0.0) {
      u1_0 = 1;
    } else {
      u1_0 = -1;
    }

    y = atan2(static_cast<real_T>(u0_0), static_cast<real_T>(u1_0));
  } else if (u1 == 0.0) {
    if (u0 > 0.0) {
      y = RT_PI / 2.0;
    } else if (u0 < 0.0) {
      y = -(RT_PI / 2.0);
    } else {
      y = 0.0;
    }
  } else {
    y = atan2(u0, u1);
  }

  return y;
}

static void io_and_serial_SystemCore_setup(px4_internal_block_PWM_io_and_T *obj,
  boolean_T varargin_1, boolean_T varargin_2)
{
  uint16_T status;
  obj->isSetupComplete = false;
  obj->isInitialized = 1;
  obj->isMain = true;
  obj->pwmDevObj = MW_PWM_OUTPUT_MAIN_DEVICE_PATH;
  status = pwm_open(&obj->pwmDevHandler, obj->pwmDevObj,
                    &obj->actuatorAdvertiseObj, &obj->armAdvertiseObj);
  obj->errorStatus = static_cast<uint16_T>(obj->errorStatus | status);
  obj->servoCount = 0;
  status = pwm_getServoCount(&obj->pwmDevHandler, &obj->servoCount);
  obj->errorStatus = static_cast<uint16_T>(obj->errorStatus | status);
  if (varargin_1) {
    status = pwm_arm(&obj->pwmDevHandler, &obj->armAdvertiseObj);
    obj->isArmed = true;
  } else {
    status = pwm_disarm(&obj->pwmDevHandler, &obj->armAdvertiseObj);
    obj->isArmed = false;
  }

  obj->errorStatus = static_cast<uint16_T>(obj->errorStatus | status);
  status = pwm_setPWMRate(&obj->pwmDevHandler, obj->isMain);
  obj->errorStatus = static_cast<uint16_T>(obj->errorStatus | status);
  obj->channelMask = 3;
  status = pwm_setChannelMask(&obj->pwmDevHandler, obj->channelMask);
  obj->errorStatus = static_cast<uint16_T>(obj->errorStatus | status);
  status = pwm_setFailsafePWM(&obj->pwmDevHandler, obj->servoCount,
    obj->channelMask, obj->isMain);
  obj->errorStatus = static_cast<uint16_T>(obj->errorStatus | status);
  status = pwm_setDisarmedPWM(&obj->pwmDevHandler, obj->servoCount,
    obj->channelMask, obj->isMain, &obj->actuatorAdvertiseObj);
  obj->errorStatus = static_cast<uint16_T>(obj->errorStatus | status);
  if (obj->isMain) {
    status = pwm_forceFailsafe(&obj->pwmDevHandler, static_cast<int32_T>
      (varargin_2));
    obj->errorStatus = static_cast<uint16_T>(obj->errorStatus | status);
    status = pwm_forceTerminateFailsafe(&obj->pwmDevHandler, 0);
    obj->errorStatus = static_cast<uint16_T>(obj->errorStatus | status);
  }

  obj->isSetupComplete = true;
}

static void io_and_seria_SystemCore_setup_g(px4_internal_block_PX4SCIWrit_T *obj)
{
  MW_SCI_HardwareFlowControl_Type HardwareFlowControlValue;
  MW_SCI_Parity_Type ParityValue;
  MW_SCI_StopBits_Type StopBitsValue;
  uint32_T RxPinLoc;
  uint32_T TxPinLoc;
  void* SCIModuleVoidPtr;
  int32_T i;
  static const char_T tmp[12] = { '/', 'd', 'e', 'v', '/', 't', 't', 'y', 'A',
    'C', 'M', '0' };

  obj->isSetupComplete = false;
  obj->isInitialized = 1;
  RxPinLoc = MW_UNDEFINED_VALUE;
  TxPinLoc = MW_UNDEFINED_VALUE;
  for (i = 0; i < 12; i++) {
    io_and_serial_B.b_SCIModuleLoc[i] = tmp[i];
  }

  io_and_serial_B.b_SCIModuleLoc[12] = '\x00';
  SCIModuleVoidPtr = (void*)(&io_and_serial_B.b_SCIModuleLoc[0]);
  obj->SCIDriverObj.MW_SCIHANDLE = MW_SCI_Open(SCIModuleVoidPtr, true, RxPinLoc,
    TxPinLoc);
  MW_SCI_SetBaudrate(obj->SCIDriverObj.MW_SCIHANDLE, 9600U);
  StopBitsValue = MW_SCI_STOPBITS_1;
  ParityValue = MW_SCI_PARITY_NONE;
  MW_SCI_SetFrameFormat(obj->SCIDriverObj.MW_SCIHANDLE, 8, ParityValue,
                        StopBitsValue);
  RxPinLoc = MW_UNDEFINED_VALUE;
  TxPinLoc = MW_UNDEFINED_VALUE;
  HardwareFlowControlValue = MW_SCI_FLOWCONTROL_NONE;
  MW_SCI_ConfigureHardwareFlowControl(obj->SCIDriverObj.MW_SCIHANDLE,
    HardwareFlowControlValue, RxPinLoc, TxPinLoc);
  obj->isSetupComplete = true;
}

// Model step function
void io_and_serial_step(void)
{
  real_T rtb_Product1;
  real_T rtb_Product2;
  real_T rtb_Product3;
  real_T rtb_fcn5;
  int32_T i;
  real32_T rtb_CastToSingle1;
  uint16_T status;
  uint8_T TxDataLocChar[4];
  boolean_T b_varargout_1;

  // MATLABSystem: '<S12>/SourceBlock' incorporates:
  //   Inport: '<S13>/In1'

  b_varargout_1 = uORB_read_step(io_and_serial_DW.obj_f.orbMetadataObj,
    &io_and_serial_DW.obj_f.eventStructObj, &io_and_serial_B.b_varargout_2,
    false, 1.0);

  // Outputs for Enabled SubSystem: '<S12>/Enabled Subsystem' incorporates:
  //   EnablePort: '<S13>/Enable'

  if (b_varargout_1) {
    io_and_serial_B.In1 = io_and_serial_B.b_varargout_2;
  }

  // End of MATLABSystem: '<S12>/SourceBlock'
  // End of Outputs for SubSystem: '<S12>/Enabled Subsystem'

  // Sqrt: '<S10>/sqrt' incorporates:
  //   DataTypeConversion: '<Root>/Data Type Conversion1'
  //   Product: '<S11>/Product'
  //   Product: '<S11>/Product1'
  //   Product: '<S11>/Product2'
  //   Product: '<S11>/Product3'
  //   Sum: '<S11>/Sum'

  rtb_Product3 = sqrt(((static_cast<real_T>(io_and_serial_B.In1.q[0]) *
                        io_and_serial_B.In1.q[0] + static_cast<real_T>
                        (io_and_serial_B.In1.q[1]) * io_and_serial_B.In1.q[1]) +
                       static_cast<real_T>(io_and_serial_B.In1.q[2]) *
                       io_and_serial_B.In1.q[2]) + static_cast<real_T>
                      (io_and_serial_B.In1.q[3]) * io_and_serial_B.In1.q[3]);

  // Product: '<S5>/Product' incorporates:
  //   DataTypeConversion: '<Root>/Data Type Conversion1'

  rtb_fcn5 = io_and_serial_B.In1.q[0] / rtb_Product3;

  // Product: '<S5>/Product1' incorporates:
  //   DataTypeConversion: '<Root>/Data Type Conversion1'

  rtb_Product1 = io_and_serial_B.In1.q[1] / rtb_Product3;

  // Product: '<S5>/Product2' incorporates:
  //   DataTypeConversion: '<Root>/Data Type Conversion1'

  rtb_Product2 = io_and_serial_B.In1.q[2] / rtb_Product3;

  // Product: '<S5>/Product3' incorporates:
  //   DataTypeConversion: '<Root>/Data Type Conversion1'

  rtb_Product3 = io_and_serial_B.In1.q[3] / rtb_Product3;

  // Sum: '<Root>/Add1' incorporates:
  //   Constant: '<Root>/Constant'
  //   Constant: '<Root>/Constant3'
  //   Fcn: '<S1>/fcn4'
  //   Fcn: '<S1>/fcn5'
  //   Gain: '<Root>/Gain'
  //   Sum: '<Root>/Add'
  //   Trigonometry: '<S4>/Trigonometric Function3'

  rtb_Product3 = (rt_atan2d_snf((rtb_Product2 * rtb_Product3 + rtb_fcn5 *
    rtb_Product1) * 2.0, ((rtb_fcn5 * rtb_fcn5 - rtb_Product1 * rtb_Product1) -
    rtb_Product2 * rtb_Product2) + rtb_Product3 * rtb_Product3) +
                  io_and_serial_P.Constant_Value_f) * io_and_serial_P.Gain_Gain
    + io_and_serial_P.Constant3_Value;

  // Saturate: '<Root>/Saturation'
  if (rtb_Product3 > io_and_serial_P.Saturation_UpperSat) {
    rtb_Product3 = io_and_serial_P.Saturation_UpperSat;
  } else if (rtb_Product3 < io_and_serial_P.Saturation_LowerSat) {
    rtb_Product3 = io_and_serial_P.Saturation_LowerSat;
  }

  rtb_Product3 = floor(rtb_Product3);
  if (rtIsNaN(rtb_Product3)) {
    rtb_Product3 = 0.0;
  } else {
    rtb_Product3 = fmod(rtb_Product3, 65536.0);
  }

  // MATLABSystem: '<Root>/PX4 PWM Output' incorporates:
  //   Constant: '<Root>/Constant1'
  //   Constant: '<Root>/Constant2'
  //   Saturate: '<Root>/Saturation'

  for (i = 0; i < 8; i++) {
    io_and_serial_B.pwmValue[i] = 0U;
  }

  io_and_serial_B.pwmValue[0] = static_cast<uint16_T>(rtb_Product3);
  io_and_serial_B.pwmValue[1] = static_cast<uint16_T>(rtb_Product3);
  if (io_and_serial_P.Constant1_Value) {
    if (!io_and_serial_DW.obj.isArmed) {
      io_and_serial_DW.obj.isArmed = true;
      status = pwm_arm(&io_and_serial_DW.obj.pwmDevHandler,
                       &io_and_serial_DW.obj.armAdvertiseObj);
      io_and_serial_DW.obj.errorStatus = static_cast<uint16_T>
        (io_and_serial_DW.obj.errorStatus | status);
    }

    status = pwm_setServo(&io_and_serial_DW.obj.pwmDevHandler,
                          io_and_serial_DW.obj.servoCount,
                          io_and_serial_DW.obj.channelMask,
                          &io_and_serial_B.pwmValue[0],
                          io_and_serial_DW.obj.isMain,
                          &io_and_serial_DW.obj.actuatorAdvertiseObj);
    io_and_serial_DW.obj.errorStatus = static_cast<uint16_T>
      (io_and_serial_DW.obj.errorStatus | status);
  } else {
    status = pwm_disarm(&io_and_serial_DW.obj.pwmDevHandler,
                        &io_and_serial_DW.obj.armAdvertiseObj);
    io_and_serial_DW.obj.errorStatus = static_cast<uint16_T>
      (io_and_serial_DW.obj.errorStatus | status);
    io_and_serial_DW.obj.isArmed = false;
    status = pwm_resetServo(&io_and_serial_DW.obj.pwmDevHandler,
      io_and_serial_DW.obj.servoCount, io_and_serial_DW.obj.channelMask,
      io_and_serial_DW.obj.isMain, &io_and_serial_DW.obj.actuatorAdvertiseObj);
    io_and_serial_DW.obj.errorStatus = static_cast<uint16_T>
      (io_and_serial_DW.obj.errorStatus | status);
  }

  if (io_and_serial_DW.obj.isMain) {
    status = pwm_forceFailsafe(&io_and_serial_DW.obj.pwmDevHandler, static_cast<
      int32_T>(io_and_serial_P.Constant2_Value));
    io_and_serial_DW.obj.errorStatus = static_cast<uint16_T>
      (io_and_serial_DW.obj.errorStatus | status);
  }

  // End of MATLABSystem: '<Root>/PX4 PWM Output'

  // DataTypeConversion: '<S2>/Cast To Single1' incorporates:
  //   Saturate: '<Root>/Saturation'

  rtb_CastToSingle1 = static_cast<uint16_T>(rtb_Product3);

  // MATLABSystem: '<S2>/Serial Transmit1'
  memcpy((void *)&TxDataLocChar[0], (void *)&rtb_CastToSingle1, (uint32_T)
         ((size_t)4 * sizeof(uint8_T)));
  MW_SCI_Transmit(io_and_serial_DW.obj_b.SCIDriverObj.MW_SCIHANDLE,
                  &TxDataLocChar[0], 4U);
}

// Model initialize function
void io_and_serial_initialize(void)
{
  // Registration code

  // initialize non-finites
  rt_InitInfAndNaN(sizeof(real_T));

  // SystemInitialize for Enabled SubSystem: '<S12>/Enabled Subsystem'
  // SystemInitialize for Outport: '<S13>/Out1' incorporates:
  //   Inport: '<S13>/In1'

  io_and_serial_B.In1 = io_and_serial_P.Out1_Y0;

  // End of SystemInitialize for SubSystem: '<S12>/Enabled Subsystem'

  // Start for MATLABSystem: '<S12>/SourceBlock'
  io_and_serial_DW.obj_f.matlabCodegenIsDeleted = false;
  io_and_serial_DW.obj_f.isSetupComplete = false;
  io_and_serial_DW.obj_f.isInitialized = 1;
  io_and_serial_DW.obj_f.orbMetadataObj = ORB_ID(vehicle_odometry);
  uORB_read_initialize(io_and_serial_DW.obj_f.orbMetadataObj,
                       &io_and_serial_DW.obj_f.eventStructObj);
  io_and_serial_DW.obj_f.isSetupComplete = true;

  // Start for MATLABSystem: '<Root>/PX4 PWM Output' incorporates:
  //   Constant: '<Root>/Constant1'
  //   Constant: '<Root>/Constant2'

  io_and_serial_DW.obj.errorStatus = 0U;
  io_and_serial_DW.obj.isInitialized = 0;
  io_and_serial_DW.obj.matlabCodegenIsDeleted = false;
  io_and_serial_SystemCore_setup(&io_and_serial_DW.obj,
    io_and_serial_P.Constant1_Value, io_and_serial_P.Constant2_Value);

  // Start for MATLABSystem: '<S2>/Serial Transmit1'
  io_and_serial_DW.obj_b.isInitialized = 0;
  io_and_serial_DW.obj_b.SCIDriverObj.MW_SCIHANDLE = NULL;
  io_and_serial_DW.obj_b.matlabCodegenIsDeleted = false;
  io_and_seria_SystemCore_setup_g(&io_and_serial_DW.obj_b);
}

// Model terminate function
void io_and_serial_terminate(void)
{
  uint16_T status;

  // Terminate for MATLABSystem: '<S12>/SourceBlock'
  if (!io_and_serial_DW.obj_f.matlabCodegenIsDeleted) {
    io_and_serial_DW.obj_f.matlabCodegenIsDeleted = true;
    if ((io_and_serial_DW.obj_f.isInitialized == 1) &&
        io_and_serial_DW.obj_f.isSetupComplete) {
      uORB_read_terminate(&io_and_serial_DW.obj_f.eventStructObj);
    }
  }

  // End of Terminate for MATLABSystem: '<S12>/SourceBlock'

  // Terminate for MATLABSystem: '<Root>/PX4 PWM Output'
  if (!io_and_serial_DW.obj.matlabCodegenIsDeleted) {
    io_and_serial_DW.obj.matlabCodegenIsDeleted = true;
    if ((io_and_serial_DW.obj.isInitialized == 1) &&
        io_and_serial_DW.obj.isSetupComplete) {
      status = pwm_disarm(&io_and_serial_DW.obj.pwmDevHandler,
                          &io_and_serial_DW.obj.armAdvertiseObj);
      io_and_serial_DW.obj.errorStatus = static_cast<uint16_T>
        (io_and_serial_DW.obj.errorStatus | status);
      status = pwm_resetServo(&io_and_serial_DW.obj.pwmDevHandler,
        io_and_serial_DW.obj.servoCount, io_and_serial_DW.obj.channelMask,
        io_and_serial_DW.obj.isMain, &io_and_serial_DW.obj.actuatorAdvertiseObj);
      io_and_serial_DW.obj.errorStatus = static_cast<uint16_T>
        (io_and_serial_DW.obj.errorStatus | status);
      status = pwm_close(&io_and_serial_DW.obj.pwmDevHandler,
                         &io_and_serial_DW.obj.actuatorAdvertiseObj,
                         &io_and_serial_DW.obj.armAdvertiseObj);
      io_and_serial_DW.obj.errorStatus = static_cast<uint16_T>
        (io_and_serial_DW.obj.errorStatus | status);
    }
  }

  // End of Terminate for MATLABSystem: '<Root>/PX4 PWM Output'

  // Terminate for MATLABSystem: '<S2>/Serial Transmit1'
  if (!io_and_serial_DW.obj_b.matlabCodegenIsDeleted) {
    io_and_serial_DW.obj_b.matlabCodegenIsDeleted = true;
    if ((io_and_serial_DW.obj_b.isInitialized == 1) &&
        io_and_serial_DW.obj_b.isSetupComplete) {
      MW_SCI_Close(io_and_serial_DW.obj_b.SCIDriverObj.MW_SCIHANDLE);
    }
  }

  // End of Terminate for MATLABSystem: '<S2>/Serial Transmit1'
}

//
// File trailer for generated code.
//
// [EOF]
//
