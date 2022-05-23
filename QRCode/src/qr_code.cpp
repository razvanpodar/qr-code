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

	unsigned char align_data[25] = {
		100, 100, 100, 100, 100,
		100, 255, 255, 255, 100,
		100, 255, 100, 255, 100,
		100, 255, 255, 255, 100,
		100, 100, 100, 100, 100,
	};

	m_align = cv::Mat(5, 5, CV_8UC1, align_data);

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
	std::cout << micro_version << std::endl;

	if ((micro_version == 1)
		&& (!strcmp(m_error_correction, "M (15%)")
			|| !strcmp(m_error_correction, "Q (25%)")
			|| !strcmp(m_error_correction, "H (30%)")))
	{
		std::cout << "Error: wrong error correction level" << std::endl;
		m_code.release();
		return;
	}	
	if ((micro_version == 2 || micro_version == 3)
		&& (!strcmp(m_error_correction, "Q (25%)")
			|| !strcmp(m_error_correction, "H (30%)")))
	{
		std::cout << "Error: wrong error correction level" << std::endl;
		m_code.release();
		return;
	}
	if ((micro_version == 4) 
		&& (!strcmp(m_error_correction, "H (30%)")))
	{
		std::cout << "Error: wrong error correction level" << std::endl;
		m_code.release();
		return;
	}

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

	// Error correction level
	unsigned short ecl = micro_ecl.at(MicroECL(micro_version, m_error_correction));
	std::cout << "ECL: " << ecl << std::endl;
	
	int bit_1 = ecl & 1;
	int bit_2 = (ecl >> 1) & 1;
	int bit_3 = (ecl >> 2) & 1;

	std::cout << bit_3 << bit_2 << bit_1 << std::endl;

	if (bit_1)
		m_code.at<uchar>(8, 3) = 0;
	else
		m_code.at<uchar>(8, 3) = 255;

	if (bit_2)
		m_code.at<uchar>(8, 2) = 0;
	else
		m_code.at<uchar>(8, 2) = 255;
	
	if (bit_3)
		m_code.at<uchar>(8, 1) = 0;
	else
		m_code.at<uchar>(8, 1) = 255;

	std::cout << "code: (" << m_code.rows << ","
		<< m_code.cols << ")" << std::endl;
	//std::cout << m_code << std::endl;

	// Data mask
	int data_mask = 1;
	int data_mask_bit_1 = (data_mask) & 1;
	int data_mask_bit_2 = (data_mask >> 1) & 1;

	if (data_mask_bit_1)
		m_code.at<uchar>(8, 5) = 0;
	else
		m_code.at<uchar>(8, 5) = 255;

	if (data_mask_bit_2)
		m_code.at<uchar>(8, 4) = 0;
	else
		m_code.at<uchar>(8, 4) = 255;

	// Create buffer area
	cv::copyMakeBorder(m_code, m_code, 2, 2, 2, 2, cv::BORDER_CONSTANT, 255);

	// Save QR code
	// Will be moved to a method and called at the press of a button
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
	
	// Place alignment patterns
	if (version > 1)
	{
		std::vector<int> align_m = alignment_markers.at(version);
		//std::cout << align_m << std::endl;
		
		for (int i = 0; i < align_m.size(); i++)
		{
			for (int j = 0; j < align_m.size(); j++)
			{
				if (align_m[i] < 8 && align_m[j] < 8)
					continue;
				if (align_m[i] < 8 && align_m[j] > (m_size - 8))
					continue;
				if (align_m[j] < 8 && align_m[i] > (m_size - 8))
					continue;
				cv::Mat roi_marker(m_code, cv::Rect(align_m[i] - 2, align_m[j] - 2, 5, 5));
				std::cout << align_m[i] << ":" << align_m[j] << std::endl;

				m_align.copyTo(roi_marker);
				std::cout << "Done" << std::endl;
			}
		}
	}

	// Error correction level
	int ecl = standard_ecl.at(m_error_correction);
	
	int bit_1 = ecl & 1;
	int bit_2 = (ecl >> 1) & 1;

	if (bit_1)
		m_code.at<uchar>(8, 2) = 0;
	else
		m_code.at<uchar>(8, 2) = 255;

	if (bit_2)
		m_code.at<uchar>(8, 1) = 0;
	else
		m_code.at<uchar>(8, 1) = 255;

	// Data mask
	int data_mask = 1;
	int data_mask_bit_1 = (data_mask) & 1;
	int data_mask_bit_2 = (data_mask >> 1) & 1;
	int data_mask_bit_3 = (data_mask >> 2) & 1;

	if (data_mask_bit_1)
		m_code.at<uchar>(8, 5) = 0;
	else
		m_code.at<uchar>(8, 5) = 255;

	if (data_mask_bit_2)
		m_code.at<uchar>(8, 4) = 0;
	else
		m_code.at<uchar>(8, 4) = 255;

	if (data_mask_bit_3)
		m_code.at<uchar>(8, 3) = 0;
	else
		m_code.at<uchar>(8, 3) = 255;


	//std::cout << "code: (" << m_code.rows << "," 
	//	<< m_code.cols << ")" << std::endl;
	////std::cout << m_code << std::endl;



	// Create buffer area
	cv::copyMakeBorder(m_code, m_code, 4, 4, 4, 4, cv::BORDER_CONSTANT, 255);
}

bool QRCode::MicroMask(int i, int j, int mask)
{
	switch(mask) 
	{
	case 0:
		if (i % 2 == 0)
			return true;
	case 1:
		if (((i / 2) + (j / 3)) % 2 == 0)
			return true;
	case 2:
		if ((i * j + (i * j) % 3) % 2 == 0)
			return true;
	case 3:
		if ((i + j + (i * j) % 3) % 2 == 0)
			return true;
	default:
		return false;
	}

	return false;
}


