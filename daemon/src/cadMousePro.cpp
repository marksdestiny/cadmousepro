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
#include <iostream>

#include <QCoreApplication>
#include <QTimer>
#include <QtDBus/QDBusConnection>

#include "CadMouseConfig.h"
#include "CLI.h"
#include "CLIParser.h"
#include "HID.h"
#include "Daemon.h"
#include "generated-sources/DBusAdaptor.h"

#define APP_NAME L"cadMousePro"

int main(int argc, char** argv) {
	try {
		CLI cli(argc, argv);

		std::wcout << APP_NAME << std::endl;
		
		CLIParser cliParser;
		CLIConfiguration configuration = cliParser.parse(cli.arguments());

		if (configuration.help) {
			std::wcout << L"Usage:" << std::endl;
			std::wcout << L"    " << APP_NAME << L" [OPTIONS]" << std::endl;
			std::wcout << std::endl;
			std::wcout << L"Description:" << std::endl;
			std::wcout << L"    Configure the 3Dconnextion CadMouse Pro. Use --daemon to allow the usage of " << std::endl;
			std::wcout << L"    the mouse over cable." << std::endl;
			std::wcout << std::endl;
			std::wcout << L"Options:" << std::endl;
			std::wcout << L"    --help                      Show this help message and exit." << std::endl;
			std::wcout << L"    --frequency NUM             The polling frequency in Hz (125, 125, 250, 500 or 1000)." << std::endl;
			std::wcout << L"    --smart-scrolling BOOL      Enable or disable smart scrolling." << std::endl;
			std::wcout << L"    --lift-off-detection BOOL   Enable or disabe lift off detection." << std::endl;
			std::wcout << L"    --remap-wheel-press BOOL    " << std::endl;
			std::wcout << L"    --remap-gesture-button BOOL " << std::endl;
			std::wcout << L"    --daemon                    Periodically send feature requests to the mouse." << std::endl;

			return CLI::EXIT_CODE_SUCCESS;

		} else if (configuration.daemon) {
			// TODO: run D-Bus service
			std::wcout << L"will start daemon" << std::endl;
			std::wcout << L"uid = " << getuid() << std::endl;
			QCoreApplication qtApplication(argc, argv);

			Daemon* daemon = new Daemon(configuration.cadMouseConfig, &qtApplication);
			DBusAdaptor* dbusAdaptor = new DBusAdaptor(daemon);

			QDBusConnection connection = getuid() ? QDBusConnection::sessionBus() : QDBusConnection::systemBus();
			//QDBusConnection connection = QDBusConnection::sessionBus();
			std::wcout << (connection.isConnected() ? L"connected" : L"not connected") << std::endl;
			std::wcout << connection.name().toStdWString() << std::endl;
			std::wcout << connection.lastError().message().toStdWString() << std::endl;
			connection.registerObject("/info/globalcode/mouse/cadMousePro", daemon);
			connection.registerService("info.globalcode.mouse.cadMousePro");

			return qtApplication.exec();

		} else {
			std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;

			HIDDevice mouse = Daemon::getMouse();
			std::wcout << L"mouse opened" << std::endl;
			std::wcout << L"manufacturer:     " << mouse.getManufacturerName() << std::endl;
			std::wcout << L"product:          " << mouse.getProductName() << std::endl;
			std::wcout << L"serial number:    " << mouse.getSerialNumber() << std::endl;
			std::wcout << L"interface number: " << mouse.getInterfaceNumber() << std::endl;
			std::wcout << L"path:             " << conv.from_bytes(mouse.getPath()) << std::endl;

			Packet packet = configuration.cadMouseConfig.serialize();
			mouse.sendFeatureReport(packet);
		}

		return CLI::EXIT_CODE_SUCCESS;
	} catch (const HIDException& e) {
		std::wcout << L"HIDException: " << e.getMessage() << std::endl;
		return CLI::EXIT_CODE_UNEXPECTED_ERROR;
	} catch (const CLIException& e) {
		std::wcout << L"CLIException: " << e.getMessage() << std::endl;
		return e.getExitCode();
	}

}
