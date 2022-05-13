#pragma once

#include <string>
#include <iostream>
#include <sstream>

#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc/imgproc.hpp"

constexpr int M1_SIZE = 11;
constexpr int V1_SIZE = 21;

class QRCode
{
private:
	cv::Mat m_code;
	int m_size;

public:
	const char* m_version = "M1";
	const char* m_error_correction = "L";
	const char* m_encoding = "Alphanumeric";
	char m_input[50] = { 0 };

public:
	QRCode();
	~QRCode();

	void GenerateCode();
	bool IsEmpty();
	cv::Mat GetCode()
	{
		return m_code;
	}

private:
	void GenerateMicroCode();
	void GenerateStandardCode(int version);
};