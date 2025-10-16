#include "TString_Adv.h"


TString::TString()
{
	str = nullptr;
	len = 0;
	capacity = 0;
}

TString::TString(const TString& s)
{
	len = s.len;
	capacity = s.capacity;
	if (capacity != 0)
	{
		str = new char[capacity + 1];
		for (int i = 0; i < len; ++i)
			str[i] = s.str[i];
		str[capacity] = '\0';
	}
	else
		str = nullptr;
}

TString::TString(TString&& s) noexcept
{
	len = s.len;
	capacity = s.capacity;
	str = s.str;
	s.str = nullptr;
	s.len = 0;
	s.capacity = 0;
}

TString::TString(const char* s)
{
	if (s)
	{
		int i = 0;
		len = cstrlen(s);
		capacity = len;
		str = new char[capacity + 1];
		for (int i = 0; i < len; ++i)
			str[i] = s[i];
		str[capacity] = '\0';
	}
	else
	{
		len = 0;
		capacity = 0;
		str = nullptr;
	}
}

TString::TString(const char* s, int count)
{
	if (count == 0)
	{
		len = 0;
		capacity = 0;
		str = nullptr;
	}
	else
	{
		try
		{
			s[count - 1];
		}
		catch (...)
		{
			throw 1;
		}
		len = count;
		capacity = count;
		str = new char[capacity + 1];
		for (int i = 0; i < len; ++i)
			str[i] = s[i];
		str[capacity] = '\n';
	}
}

TString::TString(int count, char ch)
{
	len = count;
	capacity = count;
	if (len == 0)
	{
		capacity = 0;
		str = nullptr;
	}
	str = new char[capacity + 1];
	for (int i = 0; i < len; ++i)
		str[i] = ch;
	str[capacity] = '\n';
}

TString::TString(const TString& s, int pos, int count)
{
	if (count == 0)
	{
		len = 0;
		capacity = 0;
		str = nullptr;
	}
	else
	{
		try
		{
			s.str[count - 1];
		}
		catch (...)
		{
			throw 1;
		}
		len = count;
		capacity = count;
		str = new char[capacity + 1];
		for (int i = 0; i < len; ++i)
			str[i] = s.str[i + pos];
		str[capacity] = '\n';
	}
}

TString::~TString()
{
	delete[] str;
	len = 0;
	capacity = 0;
}

TString& TString::operator=(const TString& s)
{
	if (this == &s)
		return *this;
	else
	{
		len = s.len;
		capacity = s.capacity;
		delete[] str;
		str = new char[capacity + 1];
		for (int i = 0; i < len; ++i)
			str[i] = s[i];
		str[len] = '\0';
		return *this;
	}
}

TString& TString::operator=(TString&& s) noexcept
{
	if (this == &s)
		return *this;
	else if (str)
	{
		delete[] str;
	}
	len = s.len;
	capacity = s.capacity;
	str = s.str;
	s.str = nullptr;
	s.len = 0;
	s.capacity = 0;
	return *this;
}

TString& TString::operator=(const char* s)
{
	if (s == str)
		return *this;
	else if (s)
	{
		int i = 0;
		len = cstrlen(s);
		capacity = len;
		delete[] str;
		str = new char[capacity + 1];
		for (int i = 0; i < len; ++i)
			str[i] = s[i];
		str[capacity] = '\0';
	}
	else
	{
		len = 0;
		capacity = 0;
		delete[] str;
	}
	return *this;
}

TString& TString::operator=(char ch)
{
	len = 1;
	capacity = 1;
	delete[] str;
	str = new char[2];
	str[0] = ch;
	str[1] = '\0';
	return *this;
}

void TString::Reserve(int n)
{
	if (n > capacity)
	{
		char* newstr = new char[len];
		for (int i = 0; i < len; ++i)
			newstr[i] = str[i];
		delete[] str;
		capacity = n;
		str = new char[capacity + 1];
		for (int i = 0; i < len; ++i)
			str[i] = newstr[i];
		delete[] newstr;
		str[len] = '\0';
	}
}

