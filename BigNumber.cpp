#include <algorithm>
#include <cstring>

#include "BigNumber.hpp"

namespace vp {

/**
 * @brief Construct a new BigNumber:: BigNumber object \n
 * The numeric string is "0"
 * 
 */
BigNumber::BigNumber()
{
	init("0");
}

/**
 * @brief Construct a new BigNumber:: BigNumber object
 * 
 * @param val A numeric string
 */
BigNumber::BigNumber(const std::string &val)
{
	init(val);
}

/**
 * @brief Construct a new BigNumber:: BigNumber object
 * 
 * @param val           A numeric string
 * @param nMaxFracLen   Max length of the fractional part
 */
BigNumber::BigNumber(const std::string &val, std::size_t nMaxFracLen)
{
	init(val);
	m_nMaxFracLen = nMaxFracLen;
	roundDown(nMaxFracLen+1);
}

/**
 * @brief Construct a new BigNumber:: BigNumber object \n
 *  The numeric string is "0"
 * 
 * @param nMaxFracLen   Max length of the fractional part
 */
BigNumber::BigNumber(std::size_t nMaxFracLen)
{
	init("0");
	m_nMaxFracLen = nMaxFracLen;
}

/**
 * @brief Destroy the BigNumber:: BigNumber object
 * 
 */
BigNumber::~BigNumber()
{

}

/**
 * @brief Print the number
 * 
 * @param os 
 * @param rhs 
 * @return std::ostream& 
 */
std::ostream& operator<<(std::ostream& os, const BigNumber& rhs)
{
	std::string strRet;

	if (rhs.m_bIsNegativeSign) strRet.push_back('-');
	strRet.append(rhs.m_strVal.substr(0, rhs.m_strVal.length()-rhs.m_nFracLen));

	if (rhs.m_nFracLen > 0) {
		strRet.push_back('.');
		strRet.append(rhs.m_strVal.substr(rhs.m_strVal.length()-rhs.m_nFracLen, rhs.m_nFracLen));
	}

	os << strRet;
	return os;
}

/**
 * @brief Add two numbers
 * 
 * @param rhs 
 * @return BigNumber 
 */
BigNumber BigNumber::operator+(const BigNumber &rhs)
{
	BigNumber clsRet;

	BigNumber clsVal1 = *this;
	BigNumber clsVal2 = rhs;

	adjNum(clsVal1, clsVal2);

	if (!clsVal1.m_bIsNegativeSign && !clsVal2.m_bIsNegativeSign) {
		// (+X) + (+Y)
		clsRet.m_strVal = addNum(clsVal1.m_strVal, clsVal2.m_strVal);
		clsRet.m_bIsNegativeSign = false;
	}
	else if (!clsVal1.m_bIsNegativeSign && clsVal2.m_bIsNegativeSign) {
		// (+X) + (-Y)
		std::pair<bool, std::string> prVal = subNumRetWithSign(clsVal1.m_strVal, clsVal2.m_strVal);
		clsRet.m_strVal = prVal.second;
		clsRet.m_bIsNegativeSign = prVal.first;
	}
	else if (clsVal1.m_bIsNegativeSign && !clsVal2.m_bIsNegativeSign) {
		// (-X) + (+Y)
		std::pair<bool, std::string> prVal = subNumRetWithSign(clsVal2.m_strVal, clsVal1.m_strVal);
		clsRet.m_strVal = prVal.second;
		clsRet.m_bIsNegativeSign = prVal.first;
	}
	else {
		// (-X) + (-Y)
		clsRet.m_strVal = addNum(clsVal1.m_strVal, clsVal2.m_strVal);
		clsRet.m_bIsNegativeSign = true;
	}
	clsRet.m_nFracLen = clsVal1.m_nFracLen;
	clsRet.m_nMaxFracLen = getMaxFracLen(clsVal1, clsVal2);

	clsRet.trim();

	return clsRet; 
}

/**
 * @brief Add two numbers and assign
 * 
 * @param rhs 
 * @return BigNumber 
 */
auto BigNumber::operator+=(const BigNumber &rhs) -> BigNumber&
{
	*this = *this + rhs;
	return *this;
}

/**
 * @brief Substract two numbers
 * 
 * @param rhs 
 * @return BigNumber 
 */
BigNumber BigNumber::operator-(const BigNumber &rhs)
{
	BigNumber clsRet;

	BigNumber clsVal1 = *this;
	BigNumber clsVal2 = rhs;

	adjNum(clsVal1, clsVal2);

	if (!clsVal1.m_bIsNegativeSign && !clsVal2.m_bIsNegativeSign) {
		// (+X) - (+Y)
		std::pair<bool, std::string> prVal = subNumRetWithSign(clsVal1.m_strVal, clsVal2.m_strVal);
		clsRet.m_strVal = prVal.second;
		clsRet.m_bIsNegativeSign = prVal.first;
	}
	else if (!clsVal1.m_bIsNegativeSign && clsVal2.m_bIsNegativeSign) {
		// (+X) - (-Y)
		clsRet.m_strVal = addNum(clsVal1.m_strVal, clsVal2.m_strVal);
		clsRet.m_bIsNegativeSign = false;
	}
	else if (clsVal1.m_bIsNegativeSign && !clsVal2.m_bIsNegativeSign) {
		// (-X) - (+Y)
		clsRet.m_strVal = addNum(clsVal1.m_strVal, clsVal2.m_strVal);
		clsRet.m_bIsNegativeSign = true;
	}
	else {
		// (-X) - (-Y)
		std::pair<bool, std::string> prVal = subNumRetWithSign(clsVal2.m_strVal, clsVal1.m_strVal);
		clsRet.m_strVal = prVal.second;
		clsRet.m_bIsNegativeSign = prVal.first;
	}
	clsRet.m_nFracLen = clsVal1.m_nFracLen;
	clsRet.m_nMaxFracLen = getMaxFracLen(clsVal1, clsVal2);

	clsRet.trim();

	return clsRet; 
}

/**
 * @brief Substract two numbers and assign
 * 
 * @param rhs 
 * @return BigNumber 
 */
auto BigNumber::operator-=(const BigNumber &rhs) -> BigNumber&
{
	*this = *this - rhs;
	return *this;
}

/**
 * @brief Multiply two numbers
 * 
 * @param rhs 
 * @return BigNumber 
 */
BigNumber BigNumber::operator*(const BigNumber &rhs)
{
	BigNumber clsRet;

	std::string strVal;

	strVal = mulNum(m_strVal, rhs.m_strVal);
	clsRet.m_strVal = strVal;
	clsRet.m_bIsNegativeSign = m_bIsNegativeSign ^ rhs.m_bIsNegativeSign;
	clsRet.m_nFracLen = m_nFracLen + rhs.m_nFracLen;
	clsRet.m_nMaxFracLen = getMaxFracLen(*this, rhs);

	clsRet.trim();
	clsRet.round(-1*(clsRet.m_nMaxFracLen+1));

	return clsRet;
}

/**
 * @brief Multiply two numbers and assign
 * 
 * @param rhs 
 * @return BigNumber 
 */
auto BigNumber::operator*=(const BigNumber &rhs) -> BigNumber&
{
	*this = *this * rhs;
	return *this;
}

/**
 * @brief Divide two numbers
 * 
 * @param rhs 
 * @return BigNumber 
 */
BigNumber BigNumber::operator/(const BigNumber &rhs)
{
	BigNumber clsRet;

	if (rhs.m_strVal == "0") {
		throw std::runtime_error("Arithmetic error : Attempted to divide by Zero [" + m_strVal + " / " + rhs.m_strVal + "]");
	}

	BigNumber clsVal1 = *this;
	BigNumber clsVal2 = rhs;
	std::size_t nMaxFracLen = getMaxFracLen(clsVal1, clsVal2);
	std::size_t nMaxFracLen_1 = nMaxFracLen + 1;

	adjNum(clsVal1, clsVal2);
	std::string strVal = divNum(clsVal1.m_strVal, clsVal2.m_strVal, nMaxFracLen_1);
	clsRet.m_strVal = strVal;
	clsRet.m_bIsNegativeSign = clsVal1.m_bIsNegativeSign ^ clsVal2.m_bIsNegativeSign;
	clsRet.m_nFracLen = nMaxFracLen_1;
	clsRet.m_nMaxFracLen = nMaxFracLen_1;

	clsRet.trim();
	clsRet.round(-1*(nMaxFracLen_1));
	clsRet.m_nMaxFracLen = nMaxFracLen;

	return clsRet;
}

/**
 * @brief Divide two numbers and assign
 * 
 * @param rhs 
 * @return BigNumber 
 */
auto BigNumber::operator/=(const BigNumber &rhs) -> BigNumber&
{
	*this = *this / rhs;
	return *this;
}

/**
 * @brief Complare two numbers (Not equal)
 * 
 * @param rhs 
 * @return BigNumber 
 */
auto BigNumber::operator!=(const BigNumber &rhs) const -> bool
{
	return cmpNum(*this, rhs) != 0 ? true : false;
}

/**
 * @brief Complare two numbers (equal)
 * 
 * @param rhs 
 * @return BigNumber 
 */
auto BigNumber::operator==(const BigNumber &rhs) const -> bool
{
	return cmpNum(*this, rhs) == 0 ? true : false;
}

/**
 * @brief Complare two numbers (greater)
 * 
 * @param rhs 
 * @return BigNumber 
 */
auto BigNumber::operator>(const BigNumber &rhs) const -> bool
{
	return cmpNum(*this, rhs) > 0 ? true : false;
}

/**
 * @brief Complare two numbers (greater or equal)
 * 
 * @param rhs 
 * @return BigNumber 
 */
auto BigNumber::operator>=(const BigNumber &rhs) const -> bool
{
	return cmpNum(*this, rhs) >= 0 ? true : false;
}

/**
 * @brief Complare two numbers (less)
 * 
 * @param rhs 
 * @return BigNumber 
 */
auto BigNumber::operator<(const BigNumber &rhs) const -> bool
{
	return cmpNum(*this, rhs) < 0 ? true : false;
}

/**
 * @brief Complare two numbers (less or equal)
 * 
 * @param rhs 
 * @return BigNumber 
 */
auto BigNumber::operator<=(const BigNumber &rhs) const -> bool
{
	return cmpNum(*this, rhs) <= 0 ? true : false;
}

/**
 * @brief Set max denominator length of a decimal fraction
 * 
 * @param nMaxFracLen Max length of the fractional part
 * @return BigNumber& 
 */
BigNumber& BigNumber::setMaxFracLen(std::size_t nMaxFracLen)
{
	m_nMaxFracLen = nMaxFracLen;

	return *this;
}

/**
 * @brief Get max denominator length of a decimal fraction
 * 
 * @return std::size_t
 */
auto BigNumber::getMaxFracLen() -> std::size_t
{
	return m_nMaxFracLen;
}

/**
 * @brief Round the number
 * 
 * @param nPos Round position \n
 *   - 0== : Nothing happen \n
 *   - 0>  : A position at a integer part \n
 *   - 0<  : A position at a decimal part
 * @return BigNumber& 
 */
auto BigNumber::round(int nPos) -> BigNumber& 
{
	return roundAt(nPos, 5);
}

/**
 * @brief Round up the number
 * 
 * @param nPos Round position \n
 *   - 0== : Nothing happen \n
 *   - 0>  : A position at a integer part \n
 *   - 0<  : A position at a decimal part
 * @return BigNumber& 
 */
auto BigNumber::roundUp(int nPos) -> BigNumber& 
{
	return roundAt(nPos, 0);
}

/**
 * @brief Round down the number
 * 
 * @param nPos Round position \n
 *   - 0== : Nothing happen \n
 *   - 0>  : A position at a integer part \n
 *   - 0<  : A position at a decimal part
 * @return BigNumber& 
 */
auto BigNumber::roundDown(int nPos) -> BigNumber& 
{
	return roundAt(nPos, 10);
}

/**
 * @brief Convert number to string
 * 
 * @return std::string 
 */
auto BigNumber::toString() -> std::string
{
	std::string strRet;

	if (m_bIsNegativeSign) strRet.push_back('-');

	strRet.append(m_strVal.substr(0, m_strVal.length()-m_nFracLen));

	if (m_nFracLen > 0) {
		strRet.push_back('.');
		strRet.append(m_strVal.substr(m_strVal.length()-m_nFracLen, m_nFracLen));
	}

	return strRet;
}

/**
 * @brief initialize
 * 
 * @param val A numeric string
 */
auto BigNumber::init(const std::string &val) -> void 
{
	// Set string
	// ex)
	//    0.0990 --> m_strVal = 0099   , m_nFracLen = 3
	//    0.999  --> m_strVal = 0999   , m_nFracLen = 3
	//    99.99  --> m_strVal = 9999   , m_nFracLen = 2

	// Check validity
	chkValid(val);

	int nStartPos = 0;

	m_strVal          = "0";
	m_bIsNegativeSign = false;
	m_nFracLen        = 0;
	m_nMaxFracLen     = m_nDftMaxFracLen;

	if (val.front() == '-') {
		m_bIsNegativeSign = true;
		nStartPos = 1;
	}
	else if (val.front() == '+') {
		nStartPos = 1;
	}

	std::size_t nDotPos = val.find('.');
	if (nDotPos == std::string::npos) m_nFracLen = 0;
	else m_nFracLen = val.length() - (nDotPos + 1);

	m_strVal = val.substr(nStartPos, nDotPos - nStartPos);
	if (nDotPos != std::string::npos) {
		m_strVal.append(val.substr(nDotPos + 1));
	}

	if (m_nFracLen > m_nMaxFracLen) m_nMaxFracLen = m_nFracLen;

	trim();
	if (m_strVal == "0") m_bIsNegativeSign = false;
}

/**
 * @brief Check validity
 * 
 * @param val A numeric string
 */
auto BigNumber::chkValid(const std::string &val) -> void
{
	int nStartPos = 0;
	int nPtrCnt = 0;

	if (val.length() == 0) {
		throw std::invalid_argument("Invalid argument [" + val + "]");
	}

	if (val[0] == '+' || val[0] == '-') {
		nStartPos = 1;
	}

	for (int i=nStartPos; i<(int)val.length(); i++) {
		if (val[i] >= '0' && val[i] <= '9') {
			continue;
		}
		else if (val[i] == '.' && nPtrCnt == 0) {
			nPtrCnt++;
			continue;
		}
		else {
			throw std::invalid_argument("Invalid argument [" + val + "]");
		}
	}
}

/**
 * @brief Add two numbers
 * 
 * @param val1 A numeric string
 * @param val2 A numeric string
 * @return std::string 
 */
auto BigNumber::addNum(const std::string & val1, const std::string & val2) -> std::string 
{
	std::string strRet;

	auto it1 = val1.rbegin();
	auto it2 = val2.rbegin();
	int nAddVal = 0;

	while (it1 != val1.rend() || it2 != val2.rend() ) {
		int nVal1 = 0;
		int nVal2 = 0;
		if (it1 != val1.rend()) nVal1 = *it1 - '0';
		if (it2 != val2.rend()) nVal2 = *it2 - '0';

		int nVal = nVal1 + nVal2 + nAddVal;
		char cVal = (nVal%10) + '0';
		strRet.push_back(cVal);
		nAddVal = nVal/10;

		if (it1 != val1.rend()) it1++;
		if (it2 != val2.rend()) it2++;
	}

	if (nAddVal > 0)
		strRet.push_back((char)(nAddVal + '0'));

	std::reverse(strRet.begin(), strRet.end());
	
	return strRet;
}

/**
 * @brief Substract two numbers
 * 
 * @param val1 A numeric string
 * @param val2 A numeric string
 * @return std::string 
 */
auto BigNumber::subNum(const std::string & val1, const std::string & val2) -> std::string
{
	std::string strRet;
	const std::string &strVal1 = val1;
	const std::string &strVal2 = val2;

	auto it1 = strVal1.rbegin();
	auto it2 = strVal2.rbegin();
	int nMinusVal = 0;

	while (it1 != strVal1.rend() || it2 != strVal2.rend() ) {
		int nVal1 = 0;
		int nVal2 = 0;
		if (it1 != strVal1.rend()) nVal1 = *it1 - '0';
		if (it2 != strVal2.rend()) nVal2 = *it2 - '0';

		int nVal = nVal1 - nVal2 - nMinusVal;
		nMinusVal = 0;

		if (nVal < 0) {
			nVal += 10;
			nMinusVal++;
		}

		strRet.push_back((char)(nVal + '0'));

		if (it1 != strVal1.rend()) it1++;
		if (it2 != strVal2.rend()) it2++;
	}

	std::reverse(strRet.begin(), strRet.end());

	return strRet;
}

/**
 * @brief Substract two numbers
 * 
 * @param val1 A numeric string
 * @param val2 A numeric string
 * @return std::pair<bool, std::string>
 */
auto BigNumber::subNumRetWithSign(const std::string & val1, const std::string & val2) -> std::pair<bool, std::string>
{
	std::pair<bool, std::string> prRet(false, "");

	if (val1.length() > val2.length()) {
		prRet.first = false;
		prRet.second = subNum(val1, val2);
	}
	else if (val1.length() < val2.length()) {
		prRet.first = true;
		prRet.second = subNum(val2, val1);
	}
	else {
		int nDiff = std::strcmp(val1.c_str(), val2.c_str());
		if (nDiff == 0) { // val1 == val2
			prRet.first = false;
			for (int i=0; i<(int)val1.length(); i++) {
				prRet.second.push_back('0');
			}
		}
		else if (nDiff > 0) { // val1 > val2
			prRet.first = false;
			prRet.second = subNum(val1, val2);
		}
		else { // val1 < val2
			prRet.first = true;
			prRet.second = subNum(val2, val1);
		}
	}

	return prRet;
}

/**
 * @brief Multify two numbers
 * 
 * @param val1 A numeric string
 * @param val2 A numeric string
 * @return std::string 
 */
auto BigNumber::mulNum(const std::string & val1, const std::string & val2) -> std::string
{
	std::string strRet = "0";

	std::string strZeroCnt1;
	for (auto it1 = val1.rbegin(); it1 != val1.rend(); it1++) {
		int nVal1 = *it1 - '0';

		std::string strZeroCnt2;
		for (auto it2 = val2.rbegin(); it2 != val2.rend(); it2++) {
			int nVal2 = *it2 - '0';
			int nVal = nVal1 * nVal2;

			strRet = addNum(strRet, std::to_string(nVal) + strZeroCnt2 + strZeroCnt1);
			strZeroCnt2.push_back('0');
		}

		strZeroCnt1.push_back('0');
	}

	return strRet;
}

/**
 * @brief  Divide two numbers
 * 
 * @param val1 A numeric string
 * @param val2 A numeric string
 * @param nMaxFracLen   Max length of the fractional part
 * @return std::string 
 */
auto BigNumber::divNum(const std::string &val1, const std::string &val2, std::size_t nMaxFracLen) -> std::string
{
	std::string strRet;

	//
	// Trim string
	// ex)
	//   val1
	//      toString() = 0.099
	//      m_strVal   = 00990
	//      m_nFracLen = 4
	//      --> strVal1 = 990
	//   val2
	//      toString() = 00099
	//      m_strVal   = 0.0099
	//      m_nFracLen = 4
	//      --> strVal1 = 99
	//
	//   val1 / va2 --> 10
	//
	std::string strVal1 = lTrim(val1);
	const std::string strVal2 = lTrim(val2);

	for (int i=0; i<(int)nMaxFracLen; i++) {
		strVal1.push_back('0');
	}

	std::string strVal;
	for (auto it = strVal1.begin(); it != strVal1.end(); it++) {
		strVal.push_back(*it);
		strVal = lTrim(strVal);
		if (strVal.length() < strVal2.length()) {
			strRet.push_back('0');
			continue;
		}
		std::pair<bool, std::string> prVal = subNumRetWithSign(strVal, strVal2);
		int nVal = 0;
		while (!prVal.first) {
			nVal++;
			strVal = lTrim(prVal.second);
			prVal = subNumRetWithSign(strVal, strVal2);
		}
		strRet.push_back((char)(nVal+'0'));
	}

	return strRet;
}

/**
 * @brief Adjust numbers \n
 *   This makes the denominator length of the decimal fractions of the two numbers equal
 * 
 * @param val1 A number
 * @param val2 A number
 */
auto BigNumber::adjNum(BigNumber &val1, BigNumber &val2) const -> void 
{ 
	// Adjust number
	// ex)
	//    val1 : m_strVal = 0999   ,  m_nFracLen = 3  --> m_strVal = 09990   , m_nFracLne = 4
	//    val2 : m_strVal = 00999  ,  m_nFracLen = 4  --> m_strVal = 00999   , m_nFracLen = 4 
	//
	//    val1 : m_strVal = 000999 ,  m_nFracLen = 5  --> m_strVal = 000999  , m_nFracLne = 5
	//    val2 : m_strVal = 999    ,  m_nFracLen = 1  --> m_strVal = 9990000 , m_nFracLen = 5

	if (val1.m_nFracLen > val2.m_nFracLen) {
		int nDiff = val1.m_nFracLen - val2.m_nFracLen;
		for (int i=0; i <nDiff; i++) {
			val2.m_strVal.push_back('0');
		}
		val2.m_nFracLen = val1.m_nFracLen;
	}
	else if (val1.m_nFracLen < val2.m_nFracLen) {
		int nDiff = val2.m_nFracLen - val1.m_nFracLen;
		for (int i=0; i <nDiff; i++) {
			val1.m_strVal.push_back('0');
		}
		val1.m_nFracLen = val2.m_nFracLen;
	}
}

/**
 * @brief Trim string
 * 
 */
auto BigNumber::trim() -> void
{
	// Left trim
	int nLTrimCnt = 0;
	for (int i=0; i<(int)m_strVal.length() - (int)m_nFracLen - 1; i++) {
		if (m_strVal[i] == '0') {
			nLTrimCnt++;
			continue;
		}
		break;
	}
	if (nLTrimCnt > 0) {
		m_strVal = m_strVal.substr(nLTrimCnt);
	}

	// Right trim
	int nRTrimCnt = 0;
	for (int i=0; i<(int)m_nFracLen; i++) {
		if (m_strVal.back() == '0') {
			m_strVal.pop_back();
			nRTrimCnt++;
			continue;
		}
		break;
	}
	if (nRTrimCnt > 0) {
		m_nFracLen -= nRTrimCnt;
	}
	if (m_nFracLen == 0 && m_strVal.back() == '.') {
		m_strVal.pop_back();
	}
}

/**
 * @brief Round the number based in the nBaseVal
 * 
 * @param nPos      Position \n
 *   - 0== : Nothing happen \n
 *   - 0>  : A position at a integer part \n
 *   - 0<  : A position at a decimal part
 * @param nBaseVal  Base value \n
 *   ex1) \n
 *     number   : 1.3 \n
 *     nPos     : -1 \n
 *     nBaseVal : 3 \n
 *     result   => 2 \n
 *   ex2) \n
 *     number   : 1.8 \n
 *     nPos     : -1 \n
 *     nBaseVal : 7 \n
 *     result   => 1 \n
 * @return BigNumber& 
 */
auto BigNumber::roundAt(int nPos, int nBaseVal) -> BigNumber &
{
if (nPos == 0) return *this;

	int nIdx = (int)m_strVal.length() - m_nFracLen - nPos;
	if (nPos < 0) nIdx--;

	int nVal = 0;
	if (nIdx >= 0 && nIdx < (int)m_strVal.length()) {
		nVal = (int)(m_strVal[nIdx]-'0');
	}

	// RoundUp
	if (nVal >= nBaseVal) {
		if (nIdx >= (int)m_strVal.length()) {
			for (int i=0; i<nIdx-(int)m_strVal.length(); i++) {
				m_strVal.push_back('0');
				m_nFracLen++;
			}
			m_strVal = addNum(m_strVal, "1");
			trim();

			return *this;
		}
		else if (nIdx <= 0) {
			std::string strVal;
			strVal.push_back('1');

			for (int i=0; i > nIdx; i--) {
				strVal.push_back('0');
			}
			for (int i=0; i<(int)m_strVal.length(); i++) {
				strVal.push_back('0');
			}
			m_strVal = strVal;
			trim();

			return *this;
		}

		std::string strPre = addNum(m_strVal.substr(0, nIdx), "1");
		m_strVal = strPre + m_strVal.substr(nIdx);
		for (int i=nIdx; i<(int)m_strVal.length(); i++) {
			m_strVal[i] = '0';
		}
		trim();

		return *this;
	}

	// RoundDown
	if (nIdx >= (int)m_strVal.length()) {
		return *this;
	}
	else if (nIdx <= 0) {
		init("0");
		return *this;
	}

	for (int i=nIdx; i<(int)m_strVal.length(); i++) {
		m_strVal[i] = '0';
	}
	trim();

	return *this;
}

/**
 * @brief  Compare two numbers
 * 
 * @param va1l A number
 * @param val2 A number
 * @return int 
 */
auto BigNumber::cmpNum(const BigNumber & val1, const BigNumber & val2) const -> int
{
	int nRvsVal = 1;
	if (!val1.m_bIsNegativeSign && val2.m_bIsNegativeSign) { // + , -
		return 1; // val1 is greater than val2
	}
	else if (val1.m_bIsNegativeSign && !val2.m_bIsNegativeSign) { // - , +
		return -1; // val1 is lower than val2
	}
	else if (!val1.m_bIsNegativeSign && !val2.m_bIsNegativeSign) { // + , +
		nRvsVal = 1;
	}
	else { // - , -
		nRvsVal = -1;
	}


	int nLen1 = (int)val1.m_strVal.length() - (int)val1.m_nFracLen;
	int nLen2 = (int)val2.m_strVal.length() - (int)val2.m_nFracLen;

	if (nLen1 > nLen2) {
		return nRvsVal * 1; // val1 is greater than v2
	}
	else if (nLen1 < nLen2) {
		return nRvsVal * -1; // val1 is lower than val2
	}

	return nRvsVal * std::strcmp(val1.m_strVal.c_str(), val2.m_strVal.c_str());
}

/**
 * @brief  Get max denominator length of a decimal fraction
 * 
 * @param val1 A number
 * @param val2 A number
 * @return std::size_t 
 */
auto BigNumber::getMaxFracLen(const BigNumber &val1, const BigNumber &val2) -> std::size_t
{
	return val1.m_nMaxFracLen>val2.m_nMaxFracLen?val1.m_nMaxFracLen:val2.m_nMaxFracLen;
}

/**
 * @brief Left trim
 * 
 * @param val A numeric string
 * @return std::string 
 */
auto BigNumber::lTrim(std::string val) -> std::string
{
	// Left trim
	int nLTrimCnt = 0;
	for (int i=0; i<(int)val.length() - 1; i++) {
		if (val[i] == '0') {
			nLTrimCnt++;
			continue;
		}
		break;
	}
	return val.substr(nLTrimCnt);
}
}