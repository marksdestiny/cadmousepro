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

#include <iostream>
#include <QObject>
#include <QDBusContext>
#include <QDBusConnection>

#include "HID.h"
#include "CadMouseConfig.h"
#include "Common.h"

class Daemon : public QObject, public QDBusContext {
	Q_OBJECT;

private:

	static unsigned short const VENDOR_ID = 0x256f;
	static unsigned short const PRODUCT_ID_RECEIVER = 0xc652;
	static unsigned short const PRODUCT_ID_WIRELESS = 0xc654;
	static unsigned short const PRODUCT_ID_WIRED = 0xc656;

	Packet data;

	void logException(const HIDException& e) {
		std::wcout << L"HIDException: " << e.getMessage() << std::endl;
	}
public:

	Daemon(const CadMouseConfig &config, QObject* parent = nullptr);

	static HIDDevice getMouse() {
		// TODO: multiple mouse support, better mouse selection, specific settings for particular type…
		// (probably after future redesign/rewrite – this is just a simple tool written for 3DConnexion CadMouse Pro)
		std::vector<HIDDeviceInfo> devices = HIDDeviceInfo::enumerate();
		for (auto pid :{PRODUCT_ID_RECEIVER, PRODUCT_ID_WIRELESS, PRODUCT_ID_WIRED}) {
			for (HIDDeviceInfo device : devices) {
				if (device.matches(VENDOR_ID, pid, 1)) return device.open();
			}
		}
		throw HIDException(L"No suitable mouse found.");
	}

	Q_PROPERTY(bool devicePresent READ devicePresent)
	bool devicePresent() {
		std::wcout << L"checking mouse presence: ";
		try {
			getMouse();
			std::wcout << L"present" << std::endl;
			return true;
		} catch (const HIDException& e) {
			std::wcout << std::endl;
			logException(e);
			return false;
		}
	}

	Q_PROPERTY(QString deviceName READ deviceName)
	QString deviceName() {
		std::wcout << L"reading mouse name: ";
		try {
			auto productName = getMouse().getProductName();
			std::wcout << productName << std::endl;
			return QString::fromStdWString(productName);
		} catch (HIDException& e) {
			std::wcout << std::endl;
			logException(e);
			return QString();
		}
	}

	void configure(bool liftOffDetection, bool smartScrolling, bool remapWheelPress, bool remapGestureButton, int frequency);

public slots:
	void sendFeatureReport();

};