void TString::ShrinkToFit()
{
	char* newstr = new char[len];
	for (int i = 0; i < len; ++i)
		newstr[i] = str[i];
	delete[] str;
	capacity = len;
	str = new char[capacity + 1];
	for (int i = 0; i < len; ++i)
		str[i] = newstr[i];
	delete[] newstr;
	str[len] = '\0';
}

char& TString::operator[](int index)
{
	return str[index];
}

const char& TString::operator[](int index) const
{
	return str[index];
}

char& TString::At(int pos)
{
	if (pos >= len)
		throw 1;
	else
		return str[pos];
}

const char& TString::At(int pos) const
{
	if (pos >= len)
		throw 1;
	else
		return str[pos];
}

char& TString::Front()
{
	return str[0];
}

const char& TString::Front() const
{
	return str[0];
}

char& TString::Back()
{
	return str[len - 1];
}

const char& TString::Back() const
{
	return str[len - 1];
}

const char* TString::Data() const noexcept
{
	return str;
}

char* TString::Data() noexcept
{
	return str;
}

const char* TString::CStr() const noexcept
{
	return str;
}

int TString::Length() const noexcept
{
	return len;
}

bool TString::Empty() const noexcept
{
	if (len == 0)
		return true;
	return false;
}

int TString::Capacity() const noexcept
{
	return capacity;
}

TString& TString::Resize(int n)
{
	if (n <= capacity)
	{
		for (int i = len; i < n; ++i)
			str[i] = '\0';
		len = n;
		str[len] = '\0';
	}
	else
	{
		this->Reserve(n);
		for (int i = len; i < n; ++i)
			str[i] = '\0';
		len = n;
		str[len] = '\0';
	}
	return *this;
}

TString& TString::Resize(int n, char ch)
{
	if (n <= capacity)
	{
		for (int i = len; i < n; ++i)
			str[i] = ch;
		len = n;
		str[len] = '\0';
	}
	else
	{
		this->Reserve(n);
		for (int i = len; i < n; ++i)
			str[i] = ch;
		len = n;
		str[len] = '\0';
	}
	return *this;
}

void TString::Clear() noexcept
{
	if (str)
	{
		str[0] = '\0';
		len = 0;
	}
}

void TString::PushBack(char ch)
{
	if (len < capacity)
	{
		str[len] = ch;
		str[len + 1] = '\0';
		len += 1;
	}
	else
	{
		this->Reserve(capacity * 2);
		str[len] = ch;
		str[len + 1] = '\0';
		len += 1;
	}
}

void TString::PopBack()
{
	str[--len] = '\0';
}

TString& TString::Append(const TString& s)
{
	if (capacity >= len + s.len)
	{
		for (int i = len; i < len + s.len; ++i)
			str[i] = s[i - len];
		len += s.len;
		str[len] = '\0';
	}
	else
	{
		this->Reserve(len + s.len);
		for (int i = len; i < len + s.len; ++i)
			str[i] = s[i - len];
		len += s.len;
		str[len] = '\0';
	}
	return *this;
}

TString& TString::Append(const TString& s, int pos, int count)
{
	if (pos > s.len)
		throw"Error";
	else
	{
		int realCount = (count < s.len - pos) ? count : (s.len - pos);
		if (capacity >= len + realCount)
		{
			for (int i = len; i < len + realCount; ++i)
				str[i] = s[i - len + pos];
			len += realCount;
			str[len] = '\0';
		}
		else
		{
			this->Reserve(len + realCount);
			for (int i = len; i < len + realCount; ++i)
				str[i] = s[i - len + pos];
			len += realCount;
			str[len] = '\0';
		}
	}
	return *this;
}

