#pragma once

#include <QMainWindow>
#include "ui_FDnMainWidget.h"

class FDnMainWidget : public QMainWindow
{
	Q_OBJECT

public:
	FDnMainWidget(QWidget *parent = nullptr);
	~FDnMainWidget();

private:
	Ui::FDnMainWidgetClass ui;
};
