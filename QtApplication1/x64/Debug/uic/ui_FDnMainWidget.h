/********************************************************************************
** Form generated from reading UI file 'FDnMainWidget.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FDNMAINWIDGET_H
#define UI_FDNMAINWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FDnMainWidgetClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *FDnMainWidgetClass)
    {
        if (FDnMainWidgetClass->objectName().isEmpty())
            FDnMainWidgetClass->setObjectName(QString::fromUtf8("FDnMainWidgetClass"));
        FDnMainWidgetClass->resize(600, 400);
        menuBar = new QMenuBar(FDnMainWidgetClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        FDnMainWidgetClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(FDnMainWidgetClass);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        FDnMainWidgetClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(FDnMainWidgetClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        FDnMainWidgetClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(FDnMainWidgetClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        FDnMainWidgetClass->setStatusBar(statusBar);

        retranslateUi(FDnMainWidgetClass);

        QMetaObject::connectSlotsByName(FDnMainWidgetClass);
    } // setupUi

    void retranslateUi(QMainWindow *FDnMainWidgetClass)
    {
        FDnMainWidgetClass->setWindowTitle(QCoreApplication::translate("FDnMainWidgetClass", "FDnMainWidget", nullptr));
    } // retranslateUi

};

namespace Ui {
    class FDnMainWidgetClass: public Ui_FDnMainWidgetClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FDNMAINWIDGET_H