TString& TString::Append(const char* s)
{
	int slen = cstrlen(s);
	if ((len + slen <= capacity) && s)
	{
		capacity = slen + len;
		for (int i = len; i < capacity; ++i)
			str[i] = s[i - len];
		str[capacity] = '\0';
		len = capacity;
	}
	else if (s)
	{
		char* newstr = new char[len + slen];
		for (int i = 0; i < len; ++i)
			newstr[i] = str[i];
		delete[] str;
		for (int i = len; i < len + slen; ++i)
			newstr[i] = s[i - len];
		str = new char[len + slen + 1];
		for (int i = 0; i < len + slen; ++i)
			str[i] = newstr[i];
		str[len + slen] = '\0';
		delete[] newstr;
		capacity = len + slen;
		len = capacity;
	}
	return *this;
}

TString& TString::Append(const char* s, int count)
{
	if (cstrlen(s) < count)
		throw "Error";
	else
	{
		if (capacity >= len + count)
		{
			for (int i = len; i < len + count; ++i)
				str[i] = s[i - len];
			len += count;
			str[len] = '\0';
		}
		else
		{
			this->Reserve(len + count);
			for (int i = len; i < len + count; ++i)
				str[i] = s[i - len];
			len += count;
			str[len] = '\0';
		}
	}
	return *this;
}

TString& TString::Append(int count, char ch)
{
	if (capacity >= len + count)
	{
		for (int i = len; i < len + count; ++i)
			str[i] = ch;
		len += count;
		str[len] = '\0';
	}
	else
	{
		this->Reserve(len + count);
		for (int i = len; i < len + count; ++i)
			str[i] = ch;
		len += count;
		str[len] = '\0';
	}
	return *this;
}

TString& TString::operator+=(const TString& s)
{
	return this->Append(s);
}

TString& TString::operator+=(const char* s)
{
	return this->Append(s);
}

TString& TString::operator+=(char ch)
{
	this->PushBack(ch);
	return *this;
}

TString& TString::Insert(const TString& s, int pos)
{
	if (pos >= len)
		throw 1;
	else if (capacity >= len + s.len)
	{
		for (int i = len - 1; i >= pos; --i)
			str[i + s.len] = str[i];
		for (int i = pos; i < pos + s.len; ++i)
			str[i] = s[i - pos];
		len += s.len;
		str[len] = '\0';
	}
	else
	{
		this->Reserve(len + s.len);
		for (int i = len - 1; i >= pos; --i)
			str[i + s.len] = str[i];
		for (int i = pos; i < pos + s.len; ++i)
			str[i] = s[i - pos];
		len += s.len;
		str[len] = '\0';
	}
	return *this;
}

TString& TString::Insert(const TString& s, int pos, int count, int spos)
{
	if (pos >= len)
		throw 1;
	else if (spos >= s.len)
		throw 2;
	else if (spos + count >= s.len)
		throw 3;
	else if (capacity >= count + len)
	{
		for (int i = len - 1; i >= pos; --i)
			str[i + count] = str[i];
		for (int i = pos; i < pos + count; ++i)
			str[i] = s[spos + i - pos];
		len += count;
		str[len] = '\0';
	}
	else
	{
		this->Reserve(len + count);
		for (int i = len - 1; i >= pos; --i)
			str[i + count] = str[i];
		for (int i = pos; i < pos + count; ++i)
			str[i] = s[spos + i - pos];
		len += count;
		str[len] = '\0';
	}
	return *this;
}

TString& TString::Insert(const char* s, int pos)
{
	if (pos >= len)
		throw 1;
	else if (capacity >= len + cstrlen(s))
	{
		for (int i = len - 1; i >= pos; --i)
			str[i + cstrlen(s)] = str[i];
		for (int i = pos; i < pos + cstrlen(s); ++i)
			str[i] = s[i - pos];
		len += cstrlen(s);
		str[len] = '\0';
	}
	else
	{
		this->Reserve(len + cstrlen(s));
		for (int i = len - 1; i >= pos; --i)
			str[i + cstrlen(s)] = str[i];
		for (int i = pos; i < pos + cstrlen(s); ++i)
			str[i] = s[i - pos];
		len += cstrlen(s);
		str[len] = '\0';
	}
	return *this;
}

