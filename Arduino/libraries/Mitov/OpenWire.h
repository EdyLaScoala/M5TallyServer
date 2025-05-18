////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//     This software is supplied under the terms of a license agreement or    //
//     nondisclosure agreement with Mitov Software and may not be copied      //
//     or disclosed except in accordance with the terms of that agreement.    //
//         Copyright(c) 2002-2022 Mitov Software. All Rights Reserved.        //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <Mitov_SimpleList.h>

#if ( defined( VISUINO_ESP32 ) || defined( VISUINO_ESP32_C3 ) )
    #define VISUINO_ANY_ESP32
#endif

#ifdef VISUINO_ESP8266
    #ifdef __BORLANDC__
        #define __ICACHE_RAM_ATTR__
    #else // __BORLANDC__
    	#define __ICACHE_RAM_ATTR__ ICACHE_RAM_ATTR
    #endif // __BORLANDC__

	#define VISUINO_MEMORY_ALLIGN
    #undef pgm_read_float
    #define pgm_read_float(addr)            (*reinterpret_cast<const float *>(addr))
    #undef pgm_read_ptr
    #define pgm_read_ptr(addr)              (*(void **)(addr))
#else // VISUINO_ESP8266
	#ifdef VISUINO_ESP32
        #ifdef __BORLANDC__
		    #define __ICACHE_RAM_ATTR__
    	#else // __BORLANDC__
		    #define __ICACHE_RAM_ATTR__ IRAM_ATTR
    	#endif // __BORLANDC__
	#else // VISUINO_ESP32
		#define __ICACHE_RAM_ATTR__
	#endif // VISUINO_ESP32
#endif // VISUINO_ESP8266

#if defined( VISUINO_MAPLE_MINI ) || defined( VISUINO_NUCLEO_F303RE )
	#define VISUINO_STMDUINO
#endif

#if defined( VISUINO_CHIPKIT_DP32 ) || defined( VISUINO_CHIPKIT_LENNY )
	#ifndef digitalPinToInterrupt
		#ifndef NOT_AN_INTERRUPT
			#define NOT_AN_INTERRUPT -1
		#endif

		#define digitalPinToInterrupt(p) ((p) == PIN_INT0 ? 0 : ((p) == PIN_INT1 ? 1 : ((p) == PIN_INT2 ? 2 : ((p) == PIN_INT3 ? 3 : ((p) == PIN_INT4 ? 4 : NOT_AN_INTERRUPT)))))
	#endif
#endif

#if defined( VISUINO_CHIPKIT_MAX32 )
	#define NOT_AN_INTERRUPT -1
	#define digitalPinToInterrupt(p) ((p) == 2 ? 1 : ((p) == 7 ? 2 : ((p) == 21 ? 3 : ((p) == 20 ? 4 : ((p) == 3 ? 0 : NOT_AN_INTERRUPT)))))

#endif

#if ( defined( VISUINO_STMDUINO ) || defined( VISUINO_TRINKET ) || defined( VISUINO_LINKIT_ONE ) || defined( VISUINO_INTEL_EDISON ) || defined( VISUINO_DIGISPARK_PRO ) || defined( VISUINO_DIGISPARK ) )

	#ifdef VISUINO_NUCLEO_F303RE
		#undef digitalPinToInterrupt
	#endif

	#ifndef digitalPinToInterrupt
		#define digitalPinToInterrupt(p) (p)
	#endif

#endif

#if __cplusplus <= 199711L
	#define constexpr
#endif
//		#define digitalPinToInterrupt(p) (p)

#if defined( VISUINO_MAPLE_MINI )
//	char * ltoa(long l, char * buffer, int radix)
//	{
//		return itoa( l, buffer, radix );
//	}
#endif

#if ( defined( VISUINO_FREESOC2 ) || defined( VISUINO_M0 ) || defined( VISUINO_CHIPKIT ) || defined( VISUINO_INTEL_EDISON ) )
  #if ! defined( __CORE_CM3_PSOC5_H__ )
//    #define PIN_MODE uint32_t
  #endif

