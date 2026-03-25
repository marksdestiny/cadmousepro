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
#include <QLabel>
#include <QPushButton>
#include <QtDBus/QDBusPendingCall>

#include "MouseMainWindow.h"

MouseMainWindow::MouseMainWindow() {
	proxy = new InfoGlobalcodeMouseCadMouseProInterface("info.globalcode.mouse.cadMousePro", "/info/globalcode/mouse/cadMousePro", connection, this);
	upowerProxy = new OrgFreedesktopUPowerInterface("org.freedesktop.UPower", "/org/freedesktop/UPower", connection, this);

	setWindowTitle("cadMousePro");

	initStatusPanel();
	initConfigurationPanel();
	initAboutPanel();

	setCentralWidget(tabs);
	refresh();
}

void MouseMainWindow::initStatusPanel() {
	QWidget* panel = new QWidget(this);
	QFormLayout* layout = new QFormLayout(panel);

	int f = 0;

	statusProxy->setEnabled(false);
	statusUPower->setEnabled(false);
	statusDevice->setEnabled(false);
	statusBattery->setOrientation(Qt::Orientation::Horizontal);
	statusBattery->setMinimum(0);
	statusBattery->setMaximum(100);

	statusUPower->setToolTip("UPower interface is used for getting the battery level");
	statusName->setToolTip("name of the USB device – usually the wireless adaptor");
	statusBattery->setToolTip("battery charge level");

	layout->setWidget(f++, QFormLayout::FieldRole, statusProxy);
	layout->setWidget(f++, QFormLayout::FieldRole, statusUPower);
	layout->setWidget(f++, QFormLayout::FieldRole, statusDevice);
	layout->setWidget(f++, QFormLayout::FieldRole, statusName);
	layout->setWidget(f++, QFormLayout::FieldRole, statusBattery);

	QPushButton* refreshButton = new QPushButton("Refresh", panel);
	layout->setWidget(f++, QFormLayout::FieldRole, refreshButton);
	connect(refreshButton, &QPushButton::clicked, this, &MouseMainWindow::refresh);

	tabs->addTab(panel, "Status");
}

void MouseMainWindow::initConfigurationPanel() {
	QWidget* panel = new QWidget(this);
	QFormLayout* layout = new QFormLayout(panel);

	int f = 0;

	configureLiftOffDetection->setChecked(true);
	configureRemapWheelPressed->setChecked(true);
	configureRemapGestureButton->setChecked(true);
	layout->setWidget(f++, QFormLayout::FieldRole, configureLiftOffDetection);
	layout->setWidget(f++, QFormLayout::FieldRole, configureSmartScrolling);
	layout->setWidget(f++, QFormLayout::FieldRole, configureRemapWheelPressed);
	layout->setWidget(f++, QFormLayout::FieldRole, configureRemapGestureButton);
	configureFrequency250->setChecked(true);
	layout->setWidget(f++, QFormLayout::FieldRole, configureFrequency125);
	layout->setWidget(f++, QFormLayout::FieldRole, configureFrequency250);
	layout->setWidget(f++, QFormLayout::FieldRole, configureFrequency500);
	layout->setWidget(f++, QFormLayout::FieldRole, configureFrequency1000);

	QPushButton* configureButton = new QPushButton("Configure", panel);
	configureButton->setToolTip("n.b. current interface is write-only and the controls above just configures the mouse – does not show current configuration");
	layout->setWidget(f++, QFormLayout::FieldRole, configureButton);
	connect(configureButton, &QPushButton::clicked, this, &MouseMainWindow::configure);

	message->setReadOnly(true);
	layout->setWidget(f++, QFormLayout::FieldRole, new QLabel("Result:", panel));
	layout->setWidget(f++, QFormLayout::FieldRole, message);

	tabs->addTab(panel, "Configuration");
}

void MouseMainWindow::appendAboutLine(QFormLayout* layout, const QString& label, const QString& value, QWidget* parent) {
	QLabel* labelWidget = new QLabel(label, parent);
	QLabel* textWidget = new QLabel(value, parent);

	textWidget->setTextInteractionFlags(Qt::TextInteractionFlag::TextSelectableByMouse | Qt::TextInteractionFlag::TextBrowserInteraction);
	textWidget->setOpenExternalLinks(true);

	layout->addRow(labelWidget, textWidget);
}

void MouseMainWindow::initAboutPanel() {
	QWidget* panel = new QWidget(this);
	QFormLayout* layout = new QFormLayout(panel);

	appendAboutLine(layout, "Program name:", "cadMousePro", panel);
	appendAboutLine(layout, "Description:", "configuration tool for 3DConnexion CadMouse Pro Wireless", panel);
	appendAboutLine(layout, "Author:", "Ing. František Kučera (Frantovo.cz, GlobalCode.info)", panel);
	appendAboutLine(layout, "License:", "<a href='https://www.gnu.org/licenses/gpl-3.0.html'>GNU GPLv3+</a>", panel);
	appendAboutLine(layout, "Contact and support:", "<a href='https://mouse.globalcode.info/'>https://mouse.globalcode.info/</a>", panel);
	appendAboutLine(layout, "BTC:", "bc1qmt3qgzcf3a0f0tvnm70zjr0vhlchfyrjtnleqm", panel);

	tabs->addTab(panel, "About");
}

/**
 * Finds the first HID mouse managed by UPower and returns its battery level.
 * 
 * @return percentage of the current battery charge or 0 if no suitable mouse was found
 */
int MouseMainWindow::getBatteryLevel() {
	auto devices = upowerProxy->EnumerateDevices();
	devices.waitForFinished();
	for (auto device : devices.value()) {
		if (device.path().startsWith(upowerProxy->path() + "/devices/mouse_hid_") && device.path().endsWith("_battery")) {
			OrgFreedesktopUPowerDeviceInterface battery(upowerProxy->service(), device.path(), connection, this);
			if (battery.isValid()) return battery.percentage();
		}
	}
	return 0; // mouse with battery not found
}

void MouseMainWindow::refresh() {
	statusProxy->setChecked(proxy->isValid());
	statusUPower->setChecked(upowerProxy->isValid());
	statusDevice->setChecked(proxy->devicePresent());
	statusName->setText(proxy->deviceName());
	statusBattery->setValue(getBatteryLevel());
}

void MouseMainWindow::configure() {
	message->setText("…");

	int frequency; // TODO: get value from the group
	if (configureFrequency125->isChecked()) frequency = 125;
	else if (configureFrequency250->isChecked()) frequency = 250;
	else if (configureFrequency500->isChecked()) frequency = 500;
	else if (configureFrequency1000->isChecked()) frequency = 1000;
	else frequency = 250;

	auto response = proxy->configure(
			configureLiftOffDetection->isChecked(),
			configureSmartScrolling->isChecked(),
			configureRemapWheelPressed->isChecked(),
			configureRemapGestureButton->isChecked(),
			frequency);

	response.waitForFinished();
	if (response.isError()) message->setText("Error: " + response.error().message());
	else message->setText("mouse configured");

}
