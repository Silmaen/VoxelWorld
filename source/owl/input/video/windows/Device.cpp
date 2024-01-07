/**
 * @file ${FILE}
 * @author Silmaen
 * @date 03/01/2024
 * Copyright © 2024 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "Device.h"

#if defined(OWL_PLATFORM_WINDOWS)

#include "WPointer.h"
#include "math/simpleFunctions.h"
#include <initguid.h>
#include <mfapi.h>
#include <windows.h>

namespace owl::input::video::windows {

static const std::string baseName = "input::video::windows::";
constexpr uint64_t mask32 = 0xffffffff;

namespace {

struct ComControl {
	bool addRef() {
		if (refCount == 0) {
			HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
			if (FAILED(hr)) {
				OWL_CORE_ERROR("Unable to Initialize COM")
				return false;
			}
			hr = MFStartup(MF_VERSION, MFSTARTUP_NOSOCKET);
			if (FAILED(hr)) {
				OWL_CORE_ERROR("Unable to Initialize Media Foundation")
				return false;
			}
		}
		++refCount;
		return true;
	}
	void delRef() {
		if (refCount == 0)
			return;
		--refCount;
		if (refCount == 0) {
			MFShutdown();
			CoUninitialize();
		}
	}

private:
	uint64_t refCount = 0;
};
ComControl cc;

std::string getPixelFormatString(const GUID &videoFormat) {
	if (videoFormat == MFVideoFormat_AI44) return "AI44";
	else if (videoFormat == MFVideoFormat_ARGB32)
		return "ARGB32";
	else if (videoFormat == MFVideoFormat_AYUV)
		return "AYUV";
	else if (videoFormat == MFVideoFormat_DV25)
		return "DV25";
	else if (videoFormat == MFVideoFormat_DV50)
		return "DV50";
	else if (videoFormat == MFVideoFormat_MJPG)
		return "MJPG";
	else if (videoFormat == MFVideoFormat_RGB32)
		return "RGB32";
	else if (videoFormat == MFVideoFormat_RGB24)
		return "RGB24";
	else if (videoFormat == MFVideoFormat_RGB555)
		return "RGB555";
	else if (videoFormat == MFVideoFormat_RGB565)
		return "RGB565";
	else if (videoFormat == MFVideoFormat_UYVY)
		return "UYVY";
	//else if (videoFormat == MFVideoFormat_Y411) return "Y411";
	else if (videoFormat == MFVideoFormat_Y41P)
		return "Y41P";
	//else if (videoFormat == MFVideoFormat_YUV) return "YUV";
	else if (videoFormat == MFVideoFormat_YUY2)
		return "YUY2";
	else if (videoFormat == MFVideoFormat_YV12)
		return "YV12";
	else if (videoFormat == MFVideoFormat_YVU9)
		return "YVU9";
	else if (videoFormat == MFVideoFormat_NV11)
		return "NV11";
	else if (videoFormat == MFVideoFormat_NV12)
		return "NV12";
	else if (videoFormat == MFVideoFormat_P010)
		return "P010";
	else if (videoFormat == MFVideoFormat_P016)
		return "P016";
	else if (videoFormat == MFVideoFormat_P210)
		return "P210";
	else if (videoFormat == MFVideoFormat_P216)
		return "P216";
	else if (videoFormat == MFVideoFormat_v210)
		return "v210";
	else if (videoFormat == MFVideoFormat_v216)
		return "v216";
	else if (videoFormat == MFVideoFormat_v410)
		return "v410";
	else if (videoFormat == MFVideoFormat_I420)
		return "I420";
	else if (videoFormat == MFVideoFormat_IYUV)
		return "IYUV";
	//else if (videoFormat == MFVideoFormat_U420) return "U420";
	else if (videoFormat == MFVideoFormat_Y210)
		return "Y210";
	else if (videoFormat == MFVideoFormat_Y216)
		return "Y216";
	else if (videoFormat == MFVideoFormat_Y410)
		return "Y410";
	else if (videoFormat == MFVideoFormat_Y416)
		return "Y416";
	else if (videoFormat == MFVideoFormat_Y41T)
		return "Y41T";
	else if (videoFormat == MFVideoFormat_Y42T)
		return "Y42T";
	//else if (videoFormat == MFVideoFormat_YVU420_MPEG2) return "YVU420_MPEG2";
	else if (videoFormat == MFVideoFormat_DVHD)
		return "DVHD";
	else if (videoFormat == MFVideoFormat_DVSD)
		return "DVSD";
	else if (videoFormat == MFVideoFormat_DVSL)
		return "DVSL";
	else if (videoFormat == MFVideoFormat_H264)
		return "H264";
	else if (videoFormat == MFVideoFormat_H265)
		return "H265";
	else if (videoFormat == MFVideoFormat_MPEG2)
		return "MPEG2";
	else if (videoFormat == MFVideoFormat_MPG1)
		return "MPG1";
	else if (videoFormat == MFVideoFormat_MPG2)
		return "MPG2";
	//else if (videoFormat == MFVideoFormat_MPG3) return "MPG3";
	//else if (videoFormat == MFVideoFormat_MPG4) return "MPG4";
	else if (videoFormat == MFVideoFormat_WMV1)
		return "WMV1";
	else if (videoFormat == MFVideoFormat_WMV2)
		return "WMV2";
	else if (videoFormat == MFVideoFormat_WMV3)
		return "WMV3";
	else if (videoFormat == MFVideoFormat_WVC1)
		return "WVC1";
	//else if (videoFormat == MFVideoFormat_FLV1) return "FLV1";
	else if (videoFormat == MFVideoFormat_HEVC)
		return "HEVC";
	// Ajoutez d'autres formats vidéo au besoin

	// Format inconnu
	return "Unknown";
}

Device::PixelFormat getDevicePixelFormat(const GUID &videoFormat) {
	if (videoFormat == MFVideoFormat_RGB24)
		return Device::PixelFormat::RGB24;
	else if (videoFormat == MFVideoFormat_NV12)
		return Device::PixelFormat::NV12;
	else if (videoFormat == MFVideoFormat_MJPG)
		return Device::PixelFormat::MJPEG;
	else if (videoFormat == MFVideoFormat_YUY2)
		return Device::PixelFormat::YUYV;

	// Format inconnu
	return Device::PixelFormat::Unknwon;
}

}// namespace

void updateList(std::vector<shared<video::Device>> &list) {
	if (!cc.addRef()) return;
	// check if all listed devices still exists
	if (std::remove_if(list.begin(), list.end(), [](const shared<video::Device> &dev) { return !std::static_pointer_cast<Device>(dev)->isValid(); }) != list.end()) { OWL_CORE_WARN("Possible problems during video input listing.") }

	WPointer<IMFAttributes> pConfig;
	HRESULT hr = MFCreateAttributes(pConfig.addr(), 1);

	// Request video capture devices.
	if (FAILED(hr)) {
		OWL_CORE_ERROR("{}updateList: Unable to create MFAttribute.", baseName)
		return;
	}
	// define what we search: Video input!
	hr = pConfig->SetGUID(
			MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE,
			MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE_VIDCAP_GUID);
	if (FAILED(hr)) {
		OWL_CORE_ERROR("{}updateList: Unable to setup attributes.", baseName)
		return;
	}
	std::vector<WPointer<IMFActivate>> devices;
	{
		IMFActivate **pDevices;
		uint32_t count;
		hr = MFEnumDeviceSources(pConfig.get(), &pDevices, &count);
		devices.reserve(count);
		for (uint32_t i = 0; i < count; ++i)
			devices.emplace_back(pDevices[i]);
	}
	if (devices.empty()) {
		OWL_CORE_WARN("{}updateList: No devices found.", baseName)
		return;
	}
	// search for new devices.
	size_t devCounter = 0;
	for (auto &devivce: devices) {
		auto testDev = mk_shared<Device>(devivce);
		if (!testDev->isValid()) { continue; }
		// don't add a device that already exists
		if (std::find_if(list.begin(), list.end(),
						 [&testDev](const shared<video::Device> &dev) { return testDev->getBusInfo() == static_pointer_cast<Device>(dev)->getBusInfo(); }) != list.end())
			continue;
		OWL_CORE_TRACE("Found: {} ({}) [{}] ", devCounter, testDev->getName(), testDev->getBusInfo())
		list.push_back(testDev);
		++devCounter;
	}
	cc.delRef();
}

Device::Device(WPointer<IMFActivate> &mfa) : owl::input::video::Device("") {
	if (!cc.addRef()) return;
	devActive.takeOwnershipFrom(mfa);
	{
		wchar_t *fName = nullptr;
		uint32_t len;
		devActive->GetAllocatedString(MF_DEVSOURCE_ATTRIBUTE_FRIENDLY_NAME, &fName, &len);
		std::wstring ws(fName);
		name = std::string(ws.begin(), ws.end());
		CoTaskMemFree(fName);
	}
	{
		wchar_t *fName = nullptr;
		uint32_t len;
		devActive->GetAllocatedString(MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE_VIDCAP_SYMBOLIC_LINK, &fName, &len);
		std::wstring ws(fName);
		busInfo = std::string(ws.begin(), ws.end());
		CoTaskMemFree(fName);
	}
}

Device::~Device() {
	close();
	cc.delRef();
}

void Device::open() {
	OWL_CORE_INFO("Opening device {}", name)
	// Media source activation!
	HRESULT hr = devActive->ActivateObject(IID_PPV_ARGS(mediaSource.addr()));
	if (FAILED(hr)) {
		OWL_CORE_WARN("Device ({}): Unable to activate media source.", name)
		close();
		return;
	}
	// Creation of media reader
	WPointer<IMFAttributes> pConfig;
	hr = MFCreateAttributes(pConfig.addr(), 1);
	if (FAILED(hr)) {
		OWL_CORE_ERROR("Device ({}): Unable to create MFAttribute.", name)
		return;
	}
	// define what we search: Video input!
	hr = pConfig->SetGUID(
			MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE,
			MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE_VIDCAP_GUID);
	if (FAILED(hr)) {
		OWL_CORE_ERROR("Device ({}): Unable to setup attributes.", name)
		return;
	}
	hr = MFCreateSourceReaderFromMediaSource(mediaSource.get(), pConfig.get(), sourceReader.addr());
	if (FAILED(hr)) {
		OWL_CORE_WARN("Device ({}): Unable to create media reader.", name)
		close();
		return;
	}
	WPointer<IMFMediaType> mediaType;
	hr = sourceReader->GetNativeMediaType(MF_SOURCE_READER_FIRST_VIDEO_STREAM, MF_SOURCE_READER_CURRENT_TYPE_INDEX, mediaType.addr());
	if (FAILED(hr)) {
		OWL_CORE_WARN("Device ({}): Unable to get native media type.", name)
		close();
		return;
	}
	uint64_t superSize;
	mediaType->GetUINT64(MF_MT_FRAME_SIZE, &superSize);
	size = {static_cast<uint32_t>((superSize >> 32) & mask32), static_cast<uint32_t>(superSize & mask32)};
	OWL_CORE_INFO("Device ({}): Frame size seen: {} {}.", name, size.getWidth(), size.getHeight())
	GUID format;
	mediaType->GetGUID(MF_MT_SUBTYPE, &format);
	OWL_CORE_INFO("Device ({}): Native media sub type: {}.", name, getPixelFormatString(format))
	pixFormat = getDevicePixelFormat(format);
	mediaType->SetUINT32(MF_MT_INTERLACE_MODE, MFVideoInterlace_Progressive);
	mediaType->SetUINT32(MF_MT_ALL_SAMPLES_INDEPENDENT, TRUE);
	hr = sourceReader->SetCurrentMediaType(MF_SOURCE_READER_FIRST_VIDEO_STREAM, nullptr, mediaType.get());
	if (FAILED(hr)) {
		OWL_CORE_WARN("Device ({}): Unable to set native media type.", name)
		close();
		return;
	}
}

void Device::close() {
	if (sourceReader) {
		sourceReader.release();
	}
	if (mediaSource) {
		mediaSource->Shutdown();
		mediaSource.release();
	}
	size = {1, 1};
}

bool Device::isOpened() const {
	return size.surface() > 1;
}

void Device::fillFrame(shared<renderer::Texture> &frame) {
	if (!isOpened())
		return;
	// Resizing the frame.
	if (size.surface() == 0)
		return;
	if (!frame || frame->getSize() != size) {
		frame = renderer::Texture2D::create(size, false);
	}
	// For now only fill with black
	WPointer<IMFSample> sample;
	int64_t timestamp;
	u_long actualIndex, sampleFlags;
	do {
		HRESULT hr = sourceReader->ReadSample(MF_SOURCE_READER_FIRST_VIDEO_STREAM, 0, &actualIndex, &sampleFlags, &timestamp, sample.addr());
		if (FAILED(hr)) {
			OWL_CORE_WARN("Device ({}): Unable to read sample from device.", name)
			return;
		}
	} while (sampleFlags & MF_SOURCE_READERF_STREAMTICK);
	u_long count;
	sample->GetBufferCount(&count);
	if (count == 0) {
		OWL_CORE_WARN("Device ({}): No  buffer found in the sample from device.", name)
		return;
	}
	WPointer<IMFMediaBuffer> buffer;
	HRESULT hr = sample->ConvertToContiguousBuffer(buffer.addr());
	if (FAILED(hr)) {
		OWL_CORE_WARN("Device ({}): Unable to Convert buffer.", name)
		return;
	}
	byte *byteBuffer;
	u_long bCurLen = 0;
	buffer->Lock(&byteBuffer, nullptr, &bCurLen);
	std::vector<byte> convertedBuffer = getRGBBuffer(byteBuffer, static_cast<int32_t>(bCurLen));
	if (size.surface() * 3 != convertedBuffer.size()) {
		OWL_CORE_WARN("Frame size missmatch {} buffer: {}.", size.surface() * 3, convertedBuffer.size())
		buffer->Unlock();
		return;
	}
	frame->setData(convertedBuffer.data(), static_cast<uint32_t>(convertedBuffer.size()));
	buffer->Unlock();
}

bool owl::input::video::windows::Device::isValid() const {
	return !name.empty() && !busInfo.empty();
}

}// namespace owl::input::video::windows

#endif