namespace Mitov
{
	char *dtostrf (double val, signed char width, unsigned char prec, char *sout)
	{
		char fmt[20];
		sprintf(fmt, "%%%d.%df", width, prec);
		sprintf(sout, fmt, val);
		return sout;
	}
}
#endif

namespace Mitov
{
#if (( defined VISUINO_INTEL_EDISON ) || ( defined VISUINO_DIGISPARK ))
	class String : public ::String
	{
		typedef ::String inherited;

	public:
		const char *c_str() const
		{
#if ( defined VISUINO_DIGISPARK )
			return inherited::_buffer;
#else
			return inherited::buffer;
#endif
		}

	public:
		String & operator += (const String &rhs)	{concat(rhs); return (*this);}
#if ( ! defined VISUINO_DIGISPARK )
		String & operator += (const char *cstr)		{concat(cstr); return (*this);}
#endif
		String & operator += (char c)			{concat(c); return (*this);}
		String & operator += (unsigned char num)       	{concat(num); return (*this);}
		String & operator += (int num)			{concat(num); return (*this);}
		String & operator += (unsigned int num)		{concat(num); return (*this);}
		String & operator += (long num)			{concat(num); return (*this);}
		String & operator += (unsigned long num)	{concat(num); return (*this);}


		// Implement StringAdditionOperator per Arduino docs... String + __
		String operator + (const String &rhs)	{return String(*this) += rhs;}
#if ( ! defined VISUINO_DIGISPARK )
		String operator + (const char *cstr)	{return String(*this) += cstr;}
#endif
		String operator + (char c)		{return String(*this) += c;}
		String operator + (unsigned char num)	{return String(*this) += num;}
		String operator + (int num)		{return String(*this) += num;}
		String operator + (unsigned int num)	{return String(*this) += num;}
		String operator + (long num)		{return String(*this) += num;}
		String operator + (unsigned long num)	{return String(*this) += num;}

	public:
		void remove(unsigned int index)
		{
			// Pass the biggest integer as the count. The remove method
			// below will take care of truncating it at the end of the
			// string.
			remove(index, (unsigned int)-1);
		}

		void remove(unsigned int index, unsigned int count)
		{
#if ( defined VISUINO_DIGISPARK )
			if (index >= _length)
				return;

			if (count <= 0)
				return;

			if (count > _length - index)
				count = _length - index;

			char *writeTo = inherited::_buffer + index;
			_length -= count;
			strncpy( writeTo, inherited::_buffer + index + count, _length - index );
			_buffer[ _length ] = 0;
#else
			if (index >= len)
				return;

			if (count <= 0)
				return;

			if (count > len - index)
				count = len - index;

			char *writeTo = inherited::_buffer + index;
			len -= count;
			strncpy( writeTo, inherited::_buffer + index + count, len - index );
			buffer[len] = 0;
#endif
		}

#if ( defined VISUINO_DIGISPARK )
	public:
		unsigned char reserve(unsigned int size)
		{
			if ( _buffer && _capacity >= size) return 1;
			if (changeBuffer(size)) {
				if (_length == 0)
					_buffer[0] = 0;

				return 1;
			}

			return 0;
		}

	protected:
		void invalidate(void)
		{
			if( _buffer )
				free( _buffer );

			_buffer = nullptr;
			_capacity = _length = 0;
		}

		unsigned char changeBuffer(unsigned int maxStrLen)
		{
			char *newbuffer = (char *)realloc(_buffer, maxStrLen + 1);
			if (newbuffer)
			{
				_buffer = newbuffer;
				_capacity = maxStrLen;
				return 1;
			}

			return 0;
		}

		String & copy(const char *cstr, unsigned int length)
		{
			if (!reserve(length))
			{
				invalidate();
				return *this;
			}

			_length = length;
			strcpy( _buffer, cstr );
			return *this;
		}
#endif

	public:
		using inherited::inherited;

		String(const char *cstr = "")
		{
			init();
			if (cstr)
				copy(cstr, strlen(cstr));
		}

