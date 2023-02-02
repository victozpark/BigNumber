#ifndef VP_BIG_NUMBER_HPP
#define VP_BIG_NUMBER_HPP

#include <iostream>
#include <string>
#include <vector>

namespace vp {
class BigNumber
{
public:
	BigNumber();
	BigNumber(const std::string &val);
	BigNumber(const std::string &val, std::size_t nMaxFracLen);
	BigNumber(std::size_t nMaxFracLen);
	~BigNumber();

	auto friend operator<<(std::ostream& os, const BigNumber &rhs) -> std::ostream&;
	auto operator+ (const BigNumber &rhs) -> BigNumber ;
	auto operator+=(const BigNumber &rhs) -> BigNumber&;
	auto operator- (const BigNumber &rhs) -> BigNumber ;
	auto operator-=(const BigNumber &rhs) -> BigNumber&;
	auto operator* (const BigNumber &rhs) -> BigNumber ;
	auto operator*=(const BigNumber &rhs) -> BigNumber&;
	auto operator/ (const BigNumber &rhs) -> BigNumber ;
	auto operator/=(const BigNumber &rhs) -> BigNumber&;
	auto operator!=(const BigNumber &rhs) const -> bool;
	auto operator==(const BigNumber &rhs) const -> bool;
	auto operator> (const BigNumber &rhs) const -> bool;
	auto operator>=(const BigNumber &rhs) const -> bool;
	auto operator< (const BigNumber &rhs) const -> bool;
	auto operator<=(const BigNumber &rhs) const -> bool;

	auto setMaxFracLen(std::size_t nMaxFracLen) -> BigNumber&;
	auto getMaxFracLen() -> std::size_t;

	auto round(int nPos) -> BigNumber &;
	auto roundUp(int nPos) -> BigNumber &;
	auto roundDown(int nPos) -> BigNumber &;

	auto toString() -> std::string ;
private:
	std::string m_strVal;
	bool m_bIsNegativeSign;
	std::size_t m_nFracLen;

	std::size_t m_nMaxFracLen = m_nDftMaxFracLen;
	static const std::size_t m_nDftMaxFracLen = 20;

	auto init(const std::string &val) -> void;
	auto chkValid(const std::string &val) -> void;

	auto addNum(const BigNumber & val1, const BigNumber &val2) -> std::string;
	auto addNum(const std::string & val1, const std::string &val2) -> std::string;
	auto subNum(const std::string & val1, const std::string &val2) -> std::string;
	auto subNumRetWithSign(const std::string & val1, const std::string & val2) -> std::pair<bool, std::string>;
	auto mulNum(const std::string & val1, const std::string &val2) -> std::string;
	auto divNum(const std::string &val1, const std::string &val2, std::size_t nMaxFracLen) -> std::string;
	auto adjNum(BigNumber &val1, BigNumber &val2) const -> void;
	auto trim() -> void;
	auto roundAt(int nPos, int nBaseVal) -> BigNumber &;
	auto cmpNum(const BigNumber & va1l, const BigNumber & val2) const -> int;

	auto getMaxFracLen(const BigNumber &val1, const BigNumber &val2) -> std::size_t;

	// Utilities
	auto static lTrim(std::string val) -> std::string;
};
}

#endif // VP_BIG_NUMBER_HPP