#pragma once
#ifndef FORMULAS_H
#define FORMULAS_H
const float PI = 3.14159f;
class formulas {
public:
	int rng(int m, int mm);
	struct Vector3 {
		float x, y, z;
		Vector3(float x, float y, float z) : x(x), y(y), z(z) {}
		Vector3 translate(float tx, float ty, float tz) const {
			return Vector3(x + tx, y + ty, z + tz);
		}
		Vector3 add(const Vector3& other) const { //add two vectors
			return Vector3(x + other.x, y + other.y, z + other.z);
		}
		Vector3 subtract(const Vector3& other) const {
			return Vector3(x - other.x, y - other.y, z - other.z);
		}
		float dotproduct(const Vector3& other) const {
			return x * other.x + y * other.y + z * other.z;
		}
		Vector3 scalarmult(float scalar) const { //multiplies a vector by a scalar
			return Vector3(x * scalar, y * scalar, z * scalar);
		}
		Vector3 crossproduct(const Vector3& other) const { //cross product of two vectors
			return Vector3(y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x);
		}
		float length() const {
			return sqrtf(x * x + y * y + z * z);
		}
		Vector3 normalize() const {
			float len = length();
			return Vector3(x / len, y / len, z / len);
		}
	};
	struct Vector2 {
		float x, y;
		Vector2(float x, float y) : x(x), y(y) {}
	};
	struct Matrix4 {
		float m[4][4];
		Matrix4() {
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					m[i][j] = (i == j) ? 1.0f : 0.0f;
				}
			}
		}
		Matrix4(const Matrix4& other) {
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					m[i][j] = other.m[i][j];
				}
			}
		}

		Matrix4 add(const Matrix4& other) const { //add two matrices together
			Matrix4 result;
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					result.m[i][j] = m[i][j] + other.m[i][j];
				}
			}
			return result;
		}
		Matrix4 subtract(const Matrix4& other) const {
			Matrix4 result;
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					result.m[i][j] = m[i][j] - other.m[i][j];
				}
			}
			return result;
		}
		Matrix4 multiply(const Matrix4& other) const { //multiply two matrices together
			Matrix4 result;
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					result.m[i][j] = 0;
					for (int k = 0; k < 4; k++) {
						result.m[i][j] += m[i][k] * other.m[k][j];
					}
				}
			}
			return result;
		}
		static Matrix4 translate(float tx, float ty, float tz) {
			Matrix4 result;
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					if (i == j) { //diagonal
						result.m[i][j] = 1.0f;
					}
					else {
						result.m[i][j] = 0.0f;
					}
				}
			}
			result.m[0][3] = tx;
			result.m[1][3] = ty;
			result.m[2][3] = tz;
			return result;
		}
		static Matrix4 scale(float sx, float sy, float sz) {
			Matrix4 result;
			result.m[0][0] = sx;
			result.m[1][1] = sy;
			result.m[2][2] = sz;
			result.m[3][3] = 1.0f;
			return result;
		}
		static Matrix4 rotateX(float angle) {
			Matrix4 result;
			float radians = angle * (PI / 180.0f);
			result.m[1][1] = cosf(radians);
			result.m[1][2] = -sinf(radians);
			result.m[2][1] = sinf(radians);
			result.m[2][2] = cosf(radians);
			return result;
		}
		static Matrix4 rotateY(float angle) {
			Matrix4 result;
			float radians = angle * (PI / 180.0f);
			result.m[0][0] = cosf(radians);
			result.m[0][2] = sinf(radians);
			result.m[2][0] = -sinf(radians);
			result.m[2][2] = cosf(radians);
			return result;
		}
		static Matrix4 rotateZ(float angle) {
			Matrix4 result;
			float radians = angle * (PI / 180.0f);
			result.m[0][0] = cosf(radians);
			result.m[0][1] = -sinf(radians);
			result.m[1][0] = sinf(radians);
			result.m[1][1] = cosf(radians);
			return result;
		}
		Matrix4 transpose() const {
			Matrix4 result;
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					result.m[i][j] = m[j][i];
				}
			}
			return result;
		}
		Vector3 vecmatrix(const Vector3& vec) const {
			float x = m[0][0] * vec.x + m[0][1] * vec.y + m[0][2] * vec.z + m[0][3]; //x is set to the dot product of the first row of the matrix and the vector
			float y = m[1][0] * vec.x + m[1][1] * vec.y + m[1][2] * vec.z + m[1][3];
			float z = m[2][0] * vec.x + m[2][1] * vec.y + m[2][2] * vec.z + m[2][3];
			float w = m[3][0] * vec.x + m[3][1] * vec.y + m[3][2] * vec.z + m[3][3];
			if (w != 1.0f && w != 0.0f) {
				x /= w;
				y /= w;
				z /= w;
			}
			return Vector3(x, y, z);
		}
		static Matrix4 perspective(float fov, float aspect_ratio, float near_clip, float far_clip) { //perspective projection matrix
			Matrix4 result;
			float f = 1.0f / tanf(fov * 0.5f * (PI / 180.0f));
			result.m[0][0] = f / aspect_ratio;
			result.m[1][1] = f;
			result.m[2][2] = (far_clip + near_clip) / (near_clip - far_clip);
			result.m[2][3] = (2.0f * far_clip * near_clip) / (near_clip - far_clip);
			result.m[3][2] = -1.0f;
			result.m[3][3] = 0.0f;
			return result;
		}
		static Matrix4 worldmatrix(float tx, float ty, float tz, float rx, float ry, float rz, float sx, float sy, float sz) { //t = translation, r = rotation, s = scale
			Matrix4 result = Matrix4::translate(tx, ty, tz).multiply(Matrix4::rotateX(rx)).multiply(Matrix4::rotateY(ry)).multiply(Matrix4::rotateZ(rz)).multiply(Matrix4::scale(sx, sy, sz));
			return result;
		}
		static Matrix4 viewmatrix(float camx, float camy, float camz) {
			Matrix4 result = Matrix4::rotateX(camx).multiply(Matrix4::rotateY(camy)).multiply(Matrix4::rotateZ(camz)).multiply(Matrix4::translate(-camx, -camy, -camz));
			return result;
		}
		static Vector3 projectVector(const Vector3& vectoroni, const Matrix4& world, const Matrix4& view, const Matrix4& projection) {
			Matrix4 result = world.multiply(view).multiply(projection);
			return result.vecmatrix(vectoroni);
		}
		static Vector2 project2D(const Vector3& vec3, float screenWidth, float screenHeight) { //converts a 3D vector to 2D to display
			float halfScreenWidth = screenWidth / 2;
			float halfScreenHeight = screenHeight / 2;
			float zNormalized = std::max(0.001f, vec3.z + 1.0f);
			float projectedX = vec3.x / zNormalized * halfScreenWidth + halfScreenWidth;
			float projectedY = vec3.y / zNormalized * halfScreenHeight + halfScreenHeight;
			return Vector2(projectedX, projectedY);
		}
		static Vector3 norm(const Vector3& vec3) { //normalizes a vector to NDC
			float x = (vec3.x + 1.0f) / 2.0f;
			float y = (vec3.y + 1.0f) / 2.0f;
			float z = (vec3.z + 1.0f) / 2.0f;
			return Vector3(x, y, z);
		}
	};
};

#endif
;
