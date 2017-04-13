// Copyright 2016-2017 John Jackson. All Rights Reserved.
// File: Quaternion.cpp

#include <Math/Quaternion.h>

namespace Enjon 
{ 
	Vec3 Quaternion::XYZ()
	{
		return Vec3(x, y, z);
	}

	f32 Quaternion::Length()
	{
		return sqrt(x*x + y*y + z*z + w*w);
	}
	
	Quaternion Quaternion::Normalize()
	{
		Quaternion Q = *this;
		auto L = this->Length();

		Q.x /= L;
		Q.y /= L;
		Q.z /= L;
		Q.w /= L;

		return Q;
	}

	Quaternion Quaternion::Conjugate() const
	{
		return Quaternion( -x, -y, -z, w );
	}

	// Negative unary operator overload
	Quaternion Quaternion::operator-() const
	{
		return *this * -1.0f;
	}

	Quaternion Quaternion::operator+(const Quaternion& Other) const
	{
		Quaternion C;

		C.x = x + Other.x;
		C.y = y + Other.y;
		C.z = z + Other.z;
		C.w = w + Other.w;

		return C;
	}

	Quaternion Quaternion::operator-(const Quaternion& Other) const
	{
		Quaternion C;

		C.x = x - Other.x;
		C.y = y - Other.y;
		C.z = z - Other.z;
		C.w = w - Other.w;

		return C;
	}

	Quaternion Quaternion::operator*(const Quaternion& Other) const 
	{
		Quaternion Result; 

		Result.x = w * Other.x + Other.w * x + y * Other.z - Other.y * z;
		Result.y = w * Other.y + Other.w * y + z * Other.x - Other.z * x;
		Result.z = w * Other.z + Other.w * z + x * Other.y - Other.x * y;
		Result.w = w * Other.w - x * Other.x - y * Other.y - z * Other.z; 

		return Result;	
	}

	Quaternion Quaternion::operator*(const f32& V) const
	{
		Quaternion C = *this;

		C.x *= V;
		C.y *= V;
		C.z *= V;
		C.w *= V;

		return C;
	}


	Quaternion Quaternion::operator/(const f32& V) const
	{
		Quaternion C = *this;
		
		C.x /= V;
		C.y /= V;
		C.z /= V;
		C.w /= V;

		return C;	
	}

	bool Quaternion::operator==(const Quaternion& Other) const
	{
		if (Other.x != x) return false;
		if (Other.y != y) return false;
		if (Other.z != z) return false;
		if (Other.w != w) return false;

		return true;		
	}

	bool Quaternion::operator!=(const Quaternion& Other) const
	{
		return !(Other == *this);
	}

	std::ostream& operator<<(std::ostream& stream, const Quaternion& Q)
	{
		stream << "Quaternion: (" << Q.x << ", " << Q.y << ", " << Q.z << ", " << Q.w << ")";
		return stream;	
	}

	Vec3 Quaternion::Rotate( const Vec3& v ) const
	{
		// nVidia SDK implementation

		Vec3 uv;
		Vec3 uuv;
		Vec3 qVec( this->x, this->y, this->z );
		uv = qVec.Cross( v );
		uuv = qVec.Cross( uv );
		uv *= ( 2.0f * w );
		uuv *= 2.0f;

		return v + uv + uuv;
	}

	f32 Quaternion::Dot(Quaternion& Q)
	{ 
		return x*Q.x + y*Q.y + z*Q.z + w*Q.w;
	}

	// Returns cross product with another quaternion
	Quaternion Quaternion::Cross(const Quaternion& Other) const
	{
		Quaternion C;

		C.x = w*Other.x + x*Other.w + y*Other.z - z*Other.y;
		C.y = w*Other.y + y*Other.w + z*Other.x - x*Other.z;
		C.z = w*Other.z + z*Other.w + x*Other.y - y*Other.x;
		C.w = w*Other.w - x*Other.x - y*Other.y - z*Other.z; 		

		return C;	
	}
			
	// Returns inverse of quaternion
	Quaternion Quaternion::Inverse( )
	{ 
		return Conjugate( ) / Dot( *this );
	}

	Quaternion Quaternion::Inverse(Quaternion& Q)
	{
		Quaternion C = Q.Conjugate() / Q.Dot(Q);
		return C;
	}

	f32 Quaternion::Angle()
	{
		return 2.0f * std::acos(w);
	}

	Vec3 Quaternion::Axis()
	{
		// 1 - cos(theta/2)^2 = sin(theta/2);
		f32 S2 = 1.0f - w * w;

		// If negative, return
		if (S2 <= 0.0f) return Vec3(0.0f, 0.0f, 1.0f);

		f32 InverseS2 = 1.0f / std::sqrt(S2);

		return Vec3(x, y, z) * InverseS2;
	}

	f32 Quaternion::Roll()
	{
		return std::atan2(2.0f*x*y + z*w, x*x + w*w - y*y - z*z);
	}

	f32 Quaternion::Pitch()
	{
		return std::atan2(2.0f*y*z + w*x, w*w - x*x - y*y + z*z);
	}

	f32 Quaternion::Yaw()
	{
		return std::asin(-2.0f * (x*z - w*y));
	}

	Vec3 Quaternion::EulerAngles()
	{
		return Vec3(this->Pitch(), this->Yaw(), this->Roll());
		double ysqr = y * y;

		// roll (x-axis rotation)
		double t0 = +2.0 * ( w * x + y * z );
		double t1 = +1.0 - 2.0 * ( x * x + ysqr );
		double roll = std::atan2( t0, t1 );

		// pitch (y-axis rotation)
		double t2 = +2.0 * ( w * y - z * x );
		t2 = t2 > 1.0 ? 1.0 : t2;
		t2 = t2 < -1.0 ? -1.0 : t2;
		double pitch = std::asin( t2 );

		// yaw (z-axis rotation)
		double t3 = +2.0 * ( w * z + x * y );
		double t4 = +1.0 - 2.0 * ( ysqr + z * z );
		double yaw = std::atan2( t3, t4 );

		return Vec3( roll, pitch, yaw );
	} 

	Vec3 Quaternion::operator*(const Vec3& V) const
	{
		auto Qxyz = Vec3(x, y, z); 
		const Vec3 t = 2.0f * Qxyz.Cross( V );
		return ( V + w * t + Qxyz.Cross( t ) ); 
	} 
}

