TString& TString::Insert(const char* s, int pos, int count)
{
	if (pos >= len)
		throw 1;
	else if (count >= cstrlen(s))
		throw 3;
	else if (capacity >= count + len)
	{
		for (int i = len - 1; i >= pos; --i)
			str[i + count] = str[i];
		for (int i = pos; i < pos + count; ++i)
			str[i] = s[i - pos];
		len += count;
		str[len] = '\0';
	}
	else
	{
		this->Reserve(len + count);
		for (int i = len - 1; i >= pos; --i)
			str[i + count] = str[i];
		for (int i = pos; i < pos + count; ++i)
			str[i] = s[i - pos];
		len += count;
		str[len] = '\0';
	}
	return *this;
}

TString& TString::Insert(int pos, int count, char ch)
{
	char* newstr = new char[count + 1];
	for (int i = 0; i < count; ++i)
		newstr[i] = ch;
	newstr[count] = '\0';
	this->Insert(newstr, pos);
	delete[] newstr;
	return *this;
}

TString& TString::Erase(int pos, int count)
{
	if (pos >= len)
		throw 1;
	else
	{
		int rlen = (count < len - pos) ? count : (len - pos);
		len -= rlen;
		str[len] = '\0';
	}
	return *this;
}

TString& TString::Replace(int pos, int count, const TString& s)
{
	this->Erase(pos, count);
	this->Insert(s, pos);
	return *this;
}

TString& TString::Replace(int pos, int count, const TString& s, int spos, int scount)
{
	this->Erase(pos, count);
	this->Insert(s, pos, scount, spos);
	return *this;
}

TString& TString::Replace(int pos, int count, const char* s)
{
	this->Erase(pos, count);
	this->Insert(s, pos);
	return *this;
}

TString& TString::Replace(int pos, int count, int chcount, char ch)
{
	this->Erase(pos, count);
	this->Insert(pos, chcount, ch);
	return *this;
}

TString& TString::Assign(const TString& s)
{
	this->Clear();
	this->Append(s);
	return *this;
}

TString& TString::Assign(const TString& s, int pos, int count)
{
	this->Clear();
	this->Append(s, pos, count);
	return *this;
}

TString& TString::Assign(const char* s)
{
	this->Clear();
	this->Append(s);
	return *this;
}

TString& TString::Assign(const char* s, int count)
{
	this->Clear();
	this->Append(s, count);
	return *this;
}

TString& TString::Assign(int count, char ch)
{
	this->Clear();
	this->Append(count, ch);
	return *this;
}

int TString::Find(const TString& s, int pos) const noexcept
{
	if (pos >= len)
		return -1;
	for (int i = pos; i < len - s.len + 1; ++i)
	{
		if (str[i] == s[0])
		{
			bool l = true;
			for (int j = 1; j < s.len; ++j)
			{
				if (str[i + j] != s[j])
				{
					l = false;
					break;
				}
			}
			if (l == true)
				return i;
		}
	}
	return -1;
}

int TString::Find(const char* s, int pos) const
{
	if (pos >= len)
		throw 1;
	for (int i = pos; i < len - cstrlen(s) + 1; ++i)
	{
		if (str[i] == s[0])
		{
			bool l = true;
			for (int j = 1; j < cstrlen(s); ++j)
			{
				if (str[i + j] != s[j])
				{
					l = false;
					break;
				}
			}
			if (l == true)
				return i;
		}
	}
	return -1;
}

int TString::Find(int n, const char* s, int pos) const
{
	if (pos >= len)
		throw 1;
	if (n > cstrlen(s))
		throw 1;
	for (int i = pos; i < len - n + 1; ++i)
	{
		if (str[i] == s[0])
		{
			bool l = true;
			for (int j = 1; j < n; ++j)
			{
				if (str[i + j] != s[j])
				{
					l = false;
					break;
				}
			}
			if (l == true)
				return i;
		}
	}
	return -1;
}

int TString::Find(char ch, int pos) const noexcept
{
	if (pos >= len)
		return -1;
	for (int i = pos; i < len; ++i)
		if (str[i] == ch)
			return i;
	return -1;
}

