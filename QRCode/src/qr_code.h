#pragma once

#include <string>
#include <iostream>
#include <sstream>
#include <map>
#include <vector>

#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc/imgproc.hpp"

constexpr int M1_SIZE = 11;
constexpr int V1_SIZE = 21;

struct MicroECL 
{
	int version;
	const char* ecl;

	MicroECL(int v, const char* tecl)
		: version(v), ecl(tecl)
	{ }

	bool operator< (const MicroECL& m_ecl) const
	{
		int d1 = version;
		int d2 = m_ecl.version;

		int i = 0;
		while (ecl[i] != '\0')
		{
			d1 += ecl[i];
			i++;
		}

		i = 0;
		while (m_ecl.ecl[i] != '\0')
		{
			d2 += m_ecl.ecl[i];
			i++;
		}

		return d1 < d2;
	}
};

const std::map<MicroECL, unsigned short> micro_ecl = {
	{ MicroECL(1, "L (7%)"), 0 },
	{ MicroECL(2, "L (7%)"), 1 },
	{ MicroECL(2, "M (15%)"), 2 },
	{ MicroECL(3, "L (7%)"), 3 },
	{ MicroECL(3, "M (15%)"), 4 },
	{ MicroECL(4, "L (7%)"), 5 },
	{ MicroECL(4, "M (15%)"), 6 },
	{ MicroECL(4, "Q (25%)"), 7 }
};

const std::map<const char*, unsigned short> standard_ecl = {
	{"L (7%)", 1},
	{"M (15%)", 0},
	{"Q (25%)", 3},
	{"H (30%)", 2}
};

const std::map<int, std::vector<int>> alignment_markers = {
{2, std::vector<int> { 6, 18 }}, {3, std::vector<int> { 6, 22 }},
{4, std::vector<int> { 6, 26 }}, {5, std::vector<int> { 6, 30 }},
{6, std::vector<int> { 6, 34 }}, {7, std::vector<int> { 6, 22, 38 }},
{8, std::vector<int> { 6, 24, 42 }}, {9, std::vector<int> { 6, 26, 46 }},
{10, std::vector<int> { 6, 28, 50 }}, {11, std::vector<int> { 6, 30, 54 }},
{12, std::vector<int> { 6, 32, 58 }}, {13, std::vector<int> { 6, 34, 62 }},
{14, std::vector<int> { 6, 26, 46, 66 }},
{15, std::vector<int> { 6, 26, 48, 70 }},
{16, std::vector<int> { 6, 26, 50, 74 }},
{17, std::vector<int> { 6, 30, 54, 78 }},
{18, std::vector<int> { 6, 30, 56, 82 }},
{19, std::vector<int> { 6, 30, 58, 86 }},
{20, std::vector<int> { 6, 34, 62, 90 }},
{21, std::vector<int> { 6, 28, 50, 72, 94 }},
{22, std::vector<int> { 6, 26, 50, 74, 98 }},
{23, std::vector<int> { 6, 30, 54, 78, 102 }},
{24, std::vector<int> { 6, 28, 54, 80, 106 }},
{25, std::vector<int> { 6, 32, 58, 84, 110 }},
{26, std::vector<int> { 6, 30, 58, 86, 114 }},
{27, std::vector<int> { 6, 34, 62, 90, 118 }},
{28, std::vector<int> { 6, 26, 50, 74, 98, 122 }},
{29, std::vector<int> { 6, 30, 54, 78, 102, 126 }},
{30, std::vector<int> { 6, 26, 52, 78, 104, 130 }},
{31, std::vector<int> { 6, 30, 56, 82, 108, 134 }},
{32, std::vector<int> { 6, 34, 60, 86, 112, 138 }},
{33, std::vector<int> { 6, 30, 58, 86, 114, 142 }},
{34, std::vector<int> { 6, 34, 62, 90, 118, 146 }},
{35, std::vector<int> { 6, 30, 54, 78, 102, 126, 150 }},
{36, std::vector<int> { 6, 24, 50, 76, 102, 128, 154 }},
{37, std::vector<int> { 6, 28, 54, 80, 106, 132, 158 }},
{38, std::vector<int> { 6, 32, 58, 84, 110, 136, 162 }},
{39, std::vector<int> { 6, 26, 54, 82, 110, 138, 166 }},
{40, std::vector<int> { 6, 30, 58, 86, 114, 142, 170 }}
};

class QRCode
{
private:
	cv::Mat m_code;
	cv::Mat m_locator;
	cv::Mat m_align;
	int m_size;

public:
	const char* m_version = "M1";
	const char* m_error_correction = "L (7%)";
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
	bool MicroMask(int i, int j, int mask);
	bool StandardMask(int mask);
};