/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-    */
/* ex: set filetype=cpp softtabstop=4 shiftwidth=4 tabstop=4 cindent expandtab: */

/*
  Author(s):  Anton Deguet, Youri Tan
  Created on: 2013-08-24

  (C) Copyright 2013-2015 Johns Hopkins University (JHU), All Rights Reserved.

--- begin cisst license - do not edit ---

This software is provided "as is" under an open source license, with
no warranty.  The complete license can be found in license.txt and
http://www.cisst.org/cisst/license.txt.

--- end cisst license ---
*/

#ifndef _mtsIntuitiveResearchKitSUJQtWidget_h
#define _mtsIntuitiveResearchKitSUJQtWidget_h

#include <sawIntuitiveResearchKit/mtsIntuitiveResearchKitArmQtWidget.h>
#include <cisstParameterTypes/prmPositionJointGet.h>

class mtsIntuitiveResearchKitSUJQtWidget: public mtsIntuitiveResearchKitArmQtWidget
{
    Q_OBJECT;
    CMN_DECLARE_SERVICES(CMN_DYNAMIC_CREATION_ONEARG, CMN_LOG_ALLOW_DEFAULT);

public:
    mtsIntuitiveResearchKitSUJQtWidget(const std::string & componentName, double periodInSeconds = 50.0 * cmn_ms);
    ~mtsIntuitiveResearchKitSUJQtWidget() {}

    void Startup(void);

protected:
    void setupUiDerived(void);
    void timerEventDerived(void);

    vctQtWidgetDynamicVectorDoubleRead * QVJointWidget;
    vctQtWidgetDynamicVectorDoubleRead * QVBrakeCurrentWidget;
    vctQtWidgetDynamicVectorDoubleRead * QVExtraVoltageWidget;

    vctQtWidgetDynamicVectorDoubleWrite * QVPotentiometerRecalibrationStartWidget;
    vctQtWidgetDynamicVectorDoubleWrite * QVPotentiometerRecalibrationFinishWidget;

    QPushButton * QPBLiftDown;
    QPushButton * QPBLiftUp;

    mtsFunctionWrite Clutch;
    mtsFunctionWrite SetLiftVelocity;

    prmPositionJointGet PositionJointParam;
    vctDoubleVec PrimaryJointOffset;
    vctDoubleVec SecondaryJointOffset;
    vctDoubleVec JointPositionStart;
    vctDoubleVec JointPositionFinish;
    vctDoubleVec JointVoltageStart;
    vctDoubleVec JointVoltageFinish;
    double BrakeCurrent;

    mtsFunctionRead GetPositionJoint;
    mtsFunctionRead GetBrakeCurrent;

    mtsFunctionRead GetPrimaryVoltages;
    mtsFunctionRead GetSecondaryVoltages;
    mtsFunctionRead GetExtraVoltages;
    vctDoubleMat mJointsRecalibrationMatrix;
    vctDoubleVec mVoltages[2];
    vctDoubleVec mVoltagesExtra;
    mtsFunctionWrite SetRecalibratioMatrix;

protected slots:
    void SlotManualRecalibration(void);
    void SlotClutchPressed(void);
    void SlotClutchReleased(void);

    void SlotVelocityDownPressed(void);
    void SlotVelocityUpPressed(void);
    void SlotVelocityReleased(void);

    void SlotRecalibrationStartChanged(void);
    void SlotRecalibrationFinishChanged(void);

};

CMN_DECLARE_SERVICES_INSTANTIATION(mtsIntuitiveResearchKitSUJQtWidget);

#endif // _mtsIntuitiveResearchKitSUJQtWidget_h