int TString::RFind(const TString& s, int pos) const noexcept
{
	if (pos >= len)
		return -1;
	int cou = -1;
	for (int i = pos; i < len - s.len + 1; ++i)
	{
		if (str[i] == s[0])
		{
			bool l = true;
			for (int j = 1; j < s.len; ++j)
			{
				if (str[i + j] != s[j])
				{
					l = false;
					break;
				}
			}
			if (l == true)
				cou = i;
		}
	}
	return cou;
}

int TString::RFind(const char* s, int pos) const
{
	if (pos >= len)
		throw 1;
	int cou = -1;
	for (int i = pos; i < len - cstrlen(s) + 1; ++i)
	{
		if (str[i] == s[0])
		{
			bool l = true;
			for (int j = 1; j < cstrlen(s); ++j)
			{
				if (str[i + j] != s[j])
				{
					l = false;
					break;
				}
			}
			if (l == true)
				cou = i;
		}
	}
	return cou;
}

int TString::RFind(int n, const char* s, int pos) const
{
	if (pos >= len)
		throw 1;
	if (n > cstrlen(s))
		throw 1;
	int cou = -1;
	for (int i = pos; i < len - n + 1; ++i)
	{
		if (str[i] == s[0])
		{
			bool l = true;
			for (int j = 1; j < n; ++j)
			{
				if (str[i + j] != s[j])
				{
					l = false;
					break;
				}
			}
			if (l == true)
				cou = i;
		}
	}
	return cou;
}

int TString::RFind(char ch, int pos) const noexcept
{
	if (pos >= len)
		return -1;
	int cou = -1;
	for (int i = pos; i < len; ++i)
		if (str[i] == ch)
		{
			cou = i;
		}
	return cou;
}

int TString::FindFirstOf(const TString& s, int pos) const noexcept
{
	if (pos >= len)
		return -1;
	for (int i = pos; i < len; ++i)
	{
		if (s.Find(str[i], 0) != -1)
			return i;
	}
	return -1;
}

int TString::FindFirstOf(const char* s, int pos) const
{
	if (pos >= len)
		return -1;
	TString sc(s);
	for (int i = pos; i < len; ++i)
	{
		if (sc.Find(str[i], 0) != -1)
			return i;
	}
	return -1;
}

int TString::FindFirstOf(int n, const char* s, int pos) const
{
	if (pos >= len)
		return -1;
	TString sc(s, n);
	for (int i = pos; i < len; ++i)
	{
		if (sc.Find(str[i], 0) != -1)
			return i;
	}
	return -1;
}

int TString::FindFirstOf(char ch, int pos) const noexcept
{
	return this->Find(ch, pos);
}

int TString::FindLastOf(const TString& s, int pos) const noexcept
{
	int ind = -1;
	if (pos >= len)
		return -1;
	for (int i = 0; i < len; ++i)
	{
		if (s.RFind(str[i], pos) != -1)
			ind = i;
	}
	return ind;
}

int TString::FindLastOf(const char* s, int pos) const
{
	int ind = -1;
	if (pos >= len)
		return -1;
	TString sc(s);
	for (int i = 0; i < len; ++i)
	{
		if (sc.RFind(str[i], pos) != -1)
			ind = i;
	}
	return ind;
}

int TString::FindLastOf(int n, const char* s, int pos) const
{
	int ind = -1;
	if (pos >= len)
		return -1;
	TString sc(s, n);
	for (int i = 0; i < len; ++i)
	{
		if (sc.RFind(str[i], pos) != -1)
			ind = i;
	}
	return ind;
}

int TString::FindLastOf(char ch, int pos) const noexcept
{
	return this->RFind(ch, pos);
}

int TString::FindFirstNotOf(const TString& s, int pos) const noexcept
{
	if (pos >= len)
		return -1;
	for (int i = pos; i < len; ++i)
	{
		if (s.Find(str[i], 0) == -1)
			return i;
	}
	return -1;
}

int TString::FindFirstNotOf(const char* s, int pos) const
{
	if (pos >= len)
		return -1;
	TString sc(s);
	for (int i = pos; i < len; ++i)
	{
		if (sc.Find(str[i], 0) == -1)
			return i;
	}
	return -1;
}

