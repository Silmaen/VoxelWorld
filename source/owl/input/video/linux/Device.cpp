/**
 * @file Device.cpp
 * @author Silmaen
 * @date 03/01/2024
 * Copyright © 2024 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "Device.h"

#if defined(OWL_PLATFORM_LINUX)
#include "../Manager.h"

#include <fcntl.h>
#include <filesystem>
#include <linux/media.h>
#include <linux/videodev2.h>
#include <sys/ioctl.h>
#include <sys/mman.h>

namespace owl::input::video::linux64 {

namespace {

std::string getPixelFormatString(const uint32_t pixelFormat) {
	switch (pixelFormat) {
		case V4L2_PIX_FMT_RGB24:
			return "RGB24";
		case V4L2_PIX_FMT_YUV420:
			return "YUV420";
		case V4L2_PIX_FMT_YUYV:
			return "YUYV";
		case V4L2_PIX_FMT_UYVY:
			return "UYVY";
		case V4L2_PIX_FMT_NV12:
			return "NV12";
		case V4L2_PIX_FMT_YVU420:
			return "YVU420";
		case V4L2_PIX_FMT_YUV422P:
			return "YUV422P";
		case V4L2_PIX_FMT_YUV411P:
			return "YUV411P";
		case V4L2_PIX_FMT_YUV410:
			return "YUV410";
		case V4L2_PIX_FMT_NV21:
			return "NV21";
		case V4L2_PIX_FMT_NV16:
			return "NV16";
		case V4L2_PIX_FMT_GREY:
			return "GREY";
		case V4L2_PIX_FMT_YVU410:
			return "YVU410";
		case V4L2_PIX_FMT_Y41P:
			return "Y41P";
		case V4L2_PIX_FMT_BGR24:
			return "BGR24";
		case V4L2_PIX_FMT_ABGR32:
			return "ABGR32";
		case V4L2_PIX_FMT_ARGB32:
			return "ARGB32";
		case V4L2_PIX_FMT_YVU420M:
			return "YVU420M";
		case V4L2_PIX_FMT_YVU422M:
			return "YVU422M";
		case V4L2_PIX_FMT_YVU444M:
			return "YVU444M";
		case V4L2_PIX_FMT_SBGGR8:
			return "SBGGR8";
		case V4L2_PIX_FMT_SGBRG8:
			return "SGBRG8";
		case V4L2_PIX_FMT_SGRBG8:
			return "SGRBG8";
		case V4L2_PIX_FMT_SRGGB8:
			return "SRGGB8";
		case V4L2_PIX_FMT_MJPEG:
			return "MJPEG";
		// Ajoutez d'autres formats selon vos besoins
		default:
			return "Unknown";
	}
}

std::string unFourCC(const uint32_t pixelFormat) {
	constexpr uint32_t mmask = 0xff;
	char fcc[4];
	fcc[0] = static_cast<char>(pixelFormat & mmask);
	fcc[1] = static_cast<char>((pixelFormat >> 8) & mmask);
	fcc[2] = static_cast<char>((pixelFormat >> 16) & mmask);
	fcc[3] = static_cast<char>((pixelFormat >> 24) & mmask);
	return fcc;
}

Device::PixelFormat getDevicePixelFormat(const uint32_t pixelFormat) {
	switch (pixelFormat) {
		case V4L2_PIX_FMT_RGB24:
			return Device::PixelFormat::RGB24;
		case V4L2_PIX_FMT_NV12:
			return Device::PixelFormat::NV12;
		case V4L2_PIX_FMT_YUYV:
			return Device::PixelFormat::YUYV;
		case V4L2_PIX_FMT_MJPEG:
			return Device::PixelFormat::MJPEG;
		default:
			return Device::PixelFormat::Unknwon;
	}
}

uint32_t getV4L2PixelFormat(const Device::PixelFormat &pixelFormat) {
	switch (pixelFormat) {
		case Device::PixelFormat::RGB24:
			return V4L2_PIX_FMT_RGB24;
		case Device::PixelFormat::NV12:
			return V4L2_PIX_FMT_NV12;
		case Device::PixelFormat::YUYV:
			return V4L2_PIX_FMT_YUYV;
		case Device::PixelFormat::MJPEG:
			return V4L2_PIX_FMT_MJPEG;
		default:
			return 0;
	}
}

}// namespace


void updateList(std::vector<shared<video::Device>> &list) {
	// check if all listed devices still exists
	if (std::remove_if(list.begin(), list.end(), [](const shared<video::Device> &dev) { return !std::static_pointer_cast<Device>(dev)->isValid(); }) != list.end()) { OWL_CORE_WARN("Possible problems during video input listing.") }

	// search for new devices.
	size_t devCounter = 0;
	for (size_t iCam = 0; iCam < maxDevices; ++iCam) {
		auto testDev = mk_shared<Device>(fmt::format("/dev/video{}", iCam).c_str());
		if (!testDev->isValid()) { continue; }
		// don't add a device that already exists
		if (std::find_if(list.begin(), list.end(),
						 [&testDev](const shared<video::Device> &dev) { return testDev->getBusInfo() == static_pointer_cast<Device>(dev)->getBusInfo(); }) != list.end())
			continue;
		OWL_CORE_TRACE("Found: {} ({}) [{}] ", devCounter, testDev->getName(), testDev->getBusInfo())
		list.push_back(testDev);
		++devCounter;
	}
}

Device::Device(std::string _file)
	: video::Device{""}, file{std::move(_file)} {
	if (!exists(std::filesystem::path(file))) return;

	const int fd = ::open(file.c_str(), O_RDONLY);
	if (!fd) {
		OWL_CORE_WARN("Unable to open device file {}", file)
		return;
	}
	v4l2_capability capability{};
	const int vcap = ioctl(fd, VIDIOC_QUERYCAP, &capability);
	if (vcap < 0) {
		OWL_CORE_WARN("Unable to query the device capabilities.")
		::close(fd);
		return;
	}
	if (vcap == 0) {
		name = reinterpret_cast<const char *>(capability.card);
		busInfo = reinterpret_cast<const char *>(capability.bus_info);
	} else {
		// second method
		media_device_info mdi{};
		if (ioctl(fd, MEDIA_IOC_DEVICE_INFO, &mdi) > 0) {
			if (mdi.bus_info[0]) busInfo = mdi.bus_info;
			else
				busInfo = fmt::format("platform: {}", mdi.driver);
			if (mdi.model[0]) name = mdi.model;
			else
				name = mdi.driver;
		}
	}
	::close(fd);
}

Device::~Device() { close(); };

void Device::open() {
	OWL_CORE_INFO("Opening device ({}): {}", file, name)
	close();
	fileHandler = ::open(file.c_str(), O_RDWR);
	if (fileHandler <= 0) {
		OWL_CORE_WARN("({}) Unable to open the device.", file)
		return;
	}
	// manage format
	{
		v4l2_format fmt{};
		fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		if (ioctl(fileHandler, VIDIOC_G_FMT, &fmt) < 0) {
			OWL_CORE_WARN("({}) Unable to get format of device.", file);
			close();
			return;
		}
		if (!isPixelFormatSupported(getDevicePixelFormat(fmt.fmt.pix.pixelformat))) {
			OWL_CORE_WARN("({}) Native pixel format unsupported!", file, getPixelFormatString(V4L2_PIX_FMT_NV12))
			OWL_CORE_WARN("({}) fourcc: {}", file, unFourCC(fmt.fmt.pix.pixelformat))
			fmt.fmt.pix.pixelformat = getV4L2PixelFormat(getFirstSupportedPixelFormat());
			if (ioctl(fileHandler, VIDIOC_S_FMT, &fmt) < 0) {
				OWL_CORE_WARN("({}) Unable to set format of device", file)
				close();
				return;
			}
			if (!isPixelFormatSupported(getDevicePixelFormat(fmt.fmt.pix.pixelformat))) {
				OWL_CORE_WARN("({}) Unable to set device to a suitable format", file)
				close();
				return;
			}
		}
		if (ioctl(fileHandler, VIDIOC_S_FMT, &fmt) < 0) {
			OWL_CORE_WARN("({}) Unable to set format of device", file)
			close();
			return;
		}
		OWL_CORE_INFO("({}) Using size is {} x {}", file, fmt.fmt.pix.width, fmt.fmt.pix.height)
		OWL_CORE_INFO("({}) Using pixel format: {}", file, getPixelFormatString(fmt.fmt.pix.pixelformat))
		pixFormat = getDevicePixelFormat(fmt.fmt.pix.pixelformat);
		size = {fmt.fmt.pix.width,
				fmt.fmt.pix.height};
	}
	// request buffer from device.
	{
		v4l2_requestbuffers requestBuffers{};
		requestBuffers.count = 1;// only one buffer requested.
		requestBuffers.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		requestBuffers.memory = V4L2_MEMORY_MMAP;
		if (ioctl(fileHandler, VIDIOC_REQBUFS, &requestBuffers) < 0) {
			OWL_CORE_WARN("({}) Unable to request buffer.", file)
			close();
			return;
		}
	}
	// define the bufferInfo
	{
		memset(&bufferInfo, 0, sizeof(bufferInfo));
		bufferInfo.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		bufferInfo.memory = V4L2_MEMORY_MMAP;
		bufferInfo.index = 0;
	}
	// query buffer from device
	{
		if (ioctl(fileHandler, VIDIOC_QUERYBUF, &bufferInfo) < 0) {
			OWL_CORE_WARN("({}) Unable to query buffer.", file)
			close();
			return;
		}
		OWL_CORE_INFO("({}) Buffer info, length {} , offset {}.", file, bufferInfo.length, bufferInfo.m.offset)
		buffer = mmap(nullptr, bufferInfo.length, PROT_READ | PROT_WRITE, MAP_SHARED, fileHandler, bufferInfo.m.offset);
		if (buffer == MAP_FAILED) {
			OWL_CORE_ERROR("({}) Unable to map the device buffer.", file)
			close();
			return;
		}
	}
	//Activate the streaming
	{
		uint32_t type = bufferInfo.type;
		if (ioctl(fileHandler, VIDIOC_STREAMON, &type) < 0) {
			OWL_CORE_WARN("({}) Unable to start the streaming.", file)
			close();
			return;
		}
		streaming = true;
	}
	// queue the buffer
	if (ioctl(fileHandler, VIDIOC_QBUF, &bufferInfo) < 0) {
		OWL_CORE_WARN("Device ({}) unable to queue the buffer.", file)
		close();
		return;
	}
}

void Device::close() {
	if (!isOpened()) return;
	if (buffer)
		munmap(buffer, bufferInfo.length);
	if (streaming) {
		if (ioctl(fileHandler, VIDIOC_STREAMOFF, &bufferInfo) < 0) {
			OWL_CORE_WARN("({}) Unable to close the streaming.", file)
		}
	}
	::close(fileHandler);
	fileHandler = 0;
	size = {1, 1};
	buffer = nullptr;
	streaming = false;
}

bool Device::isOpened() const { return fileHandler != 0; }

void Device::fillFrame(shared<renderer::Texture> &frame) {
	if (!streaming) return;// need to be open and ready!
	// recreate the frame if not the right size.
	if (!frame || frame->getSize() != size) {
		frame = renderer::Texture2D::create(size, false);
	}
	// dequeue the buffer
	if (ioctl(fileHandler, VIDIOC_DQBUF, &bufferInfo) < 0) {
		OWL_CORE_WARN("Device ({}) unable to dequeue the buffer.", file)
		return;
	}

	std::vector<uint8_t> convertedBuffer = getRGBBuffer(static_cast<const uint8_t *>(buffer), bufferInfo.bytesused);
	// frames are written after dequeing the buffer
	if (!convertedBuffer.empty()) {
		if (size.surface() * 3 != convertedBuffer.size()) {
			OWL_CORE_WARN("Device ({}) buffer size missmatch: {}, expecting {}.", file, bufferInfo.bytesused, size.surface() * 3)
		} else {
			frame->setData(convertedBuffer.data(), convertedBuffer.size());
		}
	}
	// queue the buffer
	if (ioctl(fileHandler, VIDIOC_QBUF, &bufferInfo) < 0) {
		OWL_CORE_WARN("Device ({}) unable to queue the buffer.", file)
	}
}

bool Device::isValid() const {
	// fast out: open is assumed valid.
	if (isOpened()) return true;

	// check if realy initiated.
	if (file.empty()) return false;
	if (name.empty()) return false;
	if (busInfo.empty()) return false;

	// check if still exists and still a video source.
	if (!exists(std::filesystem::path(file))) return false;
	const int fd = ::open(file.c_str(), O_RDWR);
	if (!fd) return false;
	// check have supported capabilities
	{
		v4l2_capability capability{};
		if (ioctl(fd, VIDIOC_QUERYCAP, &capability) < 0) {
			OWL_CORE_TRACE("Device {} ({}) unable to get Video capability.", name, file)
			::close(fd);
			return false;
		}
		if (!(capability.capabilities & V4L2_CAP_VIDEO_CAPTURE)) {
			OWL_CORE_TRACE("Device {} ({}) does not have Video capability.", name, file)
			::close(fd);
			return false;// not a capture device.
		}
		if (!(capability.capabilities & V4L2_CAP_STREAMING)) {
			OWL_CORE_TRACE("Device {} ({}) does not have streaming capability.", name, file)
			::close(fd);
			return false;// no read capability.
		}
	}
	// check have supported pixelformat.
	{
		if (getFirstSupportedPixelFormat(fd) == PixelFormat::Unknwon) {
			OWL_CORE_TRACE("Device {} ({}) Does not have a supported pixel format.", name, file)
			::close(fd);
			printSupportedFormat();
			return false;
		}
	}
	::close(fd);
	// everything seems ok.
	return true;
}

void Device::printSupportedFormat() const {
	if (!exists(std::filesystem::path(file))) return;
	const int fd = ::open(file.c_str(), O_RDWR);
	if (!fd) return;
	// Structure pour interroger les formats de pixel
	v4l2_fmtdesc formatDescription{};
	formatDescription.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

	// Boucle pour interroger chaque format de pixel
	while (ioctl(fd, VIDIOC_ENUM_FMT, &formatDescription) == 0) {
		OWL_CORE_INFO("({}) index {} format {}", file, formatDescription.index, getPixelFormatString(formatDescription.pixelformat))
		// Incrémentation pour passer au format de pixel suivant
		++formatDescription.index;
	}
	::close(fd);
}

Device::PixelFormat Device::getFirstSupportedPixelFormat(int32_t fd) const {
	if (fd == -1)
		fd = fileHandler;
	if (fd == -1)
		return PixelFormat::Unknwon;
	v4l2_fmtdesc formatDescription{};
	formatDescription.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	while (ioctl(fd, VIDIOC_ENUM_FMT, &formatDescription) == 0) {
		if (const auto pix = getDevicePixelFormat(formatDescription.pixelformat); isPixelFormatSupported(pix))
			return pix;
		++formatDescription.index;
	}
	return PixelFormat::Unknwon;
}

}// namespace owl::input::video::linux64

#endif
