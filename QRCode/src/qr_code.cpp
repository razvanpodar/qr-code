#include "qr_code.h"

QRCode::QRCode()
{
	// Empty
}

QRCode::~QRCode()
{
	m_code.release();
}

void QRCode::GenerateCode()
{
	int version;
	std::string v = std::string(m_version);	
	std::istringstream(v) >> version;

	unsigned char data[64] = {
		100, 100, 100, 100, 100, 100, 100, 255,
		100, 255, 255, 255, 255, 255, 100, 255,
		100, 255, 100, 100, 100, 255, 100, 255,
		100, 255, 100, 100, 100, 255, 100, 255,
		100, 255, 100, 100, 100, 255, 100, 255,
		100, 255, 255, 255, 255, 255, 100, 255,
		100, 100, 100, 100, 100, 100, 100, 255,
		255, 255, 255, 255, 255, 255, 255, 255
	};

	m_locator = cv::Mat(8, 8, CV_8UC1, data);

	if (version > 0)
		GenerateStandardCode(version);
	else
		GenerateMicroCode();

}

bool QRCode::IsEmpty()
{
	return m_code.empty();
}

void QRCode::GenerateMicroCode()
{
	int micro_version = m_version[1] - 48;

	m_size = M1_SIZE + ((micro_version - 1) * 2);

	// TODO: Set type based on the encoding
	m_code = cv::Mat(m_size, m_size, CV_8UC1, 255);

	for (int i = 0; i < m_size; i++)
	{
		for (int j = 0; j < m_size; j++)
		{
			if ((i + j) % 2 == 0)
				m_code.at<uchar>(i, j) = 100;
			else
				m_code.at<uchar>(i, j) = 255;
		}
	}

	// Place locator
	cv::Mat roi(m_code, cv::Rect(0, 0, 8, 8));
	m_locator.copyTo(roi);

	std::cout << "code: (" << m_code.rows << ","
		<< m_code.cols << ")" << std::endl;
	//std::cout << m_code << std::endl;

	// Create buffer area
	cv::copyMakeBorder(m_code, m_code, 2, 2, 2, 2, cv::BORDER_CONSTANT, 255);

	//cv::imwrite("test.png", m_code);
}

void QRCode::GenerateStandardCode(int version)
{
	m_size = V1_SIZE + ((version - 1) * 4) ;

	// TODO: Set type based on the encoding
	m_code = cv::Mat(m_size, m_size, CV_8UC1, 255);

	int colors[2] = { 155, 255 };

	for (int i = 0; i < m_size; i++)
	{
		for (int j = 0; j < m_size; j++)
		{
			
			if ((i + j) % 2 == 0)
				m_code.at<uchar>(i, j) = 100;
			else
				m_code.at<uchar>(i, j) = 255;
		}

	}

	// Place locators
	cv::Mat roi_1(m_code, cv::Rect(0, 0, 8, 8));
	m_locator.copyTo(roi_1);

	cv::Mat roi_2(m_code, cv::Rect(m_size - 8, 0, 8, 8));
	cv::Mat x_mirrored_locator;
	cv::flip(m_locator, x_mirrored_locator, 1);
	x_mirrored_locator.copyTo(roi_2);	
	
	cv::Mat roi_3(m_code, cv::Rect(0, m_size - 8, 8, 8));
	cv::Mat y_mirrored_locator;
	cv::flip(m_locator, y_mirrored_locator, 0);
	y_mirrored_locator.copyTo(roi_3);
	
	std::cout << "code: (" << m_code.rows << "," 
		<< m_code.cols << ")" << std::endl;
	//std::cout << m_code << std::endl;

	// Create buffer area
	cv::copyMakeBorder(m_code, m_code, 4, 4, 4, 4, cv::BORDER_CONSTANT, 255);
}