int TString::FindFirstNotOf(int n, const char* s, int pos) const
{
	if (pos >= len)
		return -1;
	TString sc(s, n);
	for (int i = pos; i < len; ++i)
	{
		if (sc.Find(str[i], 0) == -1)
			return i;
	}
	return -1;
}

int TString::FindFirstNotOf(char ch, int pos) const noexcept
{
	if (pos >= len)
		return -1;
	for (int i = pos; i < len; ++i)
	{
		if (str[i] != ch)
			return i;
	}
	return -1;
}

int TString::FindLastNotOf(const TString& s, int pos) const noexcept
{
	int ind = -1;
	if (pos >= len)
		return -1;
	for (int i = pos; i < len; ++i)
	{
		if (s.RFind(str[i], 0) == -1)
			ind = i;
	}
	return ind;
}

int TString::FindLastNotOf(const char* s, int pos) const
{
	int ind = -1;
	if (pos >= len)
		return -1;
	TString sc(s);
	for (int i = pos; i < len; ++i)
	{
		if (sc.RFind(str[i], 0) == -1)
			ind = i;
	}
	return ind;
}

int TString::FindLastNotOf(int n, const char* s, int pos) const
{
	int ind = -1;
	if (pos >= len)
		return -1;
	TString sc(s, n);
	for (int i = pos; i < len; ++i)
	{
		if (sc.RFind(str[i], 0) == -1)
			ind = i;
	}
	return ind;
}

int TString::FindLastNotOf(char ch, int pos) const noexcept
{
	int ind = -1;
	if (pos >= len)
		return -1;
	for (int i = pos; i < len; ++i)
	{
		if (str[i] != ch)
			ind = i;
	}
	return ind;
}

TString TString::SubStr(int pos, int count)
{
	if (pos >= len)
		throw 1;
	int real = (count == -1 || pos + count > len) ? len - pos : count;
	TString a;
	a.Reserve(real);
	for (int i = pos; i < pos + real; ++i)
		a.PushBack(str[i]);
	return TString(a);
}

void TString::Swap(TString& s)
{
	TString a = move(*this);
	*this = move(s);
	s = move(a);
}

int TString::Compare(const TString& s) const noexcept
{
	for (int i = 0; i < ((len > s.len) ? s.len : len); ++i)
		if (str[i] != s[i])
			return static_cast<int>(str[i]) - static_cast<int>(s[i]);
	return len - s.len;
}

int TString::Compare(int pos, int count, const TString& s) const
{
	int rlen = (count < s.len - pos) ? count : (s.len - pos);
	for (int i = 0; i < ((len > rlen) ? rlen : len); ++i)
		if (str[i] != s[pos + i])
			return static_cast<int>(str[i]) - static_cast<int>(s[pos + i]);
	return len - rlen;
}

int TString::Compare(int pos, int count, const TString& s, int spos, int scount) const
{
	int rlen = (count < len - pos) ? count : (len - pos);
	int srlen = (scount < s.len - spos) ? scount : (s.len - spos);
	for (int i = 0; i < ((rlen > srlen) ? srlen : rlen); ++i)
		if (str[pos + i] != s[spos + i])
			return static_cast<int>(str[pos + i]) - static_cast<int>(s[spos + i]);
	return rlen - srlen;
}

int TString::Compare(const char* s) const
{
	int slen = cstrlen(s);
	for (int i = 0; i < ((len > slen) ? slen : len); ++i)
		if (str[i] != s[i])
			return static_cast<int>(str[i]) - static_cast<int>(s[i]);
	return len - slen;
}

int TString::Compare(int pos, int count, const char* s) const
{
	int slen = cstrlen(s);
	int rlen = (count < slen - pos) ? count : (slen - pos);
	for (int i = 0; i < ((len > rlen) ? rlen : len); ++i)
		if (str[i] != s[pos + i])
			return static_cast<int>(str[i]) - static_cast<int>(s[pos + i]);
	return len - rlen;
}

