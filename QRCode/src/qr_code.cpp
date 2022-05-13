#include "qr_code.h"

QRCode::QRCode()
{
	// Empty
}

QRCode::~QRCode()
{
	// Empty
	m_code.release();
}

void QRCode::GenerateCode()
{
	int version;
	std::string v = std::string(m_version);	
	std::istringstream(v) >> version;

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

	int colors[2] = { 155, 255 };

	for (int i = 0; i < m_size; i++)
	{
		for (int j = 0; j < m_size; j++)
		{
			if ((i + j) % 2 == 0)
				m_code.at<uchar>(i, j) = 255;
			else
				m_code.at<uchar>(i, j) = 155;
		}

	}

	std::cout << "code: (" << m_code.rows << ","
		<< m_code.cols << ")" << std::endl;

	// Create buffer area
	cv::copyMakeBorder(m_code, m_code, 2, 2, 2, 2, cv::BORDER_CONSTANT, 255);

	//cv::imwrite("test.png", m_code);
}

void QRCode::GenerateStandardCode(int version)
{
	m_size = V1_SIZE + ((version - 1) * 4) ;

	// TODO: Set type based on the encoding
	m_code = cv::Mat(m_size, m_size, CV_8UC1, 155);

	int colors[2] = { 155, 255 };

	for (int i = 0; i < m_size; i++)
	{
		for (int j = 0; j < m_size; j++)
		{
			
			if ((i + j) % 2 == 0)
				m_code.at<uchar>(i, j) = 255;
			else
				m_code.at<uchar>(i, j) = 155;
		}

	}
	
	std::cout << "code: (" << m_code.rows << "," 
		<< m_code.cols << ")" << std::endl;

	// Create buffer area
	cv::copyMakeBorder(m_code, m_code, 4, 4, 4, 4, cv::BORDER_CONSTANT, 255);
}