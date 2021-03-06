/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-    */
/* ex: set filetype=cpp softtabstop=4 shiftwidth=4 tabstop=4 cindent expandtab: */

/*
  Author(s):  Anton Deguet, Zihan Chen
  Created on: 2013-05-17

  (C) Copyright 2013-2015 Johns Hopkins University (JHU), All Rights Reserved.

--- begin cisst license - do not edit ---

This software is provided "as is" under an open source license, with
no warranty.  The complete license can be found in license.txt and
http://www.cisst.org/cisst/license.txt.

--- end cisst license ---
*/

// system include
#include <iostream>

// cisst
#include <cisstMultiTask/mtsInterfaceRequired.h>
#include <sawIntuitiveResearchKit/mtsIntuitiveResearchKitConsoleQtWidget.h>

#include <QMessageBox>
#include <QCloseEvent>
#include <QCoreApplication>
#include <QScrollBar>
#include <QGridLayout>
#include <QGroupBox>
#include <QRadioButton>
#include <QButtonGroup>
#include <QTime>

CMN_IMPLEMENT_SERVICES(mtsIntuitiveResearchKitConsoleQtWidget);

mtsIntuitiveResearchKitConsoleQtWidget::mtsIntuitiveResearchKitConsoleQtWidget(const std::string & componentName):
    mtsComponent(componentName)
{
    mtsInterfaceRequired * interfaceRequiredMain = AddInterfaceRequired("Main");
    if (interfaceRequiredMain) {
        interfaceRequiredMain->AddFunction("SetRobotsControlState", Console.SetRobotsControlState);
        interfaceRequiredMain->AddFunction("TeleopEnable", Console.TeleopEnable);
        interfaceRequiredMain->AddEventHandlerWrite(&mtsIntuitiveResearchKitConsoleQtWidget::ErrorEventHandler,
                                                    this, "Error");
        interfaceRequiredMain->AddEventHandlerWrite(&mtsIntuitiveResearchKitConsoleQtWidget::WarningEventHandler,
                                                    this, "Warning");
        interfaceRequiredMain->AddEventHandlerWrite(&mtsIntuitiveResearchKitConsoleQtWidget::StatusEventHandler,
                                                    this, "Status");
    }
    setupUi();
}

void mtsIntuitiveResearchKitConsoleQtWidget::Configure(const std::string & filename)
{
    CMN_LOG_CLASS_INIT_VERBOSE << "Configure: " << filename << std::endl;
}

void mtsIntuitiveResearchKitConsoleQtWidget::Startup(void)
{
    CMN_LOG_CLASS_INIT_VERBOSE << "mtsIntuitiveResearchKitConsoleQtWidget::Startup" << std::endl;
    if (!parent()) {
        show();
    }
}

void mtsIntuitiveResearchKitConsoleQtWidget::Cleanup(void)
{
    this->hide();
    CMN_LOG_CLASS_INIT_VERBOSE << "mtsIntuitiveResearchKitConsoleQtWidget::Cleanup" << std::endl;
}

void mtsIntuitiveResearchKitConsoleQtWidget::closeEvent(QCloseEvent * event)
{
    int answer = QMessageBox::warning(this, tr("mtsIntuitiveResearchKitConsoleQtWidget"),
                                      tr("Do you really want to quit this application?"),
                                      QMessageBox::No | QMessageBox::Yes);
    if (answer == QMessageBox::Yes) {
        event->accept();
        QCoreApplication::exit();
    } else {
        event->ignore();
    }
}

void mtsIntuitiveResearchKitConsoleQtWidget::SlotSetStateButton(QAbstractButton * radioButton)
{
    std::string request = radioButton->text().toStdString();
    if (request == "Idle") {
        Console.SetRobotsControlState(std::string("DVRK_UNINITIALIZED"));
    } else if (request == "Home") {
        Console.SetRobotsControlState(request);
    } else if (request == "Teleop") {
        Console.TeleopEnable(true);
    }
}

void mtsIntuitiveResearchKitConsoleQtWidget::SlotTextChanged(void)
{
    QTEMessages->verticalScrollBar()->setSliderPosition(QTEMessages->verticalScrollBar()->maximum());
}

void mtsIntuitiveResearchKitConsoleQtWidget::setupUi(void)
{
    QGridLayout * frameLayout = new QGridLayout;

    QGroupBox * groupBox = new QGroupBox("Desired state");
    QRadioButton * idleButton = new QRadioButton("Idle");
    QRadioButton * homeButton = new QRadioButton("Home");
    QRadioButton * teleOpButton = new QRadioButton("Teleop");
    // QRadioButton * gcButton = new QRadioButton("Gravity");
    // QRadioButton * clutchButton = new QRadioButton("Clutch");
    idleButton->setChecked(true);
    QButtonGroup * group = new QButtonGroup;
    group->addButton(idleButton);
    group->addButton(homeButton);
    group->addButton(teleOpButton);
    // group->addButton(gcButton);
    // group->addButton(clutchButton);
	group->setExclusive(true);

    QVBoxLayout * vbox = new QVBoxLayout;
    vbox->addWidget(idleButton);
    vbox->addWidget(homeButton);
    vbox->addWidget(teleOpButton);
    // vbox->addWidget(gcButton);
    // vbox->addWidget(clutchButton);
    vbox->addStretch(1);
    groupBox->setLayout(vbox);
    frameLayout->addWidget(groupBox, 0, 0);

    QTEMessages = new QTextEdit();
    QTEMessages->setReadOnly(true);
    QTEMessages->ensureCursorVisible();
    frameLayout->addWidget(QTEMessages, 0, 1);

    QVBoxLayout * mainLayout = new QVBoxLayout;
    mainLayout->addLayout(frameLayout);
    setLayout(mainLayout);

    setWindowTitle("Intuitive Research Kit");
    resize(sizeHint());

    connect(group, SIGNAL(buttonClicked(QAbstractButton*)),
            this, SLOT(SlotSetStateButton(QAbstractButton*)));
    connect(this, SIGNAL(SignalAppendMessage(QString)),
            QTEMessages, SLOT(append(QString)));
    connect(this, SIGNAL(SignalSetColor(QColor)),
            QTEMessages, SLOT(setTextColor(QColor)));
    connect(QTEMessages, SIGNAL(textChanged()),
            this, SLOT(SlotTextChanged()));
}

void mtsIntuitiveResearchKitConsoleQtWidget::ErrorEventHandler(const std::string & message)
{
    emit SignalSetColor(QColor("red"));
    emit SignalAppendMessage(QTime::currentTime().toString("hh:mm:ss") + QString(" Error: ") + QString(message.c_str()));
}

void mtsIntuitiveResearchKitConsoleQtWidget::WarningEventHandler(const std::string & message)
{
    emit SignalSetColor(QColor("darkRed"));
    emit SignalAppendMessage(QTime::currentTime().toString("hh:mm:ss") + QString(" Warning: ") + QString(message.c_str()));
}

void mtsIntuitiveResearchKitConsoleQtWidget::StatusEventHandler(const std::string & message)
{
    emit SignalSetColor(QColor("black"));
    emit SignalAppendMessage(QTime::currentTime().toString("hh:mm:ss") + QString(" Status: ") + QString(message.c_str()));
}