		String(const ::String &str)
		{
			init();
			*this = str;
		}


#if ( defined VISUINO_DIGISPARK )
		explicit String( uint8_t AValue, uint8_t base=10) : inherited( (unsigned int)AValue, base )
		{
		}

#endif
		explicit String(float value, uint8_t decimalPlaces=2)
		{
			init();
			char buf[33];
			*this = dtostrf(value, (decimalPlaces + 2), decimalPlaces, buf);
		}

		explicit String(double value, uint8_t decimalPlaces=2)
		{
			init();
			char buf[33];
			*this = dtostrf(value, (decimalPlaces + 2), decimalPlaces, buf);
		}

	};

#else
    #ifdef VISUINO_ANY_ESP32
	class String : public ::String
    {
        typedef ::String inherited;

	public:
        inline const char *buffer() const { return inherited::buffer(); }
        inline void invalidate(void) { inherited::invalidate(); }
        inline unsigned int len() const { return inherited::len(); }

	public:
        String substring(unsigned int beginIndex) const { inherited::substring( beginIndex ); }
        String substring(unsigned int beginIndex, unsigned int endIndex) const { inherited::substring( beginIndex, endIndex ); }

	public:
/*
		String & operator += (const String &rhs)	{concat(rhs); return (*this);}
		String & operator += (const char *cstr)		{concat(cstr); return (*this);}
		String & operator += (char c)			{concat(c); return (*this);}
		String & operator += (unsigned char num)       	{concat(num); return (*this);}
		String & operator += (int num)			{concat(num); return (*this);}
		String & operator += (unsigned int num)		{concat(num); return (*this);}
		String & operator += (long num)			{concat(num); return (*this);}
		String & operator += (unsigned long num)	{concat(num); return (*this);}

		// Implement StringAdditionOperator per Arduino docs... String + __
		String operator + (const String &rhs)	{return String(*this) += rhs;}
		String operator + (const char *cstr)	{return String(*this) += cstr;}
		String operator + (char c)		{return String(*this) += c;}
		String operator + (unsigned char num)	{return String(*this) += num;}
		String operator + (int num)		{return String(*this) += num;}
		String operator + (unsigned int num)	{return String(*this) += num;}
		String operator + (long num)		{return String(*this) += num;}
		String operator + (unsigned long num)	{return String(*this) += num;}
*/
    public:
        using inherited::inherited;

        String( inherited &AOther ) : inherited( AOther )
        {
        }

        String( inherited AOther ) : inherited( AOther )
        {
        }

        String( float AVale, uint8_t ABase ) : inherited( AVale, (unsigned int)ABase )
        {
        }

    };

String operator +(const String &lhs, const String &rhs)
{
    String AResult( lhs );
    if(!AResult.concat(rhs.buffer(), rhs.len()))
        AResult.invalidate();

    return AResult;
}

String operator +(const String &lhs, const char *cstr)
{
    String AResult( lhs );
    if(!cstr || !AResult.concat(cstr, strlen(cstr)))
        AResult.invalidate();

    return AResult;
}

String operator +( const char *cstr, const String &rhs)
{
    String AResult( cstr );
    return AResult + rhs;
}

String operator +(const String &lhs, char c)
{
    String AResult( lhs );
    if(!AResult.concat(c))
        AResult.invalidate();

    return AResult;
}

String operator +(char c, const String &rhs)
{
    String AResult( c );
    return AResult + rhs;
}

String operator +(const String &lhs, unsigned char num)
{
    String AResult( lhs );
    if(!AResult.concat(num))
        AResult.invalidate();

    return AResult;
}

String operator +(unsigned char num, const String &rhs)
{
    String AResult( num );
    return AResult + rhs;
}

String operator +(const String &lhs, int num)
{
    String AResult( lhs );
    if(!AResult.concat(num))
        AResult.invalidate();

    return AResult;
}

String operator +(int num, const String &rhs)
{
    String AResult( num );
    return AResult + rhs;
}

String operator +(const String &lhs, unsigned int num)
{
    String AResult( lhs );
    if(!AResult.concat(num))
        AResult.invalidate();

    return AResult;
}

String operator +(unsigned int num, const String &rhs)
{
    String AResult( num );
    return AResult + rhs;
}

String operator +(const String &lhs, long num)
{
    String AResult( lhs );
    if(!AResult.concat(num))
        AResult.invalidate();

    return AResult;
}

String operator +(long num, const String &rhs)
{
    String AResult( num );
    return AResult + rhs;
}

String operator +(const String &lhs, unsigned long num)
{
    String AResult( lhs );
    if(!AResult.concat(num))
        AResult.invalidate();

    return AResult;
}

String operator +(unsigned long num, const String &rhs)
{
    String AResult( num );
    return AResult + rhs;
}

String operator +(const String &lhs, float num)
{
    String AResult( lhs );
    if(!AResult.concat(num))
        AResult.invalidate();

    return AResult;
}

String operator +(float num, const String &rhs)
{
    String AResult( num );
    return AResult + rhs;
}

String operator +(const String &lhs, double num)
{
    String AResult( lhs );
    if(!AResult.concat(num))
        AResult.invalidate();

    return AResult;
}

String operator +(double num, const String &rhs)
{
    String AResult( num );
    return AResult + rhs;
}

String operator +(const String &lhs, long long num)
{
    String AResult( lhs );
    if(!AResult.concat(num))
        AResult.invalidate();

    return AResult;
}

String operator +(long long num, const String &rhs)
{
    String AResult( num );
    return AResult + rhs;
}

String operator +(const String &lhs, unsigned long long num)
{
    String AResult( lhs );
    if(!AResult.concat(num))
        AResult.invalidate();

    return AResult;
}

String operator +(unsigned long long num, const String &rhs)
{
    String AResult( num );
    return AResult + rhs;
}

String operator + (const String &lhs, const __FlashStringHelper *rhs)
{
    String AResult( lhs );
    if (!AResult.concat(rhs))
        AResult.invalidate();

    return AResult;
}

