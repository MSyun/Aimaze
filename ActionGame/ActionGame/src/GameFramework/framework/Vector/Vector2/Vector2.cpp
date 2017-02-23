// 2Dベクトル
// 2017. 1.29	プログラム作成
// author		SyunMizuno

#include	"Vector2.h"
#include	<math.h>



Vector2::Vector2(float x, float y) {
	this->x = x;
	this->y = y;
}
Vector2::Vector2(const float* val) {
	this->x = val[0];
	this->y = val[1];
}
Vector2::Vector2(const float& val) {
	this->x = val;
	this->y = val;
}
Vector2::Vector2(const Vector2& vec) {
	this->x = vec.x;
	this->y = vec.y;
}

Vector2& Vector2::operator +=(const Vector2& vec) {
	this->x += vec.x;
	this->y += vec.y;
	return *this;
}
Vector2& Vector2::operator -=(const Vector2& vec) {
	this->x -= vec.x;
	this->y -= vec.y;
	return *this;
}
Vector2& Vector2::operator *=(const float fDat) {
	this->x *= fDat;
	this->y *= fDat;
	return *this;
}
Vector2& Vector2::operator /=(const float fDat) {
	if (fDat == 0.0f)	return *this;
	this->x /= fDat;
	this->y /= fDat;
	return *this;
}

Vector2& Vector2::operator =(const Vector2& vec) {
	this->x = vec.x;
	this->y = vec.y;
	return *this;
};
Vector2 Vector2::operator +(const Vector2& vec) const {
	Vector2 v2;

	v2.x = this->x + vec.x;
	v2.y = this->y + vec.y;
	return v2;
};
Vector2 Vector2::operator -(const Vector2& vec) const {
	Vector2 v2;

	v2.x = this->x - vec.x;
	v2.y = this->y - vec.y;
	return v2;
}
Vector2 Vector2::operator -() const {
	return Vector2(-x, -y);
}
Vector2 Vector2::operator *(const float fDat) const {
	Vector2 v2;

	v2.x = this->x * fDat;
	v2.y = this->y * fDat;
	return v2;
};
Vector2 Vector2::operator /(const float fDat) const {
	Vector2 v2;

	if (fDat == 0.0f)	return *this;

	v2.x = this->x / fDat;
	v2.y = this->y / fDat;
	return v2;
};
Vector2 Vector2::operator *(const Vector2& vec) const {
	Vector2 v2;

	v2.x = this->x * vec.x;
	v2.y = this->y * vec.y;
	return v2;
}
Vector2 Vector2::operator /(const Vector2& vec) const {
	Vector2 v2;

	if (vec.x == 0.0f)	v2.x = this->x;
	else				v2.x = this->x / vec.x;
	if (vec.y == 0.0f)	v2.y = this->y;
	else				v2.y = this->y / vec.y;
	return v2;
}

bool Vector2::operator ==(const Vector2& vec) const {
	return (this->x == vec.x) && (this->y == vec.y);
}
bool Vector2::operator !=(const Vector2& vec) const {
	return !(*this == vec);
}

void Vector2::zero() {
	this->x = this->y = 0.0f;
}
void Vector2::one() {
	this->x = this->y = 1.0f;
}

void Vector2::normalize() {
	float msgSq = x * x + y * y;
	if (msgSq > 0.0f) {
		float oneOverMsg = 1.0f / sqrtf(msgSq);
		x *= oneOverMsg;
		y *= oneOverMsg;
	}
}
Vector2 Vector2::getNormalize() const {
	Vector2 norm;
	float msgSq = x * x + y * y;
	if (msgSq > 0.0f) {
		float oneOverMsg = 1.0f / sqrtf(msgSq);
		norm.x = x * oneOverMsg;
		norm.y = y * oneOverMsg;
	} else {
		norm.x = x;
		norm.y = y;
	}
	return norm;
}
float Vector2::length() const {
	return sqrtf(x * x + y * y);
}