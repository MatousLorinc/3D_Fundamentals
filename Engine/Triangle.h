#pragma once
template<class V>
class Triangle
{
public:
	V v0;
	V v1;
	V v2;
	V vn;
	V vt0;
	V vt1;
	V vt2;
	bool cullFlag;
};
