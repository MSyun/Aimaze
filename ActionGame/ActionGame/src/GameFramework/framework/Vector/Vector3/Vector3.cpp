// 2Dベクトル
// 2017. 1.29	プログラム作成
// author		SyunMizuno

#include	"Vector3.h"
#include	<math.h>



Vector3::Vector3(float x, float y, float z) {
	this->x = x;
	this->y = y;
	this->z = z;
}
Vector3::Vector3(const float* val) {
	this->x = val[0];
	this->y = val[1];
	this->z = val[2];
}
Vector3::Vector3(const float& val) {
	this->x = val;
	this->y = val;
	this->z = val;
}
Vector3::Vector3(const Vector3& vec) {
	this->x = vec.x;
	this->y = vec.y;
	this->z = vec.z;
}

Vector3& Vector3::operator +=(const Vector3& vec) {
	this->x += vec.x;
	this->y += vec.y;
	this->z += vec.z;
	return *this;
}
Vector3& Vector3::operator -=(const Vector3& vec) {
	this->x -= vec.x;
	this->y -= vec.y;
	this->z -= vec.z;
	return *this;
}
Vector3& Vector3::operator *=(const float fDat) {
	this->x *= fDat;
	this->y *= fDat;
	this->z *= fDat;
	return *this;
}
Vector3& Vector3::operator /=(const float fDat) {
	if (fDat == 0.0f)	return *this;
	this->x /= fDat;
	this->y /= fDat;
	this->z /= fDat;
	return *this;
}

Vector3& Vector3::operator =(const Vector3& vec) {
	this->x = vec.x;
	this->y = vec.y;
	this->z = vec.z;
	return *this;
};
Vector3 Vector3::operator +(const Vector3& vec) const {
	Vector3 v3;

	v3.x = this->x + vec.x;
	v3.y = this->y + vec.y;
	v3.z = this->z + vec.z;
	return v3;
};
Vector3 Vector3::operator -(const Vector3& vec) const {
	Vector3 v3;

	v3.x = this->x - vec.x;
	v3.y = this->y - vec.y;
	v3.z = this->z - vec.z;
	return v3;
}
Vector3 Vector3::operator -() const {
	return Vector3(-x, -y, -z);
}
Vector3 Vector3::operator *(const float fDat) const {
	Vector3 v3;

	v3.x = this->x * fDat;
	v3.y = this->y * fDat;
	v3.z = this->z * fDat;
	return v3;
};
Vector3 Vector3::operator /(const float fDat) const {
	Vector3 v3;

	if (fDat == 0.0f)	return *this;

	v3.x = this->x / fDat;
	v3.y = this->y / fDat;
	v3.z = this->z / fDat;
	return v3;
};
Vector3 Vector3::operator *(const Vector3& vec) const {
	Vector3 v3;

	v3.x = this->x * vec.x;
	v3.y = this->y * vec.y;
	v3.z = this->z * vec.z;
	return v3;
}
Vector3 Vector3::operator /(const Vector3& vec) const {
	Vector3 v3;

	if (vec.x == 0.0f)	v3.x = this->x;
	else				v3.x = this->x / vec.x;
	if (vec.y == 0.0f)	v3.y = this->y;
	else				v3.y = this->y / vec.y;
	if (vec.y == 0.0f)	v3.z = this->z;
	else				v3.z = this->z / vec.z;
	return v3;
}

bool Vector3::operator ==(const Vector3& vec) const {
	return (this->x == vec.x) && (this->y == vec.y) && (this->z == vec.z);
}
bool Vector3::operator !=(const Vector3& vec) const {
	return !(*this == vec);
}

void Vector3::zero() {
	this->x = this->y = this->z = 0.0f;
}
void Vector3::one() {
	this->x = this->y = this->z = 1.0f;
}

void Vector3::normalize() {
	float msgSq = x * x + y * y + z * z;
	if (msgSq > 0.0f) {
		float oneOverMsg = 1.0f / sqrtf(msgSq);
		x *= oneOverMsg;
		y *= oneOverMsg;
		z *= oneOverMsg;
	}
}
Vector3 Vector3::getNormalize() const {
	Vector3 norm;
	float msgSq = x * x + y * y + z * z;
	if (msgSq > 0.0f) {
		float oneOverMsg = 1.0f / sqrtf(msgSq);
		norm.x = x * oneOverMsg;
		norm.y = y * oneOverMsg;
		norm.z = z * oneOverMsg;
	} else {
		norm.x = x;
		norm.y = y;
		norm.z = z;
	}
	return norm;
}
float Vector3::length() const {
	return sqrtf(x * x + y * y + z * z);
}