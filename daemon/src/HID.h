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

#include <array>
#include <vector>
#include <memory>
#include <unistd.h>
#include <cassert>
#include <string>
#include <vector>

#include <codecvt>
#include <locale>

#include <hidapi/hidapi.h>

using Packet = std::vector<uint8_t>;
static_assert(sizeof (uint8_t) == sizeof (unsigned char)); // unsigned char is used in the HID API library

class HIDException {
private:
	std::wstring message;
public:

	HIDException(std::wstring message) : message(message) {
	}

	virtual ~HIDException() {
	}

	const std::wstring getMessage() const {
		return message;
	}

};

class HIDDevice {
private:
	std::shared_ptr<hid_device> handle;

public:

	HIDDevice(const char *path) {
		int initError = hid_init(); // TODO: move to HIDContext class?
		if (initError) throw HIDException(L"Unable to init HID API.");
		handle.reset(hid_open_path(path), hid_close);
		if (handle == nullptr) throw HIDException(L"Unable to open HID device. Are you root? Is the device present?");
	}

	HIDDevice(unsigned short vendorId, unsigned short productId, const wchar_t *serialNumber) {
		int initError = hid_init(); // TODO: move to HIDContext class?
		if (initError) throw HIDException(L"Unable to init HID API.");
		handle.reset(hid_open(vendorId, productId, serialNumber), hid_close);
		if (handle == nullptr) throw HIDException(L"Unable to open HID device. Are you root? Is the device present?");
	}

	virtual ~HIDDevice() {
		hid_exit(); // TODO: move to HIDContext class?
	}

	const std::wstring getManufacturerName() const {
		std::array<wchar_t, 200 > buffer;
		int error = hid_get_manufacturer_string(handle.get(), buffer.data(), buffer.size());
		if (error) throw HIDException(L"Unable to get manufacturer name.");
		return buffer.data();
	}

	const std::wstring getProductName() const {
		std::array<wchar_t, 200 > buffer;
		int error = hid_get_product_string(handle.get(), buffer.data(), buffer.size());
		if (error) throw HIDException(L"Unable to get product name.");
		return buffer.data();
	}

	const std::wstring getSerialNumber() const {
		std::array<wchar_t, 200 > buffer;
		int error = hid_get_serial_number_string(handle.get(), buffer.data(), buffer.size());
		if (error) throw HIDException(L"Unable to get serial number.");
		return buffer.data();
	}

	const std::string getPath() const {
		hid_device_info *info = hid_get_device_info(handle.get());
		if (info == nullptr) throw HIDException(L"Unhable to get device info.");
		char *path = info->path;
		return path;
	}

	const int getInterfaceNumber() const {
		hid_device_info *info = hid_get_device_info(handle.get());
		if (info == nullptr) throw HIDException(L"Unhable to get device info.");
		return info->interface_number;
	}

	void sendFeatureReport(const Packet& data) {
		int written = hid_send_feature_report(handle.get(), data.data(), data.size());
		if (written < 0) throw HIDException(L"Unable to send feature report.");
	}

};

class HIDDeviceInfo {
private:
	std::string path;
	unsigned short vendorId;
	unsigned short productId;
	std::wstring serialNumber;
	int interfaceNumber;
public:

	HIDDeviceInfo(std::string path, unsigned short vendorId, unsigned short productId, std::wstring serialNumber, int interfaceNumber) 
		: path(path), vendorId(vendorId), productId(productId), serialNumber(serialNumber), interfaceNumber(interfaceNumber) {
	}

	virtual ~HIDDeviceInfo() {
	}

	HIDDevice open() {
		return HIDDevice(path.c_str());
	}

	bool matches(unsigned short vendorId, unsigned short productId, int interfaceNumber) {
		return vendorId == this->vendorId && productId == this->productId && interfaceNumber == this->interfaceNumber;
	}

	std::string getPath() const {
		return path;
	}

	unsigned short getProductId() const {
		return productId;
	}

	std::wstring getSerialNumber() const {
		return serialNumber;
	}

	unsigned short getVendorId() const {
		return vendorId;
	}

	int getInterfaceNumber() const {
		return interfaceNumber;
	}

	static std::vector<HIDDeviceInfo> enumerate(unsigned short vendorId = 0, unsigned short productId = 0) {
		std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;

		std::vector<HIDDeviceInfo> result;
		std::shared_ptr<hid_device_info> rawList(hid_enumerate(vendorId, productId), hid_free_enumeration);
		// n.b. for single USB device there might be multiple HID devices different just in di->path
		for (hid_device_info* di = rawList.get(); di; di = di->next) 
		{
#ifdef verbose
			std::wcout << L"path:             " << conv.from_bytes(di->path) << std::endl;
			std::wcout << L"vendor ID:        " << di->vendor_id << std::endl;
			std::wcout << L"product ID:       " << di->product_id << std::endl;
			std::wcout << L"release number:   " << di->release_number << std::endl;
			std::wcout << L"interface number: " << di->interface_number << std::endl;
			if (di->manufacturer_string != nullptr)
				std::wcout << L"manufacturer:     " << di->manufacturer_string << std::endl;
			if (di->product_string != nullptr)
				std::wcout << L"product:          " << di->product_string << std::endl;
			std::wcout << std::endl;
#endif
			result.push_back({di->path, di->vendor_id, di->product_id, di->serial_number, di->interface_number});
		}
		return result;
	}
};
