#include <math.h>
#include <assert.h>

#include <vega/core/math/constants.h>
#include <vega/core/math/math.h>
#include <vega/core/math/vector2.h>
#include <vega/core/math/vector3.h>
#include <vega/core/math/vector4.h>
#include <vega/core/math/quaternion.h>
#include <vega/core/math/tests.h>

#ifndef ASSERT_EPSILON
	#define ASSERT_EPSILON (1.0E-15)
#endif // ASSERT_EPSILON

#ifndef ASSERT_VALUE
	#define ASSERT_VALUE(EXPRESSION, EXPECTED) (assert(fabs((EXPRESSION) - (EXPECTED)) <= (ASSERT_EPSILON)))
#endif // ASSERT_VALUE

#ifdef VEGA_DEBUG
void math_tests()
{
	{
		{
			vector2_t c = math_vector2_zero();

			ASSERT_VALUE(c.x, 0.0);
			ASSERT_VALUE(c.y, 0.0);
		}

		{
			vector2_t c = math_vector2_from_xy(1.0, 2.0);

			ASSERT_VALUE(c.x, 1.0);
			ASSERT_VALUE(c.y, 2.0);
		}

		{
			vector2_t a = math_vector2_from_xy(3.0, 4.0);
			vector2_t b = math_vector2_from_xy(2.0, 1.0);
			vector2_t c = math_vector2_add(a, b);

			ASSERT_VALUE(c.x, 5.0);
			ASSERT_VALUE(c.y, 5.0);
		}

		{
			vector2_t a = math_vector2_from_xy(3.0, 4.0);
			vector2_t b = math_vector2_from_xy(2.0, 1.0);
			vector2_t c = math_vector2_sub(a, b);

			ASSERT_VALUE(c.x, 1.0);
			ASSERT_VALUE(c.y, 3.0);
		}

		{
			vector2_t a = math_vector2_from_xy(3.0, 4.0);
			vector2_t b = math_vector2_from_xy(2.0, 1.0);
			vector2_t c = math_vector2_mul(a, b);

			ASSERT_VALUE(c.x, 6.0);
			ASSERT_VALUE(c.y, 4.0);
		}

		{
			vector2_t a = math_vector2_from_xy(4.0, 8.0);
			vector2_t b = math_vector2_from_xy(2.0, 2.0);
			vector2_t c = math_vector2_div(a, b);

			ASSERT_VALUE(c.x, 2.0);
			ASSERT_VALUE(c.y, 4.0);
		}

		{
			vector2_t a = math_vector2_from_xy(3.0, 4.0);
			vector2_t c = math_vector2_add_scalar(a, 2.0);

			ASSERT_VALUE(c.x, 5.0);
			ASSERT_VALUE(c.y, 6.0);
		}

		{
			vector2_t a = math_vector2_from_xy(3.0, 4.0);
			vector2_t c = math_vector2_sub_scalar(a, 2.0);

			ASSERT_VALUE(c.x, 1.0);
			ASSERT_VALUE(c.y, 2.0);
		}

		{
			vector2_t a = math_vector2_from_xy(3.0, 4.0);
			vector2_t c = math_vector2_mul_scalar(a, 2.0);

			ASSERT_VALUE(c.x, 6.0);
			ASSERT_VALUE(c.y, 8.0);
		}

		{
			vector2_t a = math_vector2_from_xy(4.0, 8.0);
			vector2_t c = math_vector2_div_scalar(a, 2.0);

			ASSERT_VALUE(c.x, 2.0);
			ASSERT_VALUE(c.y, 4.0);
		}

		{
			vector2_t a = math_vector2_from_xy(4.0, 8.0);
			vector2_t b = math_vector2_norm(a);
			double c = math_vector2_length(b);

			ASSERT_VALUE(c, 1.0);
		}

		{
			vector2_t a = math_vector2_from_xy(3.0, 4.0);
			vector2_t b = math_vector2_from_xy(2.0, 1.0);
			double c = math_vector2_dot(a, b);

			ASSERT_VALUE(c, 10.0);
		}

		{
			vector2_t a = math_vector2_from_xy(3.0, 4.0);
			double c = math_vector2_length(a);

			ASSERT_VALUE(c, 5.0);
		}

		{
			vector2_t a = math_vector2_from_xy(3.0, 4.0);
			double c = math_vector2_length2(a);

			ASSERT_VALUE(c, 25.0);
		}
	}

	{
		{
			vector3_t c = math_vector3_zero();

			ASSERT_VALUE(c.x, 0.0);
			ASSERT_VALUE(c.y, 0.0);
			ASSERT_VALUE(c.z, 0.0);
		}

		{
			vector3_t c = math_vector3_from_xyz(1.0, 2.0, 3.0);

			ASSERT_VALUE(c.x, 1.0);
			ASSERT_VALUE(c.y, 2.0);
			ASSERT_VALUE(c.z, 3.0);
		}

		{
			vector3_t a = math_vector3_from_xyz(3.0, 4.0, 2.0);
			vector3_t b = math_vector3_from_xyz(2.0, 1.0, 3.0);
			vector3_t c = math_vector3_add(a, b);

			ASSERT_VALUE(c.x, 5.0);
			ASSERT_VALUE(c.y, 5.0);
			ASSERT_VALUE(c.z, 5.0);
		}

		{
			vector3_t a = math_vector3_from_xyz(3.0, 4.0, 2.0);
			vector3_t b = math_vector3_from_xyz(2.0, 1.0, 3.0);
			vector3_t c = math_vector3_sub(a, b);

			ASSERT_VALUE(c.x, 1.0);
			ASSERT_VALUE(c.y, 3.0);
			ASSERT_VALUE(c.z, -1.0);
		}

		{
			vector3_t a = math_vector3_from_xyz(3.0, 4.0, 2.0);
			vector3_t b = math_vector3_from_xyz(2.0, 1.0, 3.0);
			vector3_t c = math_vector3_mul(a, b);

			ASSERT_VALUE(c.x, 6.0);
			ASSERT_VALUE(c.y, 4.0);
			ASSERT_VALUE(c.z, 6.0);
		}

		{
			vector3_t a = math_vector3_from_xyz(4.0, 8.0, 16.0);
			vector3_t b = math_vector3_from_xyz(2.0, 2.0, 2.0);
			vector3_t c = math_vector3_div(a, b);

			ASSERT_VALUE(c.x, 2.0);
			ASSERT_VALUE(c.y, 4.0);
			ASSERT_VALUE(c.z, 8.0);
		}

		{
			vector3_t a = math_vector3_from_xyz(3.0, 4.0, 2.0);
			vector3_t c = math_vector3_add_scalar(a, 2.0);

			ASSERT_VALUE(c.x, 5.0);
			ASSERT_VALUE(c.y, 6.0);
			ASSERT_VALUE(c.z, 4.0);
		}

		{
			vector3_t a = math_vector3_from_xyz(3.0, 4.0, 2.0);
			vector3_t c = math_vector3_sub_scalar(a, 2.0);

			ASSERT_VALUE(c.x, 1.0);
			ASSERT_VALUE(c.y, 2.0);
			ASSERT_VALUE(c.z, 0.0);
		}

		{
			vector3_t a = math_vector3_from_xyz(3.0, 4.0, 2.0);
			vector3_t c = math_vector3_mul_scalar(a, 2.0);

			ASSERT_VALUE(c.x, 6.0);
			ASSERT_VALUE(c.y, 8.0);
			ASSERT_VALUE(c.z, 4.0);
		}

		{
			vector3_t a = math_vector3_from_xyz(4.0, 8.0, 2.0);
			vector3_t c = math_vector3_div_scalar(a, 2.0);

			ASSERT_VALUE(c.x, 2.0);
			ASSERT_VALUE(c.y, 4.0);
			ASSERT_VALUE(c.z, 1.0);
		}

		{
			vector3_t a = math_vector3_from_xyz(4.0, 8.0, 3.0);
			vector3_t b = math_vector3_norm(a);
			double c = math_vector3_length(b);

			ASSERT_VALUE(c, 1.0);
		}

		{
			vector3_t a = math_vector3_from_xyz(3.0, 4.0, 2.0);
			vector3_t b = math_vector3_from_xyz(2.0, 1.0, 3.0);
			double c = math_vector3_dot(a, b);

			ASSERT_VALUE(c, 16.0);
		}

		{
			vector3_t a = math_vector3_from_xyz(3.0, 4.0, 2.0);
			double c = math_vector3_length(a);

			ASSERT_VALUE(c, 5.3851648071345037);
		}

		{
			vector3_t a = math_vector3_from_xyz(3.0, 4.0, 2.0);
			double c = math_vector3_length2(a);

			ASSERT_VALUE(c, 29.0);
		}

		{
			vector3_t a = math_vector3_from_xyz(1.0, 0.0, 0.0);
			vector3_t b = math_vector3_from_xyz(0.0, 1.0, 0.0);
			vector3_t c = math_vector3_cross(a, b);

			ASSERT_VALUE(c.x, 0.0);
			ASSERT_VALUE(c.y, 0.0);
			ASSERT_VALUE(c.z, 1.0);
		}

		{
			vector3_t a = math_vector3_from_xyz(1.0, 0.0, 0.0);
			quaternion_t b = math_quaternion_identity();
			vector3_t c = math_vector3_rotate(a, b);

			ASSERT_VALUE(c.x, 1.0);
			ASSERT_VALUE(c.y, 0.0);
			ASSERT_VALUE(c.z, 0.0);
		}

		{
			vector3_t a = math_vector3_from_xyz(1.0, 0.0, 0.0);
			quaternion_t b = math_quaternion_from_euler_angles_xyz(0.0, 90.0, 0.0);
			vector3_t c = math_vector3_rotate(a, b);

			ASSERT_VALUE(c.x, 0.0);
			ASSERT_VALUE(c.y, 1.0);
			ASSERT_VALUE(c.z, 0.0);
		}

		{
			vector3_t a = math_vector3_from_xyz(1.0, 0.0, 0.0);
			quaternion_t b = math_quaternion_from_euler_angles_xyz(0.0, -90.0, 0.0);
			vector3_t c = math_vector3_rotate(a, b);

			ASSERT_VALUE(c.x, 0.0);
			ASSERT_VALUE(c.y, -1.0);
			ASSERT_VALUE(c.z, 0.0);
		}
	}

	{
		{
			vector4_t c = math_vector4_zero();

			ASSERT_VALUE(c.x, 0.0);
			ASSERT_VALUE(c.y, 0.0);
			ASSERT_VALUE(c.z, 0.0);
			ASSERT_VALUE(c.w, 0.0);
		}

		{
			vector4_t c = math_vector4_from_xyzw(1.0, 2.0, 3.0, 4.0);

			ASSERT_VALUE(c.x, 1.0);
			ASSERT_VALUE(c.y, 2.0);
			ASSERT_VALUE(c.z, 3.0);
			ASSERT_VALUE(c.w, 4.0);
		}

		{
			vector4_t a = math_vector4_from_xyzw(3.0, 4.0, 2.0, 5.0);
			vector4_t b = math_vector4_from_xyzw(2.0, 1.0, 3.0, 6.0);
			vector4_t c = math_vector4_add(a, b);

			ASSERT_VALUE(c.x, 5.0);
			ASSERT_VALUE(c.y, 5.0);
			ASSERT_VALUE(c.z, 5.0);
			ASSERT_VALUE(c.w, 11.0);
		}

		{
			vector4_t a = math_vector4_from_xyzw(3.0, 4.0, 2.0, 5.0);
			vector4_t b = math_vector4_from_xyzw(2.0, 1.0, 3.0, 6.0);
			vector4_t c = math_vector4_sub(a, b);

			ASSERT_VALUE(c.x, 1.0);
			ASSERT_VALUE(c.y, 3.0);
			ASSERT_VALUE(c.z, -1.0);
			ASSERT_VALUE(c.w, -1.0);
		}

		{
			vector4_t a = math_vector4_from_xyzw(3.0, 4.0, 2.0, 5.0);
			vector4_t b = math_vector4_from_xyzw(2.0, 1.0, 3.0, 6.0);
			vector4_t c = math_vector4_mul(a, b);

			ASSERT_VALUE(c.x, 6.0);
			ASSERT_VALUE(c.y, 4.0);
			ASSERT_VALUE(c.z, 6.0);
			ASSERT_VALUE(c.w, 30.0);
		}

		{
			vector4_t a = math_vector4_from_xyzw(4.0, 8.0, 16.0, 32.0);
			vector4_t b = math_vector4_from_xyzw(2.0, 2.0, 2.0, 2.0);
			vector4_t c = math_vector4_div(a, b);

			ASSERT_VALUE(c.x, 2.0);
			ASSERT_VALUE(c.y, 4.0);
			ASSERT_VALUE(c.z, 8.0);
			ASSERT_VALUE(c.w, 16.0);
		}

		{
			vector4_t a = math_vector4_from_xyzw(3.0, 4.0, 2.0, 5.0);
			vector4_t c = math_vector4_add_scalar(a, 2.0);

			ASSERT_VALUE(c.x, 5.0);
			ASSERT_VALUE(c.y, 6.0);
			ASSERT_VALUE(c.z, 4.0);
			ASSERT_VALUE(c.w, 7.0);
		}

		{
			vector4_t a = math_vector4_from_xyzw(3.0, 4.0, 2.0, 5.0);
			vector4_t c = math_vector4_sub_scalar(a, 2.0);

			ASSERT_VALUE(c.x, 1.0);
			ASSERT_VALUE(c.y, 2.0);
			ASSERT_VALUE(c.z, 0.0);
			ASSERT_VALUE(c.w, 3.0);
		}

		{
			vector4_t a = math_vector4_from_xyzw(3.0, 4.0, 2.0, 5.0);
			vector4_t c = math_vector4_mul_scalar(a, 2.0);

			ASSERT_VALUE(c.x, 6.0);
			ASSERT_VALUE(c.y, 8.0);
			ASSERT_VALUE(c.z, 4.0);
			ASSERT_VALUE(c.w, 10.0);
		}

		{
			vector4_t a = math_vector4_from_xyzw(4.0, 8.0, 2.0, 6.0);
			vector4_t c = math_vector4_div_scalar(a, 2.0);

			ASSERT_VALUE(c.x, 2.0);
			ASSERT_VALUE(c.y, 4.0);
			ASSERT_VALUE(c.z, 1.0);
			ASSERT_VALUE(c.w, 3.0);
		}

		{
			vector4_t a = math_vector4_from_xyzw(4.0, 8.0, 3.0, 5.0);
			vector4_t b = math_vector4_norm(a);
			double c = math_vector4_length(b);

			ASSERT_VALUE(c, 1.0);
		}

		{
			vector4_t a = math_vector4_from_xyzw(3.0, 4.0, 2.0, 5.0);
			vector4_t b = math_vector4_from_xyzw(2.0, 1.0, 3.0, 4.0);
			double c = math_vector4_dot(a, b);

			ASSERT_VALUE(c, 36.0);
		}

		{
			vector4_t a = math_vector4_from_xyzw(3.0, 4.0, 2.0, 5.0);
			double c = math_vector4_length(a);

			ASSERT_VALUE(c, 7.3484692283495345);
		}

		{
			vector4_t a = math_vector4_from_xyzw(3.0, 4.0, 2.0, 5.0);
			double c = math_vector4_length2(a);

			ASSERT_VALUE(c, 54.0);
		}
	}

	{
		{
			quaternion_t c = math_quaternion_zero();

			ASSERT_VALUE(c.x, 0.0);
			ASSERT_VALUE(c.y, 0.0);
			ASSERT_VALUE(c.z, 0.0);
			ASSERT_VALUE(c.w, 0.0);
		}

		{
			quaternion_t c = math_quaternion_identity();

			ASSERT_VALUE(c.x, 0.0);
			ASSERT_VALUE(c.y, 0.0);
			ASSERT_VALUE(c.z, 0.0);
			ASSERT_VALUE(c.w, 1.0);
		}

		{
			quaternion_t c = math_quaternion_from_xyzw(1.0, 2.0, 3.0, 4.0);

			ASSERT_VALUE(c.x, 1.0);
			ASSERT_VALUE(c.y, 2.0);
			ASSERT_VALUE(c.z, 3.0);
			ASSERT_VALUE(c.w, 4.0);
		}

		{
			quaternion_t a = math_quaternion_from_euler_angles_xyz(90.0, 0.0, 0.0);
			quaternion_t b = math_quaternion_from_euler_angles_xyz(0.0, 90.0, 0.0);
			quaternion_t c = math_quaternion_mul(a, b);

			ASSERT_VALUE(c.x, 0.5);
			ASSERT_VALUE(c.y, 0.5);
			ASSERT_VALUE(c.z, 0.5);
			ASSERT_VALUE(c.w, 0.5);
		}

		{
			quaternion_t a = math_quaternion_from_euler_angles_xyz(90.0, 0.0, 0.0);
			quaternion_t c = math_quaternion_mul_scalar(a, 90.0);

			ASSERT_VALUE(c.x, 0.0);
			ASSERT_VALUE(c.y, 63.639610306789280);
			ASSERT_VALUE(c.z, 0.0);
			ASSERT_VALUE(c.w, 63.639610306789280);
		}

		{
			quaternion_t a = math_quaternion_from_euler_angles_xyz(90.0, 0.0, 0.0);
			quaternion_t c = math_quaternion_conjugate(a);

			ASSERT_VALUE(c.x, 0.0);
			ASSERT_VALUE(c.y, -0.70710678118654757);
			ASSERT_VALUE(c.z, 0.0);
			ASSERT_VALUE(c.w, 0.70710678118654757);
		}

		{
			vector3_t a = math_vector3_from_xyz(90.0, 0.0, 0.0);
			quaternion_t b = math_quaternion_from_euler_angles(a);
			vector3_t c = math_quaternion_to_euler_angles(b);

			ASSERT_VALUE(c.x, 90.0);
			ASSERT_VALUE(c.y, 180.0);
			ASSERT_VALUE(c.z, 180.0);
		}

		{
			vector3_t a = math_vector3_from_xyz(1.0, 0.0, 0.0);
			quaternion_t c = math_quaternion_angle_axis(90.0, a);

			ASSERT_VALUE(c.x, 0.85090352453411844);
			ASSERT_VALUE(c.y, 0.0);
			ASSERT_VALUE(c.z, 0.0);
			ASSERT_VALUE(c.w, 0.52532198881772973);
		}

		{
			quaternion_t a = math_quaternion_from_xyzw(3.0, 2.0, 4.0, 5.0);
			quaternion_t c = math_quaternion_norm(a);

			ASSERT_VALUE(c.x, 0.40824829046386302);
			ASSERT_VALUE(c.y, 0.27216552697590868);
			ASSERT_VALUE(c.z, 0.54433105395181736);
			ASSERT_VALUE(c.w, 0.68041381743977170);
		}

		{
			quaternion_t a = math_quaternion_from_xyzw(3.0, 4.0, 2.0, 5.0);
			quaternion_t b = math_quaternion_from_xyzw(2.0, 1.0, 3.0, 4.0);
			double c = math_quaternion_dot(a, b);

			ASSERT_VALUE(c, 36.0);
		}

		{
			quaternion_t a = math_quaternion_from_xyzw(3.0, 4.0, 2.0, 5.0);
			double c = math_quaternion_length(a);

			ASSERT_VALUE(c, 7.3484692283495345);
		}

		{
			quaternion_t a = math_quaternion_from_xyzw(3.0, 4.0, 2.0, 5.0);
			double c = math_quaternion_length2(a);

			ASSERT_VALUE(c, 54.0);
		}
	}

	// TODO: add matrix4 tests..
}
#endif // VEGA_DEBUG
