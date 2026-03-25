/**
 * cadMousePro
 * Copyright © 2019 František Kučera (Frantovo.cz, GlobalCode.info)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */
#pragma once

#include <QMainWindow>
#include <QTabWidget>
#include <QFormLayout>
#include <QCheckBox>
#include <QRadioButton>
#include <QButtonGroup>
#include <QLineEdit>
#include <QLabel>
#include <QProgressBar>
#include <QtDBus/QDBusConnection>

#include "generated-sources/Proxy.h"
#include "generated-sources/UPowerProxy.h"

class MouseMainWindow : public QMainWindow {
	Q_OBJECT
public:
	MouseMainWindow();

	virtual ~MouseMainWindow() {
	}

private:
	QTabWidget* tabs = new QTabWidget(this);
	QCheckBox* statusProxy = new QCheckBox("connected to Daemon", this);
	QCheckBox* statusUPower = new QCheckBox("connected to UPower", this);
	QCheckBox* statusDevice = new QCheckBox("mouse found", this);
	QLabel* statusName = new QLabel(this);
	QProgressBar* statusBattery = new QProgressBar(this);

	QCheckBox* configureSmartScrolling = new QCheckBox("smart scrolling (free wheel)", this);
	QCheckBox* configureLiftOffDetection = new QCheckBox("lift-off detection", this);
	QCheckBox* configureRemapWheelPressed = new QCheckBox("remap wheel button: from 2 (same as middle) to 10", this);
	QCheckBox* configureRemapGestureButton = new QCheckBox("remap gesture button: from none to 11", this);
	QRadioButton* configureFrequency125 = new QRadioButton("125 Hz", this);
	QRadioButton* configureFrequency250 = new QRadioButton("250 Hz", this);
	QRadioButton* configureFrequency500 = new QRadioButton("500 Hz", this);
	QRadioButton* configureFrequency1000 = new QRadioButton("1000 Hz", this);

	QLineEdit* message = new QLineEdit(this);

	QDBusConnection connection = QDBusConnection::systemBus();
	InfoGlobalcodeMouseCadMouseProInterface* proxy;
	OrgFreedesktopUPowerInterface* upowerProxy;

	void initStatusPanel();
	void initConfigurationPanel();
	void initAboutPanel();

	void appendAboutLine(QFormLayout* layout, const QString& label, const QString& value, QWidget* parent);

	int getBatteryLevel();

private slots:
	void refresh();
	void configure();
};