int TString::Compare(int pos, int count, const char* s, int scount) const
{
	int slen = cstrlen(s);
	int rlen = (count < len - pos) ? count : (len - pos);
	int srlen = (scount < slen) ? scount : slen;
	for (int i = 0; i < ((rlen > srlen) ? srlen : rlen); ++i)
		if (str[pos + i] != s[i])
			return static_cast<int>(str[pos + i]) - static_cast<int>(s[i]);
	return rlen - srlen;
}

bool TString::operator==(const TString& rs)
{
	return this->Compare(rs) == 0;
}

bool TString::operator==(const char* rs)
{
	return this->Compare(rs) == 0;
}

bool TString::operator!=(const TString& rs)
{
	return this->Compare(rs) != 0;
}

bool TString::operator!=(const char* rs)
{
	return this->Compare(rs) != 0;
}

bool TString::operator>(const TString& rs)
{
	return this->Compare(rs) > 0;
}

bool TString::operator>(const char* rs)
{
	return this->Compare(rs) > 0;
}

bool TString::operator<(const TString& rs)
{
	return this->Compare(rs) < 0;
}

bool TString::operator<(const char* rs)
{
	return this->Compare(rs) < 0;
}

bool TString::operator>=(const TString& rs)
{
	return this->Compare(rs) >= 0;
}

bool TString::operator>=(const char* rs)
{
	return this->Compare(rs) >= 0;
}

TString TString::operator+(const TString& s)
{
	TString n;
	n.Reserve(len + s.len);
	for (int i = 0; i < len; ++i)
		n.PushBack(str[i]);
	for (int i = 0; i < s.len; ++i)
		n.PushBack(s[i]);
	return TString(n);
}

TString TString::operator+(const char* s)
{
	TString n;
	int slen = cstrlen(s);
	n.Reserve(len + slen);
	for (int i = 0; i < len; ++i)
		n.PushBack(str[i]);
	for (int i = 0; i < slen; ++i)
		n.PushBack(s[i]);
	return TString(n);
}

TString TString::operator+(char s)
{
	TString n(*this);
	n.PushBack(s);
	return TString(n);
}

bool TString::operator<=(const TString& rs)
{
	return this->Compare(rs) <= 0;
}

bool TString::operator<=(const char* rs)
{
	return this->Compare(rs) <= 0;
}

bool operator==(const char* ls, const TString& rs)
{
	return rs.Compare(ls) == 0;
}

bool operator!=(const char* ls, const TString& rs)
{
	return rs.Compare(ls) != 0;
}

bool operator>(const char* ls, const TString& rs)
{
	return rs.Compare(ls) <= 0;
}

bool operator<(const char* ls, const TString& rs)
{
	return rs.Compare(ls) >= 0;
}

bool operator<=(const char* ls, const TString& rs)
{
	return rs.Compare(ls) > 0;
}

bool operator>=(const char* ls, const TString& rs)
{
	return rs.Compare(ls) < 0;
}

TString operator+(const char* ls, const TString& rs)
{
	TString n;
	int slen = cstrlen(ls);
	n.Reserve(rs.len + slen);
	for (int i = 0; i < slen; ++i)
		n.PushBack(ls[i]);
	for (int i = 0; i < rs.len; ++i)
		n.PushBack(rs[i]);
	return TString(n);
}

TString operator+(char ls, const TString& rs)
{
	TString n(1, ls);
	n.Append(rs);
	return TString(n);
}

ostream& operator<<(ostream& out, TString& s)
{
	out << s.CStr() << endl;
	return out;
}

istream& operator>>(istream& in, TString& s)
{
	char buf[256];
	cout << "Enter text: ";
	in >> buf;
	s.Clear();
	s.Append(buf);
	return in;
}

int strlen(char* str)
{
	int counter = 0;
	if (str == nullptr)
		throw "str == nullptr";
	while (str[counter] != '\0')
		counter++;
	return counter;
}


int cstrlen(const char* str)
{
	int counter = 0;
	if (str == nullptr)
		throw "str == nullptr";
	while (str[counter] != '\0')
		counter++;
	return counter;
}