	#else // VISUINO_ANY_ESP32
	typedef ::String String;
    #endif // VISUINO_ANY_ESP32
#endif // (( defined VISUINO_INTEL_EDISON ) || ( defined VISUINO_DIGISPARK ))
}

namespace OpenWire
{
//---------------------------------------------------------------------------
	class Object
	{
	};
//---------------------------------------------------------------------------
	typedef void (*TPinCallback)( void *_Data );
//---------------------------------------------------------------------------
/*
	class CallBackPinImplementation
	{
	public:
		TPinCallback FCallback = nullptr;

	public:
		inline void Notify( void *_data )
		{
			if( FCallback )
				FCallback( _data );

		}

	public:
		inline bool IsConnected()
		{
			return ( FCallback != nullptr );
		}

		inline void SetCallback( TPinCallback ACallback )
		{
			FCallback = ACallback;
		}

	};
*/
//---------------------------------------------------------------------------
	class Null_PinImplementation
	{
	public:
		inline void Notify( void *_data )
		{
		}

	public:
		inline bool IsConnected()
		{
			return false;
		}

	};
//---------------------------------------------------------------------------
	template <typename T_BASIC> class StreamPin : public T_BASIC
	{
	public:
		inline StreamPin & operator ()() { return *this; }

	public:
		template <typename T> inline void SendValue( T AValue )
		{
			T_BASIC::Notify( &AValue );
		}

		inline void SendValue( Mitov::String AValue )
		{
			T_BASIC::Notify( (char *)AValue.c_str() );
		}

		inline void SendValue( const char *AValue )
		{
			T_BASIC::Notify( (char *)AValue );
		}
	};
//---------------------------------------------------------------------------
	class Null_StreamPin : public StreamPin<Null_PinImplementation>
	{
	};
//---------------------------------------------------------------------------
	template <typename T, typename T_BASIC> class TypedStreamPin : public StreamPin<T_BASIC>
	{
		typedef StreamPin<T_BASIC> inherited;

	public:
		T	Value;

	public:
		void SetValue( T AValue, bool AChangeOnly )
		{
			if( AChangeOnly )
				if( AValue == Value )
					return;

			Value = AValue;
			inherited::Notify( &Value );
		}

	};
//---------------------------------------------------------------------------
//	typedef StreamPin<CallBackPinImplementation> SourcePin;
//---------------------------------------------------------------------------
	template <
		uint16_t C_PIN,
		typename T_InterruptRaising
	> class PinRaisingInterrupt
	{
	public:
		void __ICACHE_RAM_ATTR__ InterruptHandler()
		{
			T_InterruptRaising::Call();
		}

	public:
/*
		inline PinRaisingInterrupt( void (*AInterruptRoutine)() )
		{
			// Add by Adrien van den Bossche <vandenbo@univ-tlse2.fr> for Teensy
			// ARM M4 requires the below. else pin interrupt doesn't work properly.
			// On all other platforms, its innocuous, belt and braces
			pinMode( C_PIN, INPUT );

			int AInterruptNumber = digitalPinToInterrupt( C_PIN );
			attachInterrupt( AInterruptNumber, AInterruptRoutine, RISING );
		}
*/
		inline void Init( void (*AInterruptRoutine)() )
		{
			// Add by Adrien van den Bossche <vandenbo@univ-tlse2.fr> for Teensy
			// ARM M4 requires the below. else pin interrupt doesn't work properly.
			// On all other platforms, its innocuous, belt and braces
//			pinMode( C_PIN, INPUT );

			int AInterruptNumber = digitalPinToInterrupt( C_PIN );
			attachInterrupt( AInterruptNumber, AInterruptRoutine, RISING );
		}
	};
//---------------------------------------------------------------------------
	template <
		uint16_t C_PIN,
		typename T_InterruptFalling
	> class PinFallingInterrupt
	{
	public:
		void __ICACHE_RAM_ATTR__ InterruptHandler()
		{
			T_InterruptFalling::Call();
		}

	public:
/*
		inline PinFallingInterrupt( void (*AInterruptRoutine)() )
		{
			// Add by Adrien van den Bossche <vandenbo@univ-tlse2.fr> for Teensy
			// ARM M4 requires the below. else pin interrupt doesn't work properly.
			// On all other platforms, its innocuous, belt and braces
//			pinMode( C_PIN, INPUT );

			int AInterruptNumber = digitalPinToInterrupt( C_PIN );
			attachInterrupt( AInterruptNumber, AInterruptRoutine, FALLING );
		}
*/
		inline void Init( void (*AInterruptRoutine)() )
		{
			// Add by Adrien van den Bossche <vandenbo@univ-tlse2.fr> for Teensy
			// ARM M4 requires the below. else pin interrupt doesn't work properly.
			// On all other platforms, its innocuous, belt and braces
//			pinMode( C_PIN, INPUT );

			int AInterruptNumber = digitalPinToInterrupt( C_PIN );
			attachInterrupt( AInterruptNumber, AInterruptRoutine, FALLING );
		}
	};
//---------------------------------------------------------------------------
	template <
		uint16_t C_PIN,
		typename T_InterruptRaising,
		typename T_InterruptFalling
	> class PinChangeInterrupt
	{
	public:
		void __ICACHE_RAM_ATTR__ InterruptHandler()
		{
//			Serial.println( "#" );
			if( digitalRead( C_PIN ) == HIGH )
				T_InterruptRaising::Call();

			else
				T_InterruptFalling::Call();

		}

	public:
/*
		inline PinChangeInterrupt( void (*AInterruptRoutine)() )
		{
			// Add by Adrien van den Bossche <vandenbo@univ-tlse2.fr> for Teensy
			// ARM M4 requires the below. else pin interrupt doesn't work properly.
			pinMode( C_PIN, INPUT_PULLUP );

			int AInterruptNumber = digitalPinToInterrupt( C_PIN );
			attachInterrupt( AInterruptNumber, AInterruptRoutine, CHANGE );
		}
*/
		inline void Init( void (*AInterruptRoutine)() )
		{
			// Add by Adrien van den Bossche <vandenbo@univ-tlse2.fr> for Teensy
			// ARM M4 requires the below. else pin interrupt doesn't work properly.
//			pinMode( C_PIN, INPUT );

			int AInterruptNumber = digitalPinToInterrupt( C_PIN );
			attachInterrupt( AInterruptNumber, AInterruptRoutine, CHANGE );
		}
	};
//---------------------------------------------------------------------------
}
