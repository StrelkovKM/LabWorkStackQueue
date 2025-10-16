#pragma once
#include <iostream>


using namespace std;


class TString {
protected:
	char* str;
	int len;
	int capacity;
public:
	static const int npos = -1;

	TString();//Конструктор по умолчанию
	TString(const TString& s);//Конструктор копирования
	TString(TString&& s) noexcept;//Конструктор перемещения
	TString(const char* s);//Конструктор по С-строке
	TString(const char* s, int count);//Первые count символов из s
	TString(int count, char ch);//count символов ch
	TString(const TString& s, int pos, int count);//count символов с pos позиции из s

	~TString();//Деструктор
	TString& operator=(const TString& s);
	TString& operator=(TString&& s) noexcept;
	TString& operator=(const char* s);
	TString& operator=(char ch);
	void Reserve(int n);
	void ShrinkToFit();

	char& operator[](int index);
	const char& operator[](int index) const;
	char& At(int pos);
	const char& At(int pos) const;
	char& Front();
	const char& Front() const;
	char& Back();
	const char& Back() const;
	char* Data() noexcept;
	const char* Data() const noexcept;
	const char* CStr() const noexcept;

	int Length()const noexcept;
	bool Empty() const noexcept;
	int Capacity()const noexcept;
	TString& Resize(int n);
	TString& Resize(int n, char ch);

	void Clear() noexcept;
	void PushBack(char ch);
	void PopBack();
	TString& Append(const TString& s);
	TString& Append(const TString& s, int pos, int count);
	TString& Append(const char* s);
	TString& Append(const char* s, int count);
	TString& Append(int count, char ch);
	TString& operator+=(const TString& s);
	TString& operator+=(const char* s);
	TString& operator+=(char ch);
	TString& Insert(const TString& s, int pos);
	TString& Insert(const TString& s, int pos, int count, int spos);
	TString& Insert(const char* s, int pos);
	TString& Insert(const char* s, int pos, int count);
	TString& Insert(int pos, int count, char ch);
	TString& Erase(int pos, int count);
	TString& Replace(int pos, int count, const TString& s);
	TString& Replace(int pos, int count, const TString& s, int spos, int scount);
	TString& Replace(int pos, int count, const char* s);
	TString& Replace(int pos, int count, int chcount, char ch);
	TString& Assign(const TString& s);
	TString& Assign(const TString& s, int pos, int count);
	TString& Assign(const char* s);
	TString& Assign(const char* s, int count);
	TString& Assign(int count, char ch);

	int Find(const TString& s, int pos = 0) const noexcept;
	int Find(const char* s, int pos = 0) const;
	int Find(int n, const char* s, int pos = 0) const;
	int Find(char ch, int pos = 0) const noexcept;
	int RFind(const TString& s, int pos = 0) const noexcept;
	int RFind(const char* s, int pos = 0) const;
	int RFind(int n, const char* s, int pos = 0) const;
	int RFind(char ch, int pos = 0) const noexcept;
	int FindFirstOf(const TString& s, int pos = 0) const noexcept;
	int FindFirstOf(const char* s, int pos = 0) const;
	int FindFirstOf(int n, const char* s, int pos = 0) const;
	int FindFirstOf(char ch, int pos = 0) const noexcept;
	int FindLastOf(const TString& s, int pos = 0) const noexcept;
	int FindLastOf(const char* s, int pos = 0) const;
	int FindLastOf(int n, const char* s, int pos = 0) const;
	int FindLastOf(char ch, int pos = 0) const noexcept;
	int FindFirstNotOf(const TString& s, int pos = 0) const noexcept;
	int FindFirstNotOf(const char* s, int pos = 0) const;
	int FindFirstNotOf(int n, const char* s, int pos = 0) const;
	int FindFirstNotOf(char ch, int pos = 0) const noexcept;
	int FindLastNotOf(const TString& s, int pos = 0) const noexcept;
	int FindLastNotOf(const char* s, int pos = 0) const;
	int FindLastNotOf(int n, const char* s, int pos = 0) const;
	int FindLastNotOf(char ch, int pos = 0) const noexcept;
	TString SubStr(int pos = 0, int count = npos);
	void Swap(TString& s);

	int Compare(const TString& s) const noexcept;
	int Compare(int pos, int count, const TString& s) const;
	int Compare(int pos, int count, const TString& s, int spos, int scount) const;
	int Compare(const char* s) const;
	int Compare(int pos, int count, const char* s) const;
	int Compare(int pos, int count, const char* s, int scount) const;
	bool operator==(const TString& rs);
	bool operator==(const char* rs);
	bool operator!=(const TString& rs);
	bool operator!=(const char* rs);
	bool operator>(const TString& rs);
	bool operator>(const char* rs);
	bool operator<(const TString& rs);
	bool operator<(const char* rs);
	bool operator<=(const TString& rs);
	bool operator<=(const char* rs);
	bool operator>=(const TString& rs);
	bool operator>=(const char* rs);

	TString operator+(const TString& s);
	TString operator+(const char* s);
	TString operator+(char s);

	friend bool operator==(const char* ls, const TString& rs);
	friend bool operator!=(const char* ls, const TString& rs);
	friend 	bool operator>(const char* ls, const TString& rs);
	friend bool operator<(const char* ls, const TString& rs);
	friend bool operator<=(const char* ls, const TString& rs);
	friend 	bool operator>=(const char* ls, const TString& rs);
	friend TString operator+(const char* ls, const TString& rs);
	friend TString operator+(char ls, const TString& rs);

	friend ostream& operator<<(ostream& out, TString& s);
	friend istream& operator>>(istream& in, TString& s);
};


int strlen(char* str);
int cstrlen(const char* str);